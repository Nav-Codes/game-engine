//
// Created by navjo on 2/4/2026.
//

#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <functional>

#include "BaseEvent.hpp"

class EventManager {
public:
    using Handler = std::function<void(const BaseEvent&)>;

    void emit(const BaseEvent& event) const {
        for (const auto& listener : listeners) {
            listener(event);
        }
    }

    void subscribe(const Handler& callback) {
        listeners.emplace_back(callback);
    }

private:

    std::vector<Handler> listeners;
};

#endif //EVENTMANAGER_HPP
