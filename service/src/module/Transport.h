//
// Created by Kishchenko, Ivan on 3/26/21.
//

#pragma once

#include <system_error>
#include <memory>

#include <future>

class SocketHandler {
public:
    typedef std::shared_ptr<SocketHandler> Ptr;

    virtual ~SocketHandler() = default;
};

struct Packet {
    SocketHandler::Ptr handler;
    const uint8_t *data{nullptr};
    size_t size{0};
};

typedef std::function<void(const Packet &packet)> HandlerFunction;
typedef std::future<boost::system::error_code> FutureError;
typedef std::promise<boost::system::error_code> PromiseError;

class Transport {
public:
    typedef std::shared_ptr<Transport> Ptr;
    typedef std::weak_ptr<Transport> WeakPtr;
public:

    virtual void onPacket(const HandlerFunction &handler) = 0;

    virtual FutureError send(const SocketHandler::Ptr& handler, const uint8_t* data, size_t size) = 0;
    virtual FutureError send(const SocketHandler::Ptr& handler, std::string_view data) {
        return send(handler, (uint8_t*)data.data(), data.size());
    }
};

class TransportFactory {
public:
    virtual Transport::Ptr create() = 0;
};

