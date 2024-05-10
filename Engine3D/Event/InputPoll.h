// #pragma once
// #include <Engine3D/Core/core.h>
// #include <Engine3D/Event/KeyCodes.h>
// #include <Engine3D/Event/MouseButtonCodes.h>

// namespace Engine3D{
// 	struct MousePosition{
// 		float xpos = 0.0f;
// 		float ypos = 0.0f;
// 	};

// 	class ENGINE_API InputPoll {
// 	public:
// 		inline static bool isKeyPressed(KeyCode keycode){
// 			return _instance->isKeyPressedImpl(keycode);
// 		}
		
// 		inline static bool isMouseButtonPressed(Mouse button){
// 			return _instance->isMouseButtonPressedImpl(button);
// 		}

// 		inline static MousePosition getMousePosition(){
// 			return _instance->getMousePositionImpl();
// 		}
		
// 		inline static float getMouseX(){
// 			return _instance->getMouseXImpl();
// 		}

// 		inline static float getMouseY(){
// 			return _instance->getMouseYImpl();
// 		}

// 	protected:
// 		virtual bool isKeyPressedImpl(Key keycode) = 0;
// 		virtual bool isMouseButtonPressedImpl(Mouse button) = 0;
// 		virtual MousePosition getMousePositionImpl() = 0;
		
// 		virtual float getMouseXImpl() = 0;
// 		virtual float getMouseYImpl() = 0;

// 	private:
// 		static InputPoll* _instance;
// 	};
// };
#pragma once
#include <Engine3D/Core/core.h>
#include <Engine3D/Event/KeyCodes.h>
#include <Engine3D/Event/MouseCodes.h>
#include <glm/glm.hpp>

namespace Engine3D{
    class ENGINE_API InputPoll{
    public:

        //! @note Key/Mouse event pressed!
        static bool IsKeyPressed(KeyCode keycode);

        static bool IsMousePressed(MouseCode mouseCode);

        //! @note Mouse Position
        static glm::vec2 GetMousePos();

        static float GetMouseX();

        static float GetMouseY();

    private:
        static InputPoll* instance;
    };
};