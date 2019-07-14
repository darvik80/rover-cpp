//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#include "PropertiesMap.h"
#include "exception/LogicException.h"

#include <boost/lexical_cast.hpp>

void PropertiesMap::load(const std::string &filePath) {
    throw ForbiddenException("load method forbidden for PropertiesMap");
}

std::string PropertiesMap::getString(const std::string &name) const {
    return tree.at(name);
}

bool PropertiesMap::getBoolean(const std::string &name) const {
    return boost::lexical_cast<bool>(tree.at(name));
}

int PropertiesMap::getInt(const std::string &name) const {
    return boost::lexical_cast<int>(tree.at(name));
}

long PropertiesMap::getLong(const std::string &name) const {
    return boost::lexical_cast<long>(tree.at(name));
}

double PropertiesMap::getDouble(const std::string &name) const {
    return boost::lexical_cast<double>(tree.at(name));
}

std::list<std::string> PropertiesMap::getStringArray(const std::string &name) const {
    throw ForbiddenException("getStringArray method forbidden for PropertiesMap");
}
