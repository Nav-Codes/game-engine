//
// Created by navjo on 1/21/2026.
//

#ifndef COMPONENTTYPE_HPP
#define COMPONENTTYPE_HPP
#include <cstddef>

//creating a type alias for std::size_t
//size_t is designed to represent sizes and indices because its unsigned and its very large (long, long)
using ComponentTypeID = std::size_t;

//when you have a free function and defined in a header, each .cpp file that includes this header (ComponentType.hpp)
//would get its own definition, causing a linker error, using inline merges them into one definition
//global counter generator, returns a unique number everytime we call it
//making id static, means it will always be unique (will not get reset to 0)
inline ComponentTypeID getComponentTypeID() {
    static ComponentTypeID id = 0;
    return id++;
}

//can overload this function because it has diff signatures
//template used to work with generic types
//template is inline by default
//getComponentTypeID<Position>() -> would always return 0
//getComponentTypeID<Health>() -> would always return 1
//a static local variable created and inited once

template <typename T>
ComponentTypeID getComponentTypeID() {
    static ComponentTypeID typeID = getComponentTypeID();
    return typeID;
}

#endif //COMPONENTTYPE_HPP
