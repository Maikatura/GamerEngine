#pragma once
class Scene;

class SceneSerializer
{
public:
	SceneSerializer(Scene* scene);

	void Serialize(const std::string& aFilepath);
	void SerializeRuntime(const std::string& aFilepath);

	bool Deserialize(const std::string& filepath);
	bool DeserializeRuntime(const std::string& aFilepath);
private:
	Scene* myScene;

};

