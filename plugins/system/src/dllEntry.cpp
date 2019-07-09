//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-07.
//

#include <Poco/ClassLoader.h>
#include <Poco/ClassLibrary.h>

#include <Poco/Manifest.h>
#include "SystemModule.h"

#include <iostream>

POCO_BEGIN_MANIFEST(Module)
    POCO_EXPORT_CLASS(SystemModule)
POCO_END_MANIFEST

void pocoInitializeLibrary()
{
    std::cout << "SystemLib initializing" << std::endl;
}


void pocoUninitializeLibrary()
{
    std::cout << "SystemLib uninitialzing" << std::endl;
}