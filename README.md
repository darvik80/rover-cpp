# Rover 

Service based on Boost C++ Libraries (https://boost.org/)

* JSON-RPC Server (nlohmann/json, Boost.Beast, C++ 17)
* Logging (Boost.Log)
* platform.io like IoT (https://https://platformio.org//) 

## deps
```bash
sudo apt install libssl-dev libfmt-dev libboost-all-dev nlohmann-json3-dev cmake clang git mc htop xboxdrv
```

## build
```bash
git submodule init
git submodule update
mkdir build && cd build && cmake ../ && make && make install
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

# Knowledge issues base

### Segmentation fault on debug in Orange PI
```
Program received signal SIGSEGV, Segmentation fault.
0x76fd9822 in ?? () from /lib/ld-linux-armhf.so.3
```
https://bugs.launchpad.net/gdb/+bug/1576432

### Error during pretty printers setup: Error while executing Python code
https://youtrack.jetbrains.com/issue/CPP-14177

[OdroidC2](OdroidC2.md)

### Logs

ANSI escape code: [link](https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit)

### FAQ

* *bash: warning: setlocale: LC_ALL: cannot change locale (en-US.UTF-8).*
```shell
sudo apt-get install locale-all
sudo dpkg-reconfigure locales
sudo locale-gen.
```