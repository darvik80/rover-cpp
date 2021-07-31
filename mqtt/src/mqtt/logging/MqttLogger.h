//
// Created by Kishchenko, Ivan on 10/2/20.
//

#ifndef MQTT_LOGGER_H
#define MQTT_LOGGER_H

#include <fmt/ranges.h>
#include <fmt/core.h>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>

#include <boost/log/sources/global_logger_storage.hpp>

namespace mqtt::log {
#define DECLARE_MQTT_LEVEL(LEVEL) \
    template <class... A> \
    static void LEVEL(const std::string& fmt, A&&... args) { \
        BOOST_LOG_TRIVIAL(LEVEL) << "[mqtt] " << fmt::format(fmt, std::forward<A>(args)...); \
}

    DECLARE_MQTT_LEVEL(trace)

    DECLARE_MQTT_LEVEL(debug)

    DECLARE_MQTT_LEVEL(info)

    DECLARE_MQTT_LEVEL(warning)

    DECLARE_MQTT_LEVEL(error)

    DECLARE_MQTT_LEVEL(fatal)
}

#endif //MQTT_LOGGER_H
