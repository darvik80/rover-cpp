//
// Created by Ivan Kishchenko on 2019-06-16.
//

#include "JsonBase.h"

void JsonDecoder::decode(std::istream &is) {
    // Parse the JSON
    Poco::JSON::Parser jsonParser;
    auto parsedJSON = jsonParser.parse(is);
    auto parsedResult = jsonParser.result();

    _unMarshaller->
            unMarshal(parsedResult);
}


std::string JsonEncoder::encode() {
    auto data = _marshaller->marshal();

    std::ostringstream oss;
    Poco::JSON::Stringifier::stringify(data, oss);

    return oss.str();
}