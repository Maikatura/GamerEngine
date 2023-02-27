#include "GraphicsEngine.pch.h"
#include "Scene.h"
#include <Renderer/Scene/SceneSerializer.h>
#include "Renderer/Model/Entity.h"
#include "Components/Components.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "StringCast.h"
#include "Renderer/AssetHandlers/ModelAssetHandler.h"
#include "Renderer/Particles/ParticleEmitter.h"
#include <Renderer/Light/DirectionalLight.h>
#include <Renderer/Light/EnvironmentLight.h>
#include <entt.hpp>
#include "Components/ChildComponent.h"

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

	if(entity.HasComponent<ChildComponent>())
	{
		out << YAML::Key << "ChildComponent";
		out << YAML::BeginMap; // ChildComponent

		auto& cc = entity.GetComponent<ChildComponent>();

		if(cc.HasParent())
		{
			out << YAML::Key << "Parent" << YAML::Value << cc.GetParent().GetUUID();
		}

		out << YAML::Key << "ChildrenSize" << YAML::Value << cc.GetChildren().size();
		out << YAML::Key << "Children";
		out << YAML::BeginMap; // Children

		for(int i = 0; i < cc.GetChildren().size(); i++)
		{
			out << YAML::Key << i << YAML::Value << cc.GetChildren()[i].GetUUID();
		}

		out << YAML::EndMap; // Children

		out << YAML::EndMap; // ChildComponent
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

	if(entity.HasComponent<ModelComponent>())
	{
		auto& model = entity.GetComponent<ModelComponent>();

		out << YAML::Key << "ModelComponent";
		out << YAML::BeginMap; // ModelComponent

		out << YAML::Key << "Path" << YAML::Value << Helpers::string_cast<std::string>(model.GetModel()->GetModel()->GetName());
		out << YAML::Key << "Texture";
		out << YAML::BeginMap;

		auto material = model.GetModel()->GetModel()->GetMaterial();

		if(material)
		{
			auto albedoTex = material->GetAlbedoTexture();
			if(albedoTex)
			{
				out << YAML::Key << "Albedo" << YAML::Value << Helpers::string_cast<std::string>(albedoTex->GetPath());
			}

			auto normalTex = material->GetNormalTexture();
			if(normalTex)
			{
				out << YAML::Key << "Normal" << YAML::Value << Helpers::string_cast<std::string>(normalTex->GetPath());
			}

			auto materialTex = material->GetMaterialTexture();
			if(materialTex)
			{
				out << YAML::Key << "Material" << YAML::Value << Helpers::string_cast<std::string>(materialTex->GetPath());
			}
		}


		out << YAML::EndMap;

		out << YAML::EndMap; // ModelComponent
	}

	if(entity.HasComponent<ParticleEmitter>())
	{
		auto& particle = entity.GetComponent<ParticleEmitter>();
		const auto& particleData = particle.GetEmitterSettings();
		const auto settings = particle.GetSettings();

		out << YAML::Key << "ParticleComponent";
		out << YAML::BeginMap; // ParticleEmitter

		out << YAML::Key << "EmitterSettingsSize" << YAML::Value << particleData.size();
		out << YAML::Key << "EmitterSettings";
		out << YAML::BeginMap;
		for(size_t i = 0; i < particleData.size(); i++)
		{
			out << YAML::Key << i;
			out << YAML::BeginMap;

			out << YAML::Key << "MinFrequency" << YAML::Value << particleData[i].myMinFrequency;
			out << YAML::Key << "MaxFrequency" << YAML::Value << particleData[i].myMaxFrequency;

			out << YAML::Key << "MinSpeed" << YAML::Value << particleData[i].myMinSpeed;
			out << YAML::Key << "MaxSpeed" << YAML::Value << particleData[i].myMaxSpeed;

			out << YAML::Key << "MinRotationSpeed" << YAML::Value << particleData[i].myMinRotationSpeed;
			out << YAML::Key << "MaxRotationSpeed" << YAML::Value << particleData[i].myMaxRotationSpeed;

			out << YAML::Key << "MinLifeTime" << YAML::Value << particleData[i].myMinLifeTime;
			out << YAML::Key << "MaxLifeTime" << YAML::Value << particleData[i].myMaxLifeTime;

			out << YAML::Key << "MinAngle" << YAML::Value << particleData[i].myMinAngle;
			out << YAML::Key << "MaxAngle" << YAML::Value << particleData[i].myMaxAngle;

			out << YAML::EndMap;
		}
		out << YAML::EndMap;


		out << YAML::Key << "ParticleSettingsSize" << YAML::Value << settings.size();
		out << YAML::Key << "ParticleSettings";
		out << YAML::BeginMap;
		int index = 0;
		for(auto& setting : settings)
		{
			out << YAML::Key << index++;
			out << YAML::BeginMap;

			out << YAML::Key << "Drag" << YAML::Value << setting.myDrag;
			out << YAML::Key << "RotationalDrag" << YAML::Value << setting.myRotationalDrag;
			out << YAML::Key << "Velocity" << YAML::Value << setting.myVelocity;
			out << YAML::Key << "Acceleration" << YAML::Value << setting.myAcceleration;
			out << YAML::Key << "Scale" << YAML::Value << setting.myScale;
			out << YAML::Key << "Color" << YAML::Value << setting.myColor;

			out << YAML::EndMap;
		}
		out << YAML::EndMap;

		out << YAML::EndMap; // ParticleEmitter
	}

	if(entity.HasComponent<DirectionalLightComponent>())
	{
		out << YAML::Key << "DirectionalLightComponent";
		out << YAML::BeginMap; // DirectionalLightComponent

		auto& tc = entity.GetComponent<DirectionalLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << tc.Color;
		out << YAML::Key << "Direction" << YAML::Value << tc.Direction;
		out << YAML::Key << "Intensity" << YAML::Value << tc.Intensity;

		out << YAML::EndMap; // DirectionalLightComponent
	}

	if(entity.HasComponent<SpotLightComponent>())
	{
		out << YAML::Key << "SpotLightComponent";
		out << YAML::BeginMap; // SpotLightComponent

		auto& tc = entity.GetComponent<SpotLightComponent>().mySpotLight;
		out << YAML::Key << "Color" << YAML::Value << tc->GetColor();
		out << YAML::Key << "Intensity" << YAML::Value << tc->GetIntensity();
		out << YAML::Key << "Range" << YAML::Value << tc->GetRange();
		out << YAML::Key << "InnerCone" << YAML::Value << tc->GetInnerCone();
		out << YAML::Key << "OuterCone" << YAML::Value << tc->GetOuterCone();

		out << YAML::EndMap; // SpotLightComponent
	}

	if(entity.HasComponent<PointLightComponent>())
	{
		out << YAML::Key << "PointLightComponent";
		out << YAML::BeginMap; // PointLightComponent

		auto& tc = entity.GetComponent<PointLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << tc.myPointLight->GetColor();
		out << YAML::Key << "Intensity" << YAML::Value << tc.Intensity;
		out << YAML::Key << "Range" << YAML::Value << tc.Range;

		out << YAML::EndMap; // PointLightComponent
	}



	out << YAML::EndMap; // Entity
}

