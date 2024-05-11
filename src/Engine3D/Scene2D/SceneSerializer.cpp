#include <Engine3D/Engine3DPrecompiledHeader.h>
#include <Engine3D/Scene2D/SceneSerializer.h>
#include <fstream>

namespace YAML{
	/*
	 *
	 * @param convert<> is specialized template for custom type for glm::vec3
	 * @note convert<> will encode and decode to serialize to glm vec's, through template specialization
	*/

	template<>
	struct convert<glm::vec2>{
		static Node encode(const glm::vec2& rhs){
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);

			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs){
			if(!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();

			return true;
		}
	};

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
	 * @note outputting format.
	 * */
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

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

	static std::string RigidBody2DTypeToString(RigidBody2DComponent::BodyType type){
		switch(type){
			case RigidBody2DComponent::BodyType::Static: return "Static";
			case RigidBody2DComponent::BodyType::Dynamic: return "Dynamic";
			case RigidBody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		assert(false);
		return {};
	}

	static RigidBody2DComponent::BodyType RigidBody2DTypeFromString(const std::string& type){
		if(type == "Static") return RigidBody2DComponent::BodyType::Static;
		if(type == "Dynamic") return RigidBody2DComponent::BodyType::Dynamic;
		if(type == "Kinematic") return RigidBody2DComponent::BodyType::Kinematic;
		
		return RigidBody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :  _scene(scene){}
	
	/*
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

		// assert(entity.HasComponent<EntityIDComponent>());
		if(!entity.HasComponent<EntityIDComponent>()){
			assert(false);
		}
		
		//! @note This is where we are going to be storing our Entity UUID.
		// output << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here
		output << YAML::Key << "Entity" << YAML::Value << entity.GetUUID(); // TODO: Entity ID goes here

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

		if(entity.HasComponent<RigidBody2DComponent>()){
			output <<YAML::Key << "RigidBody2DComponent";
			output << YAML::BeginMap;

			auto& r2bdComponent = entity.GetComponent<RigidBody2DComponent>();

			output << YAML::Key << "BodyType" << YAML::Value << RigidBody2DTypeToString(r2bdComponent.type);
			output << YAML::Key << "FixedRotation" << YAML::Value << r2bdComponent.hasFixedRotation;

			output << YAML::EndMap;
		}

		if(entity.HasComponent<BoxCollider2DComponent>()){
			output <<YAML::Key << "BoxCollider2DComponent";
			output << YAML::BeginMap;

			auto& bc2dComponent = entity.GetComponent<BoxCollider2DComponent>();

			output << YAML::Key << "Offset" << YAML::Value << bc2dComponent.offset;
			output << YAML::Key << "Size" << YAML::Value << bc2dComponent.size;
			output << YAML::Key << "Density" << YAML::Value << bc2dComponent.density;
			output << YAML::Key << "Friction" << YAML::Value << bc2dComponent.friction;
			output << YAML::Key << "Resitution" << YAML::Value << bc2dComponent.restitution;
			output << YAML::Key << "ResitutionThreshold" << YAML::Value << bc2dComponent.restitutionThreshold;

			output << YAML::EndMap;
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

		//! @note retrieving a node called entities
		//! @note  Checking if we have entities to serialize!
		auto entities = data["Entities"];

		if(entities){
			for(auto entity : entities){
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // @note TODO: Creating a better UUID system.

				std::string name;
				auto tagComponent = entity["Tag Component"];

				if(tagComponent) // if tag component is present.
					name = tagComponent["Tag"].as<std::string>();

				coreLogTrace("Deserializing entitiy with ID = {0}, name = {1}", uuid, name);

				// Entity deserializeEntity = _scene->createEntity(name); // @note TODO createEntity(uuid,name);
				Entity deserializeEntity = _scene->CreateEntityWithUUID(uuid, name);
				
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

				auto rigitbody2DComponent = entity["RigidBody2DComponent"];

				if(rigitbody2DComponent){
					auto& rb2d = deserializeEntity.AddComponent<RigidBody2DComponent>();
					rb2d.type = RigidBody2DTypeFromString(rigitbody2DComponent["BodyType"].as<std::string>());
					rb2d.hasFixedRotation = rigitbody2DComponent["FixedRotation"].as<bool>();
				}

				auto rigidbody2DColliderComponent = entity["BoxCollider2DComponent"];

				if(rigidbody2DColliderComponent){
					auto& bc2d = deserializeEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.offset = rigidbody2DColliderComponent["Offset"].as<glm::vec2>();
					bc2d.size = rigidbody2DColliderComponent["Size"].as<glm::vec2>();
					bc2d.density = rigidbody2DColliderComponent["Density"].as<float>();
					bc2d.density = rigidbody2DColliderComponent["Friction"].as<float>();
					bc2d.restitution = rigidbody2DColliderComponent["Resitution"].as<float>();
					bc2d.restitutionThreshold = rigidbody2DColliderComponent["ResitutionThreshold"].as<float>();
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
