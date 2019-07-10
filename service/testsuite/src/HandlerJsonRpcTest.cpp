//
// Created by Ivan Kishchenko on 2019-06-16.
//

#include "HandlerJsonRpcTest.h"

#include <json/JsonBase.h>
#include <rpc/RpcMethod.h>
#include <rpc/RpcRegistry.h>

#include "handlers/JsonRpcHandler.h"

#include <rpc/RpcRegistry.h>

/**
 * hello params
 */
BEGIN_DECLARE_DTO(HelloParams)

__DECLARE_DTO_FIELDS1(std::string, name)
END_DECLARE_DTO

/**
 * hello response
 */
BEGIN_DECLARE_DTO(HelloResult)

__DECLARE_DTO_FIELDS1(std::string, message)
END_DECLARE_DTO

class HelloFunction : public RpcFunction<HelloParams, HelloResult> {
public:
    std::string name() const override {
        return "hello.function";
    }

    HelloResult exec(const HelloParams &params) const override {
        HelloResult result;
        result.message = "Hello, " + params.name;

        return result;
    }
};

class HelloSupplier : public RpcSupplier<HelloResult> {
public:
    std::string name() const override {
        return "hello.supplier";
    }

    HelloResult exec() const override {
        HelloResult result;
        result.message = "Hello, World";

        return result;
    }
};

class HelloConsumer : public RpcConsumer<HelloParams> {
public:
    std::string name() const override {
        return "hello.consumer";
    }

    void exec(const HelloParams &params) const override {

    }
};

HandlerJsonRpcFixture::HandlerJsonRpcFixture() {
    RpcRegistry::instance().addMethod(new HelloFunction());
    RpcRegistry::instance().addMethod(new HelloSupplier());
    RpcRegistry::instance().addMethod(new HelloConsumer());
}

BOOST_FIXTURE_TEST_SUITE(HandlerJsonRpcTest, HandlerJsonRpcFixture)

    BOOST_AUTO_TEST_CASE(testHandleFunction) {
        JsonRpcRequest request;
        request.id = "1";
        request.method = "hello.function";
        auto params =  Poco::JSON::Object::Ptr(new Poco::JSON::Object());
        params->set("name", "Ivan");
        request.params = params;
        request.jsonrpc = "2.0";

        JsonRpcResponse response;

        handler.handle(request, response);

        BOOST_REQUIRE_EQUAL(std::string("2.0"), response.jsonrpc);
        BOOST_REQUIRE_EQUAL(std::string("1"), response.id.value());
        BOOST_REQUIRE_EQUAL(false, response.error.isSpecified());
        BOOST_REQUIRE_EQUAL(true, response.result.isSpecified());
    }

    BOOST_AUTO_TEST_CASE(testHandleSupplier) {
        JsonRpcRequest request;
        request.id = "1";
        request.method = "hello.supplier";
        request.jsonrpc = "2.0";

        JsonRpcResponse response;

        handler.handle(request, response);

        BOOST_REQUIRE_EQUAL(std::string("2.0"), response.jsonrpc);
        BOOST_REQUIRE_EQUAL(std::string("1"), response.id.value());
        BOOST_REQUIRE_EQUAL(false, response.error.isSpecified());
        BOOST_REQUIRE_EQUAL(true, response.result.isSpecified());
    }

    BOOST_AUTO_TEST_CASE(testHandleConsumer) {
        JsonRpcRequest request;
        request.id = "1";
        request.method = "hello.consumer";
        request.jsonrpc = "2.0";

        JsonRpcResponse response;

        handler.handle(request, response);

        BOOST_REQUIRE_EQUAL(std::string("2.0"), response.jsonrpc);
        BOOST_REQUIRE_EQUAL(std::string("1"), response.id.value());
        BOOST_REQUIRE_EQUAL(false, response.error.isSpecified());
        BOOST_REQUIRE_EQUAL(false, response.result.isSpecified());
    }

    BOOST_AUTO_TEST_CASE(testHandleMethodNotExists) {
        JsonRpcRequest request;
        request.id = "1";
        request.method = "hello.failed";
        request.jsonrpc = "2.0";

        JsonRpcResponse response;

        handler.handle(request, response);

        BOOST_REQUIRE_EQUAL(std::string("2.0"), response.jsonrpc);
        BOOST_REQUIRE_EQUAL(std::string("1"), response.id.value());
        BOOST_REQUIRE_EQUAL(true, response.error.isSpecified());
        BOOST_REQUIRE_EQUAL(-32601, response.error.value().code);
        BOOST_REQUIRE_EQUAL(false, response.result.isSpecified());
    }

BOOST_AUTO_TEST_SUITE_END()
