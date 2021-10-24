//
// Created by Ivan Kishchenko on 24.10.2021.
//

#include "Logging.h"

namespace logging {
    void addLogger(Logger *log) {
        CompositeLogger::instance().addLogger(log);
    }
}