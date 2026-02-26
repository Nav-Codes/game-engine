//
// Created by navjo on 1/21/2026.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <array>
#include <bitset>
#include <cstddef>

#include "ComponentType.hpp"

//create const that is evaluated at compile time (constexpr)
//32 components fit perfectly in 32 bit integer
constexpr std::size_t MAX_COMPONENTS = 32;

//bit ops fast and work well with checking each variable
using ComponentBitSet = std::bitset<MAX_COMPONENTS>;

//fixed size array, where each index is a component
//index corresponds to the ComponentTypeID
//we use fixed num of components, so there's no dynamic alloc for array (efficient)
using ComponentArray = std::array<void*, MAX_COMPONENTS>;

class Entity {
    bool active = true;
    ComponentArray componentArray{};
    ComponentBitSet componentBitSet{};

public:
    bool isActive() {
        return active;
    }
    void destroy() {
        active = false;
    }

    //templates need to be implemented in header
    //entity.hasComponent<Position>() -> returns true or false
    template <typename T>
    bool hasComponent() {
        return componentBitSet[getComponentTypeID<T>()];
    }

    //template that can receive multiple args
    //entity.addComponent<Position>(4, 5)
    //entity.addComponent<Health>(100)
    template <typename T, typename... mArgs>
    T& addComponent(mArgs&&...args) {

        //implements perfect forwarding
        //everything is not treated as a lvalue
        //this is more efficient with moves and temporaries
        T* component(new T{std::forward<mArgs>(args)...});

        //add new component to the array
        componentArray[getComponentTypeID<T>()] = component;

        //update bitset to true
        componentBitSet[getComponentTypeID<T>()] = true;
        return *component;
    }

    template <typename T>
    T& getComponent() {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }

    template <typename T>
    void deactivateComponent() {
        componentBitSet[getComponentTypeID<T>()] = false;
    }
};

#endif //ENTITY_HPP
