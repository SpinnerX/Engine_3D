// #pragma once
// #include <type_traits>
// #include <memory>

// // Essentially to support Windows/Mac/Linux systems

// // if we are on the Windows OS (since windows uses dll files)
// ENGINE_RENDER_PLATFORM_WINDOWS
//     #ifdef RENDER_BUILD_DLL
//         #define ENGINE_API __declspec(dllexport)
//     #else
//         #define ENGINE_API __declspec(dllimport)
//     #endif
// #else // If we are on either Linux/Mac OS, since they use __attributes__
//     #define ENGINE_API __attribute__((visibility("default")))
// #endif


// // What this does is checks a specific condition
// // Then it logs the message if it fails then calls __debug break (kind of like setting a breakpoint)
// #ifdef RENDER_ENABLE_ASSERTS
//     template<typename... T>
//     void render_assert(bool x, T&&... args){
//         if(!(x)){
//             clientLogError("Assertion Failed: {}", std::forward(args)...);
//             __debugbreak();
//         }
//     }

//     template<typename... T>
//     void render_core_assert(bool x, T&&... args){
// #if __WIN32__
//         if(!(x)){
//             coreLogError("Assertion Failed: {}", std::forward(args)...);
//             __debugbreak();
//         }
// #endif
// 		if(__builtin_expect(!(x), 0){
// 			coreLogError("Assertion Failed: {}", std::forward(args)...);
// 			__assert_rtn(__func__, __ASSERT_FILE_NAME, __LINE__, x)
// 			return;
// 		}
//     }
// #else
//     // #define RENDER_ASSERT(x, ...)
//     // #define RENDER_CORE_ASSERT(x, ...)
//     template<typename... T>
//     void render_assert(bool x, T&&... args){}

//     template<typename... T>
//     void render_core_assert(bool x, T&&... args){}
// #endif

// template<typename t1, typename t2>
// static bool is_same(){
//     return std::is_same<t1, t2>();
// }

// // Using a bitfield to go into multiple categories
// // Hence, why we are using a bitfield.
// constexpr uint64_t bit(uint64_t x){
//     return (1 << x);
// }

// namespace Engine3D{
//     static auto bind(auto* instance, auto member_bound_function){
//         return [instance, member_bound_function](auto&& arg1){
//             return (instance->*member_bound_function)(std::forward<decltype(arg1)>(arg1));
//         };
//     }

//     /**
//      * @brief 
//      * 
//      * @note Scope - because std::unique_ptr means that the type we pass will only live in the lifetime of its declaration
//      * @tparam T (template) - takes a typename as template arg
// 	 *
// 	 * NOTE: Using Scope<T> because of potential memory leakages in the codebase
//      */
//     template<typename T>
//     using Scope = std::unique_ptr<T>;
	
// 	template<typename T, typename... Args>
// 	constexpr Scope<T> CreateScope(Args&&... args){
// 		return std::make_unique<T>(std::forward<Args>(args)...);
// 	}

//     /**
//      * @brief 
//      * 
//      * @note Ref - is shared ptr that allows an object to live both inside and other areas the objects is used  (in terms of lifetime)
//      * 
//      * @tparam T 
//      */

//     template<typename T>
//     using Ref = std::shared_ptr<T>;

// 	template<typename T, typename... Args>
// 	constexpr Ref<T> CreateRef(Args&&... args){
// 		return std::make_shared<T>(std::forward<Args>(args)...);
// 	}
// };
#pragma once
#include <cstdint>
#include <memory>
#include <type_traits>
#include <cassert>

namespace Engine3D{
    #ifdef ENGINE_PLATFORM_WINDOWS
        #ifdef RENDER_BUILD_DLL
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