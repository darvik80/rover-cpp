#include "EventManager.h"

void EventManager::raiseEvent(const Event& event) {
    if (auto iter = _signals.find(typeid(event)); iter != _signals.end()) {
        iter->second(event);
    } else {
        logging::warning("[EM] dropped event {}", typeid(event).name());
    }
};