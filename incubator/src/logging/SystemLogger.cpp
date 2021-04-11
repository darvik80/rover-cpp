//
// Created by Ivan Kishchenko on 11.04.2021.
//

#include "SystemLogger.h"

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/formatting_ostream.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/stacktrace.hpp>

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp,
                            "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(threadId,
                            "ThreadID", log::attributes::current_thread_id::value_type)
auto static date_time_formatter = log::expressions::stream << log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");

void consoleFormatter(log::record_view const &rec, log::formatting_ostream &strm) {

    strm << "\033[38;5;15m[";
    date_time_formatter(rec, strm);
    strm << "] [";

    auto severity = rec[log::trivial::severity];
    if (severity) {
        // Set the color
        switch (severity.get()) {
            case log::trivial::severity_level::debug:
                strm << "\033[1;37m";
                break;

            case log::trivial::severity_level::info:
                strm << "\033[1;32m";
                break;
            case log::trivial::severity_level::warning:
                strm << "\033[1;33m";
                break;
            case log::trivial::severity_level::error:
            case log::trivial::severity_level::fatal:
                strm << "\033[1;31m";
                break;
            default:
                break;
        }
    }

    strm << std::setw(7) << std::right << severity << "\033[38;5;15m] [";
    strm << "\033[36m " << rec[threadId] << "\033[38;5;15m ] : ";
    strm << "\033[38;5;250m" << rec[log::expressions::smessage] << "\033[38;5;1m";

    switch (severity.get()) {
        case log::trivial::severity_level::error:
        case log::trivial::severity_level::fatal:
            strm << std::endl << "\033[31m" << stacktrace::stacktrace() << "\033[0m";
        default:
            break;
    }
    strm << "\033[0m";
}

void fileFormatter(log::record_view const &rec, log::formatting_ostream &strm) {
    strm << "[";
    date_time_formatter(rec, strm);
    strm << "] [";

    strm << std::setw(7) << std::right << rec[log::trivial::severity] << "] ";
    strm << "[ " << rec[threadId] << " ] : " << rec[log::expressions::smessage];
}

void SystemLogger::trace(std::string_view message) {

}

void SystemLogger::debug(std::string_view message) {

}

void SystemLogger::info(std::string_view message) {

}

void SystemLogger::warning(std::string_view message) {

}

void SystemLogger::error(std::string_view message) {

}

void SystemLogger::fatal(std::string_view message) {

}
