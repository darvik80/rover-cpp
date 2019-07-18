//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include "PluginManager.h"

#include <boost/dll/import.hpp> // for import_alias
#include <boost/dll/library_info.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "Application.h"
#include "subsystem/Logger.h"
#include "../const.h"

using namespace boost;

const char *PluginManager::name() const {
    return "pluginManager";
}

void PluginManager::postConstruct(Application &app) {
    std::string path = app.getProperties()->getString(PROP_APPLICATION_LIBRARY);

    boost::function<PluginCreator> pluginCreator;

    filesystem::directory_iterator end_iter;
    for (filesystem::directory_iterator it(path); it != end_iter; ++it) {
        app.logger().info(it->path().string());

        if (!filesystem::is_regular_file(*it) || it->path().extension() != ".dylib") {
            continue;
        }

        dll::shared_library lib(it->path());//, dll::load_mode::append_decorations);
        if (!lib.has(FN_CREATE_MODULE)) {
            // no such symbol
            continue;
        }

        pluginCreator = dll::import_alias<PluginCreator>(boost::move(lib), FN_CREATE_MODULE);
        _libs.push_back(pluginCreator);

        Module::Ptr module = pluginCreator();
        _modules[module->name()] = module;
    }
}

void PluginManager::preDestroy() {
    _modules.clear();

    _libs.clear();
}
