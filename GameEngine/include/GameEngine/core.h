// #pragma once
#ifndef CORE_H
#define CORE_H
#include <bitset>
#include <functional>
#include <type_traits>

// Essentially to support Windows/Mac/Linux systems

// if we are on the Windows OS (since windows uses dll files)
#ifdef RENDER_PLATFORM_WINDOWS
    #ifdef RENDER_BUILD_DLL
        #define RENDER_API __declspec(dllexport)
    #else
        #define RENDER_API __declspec(dllimport)
    #endif
#else // If we are on either Linux/Mac OS, since they use __attributes__
    #define RENDER_API __attribute__((visibility("default")))
#endif


// What this does is checks a specific condition
// Then it logs the message if it fails then calls __debug break (kind of like setting a breakpoint)
#ifdef RENDER_ENABLE_ASSERTS
    template<typename... T>
    void render_assert(bool x, T&&... args){
        if(!(x)){
            clientLogError("Assertion Failed: {}", std::forward(args)...);
            __debugbreak();
        }
    }

    template<typename... T>
    void render_core_assert(bool x, T&&... args){
        if(!(x)){
            coreLogError("Assertion Failed: {}", std::forward(args)...);
            __debugbreak();
        }
    }
#else
    // #define RENDER_ASSERT(x, ...)
    // #define RENDER_CORE_ASSERT(x, ...)
    template<typename... T>
    void render_assert(bool x, T&&... args){}

    template<typename... T>
    void render_core_assert(bool x, T&&... args){}
#endif

// Just to see, these were previous alternatives that were going to be used in thhe engine but WILL not be used
// Just shown here to showcase different alternatives
// std::bind is how we tell which Application function we want to bind 
// #define bind_event_function(x) std::bind(&Application::x, this, std::placeholders::_1)

// template<typename function>
// void bind_event_function(){
//     std::bind(&Application::function, this, std::placeholders::_1);
// }


// 
// @brief
// 
// T: class instance (such as the class instance like "this" keyword)
// M: is the function we are forwarding a callable object
// NOTE GOING TO USE - overhead in runtime and compile-time, and RAM usage

// template<typename T>
// auto bind_event_function(T* instance, auto M){
//     return std::bind(M, instance, std::placeholders::_1);
// }


// template<typename T>
// Window::EventCallbackFn bind_event_function(T x) {
//     return std::bind(&x, this, std::placeholders::_1);
// }

// Reasons not to use std::bind
// Instead of using std::bind because of using more ram, and runtime, compile time overhead
// using a lambda would be a better way to achhieve this
// Usage:
// we pass in an instance, and member function
// As this lambda will accept that as the parameters

// What is happening in this lambda?
// - This lambda creates another lambda that captures the instance and member function
// - From enclosing in this lambda. The new lambda which is a callable object and member function ptr
// - what we are returning is a dereferenced member function ptr of variable "member_function" on object instance
// - Then we std::forward arg1 which will just check here that if the object is a moved object or temp
//      then it becomes an rvalue
// - Though if object is persistent object then we use std::forward to decltype the arg1 getting actual type.
// - Which simply just means Invoking a bound member function with a given argument.
// - Invoking just means calling a function bounded by a class or is a specific objects method.

// This is what it would like if we were to use std::bind instead of a lambda (just so you can see the syntax)
// std::bind(&ImGuiLayer::onWindowClose, this, std::placehholders::_1);

template<typename t1, typename t2>
static bool is_same(){
    return std::is_same<t1, t2>();
}

static auto bind_function = [](auto* instance, auto member_bound_function){
    return [instance, member_bound_function](auto&& arg1){
        // if(std::is_same_v<decltype(member_bound_function), decltype(arg1)> != true){
        //     printf("from core.h printing \"Not true!\"\n");
        // }
        return (instance->*member_bound_function)(std::forward<decltype(arg1)>(arg1));
    };
};

// Using a bitfield to go into multiple categories
// Hence, why we are using a bitfield.
constexpr uint64_t bit(uint64_t x){
    return (1 << x);
}
#endif