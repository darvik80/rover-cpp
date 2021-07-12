//
// Created by Ivan Kishchenko on 11.04.2021.
//

#include "LoggingService.h"

#include "Registry.h"

#include <logging/Logging.h>

using namespace boost;

void LoggingService::postConstruct(Registry &registry) {
    auto props = registry.getProperties<LoggingProperties>();
    logging::setup(props);
}

void LoggingService::preDestroy(Registry& registry) {
}