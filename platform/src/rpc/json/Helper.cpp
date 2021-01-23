//
// Created by Ivan Kishchenko on 2019-07-13.
//

#include "Helper.h"
#include <utility>

#include <boost/property_tree/json_parser.hpp>

std::string JsonEncoder::encode() {
    return _marshaller->marshal().dump();
}

void JsonDecoder::decode(const std::string &json) {
    _unMarshaller->unMarshal(nlohmann::json::parse(json.begin(), json.end()));
}
