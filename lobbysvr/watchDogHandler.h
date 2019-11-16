#pragma once

#include <string>
#include <boost/asio.hpp>
#include <net/singleton.h>


class watchDogHandler
	: public MLN::Net::SingletonLight<watchDogHandler>
{
public:
	void registCallback(boost::shared_ptr<boost::asio::io_service> ios, unsigned short port);
	std::string inputLine(const std::string &inputMsg, boost::asio::ip::tcp::socket &sock);
	void closedConsole();

private:
	boost::weak_ptr<boost::asio::io_service> m_ios;
};