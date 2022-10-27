#pragma once

#include <Model/Entity.h>
#include <Managers/Commands/AbstractCommand.h>

class ModelCommand : public AbstractCommand
{
public:
	ModelCommand(Entity aEntity, Material aFromMaterial, Material aToMaterial);
	~ModelCommand() override;

	bool Execute() override;
	bool Undo() override;

private:
	Entity myEntity;
	Material myOldMaterial;
	Material myNewMaterial;
};

inline ModelCommand::ModelCommand(Entity aEntity, Material aFromMaterial, Material aToMaterial)
{
	myEntity = aEntity;
	myOldMaterial = aFromMaterial;
	myNewMaterial = aToMaterial;
}

inline bool ModelCommand::Execute()
{
	if(!myEntity.HasComponent<ModelComponent>())
	{
		return false;
	}

	auto& model = myEntity.GetComponent<ModelComponent>();
	model.myModel->SetMaterial(&myNewMaterial);

	return true;
}

inline bool ModelCommand::Undo()
{
	if(!myEntity.HasComponent<ModelComponent>())
	{
		return false;
	}

	auto& model = myEntity.GetComponent<ModelComponent>();
	model.myModel->SetMaterial(&myOldMaterial);

	return true;
}
