#pragma once
#include <glm/glm.hpp>

namespace Engine3D{
	/*
	 *
	 * Camera class
	 * - Renderer kind of camera
	 * - Has a projection and view matrix.
	 * - Acts as the Camera Component, not the orthographic camera itself.
	 *
	 *
	 * */
	class Camera{
	public:
		Camera() = default;
		Camera(const glm::mat4& projecMat) : _projectionMatrix(projecMat) {}
		virtual ~Camera() = default;

		const glm::mat4& getProjection() const { return _projectionMatrix; }

	protected:
		glm::mat4 _projectionMatrix = glm::mat4(1.0f);
	};
};
