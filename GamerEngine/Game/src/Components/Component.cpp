#include "Component.h"


Component::Component(bool aRunInEditor) : myRunInEditor(aRunInEditor)
{
}

bool Component::RunInEditor()
{
	return myRunInEditor;
}

void Component::SetEntity(Entity* aEntity)
{
	myEntity = aEntity;
}
