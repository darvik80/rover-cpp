//
// Created by Ivan Kishchenko on 02.09.2021.
//

#include "ZeroMQProtocol.h"

#include "ZeroMQReader.h"
#include "ZeroMQWriter.h"
#include <memory>

std::error_code ZeroMQDecoder::read(ZeroMQCharBuf &buf) {
    ZeroMQReader reader(buf);

    if (!reader.available()) {
        return std::make_error_code(std::errc::message_size);
    }

    uint8_t flag;
    if (auto err = reader.readFlag(flag)) {
        return err;
    }

    uint64_t size{0};
    if (flag & flag_long) {
        if (auto err = reader.readSize(size)) {
            return err;
        }
    } else {
        if (auto err = reader.readSize((uint8_t &) size)) {
            return err;
        }
    }

    if (size > reader.available()) {
        return std::make_error_code(std::errc::message_size);
    }

    if (flag & flag_cmd) {
        ZeroMQCommand cmd;
        uint8_t nameSize;
        if (auto err = reader.readSize(nameSize)) {
            return err;
        }

        if (auto err = reader.readString(nameSize, cmd.name)) {
            return err;
        }
        size -= 1 + cmd.name.size();

        if (cmd.name == ZERO_MQ_CMD_READY) {
            std::string prop, val;
            while (size) {
                if (auto err = reader.readSize(nameSize)) {
                    return err;
                }

                if (auto err = reader.readString(nameSize, prop)) {
                    return err;
                }

                uint32_t propSize;
                if (auto err = reader.readSize(propSize)) {
                    return err;
                }

                if (auto err = reader.readString(propSize, val)) {
                    return err;
                }
                cmd.props.emplace(prop, val);

                size -= (5 + prop.size() + val.size());
            }
        } else if (cmd.name == ZERO_MQ_CMD_SUBSCRIBE || cmd.name == ZERO_MQ_CMD_CANCEL) {
            std::string topic;
            if (auto err = reader.readString(size, topic)) {
                return err;
            }

            cmd.props.emplace(ZERO_MQ_PROP_SUBSCRIPTION, topic);
        }

        if (_cmdHandler) {
            _cmdHandler(cmd);
        }
    } else {
        if (!_msg) {
            _msg.reset(new ZeroMQMessage());
        }
        std::string val;
        if (auto err = reader.readString(size, val)) {
            return err;
        }

        *_msg << val;

        if (!(flag & flag_more)) {
            if (_msgHandler) {
                _msgHandler(*_msg);
            }
            _msg.reset();
        }
    }

    buf.consume(buf.size() - buf.in_avail());

    return {};
}

std::error_code ZeroMQEncoder::writeCmdSub(ZeroMQCharBuf &buf, ZeroMQCommand &cmd) {
    ZeroMQWriter writer(buf);

    std::string topic = cmd.props[ZERO_MQ_PROP_SUBSCRIPTION];

    uint64_t expected = 1 + cmd.name.size() + topic.size();

    if (buf.available() < expected + 1) {
        return std::make_error_code(std::errc::message_size);
    }

    writer.writeFlagAndSize(flag_cmd, expected);
    writer.writeSize((uint8_t) cmd.name.size());
    writer.writeString(cmd.name);
    writer.writeString(topic);

    return {};
}

std::error_code ZeroMQEncoder::writeCmdReady(ZeroMQCharBuf &buf, ZeroMQCommand &cmd) {
    ZeroMQWriter writer(buf);
    std::size_t expected = 1 + cmd.name.size();
    for (const auto &prop: cmd.props) {
        expected += 1 + prop.first.size();
        expected += 4 + prop.second.size();
    }

    if (buf.available() < expected + 1) {
        return std::make_error_code(std::errc::message_size);
    }

    writer.writeFlagAndSize(flag_cmd, expected);
    writer.writeSize((uint8_t) cmd.name.size());
    writer.writeString(cmd.name);
    for (const auto &prop: cmd.props) {
        writer.writeSize((uint8_t) prop.first.size());
        writer.writeString(prop.first);

        writer.writeSize((uint32_t) prop.second.size());
        writer.writeString(prop.second);
    }

    return {};
}

std::error_code ZeroMQEncoder::write(ZeroMQCharBuf &buf, ZeroMQGreeting& greeting) {
    ZeroMQWriter writer(buf);
    writer << (uint8_t) 0xFF << std::setfill((char) 0x00) << std::setw(8) << (uint8_t) 0x00 << (uint8_t) 0x7F;
    writer << (uint8_t) greeting.version.major << (uint8_t) greeting.version.minor;
    writer << std::left << std::setw(20) << greeting.mechanism;
    writer << (uint8_t) (greeting.isServer ? 0x01 : 0x00);
    writer << std::setw(31) << (uint8_t) 0x00;

    return {};
}

std::error_code ZeroMQEncoder::write(ZeroMQCharBuf &buf, ZeroMQCommand &cmd) {
    if (cmd.name == ZERO_MQ_CMD_READY) {
        return writeCmdReady(buf, cmd);
    } else if (cmd.name == ZERO_MQ_CMD_SUBSCRIBE || cmd.name == ZERO_MQ_CMD_CANCEL) {
        return writeCmdSub(buf, cmd);
    }

    return {};
}

std::error_code ZeroMQEncoder::write(ZeroMQCharBuf &buf, ZeroMQMessage &msg) {
    ZeroMQWriter writer(buf);
    std::size_t expected{0};
    for (const auto &item: msg.data) {
        expected += 1 + (item.size() > UINT8_MAX ? 1 : 8) + item.size();
    }

    if (buf.available() < expected) {
        return std::make_error_code(std::errc::message_size);
    }

    for (auto &iter: msg.data) {
        writer.writeFlagAndSize(flag_msg, iter.size());
        writer.writeString(iter);
    }

    return {};
}