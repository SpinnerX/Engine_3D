#pragma once

#include <Engine3D/Core/core.h>
#include <Engine3D/Graphics/Camera.h>
#include <glm/glm.hpp>

namespace Engine3D{
	

	/*
	 * @class Renderer3D
	 * 
	 * @function Initialize();
	 * @note Initializes the renderer when when users chooses to begin a scene.
	 * @note Initializing Cameras, and various other components the renderer may require
	 *
	 * @function Shutdown();
	 * @note Handles the cleanup when the scene ends.
	 * 
	 * @function setClearColor(glm::vec4)
	 * @note Refreshes the frame
	 *
	 * @function beginScene(camera)
	 * @note Everytime we begin a scene we pass in the instance of the camera we are passing inwards.
	 * @note Probably should remove the camera instance and probably just do, Renderer3D::begin() instead.
	 * @note For the camera probably do, auto& camera = Camera::getInstance().
	 * @note Probably by having Camera::getInstance() implemented by the other camera instances that use this.
	 *
	 *
	 *
	*/

	class ENGINE_API Renderer3D{
		struct Statistics{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;

			uint32_t getTotalVertexCount() { return quadCount * 4; }
			uint32_t getTotalIndexCount() { return quadCount * 6; }
		};
	public:
		
		// @note Used for initializing a camera.
		// @note Potential user API would like the following:
		// @API USAGE --> Renderer3D::init<EditorCamera>();
		// @note Which will potentially call ((T *)component)->attach();
		// @note The idea behind this is to allow for initializing a camera component
		static void init(Ref<Camera>& camera);

		static void begin();

		static void end();

		static void setClearColor(const glm::vec4 color = glm::vec4( 0.1f, 0.1f, 0.1f, 1 ));

		static Statistics getStats();

	private:
		// @note This is to intialize at the beginning of every scene
		static void initializeCore();
		
		// @note handling cleaning up after ending every scene
		static void shutdown();

		static void flush();
		static void refresh(); // @note Probably have this both flush and reset
		

		static void resetStats(); // @note Resetting stats to their initial values
	};
};
