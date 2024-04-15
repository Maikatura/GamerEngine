#include "GraphicsEngine.pch.h"
#include <Scene/SceneSerializer.h>
#include "Scene.h"
#include "Core/Model/Entity.h"
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "Utilites/StringCast.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Particles/ParticleEmitter.h"
#include <entt.hpp>
#include "Components/AllComponents.h"
#include "Components/CameraController.h"
#include "Components/NativeScriptComponent.h"
#include "Debugger/ConsoleHelper.h"

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
	struct convert<GamerEngine::UUID>
	{
		static Node encode(const GamerEngine::UUID& rhs)
		{
			Node node;
			node.push_back(rhs);
			return node;
		}

		static bool decode(const Node& node, GamerEngine::UUID& rhs)
		{
			rhs = node.as<uint64_t>();
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

SceneSerializer::SceneSerializer(GamerEngine::Scene* scene)
	: myScene(scene)
{}

static void SerializeEntity(YAML::Emitter& out, GamerEngine::Entity entity)
{
	std::cout << "Saving Entity: Start" << std::endl;


	out << YAML::BeginMap; // Entity
	out << YAML::Key << "Entity" << YAML::Value << (uint64_t)entity.GetUUID();

	if(entity.HasComponent<TagComponent>())
	{
		std::cout << "Saving Component: TagComponent" << std::endl;

		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap; // TagComponent

		auto& tag = entity.GetComponent<TagComponent>().Tag;
		out << YAML::Key << "Tag" << YAML::Value << tag;

		out << YAML::EndMap; // TagComponent
	}

	if(entity.HasComponent<TransformComponent>())
	{
		std::cout << "Saving Component: TransformComponent" << std::endl;
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap; // TransformComponent

		auto& tc = entity.GetComponent<TransformComponent>();
		out << YAML::Key << "Translation" << YAML::Value << tc.GetPosition();
		out << YAML::Key << "Rotation" << YAML::Value << tc.GetRotation();
		out << YAML::Key << "Scale" << YAML::Value << tc.GetScale();

		if (tc.HasParent())
		{
			out << YAML::Key << "Parent" << YAML::Value << tc.GetParent()->GetUUID();
		}

		out << YAML::Key << "ChildrenSize" << YAML::Value << tc.GetChildren().size();
		out << YAML::Key << "Children";
		out << YAML::BeginMap; // Children

		for (int i = 0; i < tc.GetChildren().size(); i++)
		{
			out << YAML::Key << i << YAML::Value << tc.GetChildren()[i]->GetUUID();
		}

		out << YAML::EndMap; // Children

		out << YAML::EndMap; // TransformComponent
	}

	if(entity.HasComponent<CameraComponent>())
	{
		std::cout << "Saving Component: CameraComponent" << std::endl;
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
		std::cout << "Saving Component: ModelComponent" << std::endl;
		auto& model = entity.GetComponent<ModelComponent>();

		out << YAML::Key << "ModelComponent";
		out << YAML::BeginMap; // ModelComponent

		out << YAML::Key << "Delay" << YAML::Value << model.GetDelay();
		out << YAML::Key << "Path" << YAML::Value << Helpers::string_cast<std::string>(model.GetModel()->GetName());
		

		

		out << YAML::Key << "TextureSize" << YAML::Value << model.GetModel()->GetNumMeshes();
		out << YAML::Key << "Texture";
		out << YAML::BeginMap;
		for(int i = 0; i < model.GetModel()->GetNumMeshes(); i++)
		{

			const auto& meshData = model.GetModel()->GetMeshData(i);

			const Material& material = meshData.MaterialData;

			out << YAML::Key << i;
			out << YAML::BeginMap;


			out << YAML::Key << "PixelShader" << YAML::Value << meshData.myPixelShader->Path();

			if(const auto albedoTex = material.GetAlbedoTexture())
			{
				out << YAML::Key << "Albedo" << YAML::Value << Helpers::string_cast<std::string>(albedoTex->GetPath());
			}
			
			if(const auto normalTex = material.GetNormalTexture())
			{
				out << YAML::Key << "Normal" << YAML::Value << Helpers::string_cast<std::string>(normalTex->GetPath());
			}

			if(const auto materialTex = material.GetMaterialTexture())
			{
				out << YAML::Key << "Material" << YAML::Value << Helpers::string_cast<std::string>(materialTex->GetPath());
			}

			out << YAML::EndMap;
		}
		out << YAML::EndMap;


		out << YAML::EndMap; // ModelComponent
	}

	if(entity.HasComponent<ParticleEmitter>())
	{
		std::cout << "Saving Component: ParticleEmitter" << std::endl;

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
		std::cout << "Saving Component: DirectionalLightComponent" << std::endl;

		out << YAML::Key << "DirectionalLightComponent";
		out << YAML::BeginMap; // DirectionalLightComponent

		auto& tc = entity.GetComponent<DirectionalLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << tc.Color;
		out << YAML::Key << "Intensity" << YAML::Value << tc.Intensity;

		out << YAML::EndMap; // DirectionalLightComponent
	}

	if(entity.HasComponent<SpotLightComponent>())
	{
		std::cout << "Saving Component: SpotLightComponent" << std::endl;

		out << YAML::Key << "SpotLightComponent";
		out << YAML::BeginMap; // SpotLightComponent

		auto& tc = entity.GetComponent<SpotLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << tc.Color;
		out << YAML::Key << "Intensity" << YAML::Value << tc.Intensity;
		out << YAML::Key << "Range" << YAML::Value << tc.Range;
		out << YAML::Key << "InnerCone" << YAML::Value << tc.InnerCone;
		out << YAML::Key << "OuterCone" << YAML::Value << tc.OuterCone;

		out << YAML::EndMap; // SpotLightComponent
	}

	if(entity.HasComponent<PointLightComponent>())
	{
		std::cout << "Saving Component: PointLightComponent" << std::endl;

		out << YAML::Key << "PointLightComponent";
		out << YAML::BeginMap; // PointLightComponent

		auto& tc = entity.GetComponent<PointLightComponent>();
		out << YAML::Key << "Color" << YAML::Value << tc.Color;
		out << YAML::Key << "Intensity" << YAML::Value << tc.Intensity;
		out << YAML::Key << "Range" << YAML::Value << tc.Range;

		out << YAML::EndMap; // PointLightComponent
	}

	if (entity.HasComponent<Network::NetworkComponent>())
	{
		std::cout << "Saving Component: NetworkComponent" << std::endl;

		out << YAML::Key << "NetworkComponent";
		out << YAML::BeginMap; // PointLightComponent

		auto& netComp = entity.GetComponent<Network::NetworkComponent>();
		out << YAML::Key << "IsServer" << YAML::Value << netComp.IsServer();
		out << YAML::Key << "Smooth" << YAML::Value << netComp.ShouldSmooth();
		out << YAML::Key << "ID" << YAML::Value << netComp.GetID();

		out << YAML::EndMap; // PointLightComponent



	}

	if (entity.HasComponent<NativeScriptComponent>())
	{
		// std::cout << "Saving Component: NativeScriptComponent" << std::endl;
		//
		// auto comp = entity.GetComponent<NativeScriptComponent>().Instance;
		// if (auto ranMover = dynamic_cast<RandomMoverComponent*>(comp))
		// {
		// 	out << YAML::Key << "RandomMoverComponent";
		// 	out << YAML::BeginMap; // RandomMoverComponent
		//
		// 	out << YAML::Key << "MinArea" << YAML::Value << ranMover->GetMinArea();
		// 	out << YAML::Key << "MaxArea" << YAML::Value << ranMover->GetMaxArea();
		//
		//
		// 	out << YAML::EndMap; // RandomMoverComponent
		//
		// }


	}


	out << YAML::EndMap; // Entity

	std::cout << "Saving Entity: Done" << std::endl;
}




void SceneSerializer::DeserializeEntity(YAML::Node aEntityNode, GamerEngine::Scene* aScene, bool isHeadless)
{
	uint64_t uuid = aEntityNode["Entity"].as<uint64_t>();

	std::string name;
	auto tagComponent = aEntityNode["TagComponent"];
	if(tagComponent)
		name = tagComponent["Tag"].as<std::string>();


	GamerEngine::Entity deserializedEntity = aScene->CreateEntityWithUUID(uuid, name);

	ConsoleHelper::Log(LogType::Info, "Loading Entity with UUID: %llu Name: %s" , uuid, name.c_str());

	auto transformComponent = aEntityNode["TransformComponent"];
	if(transformComponent)
	{
		// Entities always have transforms
		auto& tc = deserializedEntity.GetComponent<TransformComponent>();
		tc.SetPosition(transformComponent["Translation"].as<Vector3f>());
		tc.SetRotation(transformComponent["Rotation"].as<Vector3f>());
		tc.SetScale(transformComponent["Scale"].as<Vector3f>());
	}


	auto childComponent = aEntityNode["ChildComponent"];
	if(childComponent)
	{
		auto& cc = deserializedEntity.GetComponent<TransformComponent>();

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
		deserializedEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		float FoV = cameraComponent["FOV"].as<float>();
		float NearPlane = cameraComponent["NearPlane"].as<float>();
		float FarPlane = cameraComponent["FarPlane"].as<float>();
		camera.Primary = cameraComponent["Primary"].as<bool>();

		camera.Initialize(FoV, NearPlane, FarPlane);
	}

	auto modelComponent = aEntityNode["ModelComponent"];
	if(modelComponent)
	{

		ModelData modelData;

		modelData.Path = Helpers::string_cast<std::wstring>(modelComponent["Path"].as<std::string>());

		if (modelComponent["Delay"])
		{
			modelData.Delay = modelComponent["Delay"].as<float>();
		}

		int textureSize = 0;
		if (modelComponent["TextureSize"])
		{
			textureSize = modelComponent["TextureSize"].as<int>();
		}

		auto textures = modelComponent["Texture"];


		if (!isHeadless)
		{
			if (textureSize != 0)
			{
				for (size_t i = 0; i < textureSize; i++)
				{
					auto texture = textures[std::to_string(i)];

					modelData.MeshMaterialData.push_back(MeshMaterialData());

					if (texture["PixelShader"])
					{
						modelData.MeshMaterialData.back().PixelShader = Helpers::string_cast<std::wstring>(texture["PixelShader"].as<std::string>());
					}

					if (texture["Albedo"])
					{
						modelData.MeshMaterialData.back().Albedo =Helpers::string_cast<std::wstring>(texture["Albedo"].as<std::string>());
						//materials[i]->SetAlbedoTexture(TextureAssetHandler::GetTexture(albedoPath));
					}

					if (texture["Normal"])
					{
						modelData.MeshMaterialData.back().Normal = Helpers::string_cast<std::wstring>(texture["Normal"].as<std::string>());
					}

					if (texture["Material"])
					{
						modelData.MeshMaterialData.back().Material = Helpers::string_cast<std::wstring>(texture["Material"].as<std::string>());
					}
				}
			}

		}

		deserializedEntity.AddComponent<ModelComponent>(modelData);

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
		if(!isHeadless)
		{
			auto& dirLightComp = deserializedEntity.AddComponent<DirectionalLightComponent>();

			dirLightComp.Color = dirLightComponent["Color"].as<Vector3f>();
			dirLightComp.Intensity = dirLightComponent["Intensity"].as<float>();

			dirLightComp.myDirectionalLight->Init(dirLightComp.Color, dirLightComp.Intensity);
		}
		
	}

	auto spotLightComponent = aEntityNode["SpotLightComponent"];
	if(spotLightComponent)
	{
		if(!isHeadless)
		{
			auto& spotLightComp = deserializedEntity.AddComponent<SpotLightComponent>();


			spotLightComp.Color = spotLightComponent["Color"].as<Vector3f>();
			float intensity = spotLightComponent["Intensity"].as<float>();
			float range = spotLightComponent["Range"].as<float>();
			float innerCone = spotLightComponent["InnerCone"].as<float>();
			float outerCone = spotLightComponent["OuterCone"].as<float>();

			spotLightComp.Intensity = intensity;
			spotLightComp.Range = range;
			spotLightComp.InnerCone = innerCone;
			spotLightComp.OuterCone = outerCone;
		}
	}

	auto pointLightComponent = aEntityNode["PointLightComponent"];
	if(pointLightComponent)
	{
		if(!isHeadless)
		{
			auto& pointLightComp = deserializedEntity.AddComponent<PointLightComponent>();


			pointLightComp.Color = (pointLightComponent["Color"]) ? pointLightComponent["Color"].as<Vector3f>() : Vector3f();
			pointLightComp.Intensity = (pointLightComponent["Intensity"]) ? pointLightComponent["Intensity"].as<float>() : 1.0f;
			pointLightComp.Range = (pointLightComponent["Range"]) ? pointLightComponent["Range"].as<float>() : 1.0f;;
		}
	}


	auto networkComp = aEntityNode["NetworkComponent"];
	if (networkComp)
	{
		auto& netComp = deserializedEntity.AddComponent<Network::NetworkComponent>();
		netComp.SetServer((networkComp["IsServer"]) ? networkComp["IsServer"].as<bool>() : false);
		netComp.SetShouldSmooth((networkComp["Smooth"]) ? networkComp["Smooth"].as<bool>() : false);
		netComp.SetID((networkComp["ID"]) ? networkComp["ID"].as<GamerEngine::UUID>() : GamerEngine::UUID());
	}

	// auto randomMover = aEntityNode["RandomMoverComponent"];
	// if (randomMover)
	// {
	// 	deserializedEntity.AddComponent<NativeScriptComponent>().Bind<RandomMoverComponent>();
	// 	auto& ranMoverData =  deserializedEntity.AddComponent<RandomMoverData>();
	//
	// 	ranMoverData.myMinArea = randomMover["MinArea"].as<Vector3f>();
	// 	ranMoverData.myMaxArea = randomMover["MaxArea"].as<Vector3f>();
	//
	// }

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
			GamerEngine::Entity entity = { entityID, myScene };
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

bool SceneSerializer::Deserialize(const std::string& aFilepath, bool isHeadless)
{
	if(aFilepath.empty())
	{
		return false;
	}


	ModelAssetHandler::Get().Clear();

	myScene->Initialize();

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
			DeserializeEntity(entity, myScene, isHeadless);
		}
	}

	// Fix parenting :)
	const auto& view = myScene->GetRegistry().view<TransformComponent>();
	for(auto& entity1 : view)
	{
		for(auto& entity2 : view)
		{
			Ref<GamerEngine::Entity> entityOne = MakeRef<GamerEngine::Entity>(GamerEngine::Entity{ entity1, myScene });
			Ref<GamerEngine::Entity> entityTwo = MakeRef<GamerEngine::Entity>(GamerEngine::Entity{ entity2, myScene });

			if (!entityOne || !entityTwo)
			{
				return false;
			}

			if(entityOne->GetComponent<TransformComponent>().HasParent())
			{
				auto e1Id = entityOne->GetComponent<TransformComponent>().TempParent;
				auto e2Id = entityTwo->GetUUID();
				if(e1Id == e2Id)
				{
					entityOne->GetComponent<TransformComponent>().SetParent(entityTwo.get());

				}
			}

			for(size_t i = 0; i < entityOne->GetComponent<TransformComponent>().TempChildren.size(); i++)
			{
				if(entityOne->GetComponent<TransformComponent>().TempChildren[i] == entityTwo->GetUUID())
				{
					entityOne->GetComponent<TransformComponent>().SetChild(entityTwo.get());
					entityTwo->GetComponent<TransformComponent>().SetParent(entityOne.get());
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

