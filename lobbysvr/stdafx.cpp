#include "stdafx.h"

boost::shared_ptr< boost::asio::io_service > shared_ios = boost::make_shared<boost::asio::io_service>();

uint16_t	g_serverIDX = 0;
std::mt19937 *g_rand = nullptr;

uint32_t g_idx4Broker = 0;