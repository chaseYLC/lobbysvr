#pragma once

//#define BOOST_LOG_DYN_LINK // necessary when linking the boost_log library dynamically

#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

// the logs are also written to LOGFILE
#define LOGFILE "logfile.log"

// just log messages with severity >= SEVERITY_THRESHOLD are written
#define SEVERITY_THRESHOLD logging::trivial::debug

// register a global logger
BOOST_LOG_GLOBAL_LOGGER(logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)

// just a helper macro used by the macros below - don't use it in your code
#define LOG(severity) BOOST_LOG_SEV(logger::get(),boost::log::trivial::severity)

// ===== log macros =====
#define LOGT    LOG(trace)
#define LOGD    LOG(debug)
#define LOGI    LOG(info)
#define LOGW    LOG(warning)
#define LOGE    LOG(error)
#define LOGF    LOG(fatal)
