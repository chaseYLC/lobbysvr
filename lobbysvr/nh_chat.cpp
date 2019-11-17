#include "stdafx.h"
#include "nh_chat.h"
#include "lobbyUser.h"
#include "brokerConnector.h"
#include "mlnUtils.h"

void nh_chat::sendMsg(MLN::Net::Connection::ptr conn, const std::string &url, Json::Value &jv)
{
	GET_USER_LOBBY(user);

	/* channelType
	    1 : �׷�ä��
		2 : ����ä��
	*/

	// �޼��� ����.
	if (0 >= jv["msg"].asString().length() ) {
		LOGE << "msg size is zero.userIDX : " << std::to_string(user->m_userIDX);
		return;
	}

	// ���θ޼����� ��� ��� nick ���� ����.
	if (2 == jv["channelType"].asInt()
		&& 2 > jv["to"].asString().length()) {
		LOGE << "invalid target nickname. userIDX : " << std::to_string(user->m_userIDX);
		return;
	}

	// 1. set Additional values.
	jv["addition_fromIDX"] = user->m_userIDX;
	jv["addition_fromSVR"] = g_idx4Broker;
	jv["from"] = user->m_userID;
	jv["seq"] = jv[RSP_SEQ];
	jv["sendTime"] = (Json::Value::UInt64)MLN_Utils::getLocalTimeSec();

	// ���Ŀ�� ����.
	jv["identity"] = (uint32_t)(user->getConn()->getIdentity());
	BrokerConnector::sendJsonPacket("/chatSendMsg", jv);
}
