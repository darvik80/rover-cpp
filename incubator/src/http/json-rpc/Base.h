//
// Created by Ivan Kishchenko on 2019-07-13.
//

#ifndef ROVER_BASE_H
#define ROVER_BASE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>
#include <string>

namespace json {

    typedef boost::property_tree::ptree tree;

    class Encoder {
    public:
        virtual std::string encode() = 0;

        virtual ~Encoder() = default;
    };

    class Decoder {
    public:
        virtual void decode(const std::string& json) = 0;

        virtual ~Decoder() = default;
    };

    class Marshaller {
    public:
        virtual tree marshal() const = 0;

        virtual ~Marshaller() = default;
    };

    class UnMarshaller {
    public:
        virtual void unMarshal(const tree &object) = 0;

        virtual void unMarshal(const boost::optional<tree> &object) {
            if (object) {
                unMarshal(object.value());
            }
        }

        virtual ~UnMarshaller() = default;
    };
}


#endif //ROVER_BASE_H
