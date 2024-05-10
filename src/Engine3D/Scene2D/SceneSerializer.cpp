#define GLM_ENABLE_EXPERIMENTAL
#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Scene2D/SceneSerializer.h>
#include <Engine3D/Scene2D/Entity.h>
#include <Engine3D/Scene2D/Components.h>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML{
	/*
	 *
	 * @param convert<> is specialized template for custom type for glm::vec3
	 * @param encode will encode glm::vec3
	 * @param decode will decode glm::vec3
	 *
	 *
	 *
	 * */
	template<>
	struct convert<glm::vec3>{
		static Node encode(const glm::vec3& rhs){
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs){
			if(!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4>{

		static Node encode(const glm::vec4& rhs){
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs){
			if(!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
};
namespace Engine3D{

	/*
	 *
	 * @note outputting format.
	 vec3 = [1, 2, 3]
	 vec4 = [1, 2, 3, 4]
	 *
	 *
	 *
	 * */
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z  << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :  _scene(scene){
	}
	
	/*
	 *
	 *
	 * @param key - represents what is it we are serializing, such as the name specifier.
	 * @param value - is the actual value of the type we are serializing.
	 * @param BeginMap - begins to start storing everything and printing it.
	 * @note TODO: modularize the way we are serializing thesee components since there are lots of redundancy to this
	 * @note probably doing something like:
	   template<typename Component, typename Function>
	   static void serializeEntity(const std::string& componentName, Entity entity){	
		output << YAML::BeginMap; // Entity
		output << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here
		output << YAML::EndMap;

		if(entity.HasComponent<Component>()){
			Function(out); // Where out is going to be passed by reference that will handle the information related to these components.
			out << YAML::EndMap;
		}
	   }
	 * */
	static void SerializeEnttiy(YAML::Emitter& output, Entity entity){
		output << YAML::BeginMap; // Entity
		output << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here

		if(entity.HasComponent<TagComponent>()){
			output <<  YAML::Key << "Tag Component";
			output << YAML::BeginMap;
			
			auto& tag = entity.GetComponent<TagComponent>().tag;
			output << YAML::Key << "Tag" << YAML::Value << tag;

			output << YAML::EndMap; // TagComponent
		}

		if(entity.HasComponent<TransformComponent>()){
			output << YAML::Key   << "TransformComponent";
			output << YAML::BeginMap;
			
			auto& tc = entity.GetComponent<TransformComponent>();
			output << YAML::Key << "Translation" << YAML::Value << tc.translation;
			output << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
			output << YAML::Key << "Scale" << YAML::Value << tc.scale;

			output << YAML::EndMap; // TransformComponent
		}

		if(entity.HasComponent<CameraComponent>()){
			output << YAML::Key << "CameraComponent";
			output << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;

			output << YAML::Key << "Camera" << YAML::Value;
			output << YAML::BeginMap; // Camera Info
			output << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
			output << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
			output << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
			output << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
			output << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
			output << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthoNearClip();
			output << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthoFarClip();
			output << YAML::EndMap; // Camera Info

			output << YAML::Key << "Primary" << YAML::Value << cameraComponent.isPrimary;
			output << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;
			output << YAML::EndMap;
		}

		if(entity.HasComponent<SpriteRendererComponent>()){
			output << YAML::Key << "SpriteRendererComponent";
			output << YAML::BeginMap;

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();

			output << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;

			output << YAML::EndMap; // SpriteRendererComponent
		}

		output << YAML::EndMap; // Entity
	}
	
	void SceneSerializer::serializer(const std::string& filepath){
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		_scene->_registry.each([&](auto entityID){
			Entity entity = {entityID, _scene.get() };

			if(!entity){
				return;
			}

			SerializeEnttiy(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::serializerRuntime(const std::string& filepath){
	}

	bool SceneSerializer::deserialize(const std::string& filepath){
		std::ifstream stream(filepath);
		std::stringstream ss;
		ss << stream.rdbuf();

		YAML::Node data = YAML::Load(ss.str());
		if(!data["Scene"]){
			coreLogWarn("Expecting a scene for deserializer. Scene Node Required.");
			return false;
		}
		
		/* std::string sceneName = data["Scene"].as<std::string>(); */

		// @node retrieving a node called entities
		auto entities = data["Entities"];

		if(entities){
			for(auto entity : entities){
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // @note TODO: Creating a better UUID system.

				std::string name;
				auto tagComponent = entity["Tag Component"];

				if(tagComponent) // if tag component is present.
					name = tagComponent["Tag"].as<std::string>();

				coreLogTrace("Deserializing entitiy with ID = {0}, name = {1}", uuid, name);

				Entity deserializeEntity = _scene->createEntity(name); // @note TODO createEntity(uuid,name);
				
				// Deserializing Transform Component.
				auto transformComponent = entity["TransformComponent"];
				if(transformComponent){
					// @note entities will always have transforms.
					auto& tc = deserializeEntity.GetComponent<TransformComponent>();
					tc.translation = transformComponent["Translation"].as<glm::vec3>();
					tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.scale = transformComponent["Scale"].as<glm::vec3>();
				}

				// Deserializing Camera Component
				auto cameraComponent = entity["CameraComponent"];
				if(cameraComponent){
					auto& cc = deserializeEntity.AddComponent<CameraComponent>();
					auto cameraProps = cameraComponent["Camera"];
					cc.camera.setProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
					
					cc.camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.camera.setOrthoNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.camera.setOrthoFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.isPrimary = cameraComponent["Primary"].as<bool>();
					cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				// Deserializing Sprite Renderer Component.
				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if(spriteRendererComponent){
					auto& src = deserializeEntity.AddComponent<SpriteRendererComponent>();
					src.color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			}
		}
		return true;
	}

	bool SceneSerializer::deserializeRuntime(const std::string& filepath){
		render_core_assert(false);

		return false;
	}
};
