#include <utility>

#include <utility>

#include <utility>

//
// Created by Ivan Kishchenko on 2019-07-13.
//

#include "Helper.h"

#include <boost/property_tree/json_parser.hpp>

std::string JsonEncoder::encode() {
    auto raw = _marshaller->marshal();

    std::ostringstream os;
    boost::property_tree::write_json(os, raw, true);

    return os.str();
}

void JsonDecoder::decode(const std::string &json) {
    tree doc;

    std::istringstream is(json);
    boost::property_tree::read_json(is, doc);

    _unMarshaller->unMarshal(doc);
}
