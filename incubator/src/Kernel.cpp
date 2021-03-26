//
// Created by Kishchenko, Ivan on 3/24/21.
//

#include "Kernel.h"

#include <string>
#include <module/RegistryService.h>
#include <boost/asio/signal_set.hpp>

using namespace boost;

std::error_code Kernel::create() {
#ifdef PROFILE_RASPBERRY
    _content.service = std::make_shared<IoService>();
#endif

    _registry.registerModule(std::make_shared<RegistryService>());
    _registry.registerModule(std::make_shared<RegistryServiceClient>());

    for (auto &item : _registry.getModules()) {
        item->create(_content);
    }

    return {};
}

std::error_code Kernel::run() {
    IoServicePtr service = _content.service;

    asio::signal_set signals(*service);
    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif
    signals.async_wait(
            [service](boost::system::error_code errorCode, int code) {
                service->stop();
            }
    );


    while (!service->stopped()) {
        try {
            service->run();
        } catch (const std::exception &e) {
            //Log::Error("Server: Network exception: ", e.what());
        } catch (...) {
            //Log::Error("Server: Network exception: unknown");
        }
    }

    return {};
}
