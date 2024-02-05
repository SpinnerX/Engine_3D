#pragma once
#include <GameEngine/Scene/Entity.h>

namespace RendererEngine{
	/*
	 * class: ScriptableEntity
	 * @note All scripting should be part of ScriptableEntity
	 *
	 *
	 * */
	class ScriptableEntity{
	public:
		
		// @note When deleting pointer to this destructor we can delete that point in memory as a ScriptableEntity.
		virtual ~ScriptableEntity(){}

		template<typename  T>
		T& getComponent(){
			if(!_entity.hasComponent<T>()){
				coreLogError("ScriptComponent Entity with this component could not be found");
			}

			return _entity.getComponent<T>();
		}
		
		virtual void onCreate(){}

		virtual void onUpdate(Timestep ts){}
		
		virtual void onDestroy(){}

	private:
		Entity _entity;
		friend class Scene;
	};	
};
