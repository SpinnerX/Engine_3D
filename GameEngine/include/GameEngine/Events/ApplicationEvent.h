#pragma once
#include <GameEngine/Events/Event.h>
#include <sstream>

namespace RendererEngine{
    // Basically we assign to which event which
    //  Handling events for resizing the window.
    class RENDER_API WindowResizeEvent : public Event{
    public:
        WindowResizeEvent(unsigned int w, unsigned int h) : _width(w), _height(h) {}

        inline unsigned int GetWidth() const { return _width; }
        inline unsigned int GetHeight() const { return _height; }

        std::string toString() const  override {
            // return formatCpp::format("WindowResizeEvent: (w={}, h={})", _width, _height);
            std::stringstream ss;
            ss << "WindowResizeEvent: " << _width << ", " << _height;
            return ss.str();
        }

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::WindowResize; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::WindowResize"; }

        int GetCategoryFlags() const override{
            return EventCategoryApplication;
        }

        friend std::ostream& operator<<(std::ostream& outs, const WindowResizeEvent& e){
            return outs << e.toString();
        }

    private:
        unsigned int _width;
        unsigned int _height;
    };

    // Handling the Window closing event
    // Does not need much data at all.
    class RENDER_API WindowCloseEvent : public Event{
    public:
        WindowCloseEvent() {}

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::WindowClose; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::WindowClose"; }

        int GetCategoryFlags() const override{
            return EventCategoryApplication;
        }
    };


    // Handling the application local events here.
    // Idea is that thhese are here to implement, in case they are actually needed
    // to be implemented.
    class RENDER_API AppTickEvent : public Event{
    public:
        AppTickEvent() {}

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::AppTick; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::AppTick"; }

        int GetCategoryFlags() const override{
            return EventCategoryApplication;
        }
    };

    class RENDER_API AppUpdateEvent : public Event{
    public:
        AppUpdateEvent() {}

        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::AppUpdate; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::AppUpdate"; }

        int GetCategoryFlags() const override{
            return EventCategoryApplication;
        }
    };

    class RENDER_API AppRenderEvent : public Event{
    public:
        // In runtime we want to see what event type this is.
        static EventType GetStaticType() { return EventType::AppRender; }

        // We need another instance of it, so we know what the actual event type it is.
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "EventType::AppRender"; }

        int GetCategoryFlags() const override{
            return EventCategoryApplication;
        }
    };

};
