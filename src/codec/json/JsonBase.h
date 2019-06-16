//
// Created by Ivan Kishchenko on 2019-06-08.
//

#ifndef ROVER_JSONBASE_H
#define ROVER_JSONBASE_H

#include <Poco/JSON/Object.h>
#include <Poco/Optional.h>
#include <Poco/Dynamic/Var.h>
#include "../Interfaces.h"

class JsonDecoder : public IDecoder {
private:
    Poco::SharedPtr<IUnMarshaller> _unMarshaller;
public:
    explicit JsonDecoder(const Poco::SharedPtr<IUnMarshaller> &unMarshaller)
            : _unMarshaller(unMarshaller) {
    }

    void decode(std::istream &is) override;
};

class JsonEncoder : public IEncoder {
private:
    Poco::SharedPtr<IMarshaller> _marshaller;
public:
    explicit JsonEncoder(const Poco::SharedPtr<IMarshaller> &marshaller)
            : _marshaller(marshaller) {
    }

    std::string encode() override;
};

static void unMarshal(const Poco::JSON::Object::Ptr &ptr, const std::string &tag, Poco::Optional<Poco::Dynamic::Var> &value) {
    auto opt = ptr->get(tag);
    value.assign(opt);
}

static void unMarshal(const Poco::JSON::Object::Ptr &ptr, const std::string &tag, Poco::Dynamic::Var &value) {
    value = ptr->get(tag);
}

template<typename T>
static void unMarshal(const Poco::JSON::Object::Ptr &ptr, const std::string &tag, Poco::Optional<T> &value) {
    if constexpr (std::is_base_of<IUnMarshaller, T>::value) {
        T opt;
        opt.unMarshal(ptr->get(tag));
        value.assign(opt);
    } else {
        auto opt = ptr->get(tag).convert<T>();
        value.assign(opt);
    }
}

template<typename T>
static void unMarshal(const Poco::JSON::Object::Ptr &ptr, const std::string &tag, T &value) {
    if constexpr (std::is_base_of<IUnMarshaller, T>::value) {
        value.unMarshal(ptr->get(tag));
    } else {
        value = ptr->get(tag).convert<T>();
    }
}

static void marshal(Poco::JSON::Object &ptr, const std::string &tag, Poco::Dynamic::Var &value) {
    if (!value.isEmpty()) {
        ptr.set(tag, value);
    }
}

template<typename T>
static void marshal(Poco::JSON::Object &ptr, const std::string &tag, const T &value) {
    if constexpr (std::is_base_of<Poco::Optional<T>, T>::value) {
        if (value.isSpecified()) {
            marshal(value.value());
        }
    } else if constexpr (std::is_base_of<IMarshaller, T>::value) {
        ptr.set(tag, value.marshal());
    } else {
        ptr.set(tag, value);
    }
}

static void marshal(Poco::JSON::Object &ptr, const std::string &tag, const IMarshaller &value) {
    ptr.set(tag, value.marshal());
}

template<typename T>
static void marshal(Poco::JSON::Object &ptr, const std::string &tag, const Poco::Optional<T> &value) {
    if (value.isSpecified()) {
        marshal(ptr, tag, value.value());
    }
}

#define END_DECLARE_DTO \
};

