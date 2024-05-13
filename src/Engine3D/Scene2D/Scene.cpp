#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Scene2D/Scene.h>

namespace Engine3D{
	static b2BodyType engineRigidBody2DTypeToBox2DType(RigidBody2DComponent::BodyType type){
		switch (type) {
			case Engine3D::RigidBody2DComponent::BodyType::Static: return b2_staticBody;
			case Engine3D::RigidBody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
			case Engine3D::RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}
		
		coreLogWarn("Unknown Body Type");
		assert(false);

		return b2_staticBody;
	}

	//! @note Used for allowing us to copy components to entities
	//! @note Since entities may contain multiple components.
	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap){

		// auto view = src.view<Component>();

		// //! @note Copying our types of components to our Entities
		// //! @note Utiizing the UUID component
		// //! @note How we communicate via UUID to add our components to our entities
		// for(auto e : view){
		// 	UUID uuid = src.get<EntityIDComponent>(e).id;

		// 	assert((enttMap.find(uuid) != enttMap.end()));

		// 	entt::entity dstEnttID = enttMap.at(uuid);
		// 	auto& component = src.get<Component>(e);

		// 	//! @note emplace_or_replace copies/replaces a component of our entity
		// 	dst.emplace_or_replace<Component>(dstEnttID, component);
		// 	// dst.emplace<Component>(enttID, component);
		// 	// dst.emplace_or_replace(enttID, component);
		// }
		auto view = src.view<Component>();
		for (auto e : view){
			UUID uuid = src.get<EntityIDComponent>(e).id;
			assert(enttMap.find(uuid) != enttMap.end());
			entt::entity dstEnttID = enttMap.at(uuid);

			auto& component = src.get<Component>(e);
			dst.emplace_or_replace<Component>(dstEnttID, component);
		}

	}

	Scene::Scene(){
	}

	Scene::~Scene(){}

	Ref<Scene> Scene::Copy(Ref<Scene> other){
		// std::unordered_map<UUID, entt::entity> enttMap;
		// Ref<Scene> newScene = CreateRef<Scene>();

		// newScene->_viewportWidth = other->_viewportWidth;
		// newScene->_viewportHeight = other->_viewportHeight;

		// auto& sourceScene = other->_registry;
		// auto& destScene = newScene->_registry;
		// auto idView = sourceScene.view<EntityIDComponent>();

		// //! @note Creating entities to our new scene
		// for(auto e : idView){
		// 	//! @note Creating our new entities but will still contain same UUID's
		// 	UUID uuid = sourceScene.get<EntityIDComponent>(e).id;
		// 	const auto& name = sourceScene.get<TagComponent>(e).tag;
		// 	// Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
		// 	Entity newEntity = newScene->createEntity(name);
		// 	enttMap[uuid] = (entt::entity)newEntity;
		// }

		Ref<Scene> newScene = CreateRef<Scene>();
		newScene->_viewportWidth = other->_viewportWidth;
		newScene->_viewportHeight = other->_viewportHeight;

		auto& srcSceneRegistry = other->_registry;
		auto& dstSceneRegistry = newScene->_registry;

		std::unordered_map<UUID, entt::entity> enttMap;
		auto idView = srcSceneRegistry.view<EntityIDComponent>();
		for (auto e : idView){
			UUID uuid = srcSceneRegistry.get<EntityIDComponent>(e).id;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		//! @note Copying our components (except ID Comopnent and Tag Component)
		/**
		 * @note TODO --- Probably implement a better way of copying components
		*/
		CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<RigidBody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}
	
	Entity Scene::createEntity(const std::string& name){
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name){
		Entity entity = {_registry.create(), this};
		entity.AddComponent<EntityIDComponent>(uuid); //! @note Should just automate generating UUID's
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;

		return entity;
	}
	
	void Scene::destroyEntity(Entity entity){
		_registry.destroy(entity);
	}

	//! @note Starts our runtime physics simulation
	void Scene::OnRuntimeStart(){
		physicsWorld = new b2World({0.0f, -9.8f});

		// Going through ECS for rigit body 2D components
		auto view = _registry.view<RigidBody2DComponent>();

		for(auto e : view){
			// Creating our entities.
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
			
			// Defining our body
			b2BodyDef bodyDef;
			// Converting to different body type.
			bodyDef.type = engineRigidBody2DTypeToBox2DType(rb2d.type);
			bodyDef.position.Set(transform.translation.x, transform.translation.y);
			bodyDef.angle = transform.rotation.z;
			
			// Creating our body
			b2Body* body = physicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.hasFixedRotation);
			rb2d.runtimeBody = body; // @note definig that this predeffined body is going to be our runtime.

			if(entity.HasComponent<BoxCollider2DComponent>()){
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				
				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.size.x * transform.scale.x, bc2d.size.y * transform.scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.density;
				fixtureDef.friction = bc2d.friction;
				fixtureDef.restitution = bc2d.restitution;
				fixtureDef.restitutionThreshold = bc2d.restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts){
		// physicsWorld = new b2World({0.0f, -9.8f});
		{
			/*
			 *
			 * @note instead of doing [=] has been deprecated, so doing [=, this]
			 * @note [=, this] means that we are not making a copy of entity.
			 *
			 * @note TODO: Move to ScrenePlay::onScrenePlay
			 * */
			_registry.view<NativeScriptComponent>().each([=, this](auto entity, auto& nativeScriptComponent){
				// Checking for valid instance.
				// @note At beginning of each scene onUpdate()
				// @note Any entity that has NativeScriptComponent will have onCreate and onUpdate function called.
				if(!nativeScriptComponent.instance){
					nativeScriptComponent.instance = nativeScriptComponent.instantiateScript();
					nativeScriptComponent.instance->_entity = {entity, this};

					nativeScriptComponent.instance->onCreate();
				}

				nativeScriptComponent.instance->OnUpdate(ts); // TODO: Should be called in onSceneStop.
			});
		}


		// @note Doing Physics...
		// @note conceptually this is it, for simulating the physics each frame.
		{
			const int32_t velocityIterations=6;
			const int32_t positionIterations=2;
			physicsWorld->Step(ts, velocityIterations, positionIterations);

			/* physicsWorld->Step(ts, 6, 2); */
			auto view  = _registry.view<RigidBody2DComponent>();
			for(auto e : view){
				// @note rendering and grabbing transforms to render in the right place.
				// @TODO: Probably have some UUID to grab a specific entities body for physics in the future.
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

				b2Body* body = (b2Body *)rb2d.runtimeBody;
				const auto& pos = body->GetPosition();
				transform.translation.x = pos.x;
				transform.translation.y = pos.y;
				transform.rotation.z = body->GetAngle();
			}
		}



		// Rendering 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = _registry.view<TransformComponent, CameraComponent>();

			for(auto entity : view){
				auto[transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if(camera.isPrimary){
					mainCamera = &camera.camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}
		
		// Checking mainCamera exists, then if the scene does not contain camera then do not render camera.
		if(mainCamera){
			Renderer2D::Begin(mainCamera->getProjection(), cameraTransform);
			
			//! @note Drawing Sprites
			// {
			auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for(auto entity : group){
				auto[transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.color);
			}

			//! @note Drawing Circles
			{
			auto view = _registry.view<TransformComponent, CircleRendererComponent>();

			for(auto entity : view){
				auto[transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

				Renderer2D::DrawCircle(transform.GetTransform(), circle.color, circle.thickness, circle.fade, (int)entity);
			// }	
			} // End of Scope for this group of code
			
			}
			
			Renderer2D::End();
		}

	}
	
	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera){
		Renderer2D::Begin(camera);

		//! @note Drawing Sprites
		{
		auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for(auto entity : group){
			auto[transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			/* Renderer2D::DrawQuad(transform.getTransform(), sprite.color); */
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}
		}// End of Scope for this group of code

		//! @note Drawing Circles
		{
		auto view = _registry.view<TransformComponent, CircleRendererComponent>();

		for(auto entity : view){
			auto[transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

			Renderer2D::DrawCircle(transform.GetTransform(), circle.color, circle.thickness, circle.fade, (int)entity);
		}	
		} // End of Scope for this group of code

		Renderer2D::End();
		
	}
	
	void Scene::onViewportResize(uint32_t width, uint32_t height){
		_viewportWidth = width;
		_viewportHeight = height;

		// Going through all entities that have the CameraComponent
		auto view = _registry.view<CameraComponent>();

		for(auto entity: view){
			auto& cameraComponent = view.get<CameraComponent>(entity);
			
			// @note resizing non fixed aspect ratio cameras.
			// @note Checking if the user through the editor wants to change the fixed aspect ratio (resizing)
			// @note probably the only time this wouldn't work is if adding a new camera component just after the scenes resize.
			// @note viewport able to now modify based on current added camera component.
			if(!cameraComponent.fixedAspectRatio){
				cameraComponent.camera.setViewportSize(_viewportWidth, _viewportHeight);
			}
			
		}
	}

	Entity Scene::getPrimaryCamera(){
		auto view = _registry.view<CameraComponent>();
		for(auto entity : view){
			const auto& camera = view.get<CameraComponent>(entity);

			if(camera.isPrimary){
				return Entity{entity, this};
			}
		}

		return {};
	}

	void Scene::OnRuntimeStop(){
		// resetting.
		coreLogWarn("OnRuntimeStop() was called!");
		delete physicsWorld;
		physicsWorld = nullptr;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component){
		assert(false);
	}

	template<>
	void Scene::OnComponentAdded<EntityIDComponent>(Entity entity, EntityIDComponent& component){

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component){}
	
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component){
		if(_viewportWidth > 0 and _viewportHeight > 0)
			component.camera.setViewportSize(_viewportWidth, _viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component){
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component){
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component){
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component){
	}
	
	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component){
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component){
	}
};
