#pragma once
#include <Layers/Layer.h>
#include <memory>
#include <vector>
#include "Model/ModelInstance.h"
#include "Utilites/Pointers.h"

class Entity;
class ModelInstance;

class Hierarchy : public Layer
{
public:

	Hierarchy();

	void OnImGuiRender() override;

	void DrawEntityNode(Entity& aEntity);

	void DropAFile();

	void DrawEntityPopupMenu(Ref<ModelInstance> aModel);
	void DrawWindowPopupMenu();

	void DrawArmature(Entity& aEntity, Ref<ModelInstance> aModel);
	void LoopBones(const Skeleton* aSkeleton, const Bone* aBone, unsigned int aBoneID = 0);

	bool LoopThoughChildren(Ref<Entity> aEntity);
	void CheckIfUserWantToSetParent(Entity& entity);
	void IsItemHovered(Entity& aEntity);



private:

	std::vector<uint32_t> myEntityList;

	friend class Scene;
};


