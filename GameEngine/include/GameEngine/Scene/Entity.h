#pragma once
#include <GameEngine/Scene/Scene.h>
#include <GameEngine/Entt/entt.h>

namespace RendererEngine{
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
		T& addComponent(Args&&... args){
			// @note when adding component, will be a callback to scene that will handle particular event that could happen if adding a component
			T& component = _scene->_registry.emplace<T>(_entityHandler, std::forward<Args>(args)...);

			// @note meaning that adding a particular component to the scene and handling it (specifically entity)
			_scene->onComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& getComponent() {
			// Checking if entity contains this component.
			/* if(!this->hasComponent<T>()){ */
			/* 	coreLogError("Entity does not have component!"); */
			/* } */

			return _scene->_registry.get<T>(_entityHandler);
		}
		
		// Checking if entity has a component
		template<typename T>
		bool hasComponent(){
			// has<T> has been renamed to all_of<T>
			return _scene->_registry.all_of<T>(_entityHandler);
		}

		template<typename T>
		void removeComponent(){
			// Checking if the component exists
			if(this->hasComponent<T>()){
				coreLogWarn("Entity does not have component!");
			}
			
			render_core_assert(hasComponent<T>(), "Entity does not have component");

			_scene->_registry.remove<T>(_entityHandler);
		}

		operator bool() const  { return _entityHandler != entt::null; }

		operator entt::entity() const { return _entityHandler; }

		operator uint32_t() const { return (uint32_t)_entityHandler; }
		
		bool operator==(const Entity& other) const { return _entityHandler == other._entityHandler and _scene == other._scene; }
		
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity _entityHandler{entt::null};
		Scene* _scene = nullptr; // 12 bytes
	};
};
