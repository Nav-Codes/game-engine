//
// Created by navjo on 2/4/2026.
//

#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP
// #include <functional>

#include <functional>

#include "Entity.hpp"

struct CollisionEvent {
    Entity* entityA = nullptr;
    Entity* entityB = nullptr;
};

class EventManager {
public:
    // template<typename EventType>
    // void emit(const EventType& event) {
    //     //retrieve the list of subscribers to certain events
    //     auto& listeners = getListeners<EventType>();
    //     //loop all the subcribers to certain events
    //     for (auto& listener : listeners) {
    //         listener(event); //invoke the function
    //     }
    // }

    // template<typename EventType>
    // void subscribe(std::function<void(const EventType&)> callback) {
    //     //pass in the callback/callable wrapper/subscription to the list of subscriptions
    //     getListeners<EventType>().push_back(callback);
    // }

    void emit(const CollisionEvent& event) const {
        for (const auto& listener : collisionListeners) {
            listener(event);
        }
    }

    void subscribe(std::function<void(const CollisionEvent&)> callback) {
        collisionListeners.emplace_back(callback);
    }

private:
    //each event type essentially has its own std::vector of listeners without u having to manage it explicitly. this is done using the statuc local
    //std::function<void(const EventType&)> is the callable wrapper: can hold anything that can be called like a function (lambda, free function, functor, etc.)
    // template<typename EventType>
    // std::vector<std::function<void(const EventType&)>>& getListeners() {
    //     static std::vector<std::function<void(const EventType&)>> listeners;
    //     return listeners;
    // }

    std::vector<std::function<void(const CollisionEvent&)>> collisionListeners;
};

#endif //EVENTMANAGER_HPP
