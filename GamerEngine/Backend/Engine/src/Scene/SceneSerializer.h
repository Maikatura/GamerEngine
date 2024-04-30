#pragma once
namespace YAML
{
	class Node;
}


namespace GamerEngine
{
	class Scene;
}

class SceneSerializer
{
public:
	SceneSerializer(GamerEngine::Scene* scene);

	void Serialize(const std::string& aFilepath);
	void SerializeRuntime(const std::string& aFilepath);

	bool Deserialize(const std::string& filepath, bool loadTextures = true);
	bool DeserializeRuntime(const std::string& aFilepath);

	static void DeserializeEntity(YAML::Node aEntityNode, GamerEngine::Scene* aScene, std::unordered_map<uint64_t, uint64_t>& aParents, bool isHeadless = false);
private:
	GamerEngine::Scene* myScene;

};

