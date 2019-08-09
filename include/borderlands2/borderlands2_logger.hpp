
#pragma once

#include <boost/thread/shared_mutex.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

namespace D4v3 {
    namespace Borderlands2 {
        enum severity_level
        {
            debug,
            info,
            warning,
            error
        };

        
        typedef boost::log::sources::severity_channel_logger_mt<
            severity_level,     // the type of the severity level
            std::string         // the type of the channel name
        > borderlands2_logger_mt;

        BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(borderlands2_logger, D4v3::Borderlands2::borderlands2_logger_mt)
        {
            return D4v3::Borderlands2::borderlands2_logger_mt(boost::log::keywords::channel = "borderlands2");
        }
    }
}
