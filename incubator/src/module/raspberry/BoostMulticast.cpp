//
// Created by Kishchenko, Ivan on 4/1/21.
//

#include "BoostMulticast.h"
#include <iostream>

using namespace boost;

BoostMulticastSender::BoostMulticastSender(boost::asio::io_service &service, std::string_view group, int port)
        : _endpoint(asio::ip::address::from_string(group.data()), port),
          _socket(service) {
    _socket.open(_endpoint.protocol());
}

std::future<void> BoostMulticastSender::multicast(std::string_view message) {
    auto promise = std::make_shared<std::promise<void>>();
    _socket.async_send_to(
            asio::buffer(message.data(), message.size()),
            _endpoint,
            [promise](system::error_code ec, std::size_t size) {
                if (!ec) {
                    std::cout << "message sent" << std::endl;
                    promise->set_value();
                } else {
                    promise->set_exception(std::make_exception_ptr(std::system_error(ec)));
                }
            }
    );

    return promise->get_future();
}

BoostMulticastReceiver::BoostMulticastReceiver(asio::io_service &service, std::string_view listen,
                                               std::string_view group, int port)
        : _socket(service) {
    asio::ip::udp::endpoint endpoint(asio::ip::address::from_string(listen.data()), port);
    _socket.open(endpoint.protocol());
    _socket.set_option(asio::ip::udp::socket::reuse_address(true));
    _socket.bind(endpoint);

    // Join the multicast group.
    _socket.set_option(asio::ip::multicast::join_group(asio::ip::address::from_string(group.data())));
}

void BoostMulticastReceiver::receive(std::function<void(std::string_view, const SenderAddress &address)> func) {
    _socket.async_receive_from(
            boost::asio::buffer(_data),
            _senderEndpoint,
            [this, func](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    func(
                            std::string(_data.data(), length),
                            SenderAddress(_senderEndpoint.address().to_string(), _senderEndpoint.port())
                    );

                    receive(func);
                }
            }
    );
}
