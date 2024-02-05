#pragma once
#include <GameEngine/Renderer/Camera.h>

namespace RendererEngine{
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
		float getPerspectiveVerticalFOV() const { return _perspectiveFOV; }
		void setPerspectiveVerticalFOV(float verticalFov ) { _perspectiveFOV = verticalFov; recalculateProjection(); }
		
		float getPerspectiveNearClip() const { return _perspectiveNear; }
		void setPerspectiveNearClip(float nearClip) { _perspectiveNear = nearClip; recalculateProjection(); }
		
		float getPerspectiveFarClip() const { return _perspectiveFar; }
		void setPerspectiveFarClip(float farClip) { _perspectiveFar = farClip; recalculateProjection(); }
		
		float getOrthographicSize() const { return _orthographicSize; }
		void setOrthographicSize(float size) { _orthographicSize = size; recalculateProjection(); }
	
		// @note nearClip for orthographic
		// @note farClip for orthographic
		float getOrthoNearClip() const { return _orthographicNear; }
		void setOrthoNearClip(float nearClip) { _orthographicNear = nearClip; recalculateProjection(); }

		float getOrthoFarClip() const { return _orthographicFar; }
		void setOrthoFarClip(float farClip) { _orthographicFar = farClip; recalculateProjection(); }

		ProjectionType getProjectionType() const { return _projectionType; }
		
		void setProjectionType(ProjectionType type) { _projectionType = type; recalculateProjection(); }
	private:
		void recalculateProjection();

	private:
		ProjectionType _projectionType = ProjectionType::Orthographic;
		
		// @note for perspective camera.
		float _perspectiveFOV = glm::radians(45.0f);
		float _perspectiveNear = 0.01;
		float _perspectiveFar = 1000.0f;

		// @note for orthographic camera
		float _orthographicSize = 10.0f;
		float _orthographicNear = -1.0f;
		float _orthographicFar = 1.0f;




		// When the viewport size gets changed the entire projection matrix needs to be recalculated.
		float _aspectRatio = 1.0f; // Need to keep track of viewport size.
	};
};
