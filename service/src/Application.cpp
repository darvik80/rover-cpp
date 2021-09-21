//
// Created by Ivan Kishchenko on 11.04.2021.
//

#include "properties/source/JsonPropertySource.h"
#include "properties/source/EnvPropertySource.h"
#include "properties/source/CompositePropertySource.h"
#include "resources/ResourceManager.h"
#include "Application.h"

#include "logging/LoggingService.h"
#include "net/http/HttpService.h"
#include "grpc/GrpcService.h"
#include "event/EventManagerService.h"
#include "event/ApplicationEvent.h"
#include "scheduler/SchedulerService.h"
#include "serial/SerialService.h"
#include "mqtt/MqttService.h"
#include "InfoService.h"
#include "joystick/JoystickService.h"
#include "BarcodeReaderService.h"
#include "zeromq/ZeroMQService.h"

using namespace boost;

void Application::run(int argc, char **argv) {
    Registry registry(
            std::make_shared<CompositePropertySource>(
                    std::vector<PropertySource::Ptr>{
                            std::make_shared<JsonPropertySource>(ResourceManager::instance().getResourceAsString("settings.json").value()),
                            std::make_shared<EnvPropertySource>()
                    }
            )
    );

    postConstruct(registry);
    run(registry);
    preDestroy(registry);
}

#include <logging/Logging.h>
#include <type_traits>
#include <optional>
#include <vector>

namespace {
    template<class T>
    struct is_vector : std::false_type {
    };

    template<template<typename...> class C, typename U>
    struct is_vector<C<U>> {
        static constexpr bool value = std::is_same<C<U>, std::vector<U>>::value;
    };

    template<class T>
    struct is_optional : std::false_type {
    };

    template<class T>
    struct is_optional<std::optional<T>> : std::true_type {
    };

}

template<typename T>
void doPrint(const T &val) {
    if constexpr(is_optional<T>::value) {
        logger::info("it's optional");
        doPrint(*val);
    }

    if constexpr(is_vector<T>::value) {
        logger::info("it's vector");
        if (val.size()) {
            doPrint(val[0]);
        }
    }

    if constexpr(std::is_base_of<std::string, T>::value) {
        logger::info("it's string");
    }

    if constexpr(std::is_integral<T>::value) {
        logger::info("it's int");
    }
}

void Application::postConstruct(Registry &registry) {

    // { System Services
    registry.addService(std::make_shared<LoggingService>());
    registry.addService(std::make_shared<SchedulerService>(registry.getIoService()));
    registry.addService(std::make_shared<EventManagerService>());
    registry.addService(std::make_shared<HttpService>());
    registry.addService(std::make_shared<GrpcService>());
    //registry.addService(std::make_shared<MqttService>());
    // } System Services

    registry.addService(std::make_shared<InfoService>());
#if defined SUPPORT_XBOXDRV
    registry.addService(std::make_shared<xbox::JoystickService>());
#endif
#if defined SUPPORT_BARCODE
    registry.addService(std::make_shared<BarcodeReaderService>());
#endif
    //registry.addService(std::make_shared<SerialService>());
    registry.addService(std::make_shared<ZeroMQService>());

    registry.visitService([&registry](auto &service) {
        service.postConstruct(registry);
    });

    logger::info("test");
    auto d = std::optional<std::string>("is_optional");
    doPrint(d);
    auto v = std::vector<int>({1, 2, 3,});
    doPrint(v);
    std::string s = "Hello World";
    doPrint(s);
}

void Application::run(Registry &registry) {
    auto &ioc = registry.getIoService();
    asio::signal_set signals(ioc);
    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif

    auto &eventManager = registry.getService<EventManagerService>();

    signals.async_wait(
            [&eventManager](boost::system::error_code ec, int signal) {
                eventManager.raiseEvent(ApplicationCloseEvent{signal});
            }
    );

    std::function<bool(const ApplicationCloseEvent &)> fnClose = [&ioc, this](const ApplicationCloseEvent &event) -> bool {
        std::string signal = "unknown";
        switch (event.getSignal()) {
            case SIGTERM:
                signal = "SIGTERM";
                break;
#if defined(SIGQUIT)
            case SIGQUIT:
                signal = "SIGQUIT";
                break;
#endif
            case SIGINT:
                signal = "SIGINT";
                break;
            default:
                break;
        }
        info("handle signal: {}", signal);
        ioc.stop();
        return true;
    };
    registry.getService<EventManagerService>().subscribe<>(fnClose);
    std::function<bool(const ApplicationShutdownEvent &)> fnShutdown = [this](const ApplicationShutdownEvent &event) -> bool {
        info("shutdown");
        return true;
    };
    registry.getService<EventManagerService>().subscribe<>(fnShutdown);

    registry.getService<EventManagerService>().raiseEvent(ApplicationStartedEvent{});
    registry.getIoService().run();
}

void Application::preDestroy(Registry &registry) {
    registry.visitService([&registry](auto &service) {
        service.preDestroy(registry);
    });
    registry.getService<EventManagerService>().raiseEvent(ApplicationShutdownEvent{});
}

