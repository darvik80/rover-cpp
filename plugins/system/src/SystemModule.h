//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-07.
//

#ifndef ROVER_SYSTEMMODULE_H
#define ROVER_SYSTEMMODULE_H

#define BOOST_DLL_FORCE_ALIAS_INSTANTIATION

#include <boost/dll/alias.hpp> // for BOOST_DLL_ALIAS

#include "plugin/ModuleBase.h"

namespace plugin {

    class SystemModule : public ModuleBase {
    public:
        SystemModule();

        std::string name() const override;

        // Factory method
        static std::shared_ptr<Module> create() {
            return std::make_shared<SystemModule>();
        }

        virtual ~SystemModule();
    };

    BOOST_DLL_ALIAS(plugin::SystemModule::create, createModule)
}

#endif //ROVER_SYSTEMMODULE_H
