#pragma once
#include <Engine3D/Scene2D/Scene.h>
#include <Engine3D/Core/EngineLogger.h>
#include <Engine3D/Entt/entt.h>

namespace Engine3D{
	/*
	 * Entity
	 * @param id - the ID of the entity itself
	 * @note actual wrapper for entity for ECS
	 * */
	class Entity{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity&) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args){
			// @note when adding component, will be a callback to scene that will handle particular event that could happen if adding a component
			T& component = scene->_registry.emplace<T>(entityHandler, std::forward<Args>(args)...);

			// @note meaning that adding a particular component to the scene and handling it (specifically entity)
			scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent() {
			// Checking if entity contains this component.
			/* if(!this->HasComponent<T>()){ */
			/* 	coreLogError("Entity does not have component!"); */
			/* } */

			return scene->_registry.get<T>(entityHandler);
		}
		
		// Checking if entity has a component
		template<typename T>
		bool HasComponent(){
			// has<T> has been renamed to all_of<T>
			return scene->_registry.all_of<T>(entityHandler);
		}

		template<typename T>
		void RemoveComponent(){
			// Checking if the component exists
			/* if(this->HasComponent<T>()){ */
			/* 	coreLogWarn("Entity does not have component!"); */
			/* } */
			
			/* render_core_assert(HasComponent<T>(), "Entity does not have component"); */

			scene->_registry.remove<T>(entityHandler);
		}

		operator bool() const  { return entityHandler != entt::null; }

		operator entt::entity() const { return entityHandler; }

		operator uint32_t() const { return (uint32_t)entityHandler; }
		
		bool operator==(const Entity& other) const { return entityHandler == other.entityHandler and scene == other.scene; }
		
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity entityHandler{entt::null};
		Scene* scene = nullptr; // 12 bytes
	};
};
