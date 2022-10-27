#pragma once
#include <Layers/Layer.h>
#include <memory>
#include <vector>

class Entity;
class ModelInstance;

class Hierarchy : public Layer
{
public:
	bool OnImGuiRender() override;

	void DrawEntityNode(Entity& aEntity);

	void DropAFile();

	void DrawEntityPopupMenu(std::shared_ptr<ModelInstance> aModel);
	void DrawWindowPopupMenu();

	bool LoopThoughChildren(Entity& aEntity);
	void CheckIfUserWantToSetParent(Entity& entity);
	void IsItemHovered(Entity& aEntity);
	
private:

	std::vector<uint32_t> myEntityList;

	friend class Scene;
};


