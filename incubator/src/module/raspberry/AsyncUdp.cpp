//
// Created by Kishchenko, Ivan on 3/25/21.
//

#include "AsyncUdp.h"

using boost::asio::ip::udp;

AsyncUdp::AsyncUdp(IoServicePtr service, uint16_t port)
        : _socket(*service, udp::endpoint(udp::v4(), port)) {
    startReceive();
}

AsyncUdp::AsyncUdp(IoServicePtr service, std::string_view host, uint16_t port)
        : _socket(*service, udp::endpoint(udp::v4(), 0))
        , _serverHost(host), _serverPort(port) {
}

SocketHandler::Ptr AsyncUdp::connect() {
    udp::resolver resolver(_socket.get_executor());
    udp::resolver::query query(udp::v4(), _serverHost, std::to_string(_serverPort));

    return std::make_shared<UdpSocketHandler>(*resolver.resolve(query));
}

FutureError AsyncUdp::send(const SocketHandler::Ptr &handler, const uint8_t *data, size_t size) {
    auto promise = std::make_shared<PromiseError>();
    auto updHandler = dynamic_cast<UdpSocketHandler *>(handler.get());
    assert(updHandler);

    _socket.async_send_to(
            boost::asio::buffer(data, size),
            updHandler->getEndpoint(),
            [this, promise](const boost::system::error_code &error, std::size_t size) {
                handleSend(error, size);
                promise->set_value(error);
            }
    );

    return promise->get_future();
}

void AsyncUdp::startReceive() {
    _socket.async_receive_from(
            boost::asio::buffer(_recvBuffer),
            _endpoint,
            [this](const boost::system::error_code &error, std::size_t size) {
                handleReceive(_endpoint, error, _recvBuffer.data(), _recvBuffer.size());
            }
    );
}

void AsyncUdp::handleReceive(const UdpEndpoint &endpoint, const std::error_code &error, const uint8_t *data,
                             size_t size) {
    if (!error) {
        if (_handler) {
            _handler(Packet{std::make_shared<UdpSocketHandler>(endpoint), data, size});
        }
        startReceive();
    }
}

void AsyncUdp::handleSend(const std::error_code &error, std::size_t size) {
    startReceive();
}