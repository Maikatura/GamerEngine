#include "GraphicsEngine.pch.h"
#include <Scene/SceneSerializer.h>
#include "Model/Entity.h"
#include "Components/Components.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>

#include "StringCast.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Components/CameraController.h"

namespace YAML
{
	template<>
	struct convert<Vector2f>
	{
		static Node encode(const Vector2f& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Vector2f& rhs)
		{
			if(!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Vector3f>
	{
		static Node encode(const Vector3f& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Vector3f& rhs)
		{
			if(!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Vector4f>
	{
		static Node encode(const Vector4f& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, Vector4f& rhs)
		{
			if(!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

YAML::Emitter& operator<<(YAML::Emitter& out, const Vector2f& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Vector3f& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Vector4f& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

SceneSerializer::SceneSerializer(Scene* scene)
	: myScene(scene)
{}

static void SerializeEntity(YAML::Emitter& out, Entity entity)
{
	(entity.HasComponent<IDComponent>());


	out << YAML::BeginMap; // Entity
	out << YAML::Key << "Entity" << YAML::Value << (uint64_t)entity.GetUUID();

	if(entity.HasComponent<TagComponent>())
	{
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap; // TagComponent

		auto& tag = entity.GetComponent<TagComponent>().Tag;
		out << YAML::Key << "Tag" << YAML::Value << tag;

		out << YAML::EndMap; // TagComponent
	}

	if(entity.HasComponent<TransformComponent>())
	{
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap; // TransformComponent

		auto& tc = entity.GetComponent<TransformComponent>();
		out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
		out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
		out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

		out << YAML::EndMap; // TransformComponent
	}

	if(entity.HasComponent<CameraComponent>())
	{
		auto& cameraComp = entity.GetComponent<CameraComponent>();
		out << YAML::Key << "CameraComponent" << YAML::Value;
		out << YAML::BeginMap; // Camera
		out << YAML::Key << "FOV" << YAML::Value << cameraComp.myFoV;
		out << YAML::Key << "NearPlane" << YAML::Value << cameraComp.myNearPlane;
		out << YAML::Key << "FarPlane" << YAML::Value << cameraComp.myFarPlane;
		out << YAML::Key << "Primary" << YAML::Value << cameraComp.Primary;
		out << YAML::EndMap; // Camera
	}

	if (entity.HasComponent<ModelComponent>())
	{
		auto& model = entity.GetComponent<ModelComponent>();

		out << YAML::Key << "ModelComponent";
		out << YAML::BeginMap; // ModelComponent

		out << YAML::Key << "Path" << YAML::Value << Helpers::string_cast<std::string>(model.myModel->GetModel()->GetName());


		out << YAML::EndMap; // ModelComponent
	}
	

	out << YAML::EndMap; // Entity
}

void SceneSerializer::Serialize(const std::string& filepath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "SceneTesting";
	out << YAML::Key << "Path" << YAML::Value << filepath.c_str();
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	myScene->GetRegistry().each([&](auto entityID)
		{
			Entity entity = { entityID, myScene };
			if(!entity)
				return;

			SerializeEntity(out, entity);
		});
	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(filepath);
	fout << out.c_str();
}

void SceneSerializer::SerializeRuntime(const std::string& filepath)
{
	// Not implemented

}

bool SceneSerializer::Deserialize(const std::string& filepath)
{
	YAML::Node data;
	try
	{
		data = YAML::LoadFile(filepath);
	}
	catch(YAML::ParserException e)
	{
		return false;
	}

	if(!data["Scene"])
		return false;

	std::string sceneName = data["Scene"].as<std::string>();

	if (data["Path"])
	{
		myScene->SetPath(data["Path"].as<std::string>());
	}
	

	auto entities = data["Entities"];
	if(entities)
	{
		for(auto entity : entities)
		{
			uint64_t uuid = entity["Entity"].as<uint64_t>();

			std::string name;
			auto tagComponent = entity["TagComponent"];
			if(tagComponent)
				name = tagComponent["Tag"].as<std::string>();




			Entity deserializedEntity = myScene->CreateEntityWithUUID(uuid, name);

			auto transformComponent = entity["TransformComponent"];
			if(transformComponent)
			{
				// Entities always have transforms
				auto& tc = deserializedEntity.GetComponent<TransformComponent>();
				tc.Translation = transformComponent["Translation"].as<Vector3f>();
				tc.Rotation = transformComponent["Rotation"].as<Vector3f>();
				tc.Scale = transformComponent["Scale"].as<Vector3f>();
			}

			const auto cameraComponent = entity["CameraComponent"];
			if(cameraComponent)
			{
				auto& camera = deserializedEntity.AddComponent<CameraComponent>();
				deserializedEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
				camera.myFoV = cameraComponent["FOV"].as<float>();
				camera.myNearPlane = cameraComponent["NearPlane"].as<float>();
				camera.myFarPlane = cameraComponent["FarPlane"].as<float>();
				camera.Primary = cameraComponent["Primary"].as<bool>();
			}

			auto modelComponent = entity["ModelComponent"];
			if (modelComponent)
			{
				auto& modelComp = deserializedEntity.AddComponent<ModelComponent>();
				auto path = Helpers::string_cast<std::wstring>(modelComponent["Path"].as<std::string>());

				modelComp.myModel = ModelAssetHandler::GetModelInstance(path);
			}

		}
	}

	return true;
}

bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
{
	return false;
}