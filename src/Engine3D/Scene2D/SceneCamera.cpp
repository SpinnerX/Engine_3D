#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Scene2D/SceneCamera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine3D{

	
	SceneCamera::SceneCamera() {
		recalculateProjection();
	}

	void SceneCamera::setOrthographic(float size, float nearClip, float farClip){
		_projectionType = ProjectionType::Orthographic;
		orthographicSize = size;
		orthographicNear = nearClip;
		orthographicFar = farClip;
		recalculateProjection();
	}
	
	void SceneCamera::setPerspective(float size, float nearClip, float farClip){
		_projectionType = ProjectionType::Perspective;
		perspectiveFOV = size;
		perspectiveNear = nearClip;
		perspectiveFar = farClip;
		recalculateProjection();
	}
	
	void SceneCamera::setViewportSize(uint32_t width, uint32_t height){
		_aspectRatio = (float)width / (float)height;
		recalculateProjection();
	}
	
	void SceneCamera::recalculateProjection(){
		if(_projectionType == ProjectionType::Perspective){
			_projectionMatrix = glm::perspective(perspectiveFOV, _aspectRatio, perspectiveNear, perspectiveFar);
		}
		else{
			float orthoLeft = -orthographicSize * _aspectRatio * 0.5f;
			float orthoRight = orthographicSize * _aspectRatio * 0.5f;
			float orthoBottom = -orthographicSize * 0.5f;
			float orthoTop = orthographicSize * 0.5f;

			_projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthographicNear, orthographicFar);
		}

	}
};
