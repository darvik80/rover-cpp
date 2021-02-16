//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#include "LoggerSubsystem.h"

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/formatting_ostream.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>

#include "subsystem/Application.h"
#include "const.h"

using namespace boost;

LoggerSubsystem::LoggerSubsystem() = default;

const char *LoggerSubsystem::name() const {
    return "logger";
}

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp,
                            "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(threadId,
                            "ThreadID", log::attributes::current_thread_id::value_type)
auto static date_time_formatter = log::expressions::stream << log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");

void consoleFormatter(log::record_view const &rec, log::formatting_ostream &strm) {

    strm << "[";
    date_time_formatter(rec, strm);
    strm << "]";

    auto severity = rec[log::trivial::severity];
    if (severity) {
        // Set the color
        switch (severity.get()) {
            case log::trivial::severity_level::debug:
                strm << "\033[37m";
                break;

            case log::trivial::severity_level::info:
                strm << "\033[32m";
                break;
            case log::trivial::severity_level::warning:
                strm << "\033[33m";
                break;
            case log::trivial::severity_level::error:
            case log::trivial::severity_level::fatal:
                strm << "\033[31m";
                break;
            default:
                break;
        }
    }

    strm << " [" << std::setw(7) << std::right << severity << "] ";
    strm << "\033[36m[ " << rec[threadId] << " ]";
    strm << "\033[0m : " << rec[log::expressions::smessage];
}

void fileFormatter(log::record_view const &rec, log::formatting_ostream &strm) {
    strm << "[";
    date_time_formatter(rec, strm);
    strm << "] [";

    strm << std::setw(7) << std::right << rec[log::trivial::severity] << "] ";
    strm << "[ " << rec[threadId] << " ] : " << rec[log::expressions::smessage];
}

void LoggerSubsystem::postConstruct(Application &app) {
    std::string filePath = app.getProperties()->getString(PROP_APPLICATION_LOG_DIR)
                           + app.getProperties()->getString(PROP_APPLICATION_NAME) + ".log";

    log::register_simple_formatter_factory<log::trivial::severity_level, char>("Severity");

    if (app.getProperties()->getBoolean(PROP_LOG_ENABLE_FILE, true)) {
        log::add_file_log(
                log::keywords::file_name = filePath,
                log::keywords::rotation_size = 10 * 1024 * 1024,
                log::keywords::time_based_rotation = log::sinks::file::rotation_at_time_point(0, 0, 0),
                log::keywords::auto_flush = true
        )->set_formatter(&fileFormatter);
    }

    if (app.getProperties()->getBoolean(PROP_LOG_ENABLE_CONSOLE, true)) {
        log::add_console_log(std::clog)->set_formatter(&consoleFormatter);
    }

    auto logLevel = app.getProperties()->getString(PROP_LOG_LEVEL, "debug");
    auto logLevelValue = log::trivial::trace;

    if (logLevel == "trace") {
        logLevelValue = log::trivial::trace;
    } else if (logLevel == "debug") {
        logLevelValue = log::trivial::debug;
    } else if (logLevel == "info") {
        logLevelValue = log::trivial::info;
    } else if (logLevel == "warning") {
        logLevelValue = log::trivial::warning;
    } else if (logLevel == "error") {
        logLevelValue = log::trivial::error;
    } else if (logLevel == "fatal") {
        logLevelValue = log::trivial::fatal;
    }

    log::core::get()->set_filter(log::trivial::severity >= logLevelValue);

    log::add_common_attributes();
}

void LoggerSubsystem::preDestroy() {

}

void LoggerSubsystem::trace(std::string_view message) {
    BOOST_LOG_TRIVIAL(trace) << message;
}

void LoggerSubsystem::debug(std::string_view message) {
    BOOST_LOG_TRIVIAL(debug) << message;
}

void LoggerSubsystem::info(std::string_view message) {
    BOOST_LOG_TRIVIAL(info) << message;
}

void LoggerSubsystem::warning(std::string_view message) {
    BOOST_LOG_TRIVIAL(warning) << message;
}

void LoggerSubsystem::error(std::string_view message) {
    BOOST_LOG_TRIVIAL(error) << message;
}

void LoggerSubsystem::fatal(std::string_view message) {
    BOOST_LOG_TRIVIAL(fatal) << message;
}
