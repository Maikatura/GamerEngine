#pragma once
#include <Layers/Layer.h>
#include <memory>
#include <vector>
#include "Core/Model/Model.h"
#include "Utilites/Pointers.h"


namespace GamerEngine
{
	class Entity;
}

class Hierarchy : public Layer
{
public:

	Hierarchy();

	void OnImGuiRender() override;

	void DrawEntityNode(GamerEngine::Entity& aEntity);

	void DropAFile();

	void DrawEntityPopupMenu(Ref<GamerEngine::Model> aModel);
	void DrawWindowPopupMenu();

	void DrawArmature(GamerEngine::Entity& aEntity, Ref<GamerEngine::Model> aModel);
	void LoopBones(const GamerEngine::Skeleton* aSkeleton, const GamerEngine::Bone* aBone, unsigned int aBoneID = 0);

	bool LoopThoughChildren(GamerEngine::Entity& aEntity);
	void CheckIfUserWantToSetParent(GamerEngine::Entity& entity);
	void IsItemHovered(GamerEngine::Entity& aEntity);



private:

	std::vector<uint32_t> myEntityList;
	bool myEngineIsRunning = false;

	friend class Scene;
};


