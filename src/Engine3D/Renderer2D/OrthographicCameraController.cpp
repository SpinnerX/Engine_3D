#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Renderer2D/OrthographicCameraController.h>

namespace Engine3D{
	
	// Quick NOTE: Redundant and should probably change how we initialize, because for some reason bounds keeps getting considered uninitialized when initializing  with constructor using : bounds(...) 
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) : aspectRatio(aspectRatio), rotation(rotation), camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel){
        bounds = OrthographicCameraBounds(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
        camera = OrthographicCamera(bounds.left, bounds.right, bounds.bottom, bounds.top);
	}

	// OrthographicCamera& OrthographicCameraController::GetCamera() { return camera; }
	// OrthographicCamera OrthographicCameraController::GetCamera() const { return camera; }

	const OrthographicCameraBounds& OrthographicCameraController::GetBounds() const { return bounds; }

	float OrthographicCameraController::GetZoomLevel() const { return zoomLevel; }

	void OrthographicCameraController::SetZoomLevel(float level) {
		zoomLevel = level;
		CalculateView();
	}

	// A way to control our cameras
	void OrthographicCameraController::OnUpdate(Timestep ts){
		RENDER_PROFILE_FUNCTION();

        if(InputPoll::IsKeyPressed(ENGINE_KEY_D)){ // RIGHT
			cameraPosition.x -= std::cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * ts;
			cameraPosition.y -= std::sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * ts;
        }
        else if(InputPoll::IsKeyPressed(ENGINE_KEY_A)){ // LEFT
			cameraPosition.x += std::cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * ts;
			cameraPosition.y += std::sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * ts;
        }
        else if(InputPoll::IsKeyPressed(ENGINE_KEY_S)){ // UP
			cameraPosition.x += -std::sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * ts;
			cameraPosition.y += std::cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * ts;
        }
        else if(InputPoll::IsKeyPressed(ENGINE_KEY_D)){ // DOWN
			cameraPosition.x -= -std::sin(glm::radians(cameraRotation)) * cameraTranslationSpeed * ts;
			cameraPosition.y -= std::cos(glm::radians(cameraRotation)) * cameraTranslationSpeed * ts;
        }
		
		// Checking if we set the rotation speed
		if(rotation){

			if(InputPoll::IsKeyPressed(ENGINE_KEY_Q)){ // Rotate Right
				cameraRotation -= cameraRotationSpeed * ts;
			}

			if(InputPoll::IsKeyPressed(ENGINE_KEY_E)){ // Rotate Left
				cameraRotation += cameraRotationSpeed * ts;
			}
			
			if(cameraRotation > 180.0f)
				cameraRotation -= 360.0f;
			else if(cameraRotation <= -180.0f)
				cameraRotation += 360.0f;

			// Then we set the camera rotation
			camera.setRotation(cameraRotation);
		}
		
		camera.setPosition(cameraPosition);
		cameraTranslationSpeed = zoomLevel;
	}

	void OrthographicCameraController::onEvent(Event& e){
		RENDER_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(bind(this, &OrthographicCameraController::onMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(bind(this, &OrthographicCameraController::onWindowResized));
	}
	
	void OrthographicCameraController::onResize(float width, float height){
		aspectRatio = width / height;
		this->CalculateView(); // recalculates the view when we change and resize the camera controller's w and h.
	}
		
	void OrthographicCameraController::CalculateView(){
		bounds = { -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel };
		camera.setProjection(bounds.left, bounds.right, bounds.bottom, bounds.top);
	}
	
	
	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e){
		RENDER_PROFILE_FUNCTION();

		zoomLevel -= e.GetYOffset() * 0.25f; // Modify thihs const offset for window resized event event
		
		zoomLevel = std::max(zoomLevel, 0.25f); // This allows us to control our offsets for our zooming level, making sure we don't zoom too far our or zooming in too close.
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e){
		RENDER_PROFILE_FUNCTION();
		this->onResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
};
