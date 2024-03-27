#pragma once
namespace YAML
{
	class Node;
}

class Scene;

class SceneSerializer
{
public:
	SceneSerializer(Scene* scene);

	void Serialize(const std::string& aFilepath);
	void SerializeRuntime(const std::string& aFilepath);

	bool Deserialize(const std::string& filepath, bool loadTextures = true);
	bool DeserializeRuntime(const std::string& aFilepath);

	static void DeserializeEntity(YAML::Node aEntityNode, Scene* aScene, bool isHeadless = false);
private:
	Scene* myScene;

};