#define BEGIN_JSON_UNMARSHAL                                                        \
template <typename Owner>                                                           \
class JsonUnMarshaller : public IUnMarshaller {                                     \
public:                                                                             \
    explicit JsonUnMarshaller(Owner& obj) : _obj(obj) { }                           \
private:                                                                            \
    Owner& _obj;                                                                    \
public:                                                                             \
    virtual void unMarshal(const Poco::Dynamic::Var& object) override {             \
        const auto& ptr = object.extract<Poco::JSON::Object::Ptr>();
#define ITEM_JSON_UNMARSHAL(name) ::unMarshal(ptr, #name, _obj.name);
#define ITEM_JSON_UNMARSHAL_TAG(tag, name) ::unMarshal(ptr, #tag, _obj.name);
#define END_JSON_UNMARSHAL }                                                        \
};                                                                                  \
typedef JsonUnMarshaller<Owner> UnMarshaller;                                       \
Poco::SharedPtr<IUnMarshaller> unMarshaller() {                                     \
    return Poco::SharedPtr<IUnMarshaller>(new UnMarshaller(*this));                 \
}                                                                                   \
virtual void unMarshal(const Poco::Dynamic::Var& object) override {                 \
    this->unMarshaller()->unMarshal(object);                                        \
}

#define BEGIN_JSON_MARSHAL                                                          \
template <typename Owner>                                                           \
class JsonMarshaller : public IMarshaller {                                         \
private:                                                                            \
    const Owner& _obj;                                                              \
public:                                                                             \
    explicit JsonMarshaller(const Owner& obj) : _obj(obj) { }                       \
    virtual Poco::Dynamic::Var marshal() const override {                           \
        Poco::JSON::Object obj;
#define ITEM_JSON_MARSHAL(name) ::marshal(obj, #name, _obj.name);
#define ITEM_JSON_MARSHAL_TAG(tag, name) ::marshal(obj, #tag, _obj.name);
#define END_JSON_MARSHAL return Poco::Dynamic::Var(obj);                            \
    }                                                                               \
};                                                                                  \
typedef JsonMarshaller<Owner> Marshaller;                                           \
Poco::SharedPtr<IMarshaller> marshaller() const {                                   \
    return Poco::SharedPtr<IMarshaller>(new Marshaller(*this));                     \
}                                                                                   \
virtual Poco::Dynamic::Var marshal() const override {                               \
    return this->marshaller()->marshal();                                           \
}

#define BEGIN_DECLARE_DTO(T)                                                        \
class T : public IMarshaller, public IUnMarshaller, public ICloneable {             \
            typedef T Owner;                                                        \
        public:                                                                     \
            ICloneable* clone() override {                                          \
                return new T();                                                     \
            }
#define BEGIN_DECLARE_DTO_OUT(T)                                                    \
class T : public IMarshaller, public ICloneable {                                   \
            typedef T Owner;                                                        \
        public:                                                                     \
            ICloneable* clone() override {                                          \
                return new T();                                                     \
            }
#define BEGIN_DECLARE_DTO_INC(T)                                                    \
class T : public IUnMarshaller, public ICloneable {                                 \
            typedef T Owner;                                                        \
        public:                                                                     \
            ICloneable* clone() override {                                          \
                return new T();                                                     \
            }

#define __DECLARE_DTO_FIELD(cls, name) \
public: cls name;

#define __DECLARE_DTO_FIELDS1(cls, name) \
public: cls name; \
BEGIN_JSON_MARSHAL \
    ITEM_JSON_MARSHAL(name) \
END_JSON_MARSHAL \
BEGIN_JSON_UNMARSHAL \
    ITEM_JSON_UNMARSHAL(name) \
END_JSON_UNMARSHAL \

#define __DECLARE_DTO_FIELDS2(cls1, name1, cls2, name2) \
public: cls1 name1; \
public: cls2 name2; \
BEGIN_JSON_MARSHAL \
    ITEM_JSON_MARSHAL(name1) \
    ITEM_JSON_MARSHAL(name2) \
END_JSON_MARSHAL \
BEGIN_JSON_UNMARSHAL \
    ITEM_JSON_UNMARSHAL(name1) \
    ITEM_JSON_UNMARSHAL(name2) \
END_JSON_UNMARSHAL \


#define __DECLARE_DTO_FIELDS3(cls1, name1, cls2, name2, cls3, name3) \
public: cls1 name1; \
public: cls2 name2; \
public: cls3 name3; \
BEGIN_JSON_MARSHAL \
    ITEM_JSON_MARSHAL(name1) \
    ITEM_JSON_MARSHAL(name2) \
    ITEM_JSON_MARSHAL(name3) \
END_JSON_MARSHAL \
BEGIN_JSON_UNMARSHAL \
    ITEM_JSON_UNMARSHAL(name1) \
    ITEM_JSON_UNMARSHAL(name2) \
    ITEM_JSON_UNMARSHAL(name3) \
END_JSON_UNMARSHAL \

#define __DECLARE_DTO_FIELDS4(cls1, name1, cls2, name2, cls3, name3, cls4, name4) \
public: cls1 name1; \
public: cls2 name2; \
public: cls3 name3; \
public: cls3 name4; \
BEGIN_JSON_MARSHAL \
    ITEM_JSON_MARSHAL(name1) \
    ITEM_JSON_MARSHAL(name2) \
    ITEM_JSON_MARSHAL(name3) \
    ITEM_JSON_MARSHAL(name4) \
END_JSON_MARSHAL \
BEGIN_JSON_UNMARSHAL \
    ITEM_JSON_UNMARSHAL(name1) \
    ITEM_JSON_UNMARSHAL(name2) \
    ITEM_JSON_UNMARSHAL(name3) \
    ITEM_JSON_UNMARSHAL(name4) \
END_JSON_UNMARSHAL \

#endif //ROVER_JSONBASE_H
