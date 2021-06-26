//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_IRCONTROLLERSERVICE_H
#define PLATFORM_IO_IRCONTROLLERSERVICE_H

#include "Service.h"
#include "device/HX1838IRRemote.h"

class IRControllerService : public BaseService {
    IRRemote* _remote{nullptr};
public:
    explicit IRControllerService(Registry &registry);

    void postConstruct() override;

    void run() override;

    void preDestroy() override;
};


#endif //PLATFORM_IO_IRCONTROLLERSERVICE_H
