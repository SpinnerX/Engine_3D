#include "Application.h"
#include <GameEngine/platforms/Windows/WindowsInput.h>
#include <GameEngine/platforms/OpenGL/OpenGLVertexArray.h>
#include <GameEngine/Renderer/Renderer.h>

namespace RendererEngine{
    Application* Application::_instance = nullptr;
    
    Application::Application(){
        render_core_assert(!_instance, "Application already exists!");
        isRunning = true;
        _instance = this;
        _window = std::unique_ptr<Window>(Window::create());

        auto bind_function = [](auto* instance, auto M){
            return [instance, M](auto&& arg1){
                return (instance->*M)(std::forward<decltype(arg1)>(arg1));
            };
        };
        _window->setEventCallback(bind_function(this, &Application::onEvent));
        _imguiLayer = new ImGuiLayer();
        pushOverlay(_imguiLayer);

        // Vertex Array
        // Vertex Buffer
        // Index Buffer

        // gen vertex array and bind them
        // glGenVertexArrays(1, &_vertexArr);
        // glBindVertexArray(_vertexArr);
        _vertexArray.reset(VertexArray::Create());
        

        // Now we are populating it with some data
        // -0.5 is a quarter of the way from left side of the screen
        // 0.5f is the other (right) side of the window.
        // {x, y, z}
        float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

        // The goal - VertexBuffer::Create returns specific rendering API depending on specic platform dependent rendering API
        // VertexBuffer buffer = Vertex::Buffer(sizeof(vertices), vertices, GL_STATIC_DRAW);
        // buffer.bind();
        std::shared_ptr<VertexBuffer> _vertexBuffer;
        _vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

       BufferLayout bufLayout = {
            {ShaderDataType::Float3, "a_Position", true},
            {ShaderDataType::Float4, "a_Color", true},
       };

       _vertexBuffer->setLayout(bufLayout);
       _vertexArray->addVertexBuffer(_vertexBuffer);

        std::shared_ptr<IndexBuffer> _indexBuffer;
        uint32_t indices[3] = {0, 1, 2};
        _indexBuffer.reset(IndexBuffer::Create(indices,  sizeof(indices) / sizeof(uint32_t)));
        _vertexArray->setIndexBuffer(_indexBuffer);
        

        //////////////////////////
        // Square Vertex Data //
        //////////////////////////

        float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
             -0.75f,  0.75f, 0.0f,
		};

        _squareVertexArrays.reset(VertexArray::Create());
        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVB->setLayout({
            {ShaderDataType::Float3, "a_Position", true},
        });
        _squareVertexArrays->addVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices,  sizeof(squareIndices) / sizeof(uint32_t)));
        _squareVertexArrays->setIndexBuffer(squareIB);



        std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

        _shader.reset(new Shader(vertexSrc, fragmentSrc));

        std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

        _blueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
    }

    Application::~Application(){}

    void Application::pushLayer(Layer* layer){
        _layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* layer){
        _layerStack.pushOverlay(layer);
        layer->onAttach();
    }


    void Application::onEvent(Event& event){
        EventDispatcher dispatcher(event);
        // In order for dispatcher to tell which event to execute, this is where that happens

        // NOTE
        // - Dispatcher checks the incoming type event is the same as the static type in the Dispatch function
        //  then we execute that specific callback corresponding to that event.
        auto bind_function = [](auto* instance, auto member_function){
            return [instance, member_function](auto&& arg1){
                return (instance->*member_function)(std::forward<decltype(arg1)>(arg1));
            };
        };
        dispatcher.Dispatch<WindowCloseEvent>(bind_function(this, &Application::onWindowClose));

        // Iterating backwards thhrough the layer stack and thhen we called onEvent, and if it isn;t handled thhen it breaks 
        // If not a layer and an overlay then we do not continue.
        auto iter = _layerStack.end();
        while(iter != _layerStack.begin()){
            (*--iter)->onEvent(event);

            if(event._handled) break;
        }

    }


    void Application::Run(){

        while(isRunning){
            RendererCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
            RendererCommand::clear();

            Renderer::beginScene(); // BeginScene

            _blueShader->bind();
            _squareVertexArrays->bind();
            Renderer::submit(_squareVertexArrays);

            _shader->bind();
            _vertexArray->bind();
            Renderer::submit(_vertexArray); // Submitting our  objects or even meshes (or geo meshes)

            Renderer::endScene(); // EndScene

            for(Layer* layer : _layerStack){
                layer->onUpdate();
            }

            _imguiLayer->begin();
            for(Layer* layer : _layerStack){
                layer->onImguiRender();
            }
            _imguiLayer->end();

            _window->onUpdate();
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e){
        isRunning = false;
        return true;
    }
};