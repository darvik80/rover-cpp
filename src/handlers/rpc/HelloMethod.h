//
// Created by Ivan Kishchenko on 2019-06-09.
//

#ifndef ROVER_HELLOMETHOD_H
#define ROVER_HELLOMETHOD_H

#include "codec/json/JsonBase.h"
#include "MethodRpc.h"


/**
 * hello params
 */
BEGIN_DECLARE_DTO(HelloParams)
__DECLARE_DTO_FIELD(std::string, name)

    BEGIN_JSON_MARSHAL
    END_JSON_MARSHAL

    BEGIN_JSON_UNMARSHAL
            ITEM_JSON_UNMARSHAL(name)
    END_JSON_UNMARSHAL

END_DECLARE_DTO

/**
 * hello response
 */
BEGIN_DECLARE_DTO(HelloResult)
__DECLARE_DTO_FIELD(std::string, message)

    BEGIN_JSON_MARSHAL
            ITEM_JSON_MARSHAL(message)
    END_JSON_MARSHAL

    BEGIN_JSON_UNMARSHAL
    END_JSON_UNMARSHAL

END_DECLARE_DTO

class HelloMethod : public BaseMethodRpc<HelloParams, HelloResult> {
public:
    std::shared_ptr<HelloResult> exec(const HelloParams* params) override {
        auto result = std::make_shared<HelloResult>();
        result->message = "Hello, " + params->name;

        return result;
    }
};



#endif //ROVER_HELLOMETHOD_H
