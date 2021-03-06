#include "SubsystemBase.h"

Logger::Ptr SubsystemBase::devNullLogger = std::make_shared<DevNullLogger>();

std::string SubsystemBase::message(const std::string_view msg) {
    std::stringstream str;
    str << "[sub][" << name() << "] " << msg;

    return str.str();
}

void SubsystemBase::trace(std::string_view msg) {
    _logger->trace(message(msg));
}

void SubsystemBase::debug(std::string_view msg) {
    _logger->debug(message(msg));
}

void SubsystemBase::info(std::string_view msg) {
    _logger->info(message(msg));
}

void SubsystemBase::warning(std::string_view msg) {
    _logger->warning(message(msg));
}

void SubsystemBase::error(std::string_view msg) {
    _logger->error(message(msg));
}

void SubsystemBase::fatal(std::string_view msg) {
    _logger->fatal(message(msg));
}

void SubsystemBase::postConstruct(Application &app) {
    if (!app.loggerPtr()) {
        throw std::invalid_argument("Logger cannot be nullptr");
    }
    _logger = app.loggerPtr();
}

void SubsystemBase::preDestroy() {

}