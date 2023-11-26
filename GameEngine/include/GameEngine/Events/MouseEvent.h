#pragma once
#include <GameEngine/Events/Event.h>

namespace RendererEngine{
    // MouseMovedEvent
    // (x,y) - is the location of the mouse
    class RENDER_API MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y) : _mouseX(x), _mouseY(y) {}

        inline float GetX() const { return _mouseX; }
        inline float GetY() const { return _mouseY; }

        std::string toString() const override {
            // return formatCpp::format("MouseMovedEvent: ({}, {})", _mouseX, _mouseY);
            std::stringstream ss;
            ss << fmt::format("MouseMovedEvent: ({}, {})", _mouseX, _mouseY);
            return ss.str();
        }

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::MouseMoved; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::MouseMoved"; }

        int GetCategoryFlags() const override{
            return EventCategoryMouse | EventCategoryInput;
        }

    private:
        float _mouseX;
        float _mouseY;
    };

    // Same thing as MouseMovedEvent, instead we take the offset of x and y.
    // offset being where we scrolled
    // Handling x and y because there is horizontal scrolling that we can do.
    class RENDER_API MouseScrolledEvent : public Event{
    public:
        MouseScrolledEvent(float xOffset, float yOffset) : _xOffset(xOffset), _yOffset(yOffset) {}

        inline float GetXOffset() const { return _xOffset; }
        inline float GetYOffset() const { return _yOffset; }

        std::string toString() const override {
            // return formatCpp::format("MouseScrolledEvent: ({}, {})", _xOffset, _yOffset);
            std::stringstream ss;
            ss << fmt::format("MouseScrolledEvent: ({}, {})", _xOffset, _yOffset);
            return ss.str();
        }

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::MouseScrolled; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::MouseScrolled"; }

        int GetCategoryFlags() const override{
            return EventCategoryMouse | EventCategoryInput;
        }


    private:
        float _xOffset;
        float _yOffset;
    };

    // This will represent an actual base class that should not be created
    // Hence why the constructor is protected
    // Since this is just a base class for MouseButtonPressed and MouseButtonReleased
    class RENDER_API MouseButtonEvent : public Event{
    public:
        inline int GetMouseButton() const { return _button; }

        // We want to set by default that whenever we are pressing/releasing the category is that this is
        // mouse and grabbing input
        int GetCategoryFlags() const override{
            return EventCategoryMouse | EventCategoryInput;
        }

    protected:
        // protected because we shouldn't be allowed to create this MouseButtonEvent.
        // Only MouseButtonPressed and MouseButtonReleased can use this.
        MouseButtonEvent(int button) : _button(button) {}

        int _button; // containing our button hhere
    };

    class RENDER_API MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        std::string toString() const override {
            // return formatCpp::format("MouseButtonPressedEvent: {}", _button);
            std::stringstream ss;
            ss << fmt::format("MouseButtonPressedEvent: {}", _button);
            return ss.str();
        }

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::MouseButtonPressed; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::MouseButtonPressed"; }
    };

    class RENDER_API MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        std::string toString() const override {
            // return formatCpp::format("MouseButtonReleasedEvent: {}", _button);
            std::stringstream ss;
            ss << fmt::format("MouseButtonReleasedEvent: {}", _button);
            return ss.str();
        }

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::MouseButtonReleased; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::MouseButtonReleased"; }
    };
};