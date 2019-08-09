//
// Created by David Oberacker on 2019-08-02.
//

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#define BOOST_LOG_DYN_LINK 1

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks.hpp>

#include <boost/log/expressions.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/attr_fwd.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

// Supporting headers
#include <boost/log/support/date_time.hpp>


#include <borderlands2/borderlands2.hpp>
#include <borderlands2/borderlands2_logger.hpp>

// The operator is used for regular stream formatting
    std::ostream& operator<< (std::ostream& strm, D4v3::Borderlands2::severity_level level)
    {
        static const char* strings[] =
        {
            "debug",
            "info",
            "warning",
            "error"
        };

        if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
            strm << strings[level];
        else
            strm << static_cast< int >(level);

        return strm;
    }

    // Attribute value tag type
    struct severity_tag;

    // The operator is used when putting the severity level to log
    boost::log::formatting_ostream& operator<<
    (
        boost::log::formatting_ostream& strm,
        boost::log::to_log_manip< D4v3::Borderlands2::severity_level, severity_tag > const& manip
    )
    {
        static const char* strings[] =
        {
            "DBG",
            "INFO",
            "WARN",
            "ERRR"
        };

        D4v3::Borderlands2::severity_level level = manip.get();
        if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
            strm << strings[level];
        else
            strm << static_cast< int >(level);

        return strm;
    }

class Borderlands2Test : public ::testing::Test {
    public:
    
    Borderlands2Test() {
        // You can do set-up work for each test here.

        boost::log::add_common_attributes();

        auto file_sink = boost::log::add_file_log
        (
            boost::log::keywords::file_name = "borderlands2_test_%N.log",
            boost::log::keywords::rotation_size = 10 * 1024 * 1024,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
            boost::log::keywords::format = (
                boost::log::expressions::stream
                << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << " - "
                << "[ " << boost::log::expressions::attr< unsigned int >("LineID") << " ]"
                << "[ " << boost::log::expressions::attr< D4v3::Borderlands2::severity_level, severity_tag >("Severity") << " ]"
                << "[ " << boost::log::expressions::attr< std::string >("Channel") << " ]"
                << " - " << boost::log::expressions::smessage
            )
        );

        boost::log::add_console_log(
            std::clog,
            boost::log::keywords::format = (
                boost::log::expressions::stream
                << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << " - "
                << "[ " << boost::log::expressions::attr< unsigned int >("LineID") << " ]"
                << "[ " << boost::log::expressions::attr< D4v3::Borderlands2::severity_level, severity_tag >("Severity") << " ]"
                << "[ " << boost::log::expressions::attr< std::string >("Channel") << " ]"
                << " - " << boost::log::expressions::smessage
            )
        );
    }

    ~Borderlands2Test() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

TEST_F(Borderlands2Test, VerifySaveFile) {
    EXPECT_TRUE(verifySave("..\\..\\resources\\76561198034853688\\Save0001.sav"));
}

TEST_F(Borderlands2Test, InvalidPath) {
    EXPECT_FALSE(verifySave("../../../resources/76561198034853688/Save0001.sav"));
    EXPECT_FALSE(verifySave("..//../resources/76561198034853688/Save0001.sav"));
}
