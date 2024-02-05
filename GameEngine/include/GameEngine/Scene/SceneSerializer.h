#pragma once
#include <GameEngine/Scene/Scene.h>

namespace RendererEngine{
	/*
	 *
	 *
	 * @note SceneSerializer for loading/saving scenes.
	 *
	 *
	 * */
	class SceneSerializer{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		
		// @note
		void serializer(const std::string& filepath);

		// @note serializeRuntime and deserializeRuntime serializes into binary
		void serializerRuntime(const std::string& filepath);

		bool deserialize(const std::string& filepath);

		bool deserializeRuntime(const std::string& filepath);
	private:
		Ref<Scene> _scene;
	};
};
