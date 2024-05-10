#pragma once
#include <Engine3D/Renderer2D/OrthographicCamera.h>
#include <Engine3D/Core/Timestep.h>
#include <Engine3D/Event/ApplicationEvent.h>
#include <Engine3D/Event/Event.h>
#include <Engine3D/Event/MouseEvent.h>


namespace Engine3D{
	// Knowing bounds of the camera itself.	
	struct OrthographicCameraBounds{
		OrthographicCameraBounds() = default;
		OrthographicCameraBounds(float _left, float _right, float _bottom, float _top) : left(_left), right(_right), bottom(_bottom), top(_top) {}

		float left;
		float right;
		float bottom;
		float top;

		float getWidth() { return right - left; }
		float getHeight() { return top - bottom; }
	};

	class OrthographicCameraController{
	public:	
		OrthographicCameraController(float aspectRatio, bool rotation=false); // vertically when camera starts up that there is 2 units of space (then to get correct aspect ratio, have: aspectratio * 2 units)
		
		// void getProjection();
		
		void OnUpdate(Timestep ts);
		void onEvent(Event& e);

		void onResize(float width, float height);

		OrthographicCamera& GetCamera(){ return camera; }
		OrthographicCamera GetCamera() const { return camera; }

		const OrthographicCameraBounds& GetBounds() const;
		
		float GetZoomLevel() const;
		void SetZoomLevel(float level);

	private:
		void CalculateView();
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);

	private:
		float aspectRatio;
		float zoomLevel = 1.f; // Higher zoom level is, the further out you zoomed inwards.
		OrthographicCamera camera;
		OrthographicCameraBounds bounds;

		bool rotation;
		glm::vec3 cameraPosition = {0.f, 0.f, 0.f};
		float cameraRotation = 0.f;
		float cameraTranslationSpeed = 5.0f, cameraRotationSpeed = 180.0f;
	};
};
