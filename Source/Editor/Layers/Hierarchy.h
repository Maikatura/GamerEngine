#pragma once
#include <Layers/Layer.h>
#include <memory>

class Entity;
class ModelInstance;

class Hierarchy : public Layer
{
public:
	bool OnImGuiRender() override;

	void DrawEntityNode(Entity aEntity);

	void DropItem();

	void DrawEntityPopupMenu(std::shared_ptr<ModelInstance> aModel);
	void DrawWindowPopupMenu();
	
private:
	friend class Scene;
};


