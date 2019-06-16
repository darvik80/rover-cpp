//
// Created by Ivan Kishchenko on 2019-06-16.
//

#include "JsonBaseTest.h"
#include "json/JsonBase.h"
#include <cppunit/extensions/HelperMacros.h>

BEGIN_DECLARE_DTO(Message)

__DECLARE_DTO_FIELD(int, id)
__DECLARE_DTO_FIELD(std::string, message)

    BEGIN_JSON_UNMARSHAL
            ITEM_JSON_UNMARSHAL(id)

            ITEM_JSON_UNMARSHAL_TAG(message, message
            )
    END_JSON_UNMARSHAL

    BEGIN_JSON_MARSHAL
            ITEM_JSON_MARSHAL(id)
            ITEM_JSON_MARSHAL_TAG(message, message
            )
    END_JSON_MARSHAL

END_DECLARE_DTO


void JsonBaseTest::testMarshal() {
    Message message;
    message.id = 10;
    message.message = "Hello";

    auto res = message.marshal();
    const auto& ptr = res.extract<Poco::JSON::Object>();
    CPPUNIT_ASSERT_MESSAGE("id", ptr.get("id"));
    CPPUNIT_ASSERT_MESSAGE("id", ptr.get("id").isInteger());
    CPPUNIT_ASSERT_EQUAL(message.id, ptr.get("id").convert<int >());

    CPPUNIT_ASSERT_MESSAGE("message", ptr.get("message"));
    CPPUNIT_ASSERT_MESSAGE("message", ptr.get("message").isString());
    CPPUNIT_ASSERT_EQUAL(message.message, ptr.get("message").convert<std::string>());
}

void JsonBaseTest::testUnMarshal(){
    Poco::JSON::Object::Ptr ptr(new Poco::JSON::Object());
    ptr->set("id", 20);
    ptr->set("message", "hello");

    Message message;
    message.unMarshal(ptr);

    CPPUNIT_ASSERT_EQUAL(20, message.id);
    CPPUNIT_ASSERT_EQUAL(std::string("hello"), message.message);
}


CPPUNIT_TEST_SUITE_REGISTRATION(JsonBaseTest);