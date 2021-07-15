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
#include "event/EventManager.h"
#include "event/ApplicationEvent.h"
#include "scheduler/Scheduler.h"
#include "serial/SerialService.h"
#include "InfoService.h"
#include "JoystickService.h"

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

char key[KEY_UNKNOWN] = { 0 };

void initKey() {
    key[KEY_MINUS] = '-';
    key[KEY_0] = '0';
    key[KEY_1] = '1';
    key[KEY_2] = '2';
    key[KEY_3] = '3';
    key[KEY_4] = '4';
    key[KEY_5] = '5';
    key[KEY_6] = '6';
    key[KEY_7] = '7';
    key[KEY_8] = '8';
    key[KEY_9] = '9';

    key[KEY_Q] = 'Q';
    key[KEY_W] = 'W';
    key[KEY_E] = 'E';
    key[KEY_R] = 'R';
    key[KEY_T] = 'T';
    key[KEY_Y] = 'Y';
    key[KEY_U] = 'U';
    key[KEY_I] = 'I';
    key[KEY_O] = 'O';
    key[KEY_P] = 'P';

    key[KEY_A] = 'A';
    key[KEY_S] = 'S';
    key[KEY_D] = 'D';
    key[KEY_F] = 'F';
    key[KEY_G] = 'G';
    key[KEY_H] = 'H';
    key[KEY_J] = 'J';
    key[KEY_K] = 'K';
    key[KEY_L] = 'L';

    key[KEY_Z] = 'Z';
    key[KEY_X] = 'X';
    key[KEY_C] = 'C';
    key[KEY_V] = 'V';
    key[KEY_B] = 'B';
    key[KEY_N] = 'N';
    key[KEY_M] = 'M';
}

void Application::postConstruct(Registry &registry) {
    initKey();
    // { System Services
    registry.addService(std::make_shared<LoggingService>());
    registry.addService(std::make_shared<Scheduler>(registry.getIoService()));
    registry.addService(std::make_shared<EventManager>());
    registry.addService(std::make_shared<HttpService>());
    registry.addService(std::make_shared<GrpcService>());
    // } System Services

    struct input_event ev[64];
    int fd, rd, value = sizeof(struct input_event);
    char name[256] = "Unknown";
    const char *device = "/dev/input/by-id/usb-USB_Adapter_USB_Device-event-kbd";

    //Open Device
    if ((fd = open(device, O_RDONLY)) == -1) {
        error("%s is not a valid device.n", device);
    }

    //Print Device Name
    ioctl(fd, EVIOCGNAME (sizeof(name)), name);
    info("Reading From : {} ({})", device, name);

    info("data: {}, {}", sizeof(ev), sizeof(input_event));
    std::string barcode;
    while (true) {
        if ((rd = read(fd, ev, sizeof(ev))) < 0) {
            break;
        }


        auto size = rd / sizeof(input_event);

        for (int idx = 0; idx < size; idx++) {
            if (ev[idx].value == 0) {
                continue;
            }
            if (ev[idx].type == EV_KEY && ev[idx].value == 1) {
                switch (ev[idx].code) {
                    case KEY_LEFTSHIFT:
                        continue;
                    case KEY_ENTER:
                        info("barcode: {}", barcode);
                        barcode = "";
                        continue;
                    default:
                        barcode += key[ev[idx].code];
                        break;
                }
            }
        }
    }

    registry.addService(std::make_shared<InfoService>());
#if defined SUPPORT_XBOXDRV
    registry.addService(std::make_shared<JoystickService>());
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

    auto &eventManager = registry.getService<EventManager>();

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

