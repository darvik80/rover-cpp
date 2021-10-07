//
// Created by Kishchenko, Ivan on 3/25/21.
//

#pragma once

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <utility>
#include <module/Transport.h>
#include <Application.h>

typedef boost::asio::ip::udp::socket UdpSocket;
typedef boost::asio::ip::udp::endpoint UdpEndpoint;

class UdpSocketHandler : public SocketHandler {
private:
    UdpEndpoint _endpoint;
public:
    explicit UdpSocketHandler(const UdpEndpoint &endpoint)
            : _endpoint(endpoint) {
    }

    UdpSocketHandler(const UdpSocketHandler &handler)
            : _endpoint(handler._endpoint) {

    }

    UdpEndpoint &getEndpoint() {
        return _endpoint;
    }
};

class AsyncUdp : public Transport {
public:
    explicit AsyncUdp(boost::asio::io_service &service);

    void listen(uint16_t port);

    SocketHandler::Ptr connect(std::string_view host, uint16_t port);

    SocketHandler::Ptr broadcast(std::string_view host, uint16_t port);

    void onPacket(const HandlerFunction &handler) override {
        _handler = handler;
    }

    FutureError send(const SocketHandler::Ptr &handler, const uint8_t *data, size_t size) override;

private:
    UdpSocket _socket;
    UdpEndpoint _endpoint;

    HandlerFunction _handler;
    std::array<uint8_t, 2048> _recvBuffer{};
private:
    void startReceive();

    void handleReceive(
            const UdpEndpoint &endpoint,
            const boost::system::error_code &error,
            const uint8_t *data,
            size_t size
    );

    void handleSend(const boost::system::error_code &error, size_t size);
};

