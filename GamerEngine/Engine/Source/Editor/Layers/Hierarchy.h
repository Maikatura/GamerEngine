#pragma once
#include <Layers/Layer.h>

#include <memory>

class ModelInstance;

class Hierarchy : public Layer
{
public:
	bool OnImGuiRender() override;


	void DrawEntity(std::shared_ptr<ModelInstance> aModel, int aDebugNumber = 0);
	void DragAndDrop(std::shared_ptr<ModelInstance> aModel);

	void DrawEntityPopupMenu(std::shared_ptr<ModelInstance> aModel);
	void DrawWindowPopupMenu();

};


