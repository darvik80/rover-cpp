//
// Created by Ivan Kishchenko on 26.04.2021.
//

#ifndef ROVER_ENVPROPERTYSOURCE_H
#define ROVER_ENVPROPERTYSOURCE_H

#include "PropertySource.h"
#include <cstdlib>

#include <boost/lexical_cast.hpp>

// props: NetworkProperties
#define PROP_REGISTRY_PORT      "REGISTRY_PORT"         // NetworkProperties.registryPort
#define PROP_REGISTRY_HOST      "REGISTRY_HOST"         // NetworkProperties.registryHost

// props: LoggingProperties
#define PROP_LOGGING_LEVEL      "LOGGING_LEVEL"         // LoggingProperties.level
#define PROP_LOGGING_CONSOLE    "LOGGING_CONSOLE"       // LoggingProperties.console
#define PROP_LOGGING_FILE       "LOGGING_FILE"          // LoggingProperties.file

// props: HttpProperties
#define PROP_HTTP_HOST          "HTTP_HOST"             // HttpProperties.host
#define PROP_HTTP_PORT          "HTTP_PORT"             // HttpProperties.port
#define PROP_HTTP_ROOT          "HTTP_ROOT"             // HttpProperties.root

class EnvPropertySource : public PropertySource {
public:
    void getProperties(NetworkProperties &props) override {
    }

    void getProperties(LoggingProperties &props) override {
        if (auto val = getenv(PROP_LOGGING_LEVEL); val != nullptr) {
            props.level = val;
        }
        if (auto val = getenv(PROP_LOGGING_CONSOLE); val != nullptr) {
            props.console = boost::lexical_cast<bool>(val);
        }
        if (auto val = getenv(PROP_LOGGING_FILE); val != nullptr) {
            props.file = boost::lexical_cast<bool>(val);
        }
    }

    void getProperties(HttpProperties &props) override {
        if (auto val = getenv(PROP_HTTP_HOST); val != nullptr) {
            props.host = val;
        }
        if (auto val = getenv(PROP_HTTP_PORT); val != nullptr) {
            props.port = boost::lexical_cast<int>(val);;
        }
        if (auto val = getenv(PROP_HTTP_ROOT); val != nullptr) {
            props.root = val;
        }
    }
};


#endif //ROVER_ENVPROPERTYSOURCE_H
