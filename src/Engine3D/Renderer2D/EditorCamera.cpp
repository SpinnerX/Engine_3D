#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Renderer2D/EditorCamera.h>

namespace Engine3D{
    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) 
		: _fov(fov), _aspectRatio(aspectRatio), _nearClip(nearClip), _farClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)){
		updateView();
	}
    
    void EditorCamera::OnUpdate(Timestep ts){
		const glm::vec2& mouse = glm::vec2{InputPoll::GetMouseX(), InputPoll::GetMouseY()};
		if(InputPoll::IsKeyPressed(KeyCode::LeftAlt)){
			// const glm::vec2& mouse = glm::vec2{InputPoll::GetMouseX(), InputPoll::GetMouseY()};

			glm::vec2 delta = (mouse - _initialMousePosition) * 0.003f;

			_initialMousePosition = mouse;

			if(InputPoll::IsMousePressed(Mouse::ButtonMiddle))
				mousePan(delta);
			else if(InputPoll::IsMousePressed(Mouse::ButtonLeft))
				mouseRotate(delta);
			else if(InputPoll::IsMousePressed(Mouse::ButtonRight))
				mouseZoom(delta.y);
		}

		// glm::vec2 delta = (mouse - _initialMousePosition) * 0.003f;
		// glm::vec2 delta;

		//! @note Q key in editor is used for navigating pov 
		//! @note E key that will be used for 

		// if(InputPoll::IsKeyPressed(ENGINE_KEY_W)){
		// 	// delta.x++;
		// 	delta.x += (mouse.x - _initialMousePosition.x) * 0.003f;
		// }

		// if(InputPoll::IsKeyPressed(ENGINE_KEY_A)){
		// 	delta.y += (mouse.y - _initialMousePosition.y) * 0.003f;
		// }

		// if(InputPoll::IsKeyPressed(ENGINE_KEY_S)){

		// }

		// if(InputPoll::IsKeyPressed(ENGINE_KEY_D)){

		// }

		// if(InputPoll::IsKeyPressed(ENGINE_KEY_Q)){}
		// if(InputPoll::IsKeyPressed(ENGINE_KEY_E)){}



		updateView();
	}

    void EditorCamera::OnEvent(Event& e){
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(Engine3D::bind(this, &EditorCamera::onMouseScroll));
	}

    glm::vec3 EditorCamera::getUpDirection() const {
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
    
    glm::vec3 EditorCamera::getRightDirection() const{
		return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}
    
    glm::vec3 EditorCamera::getForwardDirection() const{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

    glm::quat EditorCamera::getOrientation() const{
		return glm::quat(glm::vec3(-_pitch, -_yaw, 0.0f));
	}

    void EditorCamera::updateProjection(){
		_aspectRatio = _viewportWidth / _viewportHeight;
		_projectionMatrix = glm::perspective(glm::radians(_fov), _aspectRatio, _nearClip, _farClip);
		updateView();
	}

    void EditorCamera::updateView(){
		_position = calculatePosition();
		
		glm::quat orientation = getOrientation();
		_viewMatrix = glm::translate(glm::mat4(1.0f), _position) * glm::toMat4(orientation);
		_viewMatrix = glm::inverse(_viewMatrix);
	}

    bool EditorCamera::onMouseScroll(MouseScrolledEvent& e){
		float delta = e.GetYOffset() * 0.1f;
		mouseZoom(delta);
		updateView();
		return false;
	}

    void EditorCamera::mousePan(const glm::vec2& delta){
		auto[xSpeed, ySpeed] = panSpeed();

		_focalPoint += -getRightDirection() * delta.x * xSpeed * _distance;
		_focalPoint += getUpDirection() * delta.y * ySpeed * _distance;
	}
    
    void EditorCamera::mouseRotate(const glm::vec2& delta){
		float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
		_yaw += yawSign * delta.x * rotationSpeed();
		_pitch += delta.y * rotationSpeed();
	}
    
    void EditorCamera::mouseZoom(float delta){
		_distance -= delta * zoomSpeed();

		if(_distance < 1.0f){
			_focalPoint += getForwardDirection();
			_distance = 1.0f;
		}
	}

    glm::vec3 EditorCamera::calculatePosition() const{
		return _focalPoint - getForwardDirection() * _distance;
	}

    std::pair<float, float> EditorCamera::panSpeed() const{
		float x = std::min(_viewportWidth / 1000.0f, 2.4f); // max = 2.4.f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
		
		float y = std::min(_viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return {xFactor, yFactor};
	}
    
    float EditorCamera::rotationSpeed() const{ return 0.8f; }

    float EditorCamera::zoomSpeed() const{
		float distance = _distance * 0.2f;
		distance = std::max(distance, 0.0f);

		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}
	
};