static void DeserializeEntity(YAML::Node aEntityNode, Scene* aScene)
{
	uint64_t uuid = aEntityNode["Entity"].as<uint64_t>();

	std::string name;
	auto tagComponent = aEntityNode["TagComponent"];
	if(tagComponent)
		name = tagComponent["Tag"].as<std::string>();


	Entity deserializedEntity = aScene->CreateEntityWithUUID(uuid, name);


	auto transformComponent = aEntityNode["TransformComponent"];
	if(transformComponent)
	{
		// Entities always have transforms
		auto& tc = deserializedEntity.GetComponent<TransformComponent>();
		tc.Translation = transformComponent["Translation"].as<Vector3f>();
		tc.Rotation = transformComponent["Rotation"].as<Vector3f>();
		tc.Scale = transformComponent["Scale"].as<Vector3f>();
	}


	auto childComponent = aEntityNode["ChildComponent"];
	if(childComponent)
	{
		auto& cc = deserializedEntity.GetComponent<ChildComponent>();

		if(childComponent["Parent"])
		{
			auto entityID = childComponent["Parent"].as<uint64_t>();
			cc.TempParent = entityID;
		}

		if(childComponent["ChildrenSize"])
		{
			int size = childComponent["ChildrenSize"].as<int>();

			if(size > 0)
			{
				auto children = childComponent["Children"];

				if(children)
				{
					for(int i = 0; i < size; i++)
					{
						auto entityID = children[std::to_string(i)].as<uint64_t>();
						cc.TempChildren.push_back(entityID);
					}
				}
			}

		}
	}


	const auto cameraComponent = aEntityNode["CameraComponent"];
	if(cameraComponent)
	{
		auto& camera = deserializedEntity.AddComponent<CameraComponent>();
		/*deserializedEntity.AddComponent<CameraControllerData>();
		deserializedEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();*/
		//deserializedEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		float FoV = cameraComponent["FOV"].as<float>();
		float NearPlane = cameraComponent["NearPlane"].as<float>();
		float FarPlane = cameraComponent["FarPlane"].as<float>();
		camera.Primary = cameraComponent["Primary"].as<bool>();

		camera.Initialize(FoV, NearPlane, FarPlane);
	}

	auto modelComponent = aEntityNode["ModelComponent"];
	if(modelComponent)
	{
		auto& modelComp = deserializedEntity.AddComponent<ModelComponent>();
		auto path = Helpers::string_cast<std::wstring>(modelComponent["Path"].as<std::string>());

		auto modelInstance = ModelAssetHandler::GetModelInstance(path);
		modelComp.SetModel(modelInstance);


		auto textures = modelComponent["Texture"];

		if(textures["Albedo"])
		{
			modelComp.GetModel()->GetMaterial()->SetAlbedoTexture(TextureAssetHandler::GetTexture(Helpers::string_cast<std::wstring>(textures["Albedo"].as<std::string>())));
		}

		if(textures["Normal"])
		{
			modelComp.GetModel()->GetMaterial()->SetNormalTexture(TextureAssetHandler::GetTexture(Helpers::string_cast<std::wstring>(textures["Normal"].as<std::string>())));
		}

		if(textures["Material"])
		{
			modelComp.GetModel()->GetMaterial()->SetMaterialTexture(TextureAssetHandler::GetTexture(Helpers::string_cast<std::wstring>(textures["Material"].as<std::string>())));
		}

	}

	auto particleComponent = aEntityNode["ParticleComponent"];
	if(particleComponent)
	{
		auto& particleComp = deserializedEntity.AddComponent<ParticleEmitter>();

		std::vector<ParticleEmitterBehaviourTimePoint> behaviourData;
		int emittorSettingsSize = 0;
		int particleSettingsSize = 0;

		if(particleComponent["EmitterSettingsSize"])
		{
			emittorSettingsSize = particleComponent["EmitterSettingsSize"].as<int>();
		}

		particleComp.Init();

		if(particleComponent["ParticleSettingsSize"])
		{
			particleSettingsSize = particleComponent["ParticleSettingsSize"].as<int>();
		}

		if(emittorSettingsSize > 0)
		{
			YAML::Node behaviours;
			if(particleComponent["EmitterSettings"])
			{
				behaviours = particleComponent["EmitterSettings"];
			}

			particleComp.GetEmitterSettings().clear();

			for(size_t i = 0; i < emittorSettingsSize; i++)
			{
				auto behaviour = behaviours[std::to_string(i)];

				ParticleEmitterBehaviourTimePoint emittorData;
				emittorData.myMinFrequency = behaviour["MinFrequency"].as<float>();
				emittorData.myMaxFrequency = behaviour["MaxFrequency"].as<float>();

				emittorData.myMinSpeed = behaviour["MinSpeed"].as<float>();
				emittorData.myMaxSpeed = behaviour["MaxSpeed"].as<float>();

				emittorData.myMinRotationSpeed = behaviour["MinRotationSpeed"].as<float>();
				emittorData.myMaxRotationSpeed = behaviour["MaxRotationSpeed"].as<float>();

				emittorData.myMinLifeTime = behaviour["MinLifeTime"].as<float>();
				emittorData.myMaxLifeTime = behaviour["MaxLifeTime"].as<float>();

				emittorData.myMinAngle = behaviour["MinAngle"].as<Vector3f>();
				emittorData.myMaxAngle = behaviour["MaxAngle"].as<Vector3f>();

				particleComp.GetEmitterSettings().push_back(emittorData);
			}
		}

		if(particleSettingsSize > 0)
		{
			YAML::Node settings;
			if(particleComponent["ParticleSettings"])
			{
				settings = particleComponent["ParticleSettings"];
			}

			particleComp.GetSettings().clear();


			for(size_t i = 0; i < emittorSettingsSize; i++)
			{
				auto behaviour = settings[std::to_string(i)];

				ParticleBehaviourTimePoint emittorSettingData;
				emittorSettingData.myDrag = behaviour["Drag"].as<float>();
				emittorSettingData.myRotationalDrag = behaviour["RotationalDrag"].as<float>();

				emittorSettingData.myVelocity = behaviour["Velocity"].as<Vector3f>();
				emittorSettingData.myAcceleration = behaviour["Acceleration"].as<Vector3f>();
				emittorSettingData.myScale = behaviour["Scale"].as<Vector3f>();

				emittorSettingData.myColor = behaviour["Color"].as<Vector4f>();

				particleComp.GetSettings().push_back(emittorSettingData);
			}
		}
	}

	auto dirLightComponent = aEntityNode["DirectionalLightComponent"];
	if(dirLightComponent)
	{
		auto& dirLightComp = deserializedEntity.AddComponent<DirectionalLightComponent>();

		dirLightComp.Color = dirLightComponent["Color"].as<Vector3f>();
		dirLightComp.Direction = dirLightComponent["Direction"].as<Vector3f>();
		dirLightComp.Intensity = dirLightComponent["Intensity"].as<float>();

		dirLightComp.myDirectionalLight->Init(dirLightComp.Color, dirLightComp.Intensity);
		dirLightComp.myDirectionalLight->SetDirection(dirLightComp.Direction);
		dirLightComp.myDirectionalLight->SetPosition(0, 0	,0);
	}

	auto spotLightComponent = aEntityNode["SpotLightComponent"];
	if(spotLightComponent)
	{
		auto& spotLightComp = deserializedEntity.AddComponent<SpotLightComponent>();


		Vector4f color = spotLightComponent["Color"].as<Vector4f>();
		float intensity = spotLightComponent["Intensity"].as<float>();
		float range = spotLightComponent["Range"].as<float>();
		float innerCone = spotLightComponent["InnerCone"].as<float>();
		float outerCone = spotLightComponent["OuterCone"].as<float>();

		auto light = spotLightComp.mySpotLight;

		light->SetColor({ color.x,  color.y,  color.z });
		light->SetIntensity(intensity);
		light->SetRange(range);
		light->SetInnerCone(innerCone);
		light->SetOuterCone(outerCone);
	}

	auto pointLightComponent = aEntityNode["PointLightComponent"];
	if(pointLightComponent)
	{
		auto& pointLightComp = deserializedEntity.AddComponent<PointLightComponent>();



		Vector4f color = pointLightComponent["Color"].as<Vector4f>();
		float intensity = pointLightComponent["Intensity"].as<float>();
		float range = pointLightComponent["Range"].as<float>();

		auto light = pointLightComp.myPointLight;

		light->SetColor({ color.x,  color.y,  color.z });
		pointLightComp.Intensity = intensity;
		pointLightComp.Range = range;

	}

}

