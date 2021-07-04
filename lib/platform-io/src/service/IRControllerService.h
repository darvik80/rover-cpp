//
// Created by Ivan Kishchenko on 26.06.2021.
//

#ifndef PLATFORM_IO_IRCONTROLLERSERVICE_H
#define PLATFORM_IO_IRCONTROLLERSERVICE_H

#include "service/Service.h"
#include <device/controller/IRRemote.h>

class IRControllerService : public BaseService {
    IRRemote* _remote{nullptr};
public:
    explicit IRControllerService(Registry &registry, IRRemote* remote);

    void run() override;
};


#endif //PLATFORM_IO_IRCONTROLLERSERVICE_H
