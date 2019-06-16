//
// Created by Ivan Kishchenko on 2019-06-16.
//

#include "HandlerJsonRpcTest.h"

#include <json/JsonBase.h>
#include <rpc/RpcMethod.h>
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

void HandlerJsonRpcTest::testHandleFunction() {
    JsonRpcRequest request;
    request.id = "1";
    request.method = "hello.function";
    auto params =  Poco::JSON::Object::Ptr(new Poco::JSON::Object());
    params->set("name", "Ivan");
    request.params = params;
    request.jsonrpc = "2.0";

    JsonRpcResponse response;

    handler.handle(request, response);

    CPPUNIT_ASSERT_EQUAL(std::string("2.0"), response.jsonrpc);
    CPPUNIT_ASSERT_EQUAL(std::string("1"), response.id.value());
    CPPUNIT_ASSERT_EQUAL(false, response.error.isSpecified());
    CPPUNIT_ASSERT_EQUAL(true, response.result.isSpecified());
}

void HandlerJsonRpcTest::testHandleSupplier() {
    JsonRpcRequest request;
    request.id = "1";
    request.method = "hello.supplier";
    request.jsonrpc = "2.0";

    JsonRpcResponse response;

    handler.handle(request, response);

    CPPUNIT_ASSERT_EQUAL(std::string("2.0"), response.jsonrpc);
    CPPUNIT_ASSERT_EQUAL(std::string("1"), response.id.value());
    CPPUNIT_ASSERT_EQUAL(false, response.error.isSpecified());
    CPPUNIT_ASSERT_EQUAL(true, response.result.isSpecified());
}

void HandlerJsonRpcTest::testHandleConsumer() {
    JsonRpcRequest request;
    request.id = "1";
    request.method = "hello.consumer";
    request.jsonrpc = "2.0";

    JsonRpcResponse response;

    handler.handle(request, response);

    CPPUNIT_ASSERT_EQUAL(std::string("2.0"), response.jsonrpc);
    CPPUNIT_ASSERT_EQUAL(std::string("1"), response.id.value());
    CPPUNIT_ASSERT_EQUAL(false, response.error.isSpecified());
    CPPUNIT_ASSERT_EQUAL(false, response.result.isSpecified());
}

void HandlerJsonRpcTest::testHandleMethodNotExists() {
    JsonRpcRequest request;
    request.id = "1";
    request.method = "hello.failed";
    request.jsonrpc = "2.0";

    JsonRpcResponse response;

    handler.handle(request, response);

    CPPUNIT_ASSERT_EQUAL(std::string("2.0"), response.jsonrpc);
    CPPUNIT_ASSERT_EQUAL(std::string("1"), response.id.value());
    CPPUNIT_ASSERT_EQUAL(true, response.error.isSpecified());
    CPPUNIT_ASSERT_EQUAL(-32601, response.error.value().code);
    CPPUNIT_ASSERT_EQUAL(false, response.result.isSpecified());
}

void HandlerJsonRpcTest::setUp() {
    TestFixture::setUp();

    RpcRegistry::instance().addMethod(new HelloFunction());
    RpcRegistry::instance().addMethod(new HelloSupplier());
    RpcRegistry::instance().addMethod(new HelloConsumer());
}
