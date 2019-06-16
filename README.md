# Rover 

Service based on POCO C++ Libraries (https://pocoproject.org/)

* JSON-RPC Server (POCO::Json, C++ 17)
* macchina.io like IoT (https://macchina.io/) 

## build
```bash
mkdir -p build && cd build
cmake ../
make all CTEST_OUTPUT_ON_FAILURE=1 test

```

## json-rpc

### dto examples:
```c++
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

```

```c++
/**
 * hello params
 */
BEGIN_DECLARE_DTO(HelloParams)

__DECLARE_DTO_FIELD(std::string, name)
END_DECLARE_DTO

/**
 * hello response
 */
BEGIN_DECLARE_DTO(HelloResult)

__DECLARE_DTO_FIELD(std::string, message)
END_DECLARE_DTO

```

### methods examples

```c++

class HelloFunction : public RpcFunction<HelloParams, HelloResult> {
public:
    std::string name() const override {
        return "hello.function";
    }

    HelloResult exec(const HelloParams &params) override {
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

    HelloResult exec() override {
        HelloResult result;
        result.message = "Hello World!";

        return result;
    }
};

class HelloConsumer : public RpcConsumer<HelloParams> {
public:
    std::string name() const override {
        return "hello.consumer";
    }

    void exec(const HelloParams &params) override {
        // TODO: make something
    }
};
```
