#include "stdafx.h"
#include "keyEventHandler.h"

#ifdef _WIN32
#include <Winuser.h>
#include <conio.h>
#endif

#include <thread>

#include <net/test_interface.h>
#include <net/logger.h>

#pragma warning( disable:4996)


void keyEventHandler::processKey(const unsigned int vKey)
{
	switch (vKey)
	{
	#ifdef _WIN32
    case VK_ESCAPE:
    #else
    case 0x1B:
    #endif
    {
        LOGI << "pressed ESC key ...";

        auto ios = m_ios.lock();
        if (ios)
        {
            if (false == ios->stopped()){
                LOGI << "terminating process...";
                ios->stop();
            }
        }
    }
		break;
	case 0x70:/*VK_F1:*/
	{
	}
		break;
	case 0x71:/*VK_F1:*/
	{
	}
	break;

	}//switch (key)
}

void keyEventHandler::registCallback(boost::shared_ptr<boost::asio::io_service> ios)
{
	m_ios = ios;

	MLN::Net::TestIntarface::instance()->setMyKeyEventCallback(
		std::bind(&keyEventHandler::processKey
		, this
		, std::placeholders::_1));
}
