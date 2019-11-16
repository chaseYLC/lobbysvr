#pragma once

#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/bind.hpp>
#include <boost/chrono.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/pool/pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/unordered_map.hpp>
#include <memory>
#include <random>

#include <net/logger.h>


extern boost::shared_ptr< boost::asio::io_service > shared_ios;

extern uint16_t	g_serverIDX;
extern std::mt19937 *g_rand;
extern uint32_t g_idx4Broker;

typedef struct tagTSV_ID {
	enum {
		DB_ERROR = 1,

		REDIS_CLIENT = 2,
	};
}TSV_ID;

typedef struct tagAcceptorIDX {
	enum {
		BROKER = 0,
		LOBBY = 1,
		ROOM = 2,
	};
}ACCPETOR_IDX;


#define RAND_NO		((*g_rand)())


#define RSP_SEQ	"packetSequenceNum"
#define RSP_RC	"resultCode"
#define RSP_RM	"resultMsg"
#define RSP_OK	"OK"
#define RSP_RC_SYSTEM_ERROR		99


