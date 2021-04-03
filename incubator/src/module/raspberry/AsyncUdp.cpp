//
// Created by Kishchenko, Ivan on 3/25/21.
//

#include "AsyncUdp.h"

using boost::asio::ip::udp;

AsyncUdp::AsyncUdp(const IoServicePtr& service) : _socket(*service) {
}

void AsyncUdp::listen(uint16_t port) {
    boost::system::error_code ec;
    _socket.open(udp::v4(), ec);
    boost::asio::detail::throw_error(ec, "open");

    UdpEndpoint endpoint(udp::v4(), port);
    _socket.bind(endpoint, ec);
    boost::asio::detail::throw_error(ec, "bind");
}

SocketHandler::Ptr AsyncUdp::connect(std::string_view host, uint16_t port) {
    boost::system::error_code ec;
    _socket.open(udp::v4(), ec);
    boost::asio::detail::throw_error(ec, "open");

    udp::resolver resolver(_socket.get_executor());
    udp::resolver::query query(udp::v4(), host.data(), std::to_string(port));

    return std::make_shared<UdpSocketHandler>(*resolver.resolve(query));
}

SocketHandler::Ptr AsyncUdp::broadcast(std::string_view host, uint16_t port) {
    boost::system::error_code ec;
    _socket.open(udp::v4(), ec);
    boost::asio::detail::throw_error(ec, "open");

    _socket.set_option(UdpSocket::reuse_address(true));
    _socket.set_option(UdpSocket::broadcast(true));

    udp::resolver::query query(udp::v, host.data(), std::to_string(port));

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