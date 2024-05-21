#pragma once
#include <cstdint>
#include <memory>
#include <type_traits>
#include <cassert>

namespace Engine3D{
    #ifdef ENGINE_PLATFORM_WINDOWS
        #ifdef ENGINE_BUILD_DLL
            #define ENGINE_API __declspec(dllexport)
        #else
            #define ENGINE_API __declspec(dllimport)
        #endif
    #else // If we are on either Linux/Mac OS, since they use __attributes__
        #define ENGINE_API __attribute__((visibility("default")))
    #endif

    //! @note Helper function to initialize our bitfields
    constexpr uint16_t bit(uint16_t x){
        return (1 << x);
    }

    template<typename... T>
    static void render_core_assert(bool x, const T&&... args){
        assert(x);
    }

    template<typename T, typename MemberBoundFunction>
    static constexpr bool is_member_function = (std::is_member_function_pointer<MemberBoundFunction>::value);


    //! @note Quick implementation similar to std::bind
    //! @note Inefficiency's with std::bind and how it also uses more ram.
    static auto bind(auto* instance, auto member_bound_function){
        return [instance, member_bound_function](auto&& arg1){
            return (instance->*member_bound_function)(std::forward<decltype(arg1)>(arg1));
        };
    }

    template<typename T, typename BoundFunc>
    static auto bind2(T* instance, BoundFunc member_bound_function){
        // static_assert(!std::is_member_pointer<decltype(member_bound_function)>::value, "Not a member function of that instance");

        // std::cout << std::boolalpha << is_member_function<T, BoundFunc> << '\n';
        return [instance, member_bound_function](auto&& params){
            return (instance->*member_bound_function)(std::forward<decltype(params)>(params));
        };
    }

    //! @note Some aliases for the smart pointers
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args){
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args){
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
};