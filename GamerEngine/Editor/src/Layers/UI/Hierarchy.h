#pragma once
#include <Layers/Layer.h>
#include <memory>
#include <vector>

#include "Renderer/Model/Model.h"

class Entity;
class ModelInstance;

class Hierarchy : public Layer
{
public:

	Hierarchy();

	void OnImGuiRender() override;

	void DrawEntityNode(Entity& aEntity);

	void DropAFile();

	void DrawEntityPopupMenu(std::shared_ptr<ModelInstance> aModel);
	void DrawWindowPopupMenu();

	void DrawArmature(std::shared_ptr<ModelInstance> aModel);
	void LoopBones(const Skeleton* aSkeleton, const Skeleton::Bone* aBone);

	bool LoopThoughChildren(Entity& aEntity);
	void CheckIfUserWantToSetParent(Entity& entity);
	void IsItemHovered(Entity& aEntity);



private:

	std::vector<uint32_t> myEntityList;

	friend class Scene;
};


