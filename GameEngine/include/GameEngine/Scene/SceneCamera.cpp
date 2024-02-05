#include <GameEnginePrecompiledHeader.h>
#include <GameEngine/Scene/SceneCamera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace RendererEngine{

	
	SceneCamera::SceneCamera() {
		recalculateProjection();
	}

	void SceneCamera::setOrthographic(float size, float nearClip, float farClip){
		_projectionType = ProjectionType::Orthographic;
		_orthographicSize = size;
		_orthographicNear = nearClip;
		_orthographicFar = farClip;
		recalculateProjection();
	}
	
	void SceneCamera::setPerspective(float size, float nearClip, float farClip){
		_projectionType = ProjectionType::Perspective;
		_perspectiveFOV = size;
		_perspectiveNear = nearClip;
		_perspectiveFar = farClip;
		recalculateProjection();
	}
	
	void SceneCamera::setViewportSize(uint32_t width, uint32_t height){
		_aspectRatio = (float)width / (float)height;
		recalculateProjection();
	}
	
	void SceneCamera::recalculateProjection(){
		if(_projectionType == ProjectionType::Perspective){
			_projectionMatrix = glm::perspective(_perspectiveFOV, _aspectRatio, _perspectiveNear, _perspectiveFar);
		}
		else{
			float orthoLeft = -_orthographicSize * _aspectRatio * 0.5f;
			float orthoRight = _orthographicSize * _aspectRatio * 0.5f;
			float orthoBottom = -_orthographicSize * 0.5f;
			float orthoTop = _orthographicSize * 0.5f;

			_projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, _orthographicNear, _orthographicFar);
		}

	}
};
