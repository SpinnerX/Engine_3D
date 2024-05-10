#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <OpenGL/OpenGLWindow.h>

namespace Engine3D{
    static bool _glfwInitialized = false;

    static void GLFWErrorCallback(int error, const char* description){
        coreLogError("GLFWErrorCallback MSG --- (Error Code -> {}): {}", error, description);
    }

    Window* Window::create(const WindowProps& props){
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props){
		RENDER_PROFILE_FUNCTION();

        init(props);
    }

    WindowsWindow::~WindowsWindow(){
		RENDER_PROFILE_FUNCTION();

        shutdown();
    }

    void WindowsWindow::init(const WindowProps& props){
		RENDER_PROFILE_FUNCTION();

        _data.title = props.title;
        _data.width = props.width;
        _data.height = props.height;

        // Logging in window data being passed into this function
        coreLogInfo("Creating Window {}: ({}, {})", props.title, props.width, props.height);

        // // We should check if GLFW is initialized before proceeding
        if(!_glfwInitialized){
			RENDER_PROFILE_SCOPE("glfwInit");
            // TODO: glfwTerminate on system shutfown
            // Have to specify these on macOS
            // to prevent 1200x800 from becoming 2400x1600
            int success = glfwInit();
            // render_core_assert(success, "Could not initialize GLFW!"); // Chhecking if glfw initialized successfully and then set that variable to true
            // Setting an error callback
            glfwSetErrorCallback(GLFWErrorCallback);
            _glfwInitialized = true;
        }

        // Have to specify this as a requirement before using opengl/glad or else you'll get a segfault
        /* glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); */
        /* glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
		
		{
		RENDER_PROFILE_SCOPE("glfwCreateWindow");
        _window = glfwCreateWindow((int)props.width, (int)props.height, _data.title.c_str(), nullptr, nullptr);
		}

        // This is where we are going to initialize the context
        // Telling the renderer context is going to be new OpenGL context (or GraphicsContext)
        // NOTE: Now, thhat this is implemented, this will allow us to if we wanted to get a specific
        //      platform dependent Renderer Context like D3DContext(), then it could do that
        // - Meaning we would just need to implement Init() and SwapBuffers(), to support that platform
        //  and things would work
		_context = CreateScope<OpenGLContext>(_window);
        _context->Init();

        glfwSetWindowUserPointer(_window, &_data);
        setVSync(true);


        // Setting GLFW callbacks
        
        // Starting to dispatch our callback functions to specific event handlers.

        // This is creating a WindowResizeEvent callback
        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int w, int h){
            WindowData& data =  *(WindowData *)glfwGetWindowUserPointer(window);
            data.width = w;
            data.height = h;

            // This is how we are going to be setting the callback from Application to event
            WindowResizeEvent event(w, h);
            data.callback(event);
        });

        // This is how we are dispatching the WindowCloseEvent callbacks to that specific event.
        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window){
            WindowData& data =  *(WindowData *)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.callback(event);
        });

        // - A thing to NOTE is that at some point because this key code is GLFW-specific.
        // We should definitely convert the key code into our engines specific key code because on other platforms
        // - where we don't use GLFW key codes, and do not want to be tied to only using those GLFW key codes.
        // - To basically have our own.
        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int modes){
            WindowData& data =  *(WindowData *)glfwGetWindowUserPointer(window);

            // So when are doing a key call back the function takes in GLFWwindow, int, int, int
            //  action specified what action is occuring when with our key events, like are we pressing, releasing or repeating key events.
            // Event Usage: event(key, count) and for release is event(key)
            switch(action){
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                    data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(static_cast<KeyCode>(key));
                    data.callback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                    data.callback(event);
                    break;
                }
            }
        });

        // Callback for the Key Type Event
        // Essentially for typing text into a text box
        // NOTE: keycode is the character that we are currently typing
        glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keycode){
            WindowData& data =  *(WindowData *)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(static_cast<KeyCode>(keycode));
            data.callback(event);
        });

        // Setting up our event dispatchers for mouse events (ref to more a descriptive explanation at the top for key events as it applied to mouse events)
        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int modes){
            WindowData& data =  *(WindowData *)glfwGetWindowUserPointer(window);

            switch(action){
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                    data.callback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                    data.callback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset){
            WindowData& data =  *(WindowData *)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.callback(event);
        });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos){
            WindowData& data =  *(WindowData *)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.callback(event);
        });
    }

    void WindowsWindow::shutdown(){
		RENDER_PROFILE_FUNCTION();

        glfwDestroyWindow(_window);
    }

    void WindowsWindow::OnUpdate(){
		RENDER_PROFILE_FUNCTION();

        glfwPollEvents();
        _context->swapBuffers(); // The SwapBuffers will handle renderers swap chains
    }

    void WindowsWindow::setVSync(bool enabled){
		RENDER_PROFILE_FUNCTION();

        // we are chhecking if we'd like to enable vsync.
        if(enabled){
            glfwSwapInterval(1);
        }
        else{
            glfwSwapInterval(0);
        }

        _data.vSync = enabled;
    }

};
