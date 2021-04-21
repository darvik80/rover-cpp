//
// Created by Ivan Kishchenko on 21.04.2021.
//

#ifndef ROVER_RESOURCEMANAGER_H
#define ROVER_RESOURCEMANAGER_H

#include <cstddef>
#include <memory>
#include <vector>
#include <string>

class Resource {
public:
    typedef std::shared_ptr<Resource> Ptr;
public:
    virtual const void* getOffset() = 0;
    virtual size_t getSize() = 0;

    virtual ~Resource() = default;
};

class ResourceManager {
public:
    typedef std::shared_ptr<ResourceManager> Ptr;
public:
    virtual std::vector<std::string> getResourceNames() = 0;
    virtual Resource::Ptr getResource(std::string_view name) = 0;

    static ResourceManager& instance();
};


#endif //ROVER_RESOURCEMANAGER_H
