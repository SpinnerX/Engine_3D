#pragma once
#include <GameEngine/core.h>
#include <string>
#include <functional>
#include <fmt/format.h>


namespace RendererEngine{

    // ** Current design of event system **
    // Events in RendererEngine is currently blocking, meaning when an event occurs it
    // immediately gets dispatched and must be dealt with right then and there.
    // Foir the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage.

    enum class EventType{
        None=0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // EventCategories
    // May want to filter out certain events
    // In other words,receiving all events from application into some kind of
    //  receiving events from my application into some event class.
    // Only carying about the keyboard events
    // Example Scenario - If I wanted to log in mouse events, and do we really
    // want to check to see if the event type is pressed, released, or scrolled
    // So, instead we can just say "give me all the mouse events"
    // Hence why we have EventCategoryEnum
    enum EventCategory{
        None = 0,
        EventCategoryApplication =  bit(0),
        EventCategoryInput =        bit(1),
        EventCategoryKeyboard =     bit(2),
        EventCategoryMouse =        bit(3),
        EventCategoryMouseButton =  bit(4)
    };

    // #define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; } \
    //                                virtual EventType GetEventType() const override { return GetStaticType(); } \
    //                                virtual const char* GetName() const override { return #type; }

    // #define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class RENDER_API Event{
        friend class EventDispatcher;
    public:
        virtual EventType GetEventType() const = 0; // type of event
        virtual const char* GetName() const = 0; // name of the event (used for debugging purposes)
        virtual int GetCategoryFlags() const = 0; // getting our category flags
        virtual std::string toString() const { return GetName(); } // toString returns std::string by default

        // Checking if the category is set in the bitfields.
        // Simply saying is this category in our flags.
        inline bool isInCategory(EventCategory category){
            return GetCategoryFlags() & category;
        }

        friend std::ostream& operator<<(std::ostream& os, const Event& e){
		    return os << e.toString();
	    }
        bool _handled = false;
    };

    // Event dispatcher is just dispatching different event types
    // - Dispatchher system is just a way to dispatch events based on their types
    // - If we receive an event our event function gets called, we receive as a ref.
    // - We can create an insatnce of thihs class with an event that we receive, then we can call the Dispatch function from this class.
    class EventDispatcher{
        template<typename T>
        using EventFunction = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& event) : _event(event) {}

        // This event Dispatch function takes in an Event Function.
        // Where T& can be of any event type like WindowResizeEvent, etc.
        // If the event that your trying to dispatch matches that type of this function
        // Then it will run this function with that function, otherwise return false.
        template<typename T>
        bool Dispatch(EventFunction<T> func){
            // This is so we are not manually checking the Event types.
            if(_event.GetEventType() == T::GetStaticType()){
                _event._handled = func(*(T*)&_event);
                return true;
            }

            return false;
        }

    private:
        Event& _event;
    };

};

namespace fmt{
    template<>
    struct fmt::formatter<RendererEngine::Event> {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

        auto format(const RendererEngine::Event& p, format_context& ctx) const -> format_context::iterator {
            // ctx.out() is an output iterator to write to.
            std::stringstream stream;
            stream << p;
            return format_to(ctx.out(), "{}", stream.str());
        }
    };
};