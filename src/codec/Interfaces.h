//
// Created by Ivan Kishchenko on 2019-06-09.
//

#ifndef ROVER_INTERFACES_H
#define ROVER_INTERFACES_H

#include <string>

class IEncoder {
public:
    virtual std::string encode() = 0;

    virtual ~IEncoder() = default;
};

class IDecoder {
public:
    virtual void decode(std::istream &is) = 0;

    virtual ~IDecoder() = default;
};

class IMarshaller {
public:
    virtual Poco::Dynamic::Var marshal() const = 0;

    virtual ~IMarshaller() = default;
};

class ICloneable {
public:
    virtual ICloneable* clone() = 0;

    virtual ~ICloneable() = default;
};

class IUnMarshaller {
public:
    virtual void unMarshal(const Poco::Dynamic::Var& object) = 0;
    virtual void unMarshal(const Poco::Optional<Poco::Dynamic::Var>& object) {
        if (object.isSpecified()) {
            unMarshal(object.value());
        }
    }

    virtual ~IUnMarshaller() = default;
};

#endif //ROVER_INTERFACES_H
