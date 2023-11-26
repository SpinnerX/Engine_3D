#pragma once
#include <GameEngine/Events/Event.h>

namespace RendererEngine{
    // KeyEvent Base Class
    class RENDER_API KeyEvent : public Event{
    public:
        inline virtual int GetKeyCode() const {
            return _keyCode;
        }

        int GetCategoryFlags() const override{
            return EventCategoryKeyboard | EventCategoryInput;
        }

    protected:
        // Have this protected constructor
        // Because you shouldn't be able to create a KeyEvent, essentially
        KeyEvent(int keycode) : _keyCode(keycode) {}

        int _keyCode;
    };

    // Handling when a key event has been pressed.  (It is a type of KeyEvent)
    // repeatCount is just a handler if the user decides to press the key for who knows how long
    // Thinking of hhaving repeatCount.
    class RENDER_API KeyPressedEvent : public KeyEvent{
    public:
        KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), _repeatCount(repeatCount) {}

        inline int GetRepeatCount() const { return _repeatCount; }

        std::string toString() const override{
            std::stringstream ss;
            // ss << formatCpp::format("KeyPressed Event: {} ({} repeats)",  _keyCode, _repeatCount);
            // return formatCpp::format("KeyPressed Event: {} ({} repeats)",  _keyCode, _repeatCount);
            ss << fmt::format("KeyPressedEvent: {} ({} repeats)", _keyCode, _repeatCount);
            return ss.str();
        }

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::KeyPressed; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::KeyPressed"; }

    private:
        int _repeatCount;
    };

    class  RENDER_API KeyReleasedEvent : public KeyEvent{
    public:
        KeyReleasedEvent(int keycode) : KeyEvent(keycode){}

        std::string toString() const override {
            // return formatCpp::format("KeyReleasedEvent: {}", _keyCode);
            std::stringstream ss;
            ss << fmt::format("KeyReleasedEvent: {}", _keyCode);
            return ss.str();
        }

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::KeyReleased; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::KeyReleased"; }
    };

    class RENDER_API KeyTypedEvent : public KeyEvent{
    public:
        KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

        std::string toString() const override{
            std::stringstream ss;
            ss << fmt::format("KeyTypedEvent: {}", _keyCode);
            return ss.str();
        }

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::KeyTyped; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::KeyTyped"; }
    };

};