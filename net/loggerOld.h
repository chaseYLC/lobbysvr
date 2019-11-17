#pragma once

#include <boost/log/sinks/sink.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/container/vector.hpp>
#include <string>
#include "singleton.h"

namespace MLN
{
	namespace Log
	{
		class Logger
			: public MLN::Net::Singleton<Logger>
		{
		public:
			enum severity_level{
				trace,
				debug,
				info,
				warning,
				error,
				fatal
			};

			enum backend_type
			{
				LOG_TYPE_NONE = 0,
				LOG_TYPE_CONSOL,
				LOG_TYPE_FILE,
				LOG_TYPE_TRACER,
			};

			typedef boost::log::sources::severity_logger_mt< MLN::Log::Logger::severity_level> mln_logger_type;

			struct LogConfig
			{
				std::string _channel = "default";
				bool _useLogConsole = true;
				bool _useLogFile = true;
				bool _useLogTrace = false;
				bool _useLogUDP = false;
				std::string _dirName = "log";
				std::string _fileName = "default";

				Logger::severity_level _level = Logger::severity_level::trace;

				uint32_t _rotation_time = std::numeric_limits<uint32_t>::digits;
				uint64_t _rotation_size = std::numeric_limits<uint64_t>::digits;
			};

		public:

			bool init(const std::string &path);
			void Dump(void* offset, size_t size, const char* dump_title = 0);

			//void LogT(const char *pFmt, ...);	// trace
			//void LogD(const char *pFmt, ...);	// debug
			//void LogI(const char *pFmt, ...);	// info
			//void LogW(const char *pFmt, ...);	// warning
			//void LogE(const char *pFmt, ...);	// error
			//void LogF(const char *pFmt, ...);	// fatal

			/*void Log(const severity_level level, const char *pFmt, ...);*/
			void Log_simple(const severity_level level, const char *text);

			/*void Log(const int err);*/

			Logger();
			~Logger();

		public:
			static mln_logger_type _logger;

		private:
			bool resetConfig(boost::property_tree::ptree &pt);
			bool resetConfig(const LogConfig& conf);

			bool InitBoostLog();
			void RemoveAllSinks();

		private:
			using SinkPtr = boost::shared_ptr< boost::log::sinks::sink >;
			boost::container::vector<SinkPtr> _sinks;
			
			LogConfig _config;
		};

		Logger::mln_logger_type Logger::_logger = Logger::mln_logger_type();

	};//namespace Log
};

#ifndef __LOG_TEMPLATE__

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

// #define __LOG_TEMPLATE__(_LEVEL) BOOST_LOG_SEV(boost::log::sources::severity_channel_logger_mt< MLN::Log::Logger::severity_level >(boost::log::keywords::channel = "default"), _LEVEL) << __FILE__ << "(" << __LINE__ << ") : "

#define __LOG_TEMPLATE__(_LEVEL) BOOST_LOG_SEV(MLN::Log::Logger::_logger, _LEVEL) << __FILE__ << "(" << __LINE__ << ") : "

// typedef boost::log::sources::severity_channel_logger_mt< MLN::Log::Logger::severity_level, std::string > mln_channel_logger_type;
// #define __LOG_TEMPLATE2__(_LEVEL) BOOST_LOG_CHANNEL_SEV(mln_channel_logger_type(), "default", _LEVEL) << __FILE__ << "(" << __LINE__ << ") : "

typedef boost::log::sources::severity_channel_logger_mt< MLN::Log::Logger::severity_level, std::string > mln_channel_logger_type;
#define __LOG_TEMPLATE2__(_LEVEL) BOOST_LOG_SEV(mln_channel_logger_type(), _LEVEL) << __FILE__ << "(" << __LINE__ << ") : "

#define LOGT __LOG_TEMPLATE__(MLN::Log::Logger::severity_level::trace)
#define LOGD __LOG_TEMPLATE__(MLN::Log::Logger::severity_level::debug)
#define LOGI __LOG_TEMPLATE__(MLN::Log::Logger::severity_level::info)
#define LOGW __LOG_TEMPLATE__(MLN::Log::Logger::severity_level::warning)
#define LOGE __LOG_TEMPLATE__(MLN::Log::Logger::severity_level::error)
#define LOGF __LOG_TEMPLATE__(MLN::Log::Logger::severity_level::fatal)

// #define LOGT __LOG_TEMPLATE2__(MLN::Log::Logger::severity_level::trace)
// #define LOGD __LOG_TEMPLATE2__(MLN::Log::Logger::severity_level::debug)
// #define LOGI __LOG_TEMPLATE2__(MLN::Log::Logger::severity_level::info)
// #define LOGW __LOG_TEMPLATE2__(MLN::Log::Logger::severity_level::warning)
// #define LOGE __LOG_TEMPLATE2__(MLN::Log::Logger::severity_level::error)
// #define LOGF __LOG_TEMPLATE2__(MLN::Log::Logger::severity_level::fatal)

#endif//__LOG_TEMPLATE__