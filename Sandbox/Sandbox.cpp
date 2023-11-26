#include <GameEngine/GameEngine.h>
// #include <glm/glm.hpp>
// #include <glm/vec3.hpp>  // glm::vec3
// #include <glm/vec4.hpp>  // glm::vec4
// #include <glm/mat4x4.hpp> // glm::mat4
// #include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::persepctive

// glm::mat4 camera(float translate, glm::vec2 const& rotate){
//     glm::mat4 projection = glm::perspective(glm::radians(45.f), 4.f / 3.f, 0.1f, 100.f);
//     glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -translate));
//     view = glm::rotate(view, rotate.y, glm::vec3(-1.f, 0.f, 0.f));
//     view = glm::rotate(view, rotate.x, glm::vec3(0.f, 1.f, 0.f));

//     glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(0.5f));

//     return projection * view * model;
// }

// This is just an example on how to make a layer
class ExampleLayer : public RendererEngine::Layer{
public:
    ExampleLayer() : Layer("Example"){
        // auto cam = camera(5.f, {0.5f, 0.5f});
    }

    void onUpdate() override {
        clientLogInfo("ExampleLayer::Update");


        if(RendererEngine::InputPoll::isKeyPressed(RENDER_KEY_TAB)){
            clientLogTrace("Tab key has been pressed!\n");
        }
    }

    void onEvent(RendererEngine::Event& event) override{
        clientLogTrace("{}", event);
        if(event.GetEventType() == RendererEngine::EventType::KeyPressed){
            RendererEngine::KeyPressedEvent& e = (RendererEngine::KeyPressedEvent&)event;
            clientLogTrace("{}\n", (char)e.GetKeyCode());
        }
    }
};

class Sandbox : public RendererEngine::Application{
public:
    Sandbox() {
        pushLayer(new ExampleLayer());
        // pushOverlay(new RendererEngine::ImGuiLayer());
    }

    ~Sandbox() {}
};

RendererEngine::Application* RendererEngine::CreateApplication(){
    return new Sandbox();
}