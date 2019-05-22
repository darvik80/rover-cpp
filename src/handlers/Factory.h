//
// Created by Ivan Kishchenko on 2019-05-22.
//

#ifndef ROVER_FACTORY_H
#define ROVER_FACTORY_H


#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace handlers {

    class Factory : public Poco::Net::HTTPRequestHandlerFactory {
    private:
        Poco::Net::HTTPRequestHandler *createRequestHandler(
                const Poco::Net::HTTPServerRequest &request) override;
    };
}

#endif //ROVER_FACTORY_H
