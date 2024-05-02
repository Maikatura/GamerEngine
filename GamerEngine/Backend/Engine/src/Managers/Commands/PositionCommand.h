#pragma once
#include "AbstractCommand.h"
#include "Core/Model/Entity.h"


class PositionCommand : public AbstractCommand
{
public:
	PositionCommand(Ref<GamerEngine::Entity> aEntity, GamerEngine::Transform aFromPosition, GamerEngine::Transform aToPosition);
	~PositionCommand() override;

	bool Execute() override;
	bool Undo() override;

private:
	Ref<GamerEngine::Entity> myEntity;
	GamerEngine::Transform myOldPosition;
	GamerEngine::Transform myNewPosition;
};

inline PositionCommand::PositionCommand(Ref<GamerEngine::Entity> aEntity, GamerEngine::Transform aFromPosition, GamerEngine::Transform aToPosition)
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
	if (!myEntity->HasComponent<GamerEngine::TransformComponent>())
	{
		return false;
	}

	auto& transform = myEntity->GetComponent<GamerEngine::TransformComponent>();
	//transform = myNewPosition;

	return true;
}

inline bool PositionCommand::Undo()
{
	if(!myEntity->HasComponent<GamerEngine::TransformComponent>())
	{
		return false;
	}

	auto& transform = myEntity->GetComponent<GamerEngine::TransformComponent>();
	//transform = myOldPosition;

	return true;
}
