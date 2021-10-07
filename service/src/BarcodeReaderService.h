//
// Created by Ivan Kishchenko on 15.07.2021.
//

#pragma once

#ifdef SUPPORT_BARCODE

#include <BaseService.h>
#include <linux/input.h>

class BarcodeReaderService : public BaseService {
    input_event _events[64];
    std::unique_ptr<boost::asio::posix::stream_descriptor> _stream;

    std::string _barcode;
public:
    const char *name() override {
        return "barcode";
    }

    void postConstruct(Registry &registry) override;
    void onRead(const boost::system::error_code &error, std::size_t readable);
};

#endif
