#pragma once
#include "Scene.h"

class SceneSerializer
{
public:
	SceneSerializer(Scene* scene);

	void Serialize(const std::string& filepath);
	void SerializeRuntime(const std::string& filepath);

	bool Deserialize(const std::string& filepath);
	bool DeserializeRuntime(const std::string& filepath);
private:
	Scene* myScene;

};

