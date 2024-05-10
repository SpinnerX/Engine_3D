#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Renderer2D/OrthographicCamera.h>

namespace Engine3D{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) : _projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), _viewMatrix(1.0f) {
		RENDER_PROFILE_FUNCTION();
        _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
    }

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top){
		RENDER_PROFILE_FUNCTION();
		_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); // Reseetting project and view matrix
        _viewProjectionMatrix = _projectionMatrix * _viewMatrix; // Need to reset the orthographic matrix once we set the projection.
	}

    void OrthographicCamera::recalculateViewMatrix(){
		RENDER_PROFILE_FUNCTION();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position)
							  * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0, 0, 1));

        _viewMatrix = glm::inverse(transform);
        _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
    }
};
