//
// Created by Ivan Kishchenko on 01.08.2021.
//

#include <logging/Logging.h>
#include "MqttLogger.h"
#include "EMLogger.h"

int main(int argc, char *argv[]) {
    log::LoggingProperties logProps;
    logProps.level="info";
    log::setup(logProps);

    log::info("Info");
    log::warning("Warning");
    //log::error("Error");

    mqtt::log::info("mqtt::info");
    mqtt::log::warning("mqtt::warn");

    em::log::info("em::info");
    em::log::warning("em::warn");

    return 0;
}