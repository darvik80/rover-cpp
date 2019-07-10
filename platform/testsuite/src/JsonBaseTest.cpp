//
// Created by Ivan Kishchenko on 2019-06-16.
//

#include "json/JsonBase.h"

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

#define BOOST_TEST_MODULE Platform

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(JsonBaseTest)

    BOOST_AUTO_TEST_CASE(testMarshal) {
        Message message;
        message.id = 10;
        message.message = "Hello";

        auto res = message.marshal();
        const auto &ptr = res.extract<Poco::JSON::Object::Ptr>();
        BOOST_ASSERT_MSG("id", ptr->get("id"));
        BOOST_ASSERT_MSG("id", ptr->get("id").isInteger());
        BOOST_REQUIRE_EQUAL(message.id, ptr->get("id").convert<int>());

        BOOST_ASSERT_MSG("message", ptr->get("message"));
        BOOST_ASSERT_MSG("message", ptr->get("message").isString());
        BOOST_REQUIRE_EQUAL(message.message, ptr->get("message").convert<std::string>());
    }

    BOOST_AUTO_TEST_CASE(testUnMarshal) {
        Poco::JSON::Object::Ptr ptr(new Poco::JSON::Object());
        ptr->set("id", 20);
        ptr->set("message", "hello");

        Message message;
        message.unMarshal(ptr);

        BOOST_REQUIRE_EQUAL(20, message.id);
        BOOST_REQUIRE_EQUAL(std::string("hello"), message.message);
    }

BOOST_AUTO_TEST_SUITE_END()