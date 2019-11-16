#pragma once

#include <net/singleton.h>

class keyEventHandler
	: public MLN::Net::SingletonLight<keyEventHandler>
{
public:
	void registCallback(boost::shared_ptr<boost::asio::io_service> ios);
	void processKey(const unsigned int vKey);

private:
	boost::weak_ptr<boost::asio::io_service> m_ios;

};