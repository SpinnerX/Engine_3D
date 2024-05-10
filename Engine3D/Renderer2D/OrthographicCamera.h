#pragma once
#include <glm/glm.hpp>

namespace Engine3D{

    // Orthographic Camera
    // - Contains projection matrix, and transform matrix
    // - Orthographic cameras is instead of having a frustrum
    //      instead it has essentially a rectangular prism (or cube)
    // - Orhographic cameras also have a near and far bounds.
    // - typically for orthographic rendering -1 is for near, and 1 is for far (default that'll be used)
    // - U don't really rotate them like along the z axis, though if you wanted to do a rotation like that then you can keep
    //   _rotation as float, potentially
    // - How do we put the projection matrix into the shader
    //  - This is where uniforms come in.
    //  - Uniform simply is a per draw call value that you can set to your shader
	//  ** NOTE **
	//		- Our representation of  an orthographic camera.
    class OrthographicCamera{
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
		
		void setProjection(float left, float right, float bottom, float top);

        const glm::vec3 getPosition() const { return _position; }
        float getRotation() const { return _rotation; }

        // Allowing setting camera position in the worlds
        void setPosition(const glm::vec3& position) {
            _position = position;
            recalculateViewMatrix();
        }

        void setRotation(float rotation){
            _rotation = rotation;
            recalculateViewMatrix();
        }

        const glm::mat4& getProjectionMatrix() const { return _projectionMatrix; }
        const glm::mat4& getViewMatrix() const { return _viewMatrix; }
        const glm::mat4& getViewProjectionMatrix() const { return _viewProjectionMatrix; }

    private:
        // Function call whenever we set something
        // when setting rotation, we recalculate. Or when we set position, then we can also recalculate that as well
        // Something that can happen after we changed actual transform matrix.
        void recalculateViewMatrix();

    private:
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix; // essentially when we calculate our camera is at, we want to recalculate this view mat. (Has to be inverse of transform mat of camera)

        glm::mat4 _viewProjectionMatrix;

        glm::vec3 _position = {0.f, 0.f, 0.f}; // Keeping track of camera location
        float _rotation = 0.0f; // Camera rotation

    };
};
