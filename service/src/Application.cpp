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
#include "InfoService.h"
#include "JoystickService.h"
#include "BarcodeReaderService.h"

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

void Application::postConstruct(Registry &registry) {

    // { System Services
    registry.addService(std::make_shared<LoggingService>());
    registry.addService(std::make_shared<SchedulerService>(registry.getIoService()));
    registry.addService(std::make_shared<EventManagerService>());
    registry.addService(std::make_shared<HttpService>());
    registry.addService(std::make_shared<GrpcService>());
    // } System Services

    registry.addService(std::make_shared<InfoService>());
#if defined SUPPORT_XBOXDRV
    registry.addService(std::make_shared<JoystickService>());
#endif
#if defined SUPPORT_BARCODE
    registry.addService(std::make_shared<BarcodeReaderService>());
#endif
    //registry.addService(std::make_shared<SerialService>());

    registry.visitService([&registry](auto &service) {
        service.postConstruct(registry);
    });
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
    registry.getService<EventManager>().subscribe<>(fnClose);
    std::function<bool(const ApplicationShutdownEvent &)> fnShutdown = [this](const ApplicationShutdownEvent &event) -> bool {
        info("shutdown");
        return true;
    };
    registry.getService<EventManager>().subscribe<>(fnShutdown);

    registry.getService<EventManager>().raiseEvent(ApplicationStartedEvent{});
    registry.getIoService().run();
}

void Application::preDestroy(Registry &registry) {
    registry.visitService([&registry](auto &service) {
        service.preDestroy(registry);
    });
    registry.getService<EventManager>().raiseEvent(ApplicationShutdownEvent{});
}