void SceneSerializer::Serialize(const std::string& aFilepath)
{


	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "SceneTesting";
	out << YAML::Key << "Path" << YAML::Value << aFilepath.c_str();
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

	std::ofstream fout(aFilepath);
	fout << out.c_str();
	fout.close();



}

void SceneSerializer::SerializeRuntime(const std::string& aFilepath)
{
	// Not implemented

}

bool SceneSerializer::Deserialize(const std::string& aFilepath)
{
	if(aFilepath.empty())
	{
		return false;
	}


	ModelAssetHandler::Clear();



	YAML::Node data;
	try
	{
		

		data = YAML::LoadFile(aFilepath);
	}
	catch(YAML::ParserException e)
	{
		return false;
	}

	if(!data["Scene"])
		return false;

	std::string sceneName = data["Scene"].as<std::string>();

	if(data["Path"])
	{
		myScene->SetPath(data["Path"].as<std::string>());
	}


	auto entities = data["Entities"];
	if(entities)
	{
		for(auto entity : entities)
		{
			DeserializeEntity(entity, myScene);
		}
	}

	myScene->Initialize();

	// Fix parenting :)
	const auto& view = myScene->GetRegistry().view<ChildComponent>();
	for(auto& entity1 : view)
	{
		for(auto& entity2 : view)
		{
			Entity entityOne = { entity1, myScene };
			Entity entityTwo = { entity2, myScene };

			if(entityOne.GetComponent<ChildComponent>().HasParent())
			{
				auto e1Id = entityOne.GetComponent<ChildComponent>().TempParent;
				auto e2Id = entityTwo.GetUUID();
				if(e1Id == e2Id)
				{
					entityOne.GetComponent<ChildComponent>().SetParent(entityTwo);

				}
			}

			for(size_t i = 0; i < entityOne.GetComponent<ChildComponent>().TempChildren.size(); i++)
			{
				if(entityOne.GetComponent<ChildComponent>().TempChildren[i] == entityTwo.GetUUID())
				{
					entityOne.GetComponent<ChildComponent>().SetChild(entityTwo);
					entityTwo.GetComponent<ChildComponent>().SetParent(entityOne);
				}
			}

		}
	}

	return true;
}

bool SceneSerializer::DeserializeRuntime(const std::string& aFilepath)
{
	return false;
}

