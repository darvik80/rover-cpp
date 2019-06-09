//
// Created by Ivan Kishchenko on 2019-06-08.
//

#include "JsonBase.h"
#include <Poco/JSON/Parser.h>
#include <Poco/Foundation.h>

using namespace Poco;
using namespace Poco::JSON;

void JsonDecoder::decode(std::istream& is) {
    // Parse the JSON
    Poco::JSON::Parser jsonParser;
    auto parsedJSON = jsonParser.parse(is);
    auto parsedResult = jsonParser.result();

    _unMarshaller->unMarshal(parsedResult);
}

std::string JsonEncoder::encode() {
    auto data = _marshaller->marshal();

    std::ostringstream oss;
    Poco::JSON::Stringifier::stringify(data, oss);

    return oss.str();
}
