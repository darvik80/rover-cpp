//
// Created by Ivan Kishchenko on 24.04.2021.
//

#ifndef ROVER_BASESERVICE_H
#define ROVER_BASESERVICE_H

#include "Service.h"
#include "Registry.h"

class BaseService : public Service {
public:
    int order() override {
        return 0;
    }

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry& registry) override;

protected:
    template <class... A>
    void trace(const std::string& fmt, A&&... args) {
        log::trace("[{}] {}", name(), fmt::format(fmt, std::forward<A>(args)...));
    }

    template <class... A>
    void debug(const std::string& fmt, A&&... args) {
        log::debug("[{}] {}", name(), fmt::format(fmt, std::forward<A>(args)...));
    }

    template <class... A>
    void info(const std::string& fmt, A&&... args) {
        log::info("[{}] {}", name(), fmt::format(fmt, std::forward<A>(args)...));
    }

    template <class... A>
    void warning(const std::string& fmt, A&&... args) {
        log::warning("[{}] {}", name(), fmt::format(fmt, std::forward<A>(args)...));
    }

    template <class... A>
    void error(const std::string& fmt, A&&... args) {
        log::error("[{}] {}", name(), fmt::format(fmt, std::forward<A>(args)...));
    }

    template <class... A>
    void fatal(const std::string& fmt, A&&... args) {
        log::fatal("[{}] {}", name(), fmt::format(fmt, std::forward<A>(args)...));
    }
};

template<typename T>
class BaseServiceShared : public BaseService, public std::enable_shared_from_this<T> {};

#endif //ROVER_BASESERVICE_H
