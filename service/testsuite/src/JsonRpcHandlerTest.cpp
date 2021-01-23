//
// Created by Ivan Kishchenko on 2019-06-16.
//

#include "JsonRpcHandlerTest.h"

#include <rpc/json/Helper.h>
#include <rpc/RpcMethod.h>
#include <rpc/json/Spec.h>

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

JsonRpcHandlerFixture::JsonRpcHandlerFixture() {
    _rpcHandler.addMethod(std::make_shared<HelloFunction>());
    _rpcHandler.addMethod(std::make_shared<HelloSupplier>());
    _rpcHandler.addMethod(std::make_shared<HelloConsumer>());
}

BOOST_FIXTURE_TEST_SUITE(JsonRpcHandlerTest, JsonRpcHandlerFixture)

    BOOST_AUTO_TEST_CASE(testHandleFunction) {
        JsonRpcRequest request;
        request.id = 1;
        request.method = "hello.function";
        json::tree params;
        params["name"] = "Ivan";
        request.params = params;
        request.jsonrpc = "2.0";

        JsonRpcResponse response;

        _rpcHandler.handle(request, response);

        BOOST_REQUIRE_EQUAL(std::string("2.0"), response.jsonrpc);
        BOOST_REQUIRE_EQUAL(1, response.id.value());
        BOOST_REQUIRE_EQUAL(false, response.error.is_initialized());
        BOOST_REQUIRE_EQUAL(true, response.result.is_initialized());
    }

    BOOST_AUTO_TEST_CASE(testHandleSupplier) {
        JsonRpcRequest request;
        request.id = 1;
        request.method = "hello.supplier";
        request.jsonrpc = "2.0";

        JsonRpcResponse response;

        _rpcHandler.handle(request, response);

        BOOST_REQUIRE_EQUAL(std::string("2.0"), response.jsonrpc);
        BOOST_REQUIRE_EQUAL(1, response.id.value());
        BOOST_REQUIRE_EQUAL(false, response.error.is_initialized());
        BOOST_REQUIRE_EQUAL(true, response.result.is_initialized());
    }

    BOOST_AUTO_TEST_CASE(testHandleConsumer) {
        JsonRpcRequest request;
        request.id = 1;
        request.method = "hello.consumer";
        request.jsonrpc = "2.0";

        JsonRpcResponse response;

        _rpcHandler.handle(request, response);

        BOOST_REQUIRE_EQUAL(std::string("2.0"), response.jsonrpc);
        BOOST_REQUIRE_EQUAL(1, response.id.value());
        BOOST_REQUIRE_EQUAL(false, response.error.is_initialized());
        BOOST_REQUIRE_EQUAL(false, response.result.is_initialized());
    }

    BOOST_AUTO_TEST_CASE(testHandleMethodNotExists) {
        JsonRpcRequest request;
        request.id = 1;
        request.method = "hello.failed";
        request.jsonrpc = "2.0";

        JsonRpcResponse response;

        _rpcHandler.handle(request, response);

        BOOST_REQUIRE_EQUAL(std::string("2.0"), response.jsonrpc);
        BOOST_REQUIRE_EQUAL(1, response.id.value());
        BOOST_REQUIRE_EQUAL(true, response.error.is_initialized());
        BOOST_REQUIRE_EQUAL(-32601, response.error.value().code);
        BOOST_REQUIRE_EQUAL(false, response.result.is_initialized());
    }

BOOST_AUTO_TEST_SUITE_END()
