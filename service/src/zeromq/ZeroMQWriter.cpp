//
// Created by Ivan Kishchenko on 05.09.2021.
//

#include "ZeroMQWriter.h"

std::error_code ZeroMQWriter::writeData(const void *data, std::size_t size) {
    write((const char*)data, size);
    if (!(*this)) {
        return std::make_error_code(std::errc::message_size);
    }

    return {};
}

std::error_code ZeroMQWriter::writeFlag(uint8_t flag) {
    return writeData(&flag, sizeof(uint8_t));
}

std::error_code ZeroMQWriter::writeSize(uint8_t size) {
    return writeData(&size, sizeof(uint8_t));
}

std::error_code ZeroMQWriter::writeSize(uint32_t size) {
    size = htonl(size);
    return writeData(&size, sizeof(uint32_t));
}

std::error_code ZeroMQWriter::writeSize(uint64_t size) {
    size = htonll(size);
    return writeData(&size, sizeof(uint64_t));
}

std::error_code ZeroMQWriter::writeString(std::string_view str) {
    return writeData(str.data(), str.size());
}
