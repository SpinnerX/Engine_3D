#pragma once
#include <Engine3D/Scene2D/Entity.h>

namespace Engine3D{
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
		T& GetComponent(){
			if(!_entity.HasComponent<T>()){
				coreLogError("ScriptComponent Entity with this component could not be found");
			}

			return _entity.GetComponent<T>();
		}
		
		virtual void onCreate(){}

		virtual void OnUpdate(Timestep ts){}
		
		virtual void onDestroy(){}

	private:
		Entity _entity;
		friend class Scene;
	};	
};
