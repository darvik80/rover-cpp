//
// Created by Kishchenko, Ivan on 9/5/19.
//

#include "Protocol.h"
#include <boost/algorithm/string.hpp>

using namespace boost;

namespace protocol {
    Operation::Ptr OperationManager::parse(const std::string &packet) {
        if (packet.size() < 4) {
            throw std::invalid_argument("packet is too small");
        }
        uint32_t cmd = *(uint32_t *) packet.substr(0, 4).c_str();
        switch (cmd) {
            case PROTO_INIT:
                return std::make_shared<Handshake>();
            case PROTO_MSG: {
                if (packet.size() < 5) {
                    throw std::invalid_argument("packet is too small");
                }
                std::string data = packet.substr(5);
                return std::make_shared<Message>(data);
            }
            case PROTO_SET: {
                if (packet.size() < 5) {
                    throw std::invalid_argument("packet is too small");
                }
                std::vector<std::string> params;
                std::string data = packet.substr(5);
                algorithm::split(params, data, boost::is_any_of(":"));
                return std::make_shared<SetPin>(
                        boost::lexical_cast<int>(params.at(0)),
                        boost::lexical_cast<int>(params.at(1))
                );
            }
            case PROTO_GET: {
                if (packet.size() < 5) {
                    throw std::invalid_argument("packet is too small");
                }
                std::vector<std::string> params;
                std::string data = packet.substr(5);
                algorithm::split(params, data, boost::is_any_of(":"));
                return std::make_shared<GetPin>(
                        boost::lexical_cast<int>(params.at(0))
                );
            }
            case PROTO_PUT: {
                if (packet.size() < 5) {
                    throw std::invalid_argument("packet is too small");
                }
                std::vector<std::string> params;
                std::string data = packet.substr(5);
                algorithm::split(params, data, boost::is_any_of(":"));
                return std::make_shared<PutPin>(
                        boost::lexical_cast<int>(params.at(0)),
                        boost::lexical_cast<bool>(params.at(1))
                );
            }

            case PROTO_MODE: {
                if (packet.size() < 5) {
                    throw std::invalid_argument("packet is too small");
                }
                std::vector<std::string> params;
                std::string data = packet.substr(5);
                algorithm::split(params, data, boost::is_any_of(":"));
                return std::make_shared<ModePin>(
                        boost::lexical_cast<int>(params.at(0)),
                        boost::lexical_cast<int>(params.at(1))
                );
            }

            default:
                throw std::invalid_argument("unknown operation: " + packet);
        }
    }

}
