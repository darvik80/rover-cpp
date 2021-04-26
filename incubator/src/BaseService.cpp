//
// Created by Ivan Kishchenko on 24.04.2021.
//

#include "BaseService.h"
#include "Registry.h"

void BaseService::postConstruct(Registry &registry) {
    _log = registry.getLogger();
    debug("postConstruct");
}

void BaseService::preDestroy(Registry& registry) {
    debug("preDestroy");
}

void BaseService::trace(std::string_view message) {
    std::stringstream str;
    str << "[" << name() << "] " << message;
    _log->trace(str.str());
}

void BaseService::debug(std::string_view message) {
    std::stringstream str;
    str << "[" << name() << "] " << message;
    _log->debug(str.str());
}

void BaseService::info(std::string_view message) {
    std::stringstream str;
    str << "[" << name() << "] " << message;
    _log->info(str.str());
}

void BaseService::warning(std::string_view message) {
    std::stringstream str;
    str << "[" << name() << "] " << message;
    _log->warning(str.str());
}

void BaseService::error(std::string_view message) {
    std::stringstream str;
    str << "[" << name() << "] " << message;
    _log->error(str.str());
}

void BaseService::fatal(std::string_view message) {
    std::stringstream str;
    str << "[" << name() << "] " << message;
    _log->fatal(str.str());
}
