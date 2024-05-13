#pragma once
#include <Engine3D/Core/core.h>
#include <Engine3D/Scene2D/SceneCamera.h>
#include <Engine3D/Graphics/Texture.h>
#include <Engine3D/Core/UUID.h>
#include <string>

namespace Engine3D{

	//! @note TODO --- Find a better way at storing UUID's for Entities
	//! @note For now we treat this UUID's for entities as a component
	struct EntityIDComponent{
		UUID id;

		EntityIDComponent() = default;
		EntityIDComponent(const UUID& uid) : id(uid) {}
		EntityIDComponent(const EntityIDComponent&) = default;
	};
	
	struct TagComponent{
		std::string tag;

		TagComponent() = default;
		TagComponent(const std::string& t) : tag(t){}
		TagComponent(const TagComponent&) = default;
	};

	struct TransformComponent{
		/* glm::mat4 transform = glm::mat4(1.0f); */
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& t) : translation(t) {}
			
		glm::mat4 GetTransform() const;
	};

	//! @note These rendering components enable users to set specific informations that they would like to apply to their quads
	struct SpriteRendererComponent{
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

		Ref<Texture2D> texture; // SpriteRendererComponent will contain a way for user to allow to apply textures
		float tilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : color(color){}
	};

	struct CircleRendererComponent{
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		float thickness = 1.0f; //! @note Setting circle thickness of filled into the circle
		// float fade = 0.005f;
		float fade = 1.0f;


		Ref<Texture2D> texture; // SpriteRendererComponent will contain a way for user to allow to apply textures
		float tilingFactor = 1.0f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
		CircleRendererComponent(const glm::vec4& color) : color(color){}
	};
	
	/*
	 * Camera Component
	 * - Editor would not have a component itself.
	 * - Rather that the camera would be a component with an entity.
	 *
	 * Quick NOTE
	 * - There are two types of camera in your scenes.
	 * - During game runtime, you can divide cams in two different cameras.
	 *		0.) Cameras that essentially should be rendering some kind of view of the scene, directly tied to render output to our viewport.
	 *			- Like making a 1920x1080, you want the camera to fill your screen.
	 *			- Minimizing, making the window smaller, or somewhat modifying the window.
	 *		1.) Fixed aspect ratio
	 *		2.) non-fixed aspect ratio
	 *			- Main camera 
	 *
	 * */
	struct CameraComponent{

		SceneCamera camera;

		// TODOL moving to scene.
		bool isPrimary = true; // this allows to see if this current camera is your primary camera.
		bool fixedAspectRatio = false; // Can check if true in editor meaning that the aspect ratio is fixedd. (Meaning the aspect ratio wont be changed)

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	//! @note Forward Declaration
	class ScriptableEntity;
	struct NativeScriptComponent{
		ScriptableEntity* instance = nullptr;
		
		ScriptableEntity*(*instantiateScript)(); // Usage: Just instantiates to the type we are binding (equivalent to doing std::function<void(ScriptableEntity *)>; called instantiateScript.)
		
		/*
		 *
		 *
		 *	@note takes a ptr to itself.
		 *
		 * */
		void (*destroyScript)(NativeScriptComponent *); // Usage: Destroys instance we are binding.
		
		/*
		 *
		 * @note Create new instance of this particular passed being passed.
		 * @note Deletes that instance, setting it to nullptr.
		 */
		template<typename T>
		void Bind(){
			instantiateScript = []() { return static_cast<ScriptableEntity *>(new T()); };
			destroyScript = [](NativeScriptComponent* nsc) { delete nsc; nsc=nullptr; };
		}
	};

	// Physics Component
	// @TODO: move physics material

	// Represent our body type for physics
	struct RigidBody2DComponent{
		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;

		enum class BodyType{
			Static=0,Dynamic=1, Kinematic
		};

		BodyType type = BodyType::Static;
		bool hasFixedRotation = false;
		void* runtimeBody=nullptr; // @note is actually going to be storing the body for the runtime.
		float tilingFactor = 1.0f;
	};

	struct BoxCollider2DComponent{
		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f; // @note velocity on which is the threshold when the box is bounding and the speed is below this threshold for slowly stopping it's bouncing.

		// @note offset from collider to middle of the entity.
		glm::vec2 offset = {0.0f, 0.f};
		glm::vec2 size = {0.5f, 0.5f};


		void* runtimeFixture = nullptr;
	};
};
