#include "stdafx.h"
#include <chrono>
#include <random>
#include <iostream>

#include <boost/filesystem.hpp>
#include <net/logger.h>
#include <net/netServiceAcceptor.h>
#include <net/netServiceConnector.h>
#include <net/packetEncType.h>
#include <net/test_interface.h>

#include "brokerConnector.h"
#include "environment.h"
#include "packetParserJson.h"
#include "lobbyUser.h"
#include "lobbyAcceptorReceiver.h"
#include "watchDogHandler.h"
#include "keyEventHandler.h"


#ifdef _WIN32
#else//#ifdef _WIN32
#include <unistd.h>
#include <limits.h>
#endif//#ifdef _WIN32

void post_ios_start() 
{
	LOGI << "started";
}

void app_shutdown(const std::string &msg)
{
}

void initTestInterface()
{
	auto env = Environment::instance();

	watchDogHandler::instance()->registCallback(
		shared_ios
		, env->m_testInterfacePort);

	keyEventHandler::instance()->registCallback(shared_ios);

	MLN::Net::TestIntarface::instance()->start(
		shared_ios.get()		// use nullptr if not using boost-asio
		//, MLN::Net::TestIntarface::FUNC::wait_keyboardEvent
		, MLN::Net::TestIntarface::FUNC::watchdog
		);
}

template <typename RECEIVER_TYPE>
boost::shared_ptr<MLN::Net::NetServiceAcceptor> registAcceptorReceiver(const size_t acceptorIDX, Environment::MLN_SvrSetting &setting)
{
	if (false == setting.activate) {
		return boost::shared_ptr<MLN::Net::NetServiceAcceptor>();
	}

	MLN::Net::EventReceiverAcceptorRegister<RECEIVER_TYPE> acceptorHandler;

	MLN::Net::NetService::ServiceParams serviceInitParams(
		*shared_ios.get()
		, acceptorHandler
		, JSON_PACKET_PARSING_SUPPORT::packetParser_notUseEncTest
		, JSON_PACKET_PARSING_SUPPORT::getMsgManipulatorTest()
		, setting.tick_time_ms
		, setting.keep_alive_ms
	);

	MLN::Net::NetService::AcceptorUserParams
		acceptorParams(
			""	// addr. null string is localhost.
			, setting.listenPort
			, (setting.workers != 0) ? (setting.workers) : (boost::thread::hardware_concurrency() * 2)
		);

	auto acceptor = MLN::Net::NetService::createAcceptor(
		acceptorIDX
		, serviceInitParams
		, acceptorParams
	);

	// Encryption �ڵ鷯 ���
	chase_lobbysvr::UserBasis::m_encType = MLN::Net::EncType::GREETING;

	return acceptor;
};

// #ifdef USE_CONNECTOR_FOR_BROKER
template <typename RECEIVER_TYPE>
boost::shared_ptr<MLN::Net::NetServiceConnector> registConnectorReceiver(const size_t connectorIDX, Environment::MLN_SvrSetting &setting)
{
	if (false == setting.activate) {
		return boost::shared_ptr<MLN::Net::NetServiceConnector>();
	}
	MLN::Net::EventReceiverConnectorRegister<RECEIVER_TYPE> connectorHandler;

	MLN::Net::NetService::ServiceParams connectorSvcParams(
		*shared_ios.get()
		, connectorHandler
		, JSON_PACKET_PARSING_SUPPORT::packetParser_brokerTest
		, JSON_PACKET_PARSING_SUPPORT::getMsgManipulatorTest()
		, 1000
		, 0//, 1000 * 15		// keepAliveTimeMs
	);

	MLN::Net::NetService::ConnectorUserParams
		connectorParams(
			setting.addr
			, setting.listenPort
		);

	auto connector = MLN::Net::NetService::createConnector(
		connectorIDX
		, connectorSvcParams
		, connectorParams
	);
	BrokerConnector::tryConnect();

	return connector;
};
// #endif


