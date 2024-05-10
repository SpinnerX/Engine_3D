#pragma once
#include <Engine3D/Graphics/Camera.h>

namespace Engine3D{
	/*
	 *
	 * SceneCamera
	 * @note essentially going to be this camera derived.
	 * @note its just the Scene Camera will be specified to having parameters.
	 * @note including functionality such as supporting multiple camera views.
	 * @note other things that make up this scene camera.
	 * @note make sure to set viweportSize before rendering.
	 *
	 *
	 * */
	class SceneCamera : public Camera{
	public:
		// @note storing different camera pov.
		enum class ProjectionType { Perspective=0, Orthographic=1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		
		void setOrthographic(float size, float nearClip, float farClip);
		void setPerspective(float size, float nearClip, float farClip);
		void setViewportSize(uint32_t width, uint32_t height);

		// @note setting perpective values.
		float getPerspectiveVerticalFOV() const { return perspectiveFOV; }
		void setPerspectiveVerticalFOV(float verticalFov ) { perspectiveFOV = verticalFov; recalculateProjection(); }
		
		float getPerspectiveNearClip() const { return perspectiveNear; }
		void setPerspectiveNearClip(float nearClip) { perspectiveNear = nearClip; recalculateProjection(); }
		
		float getPerspectiveFarClip() const { return perspectiveFar; }
		void setPerspectiveFarClip(float farClip) { perspectiveFar = farClip; recalculateProjection(); }
		
		float getOrthographicSize() const { return orthographicSize; }
		void setOrthographicSize(float size) { orthographicSize = size; recalculateProjection(); }
	
		// @note nearClip for orthographic
		// @note farClip for orthographic
		float getOrthoNearClip() const { return orthographicNear; }
		void setOrthoNearClip(float nearClip) { orthographicNear = nearClip; recalculateProjection(); }

		float getOrthoFarClip() const { return orthographicFar; }
		void setOrthoFarClip(float farClip) { orthographicFar = farClip; recalculateProjection(); }

		ProjectionType getProjectionType() const { return _projectionType; }
		
		void setProjectionType(ProjectionType type) { _projectionType = type; recalculateProjection(); }
	private:
		void recalculateProjection();

	private:
		ProjectionType _projectionType = ProjectionType::Orthographic;
		
		// @note for perspective camera.
		float perspectiveFOV = glm::radians(45.0f);
		float perspectiveNear = 0.01;
		float perspectiveFar = 1000.0f;

		// @note for orthographic camera
		float orthographicSize = 10.0f;
		float orthographicNear = -1.0f;
		float orthographicFar = 1.0f;




		// When the viewport size gets changed the entire projection matrix needs to be recalculated.
		float _aspectRatio = 1.0f; // Need to keep track of viewport size.
	};
};
