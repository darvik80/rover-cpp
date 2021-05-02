//
// Created by Ivan Kishchenko on 02.05.2021.
//

#include "Scheduler.h"

void Scheduler::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);
}

void Scheduler::schedule(PosixTime time, PosixDuration duration, std::function<void()> fn) {

}