bool ioServiceThread()
{
	auto env = Environment::instance();

	// auto brokerAcceptor = registAcceptorReceiver<BrokerAcceptorReceiver>(ACCPETOR_IDX::BROKER, env->m_svrBroker);
	auto lobbyAcceptor = registAcceptorReceiver<LobbyAcceptorReceiver>(ACCPETOR_IDX::LOBBY, env->m_svrLobby);

	/*if (lobbyAcceptor) {
		lobbyAcceptor->getMsgProcedure()->setCipher<UserManager>(
			MLN::Net::EncType::GREETING
			, &LobbyUserManager::createUser
			, &LobbyUserManager::decryptMessage
			, LobbyUserManager::instance());
	}*/
	#ifdef USE_CONNECTOR_FOR_BROKER
    if (lobbyAcceptor) {
		auto brokerConnector = registConnectorReceiver<BrokerConnector>(CONNECTOR_INDEX::BROKER_CONNECTOR, env->m_svrBroker);
	}
    #endif

	// IO���� ����. runService() ���� ����ŷ �˴ϴ�.
	MLN::Net::NetService::runService(post_ios_start);

	return true;
}

bool initLogService(const std::string &Path)
{
	// �α׼��� �ʱ�ȭ
	std::cout << std::endl << "init Log Service" << std::endl;

	// if (false == MLN::Log::Logger::instance()->init(Path)) {
	// 	std::cout << "failed init Log Service.  " << std::endl;
	// 	return false;
	// }

	return true;
}


void setModuleDirToWorkingDir()
{
#ifdef _WIN32    
	TCHAR szPathBuffer[MAX_PATH];

	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule, szPathBuffer, MAX_PATH);

	int nIndex = 0;
	int nCnt = static_cast<int>(_tcslen(szPathBuffer));
	for (; nCnt > 0; nCnt--)
	{
		if (szPathBuffer[nCnt] == _T('\\'))
		{
			nIndex = nCnt;
			break;
		}
	}
	szPathBuffer[nIndex] = _T('\0');

	_tchdir(szPathBuffer);
#elif __linux__
	std::string modulePath = boost::filesystem::current_path().string();
    chdir(modulePath.c_str());
    std::cout << modulePath;
#else
    // boost::filesystem::system_complete(argv[0]);
    // chdir(boost::filesystem::current_path());
    // boost::filesystem::current_path(modulePath);
    std::string modulePath = boost::filesystem::current_path().string();
    chdir(modulePath.c_str());
    std::cout << modulePath;

#endif
}

void setRandom()
{
	auto curTime = std::chrono::system_clock::now();
	auto dur = curTime.time_since_epoch();
	auto mills = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();

	g_rand = new std::mt19937((unsigned int)mills);
}

int main(int argc, char* argv[])
{
	setModuleDirToWorkingDir();

	namespace fs = boost::filesystem;
	fs::path full_path = fs::system_complete("chase_lobby.xml");
	std::string confPath = full_path.string();

	// set Random.
	setRandom();

	// init Logger
	if (false == initLogService(confPath)) {
		std::cout << "failed initLogService" << std::endl;
		return 0;
	}

	#ifdef _WIN32
    MLN::CCrashHandler::Init(confPath.c_str());
    #endif

	// init Configurations
	auto env = Environment::instance();
	assert(env && "failed Environment");
	if (false == env->Initialize(confPath.c_str())) {
		LOGE << "Config Error!";
		std::cout << "failed Environment Init" << std::endl;
		return 0;
	}
	
	#ifdef _WIN32
    SetConsoleTitleA(env->m_title.c_str());
    MLN::MSSQL::DBConsumer::Open("Sudden64", "sa", "20934159", app_shutdown);
    #endif

	// load data table.
	// DataTable::instance()->load();

	// Test Interface.
	initTestInterface();

	return ioServiceThread();
}