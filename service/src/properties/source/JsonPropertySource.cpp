//
// Created by Ivan Kishchenko on 11.04.2021.
//

#include "JsonPropertySource.h"

using namespace nlohmann;

JsonPropertySource::JsonPropertySource(std::string_view source) {
    _json = json::parse(source);
}

void JsonPropertySource::getProperties(NetworkProperties &props) {
    if (auto it = _json.find("network"); it != _json.end()) {
        if (auto key = it->find("registry_host"); key != it->end()) {
            key->get_to(props.registryHost);
        }
        if (auto key = it->find("registry_port"); key != it->end()) {
            key->get_to(props.registryPort);
        }
    } else {
        throw std::invalid_argument(std::string("The properties has not been found ") + typeid(props).name());
    }
}

void JsonPropertySource::getProperties(LoggingProperties &props) {
    if (auto it = _json.find("logging"); it != _json.end()) {
        if (auto key = it->find("level"); key != it->end()) {
            key->get_to(props.level);
        }
        if (auto key = it->find("console"); key != it->end()) {
            key->get_to(props.console);
        }
        if (auto key = it->find("file"); key != it->end()) {
            key->get_to(props.file);
        }
    }
}

void JsonPropertySource::getProperties(HttpProperties &props) {
    if (auto it = _json.find("http"); it != _json.end()) {
        if (auto key = it->find("host"); key != it->end()) {
            key->get_to(props.host);
        }
        if (auto key = it->find("port"); key != it->end()) {
            key->get_to(props.port);
        }
        if (auto key = it->find("root"); key != it->end()) {
            std::string root;
            key->get_to(root);
            props.root = root;
        }
    }
}

void JsonPropertySource::getProperties(GrpcProperties &props) {
    if (auto it = _json.find("grpc"); it != _json.end()) {
        if (auto key = it->find("host"); key != it->end()) {
            key->get_to(props.host);
        }
        if (auto key = it->find("port"); key != it->end()) {
            key->get_to(props.port);
        }
    }
}

void JsonPropertySource::getProperties(SerialProperties &props) {
    if (auto it = _json.find("serial"); it != _json.end()) {
        if (auto key = it->find("port"); key != it->end()) {
            key->get_to(props.port);
        }
        if (auto key = it->find("baud-rate"); key != it->end()) {
            key->get_to(props.baudRate);
        }
    }
}
