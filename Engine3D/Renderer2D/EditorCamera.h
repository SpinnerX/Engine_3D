#pragma once
#include <Engine3D/Graphics/Camera.h>
#include <Engine3D/Core/Timestep.h>
#include <Engine3D/Event/Event.h>
#include <Engine3D/Event/MouseEvent.h>

#include <glm/glm.hpp>

namespace Engine3D{
	
	/*
	 *
	 * @function EditorCamera
	 *
	 * @param yaw
	 * @note rotation around central vertical (normal) axis
	 *
	 * @param pitch
	 * @note rotation around central horizontal (lateral) axis
	 *
	 * @param near clip
	 * @note objects that are rendered at a further distance
	 *
	 * @param far clip
	 * @note objects that are rendered at a closer distance
	 *
	 * @param focal point
	 * @note sharpness and depth of field
	 *
	 * @function panSpeed
	 * @note handles in computing the panspeed of the camera
	 *
	 * */
	class EditorCamera : public Camera{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);
		
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline float getDistance() const { return _distance; }
		inline void setDistance(float dist) { _distance = dist; }

		inline void setViewportSize(float w, float h) { _viewportWidth = w, _viewportHeight = h; updateProjection(); }

		const glm::mat4& getViewMatrix() const { return _viewMatrix; }

		glm::mat4 getViewProjection() const { return _projectionMatrix * _viewMatrix; }
		

		glm::vec3 getUpDirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;
		const glm::vec3& getPosition() const { return _position; }
		glm::quat getOrientation() const;

		float getPitch() const { return _pitch; }
		float getYaw() const { return _yaw; }

	private:
		void updateProjection();

		void updateView();

		bool onMouseScroll(MouseScrolledEvent& e);

		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(float delta);

		glm::vec3 calculatePosition() const;
		
		/*
		 *
		 * @note dependent on how big the viewport is.
		 * */
		std::pair<float, float> panSpeed() const;
		float rotationSpeed() const;
		float zoomSpeed() const;

	private:
		float _fov=45.0f, _aspectRatio = 1.778f, _nearClip=0.1f, _farClip=1000.0f;

		glm::mat4 _viewMatrix;
		glm::vec3 _position = {0.0f, 0.0f, 0.0f};
		glm::vec3 _focalPoint = {0.0f, 0.0f, 0.0f};

		glm::vec2 _initialMousePosition = {0.0f, 0.0f};

		float _distance = 10.0f;
		float _pitch = 0.0f, _yaw=0.0f;

		float _viewportWidth=1280.0f, _viewportHeight=720.0f;
	};
};
