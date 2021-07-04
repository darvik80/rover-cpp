//
// Created by Ivan Kishchenko on 26.04.2021.
//

#ifndef ROVER_COMPOSITEPROPERTYSOURCE_H
#define ROVER_COMPOSITEPROPERTYSOURCE_H

#include "PropertySource.h"
#include <utility>
#include <vector>

class CompositePropertySource : public PropertySource {
    std::vector<PropertySource::Ptr> _sources;
private:
    template<class Props>
    void merge(Props &props) {
        for (auto &source : _sources) {
            source->getProperties(props);
        }
    }
public:
    explicit CompositePropertySource(std::vector<PropertySource::Ptr>  sources)
            : _sources(std::move(sources)) {
    }

    void getProperties(NetworkProperties &props) override {
        merge(props);
    }
    void getProperties(LoggingProperties &props) override {
        merge(props);
    }
    void getProperties(HttpProperties &props) override {
        merge(props);
    }

    void getProperties(GrpcProperties &props) override {
        merge(props);
    }

    void getProperties(SerialProperties &props) override {
        merge(props);
    }
};


#endif //ROVER_COMPOSITEPROPERTYSOURCE_H
