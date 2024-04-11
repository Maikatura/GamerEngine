#pragma once
#include "AbstractCommand.h"
#include "Core/Model/Entity.h"


class PositionCommand : public AbstractCommand
{
public:
	PositionCommand(Ref<GamerEngine::Entity> aEntity, TransformComponent aFromPosition, TransformComponent aToPosition);
	~PositionCommand() override;

	bool Execute() override;
	bool Undo() override;

private:
	Ref<GamerEngine::Entity> myEntity;
	TransformComponent myOldPosition;
	TransformComponent myNewPosition;
};

inline PositionCommand::PositionCommand(Ref<GamerEngine::Entity> aEntity, TransformComponent aFromPosition, TransformComponent aToPosition)
{
	myEntity = aEntity;
	myOldPosition = aFromPosition;
	myNewPosition = aToPosition;
}

inline PositionCommand::~PositionCommand()
{
}

inline bool PositionCommand::Execute()
{
	if (!myEntity->HasComponent<TransformComponent>())
	{
		return false;
	}

	auto& transform = myEntity->GetComponent<TransformComponent>();
	transform = myNewPosition;

	return true;
}

inline bool PositionCommand::Undo()
{
	if(!myEntity->HasComponent<TransformComponent>())
	{
		return false;
	}

	auto& transform = myEntity->GetComponent<TransformComponent>();
	transform = myOldPosition;

	return true;
}
