#include "GraphicsEngine.pch.h"
#include <Renderer/Managers/CommandManager.h>

void CommandManager::DoCommand(AbstractCommand* aCommand)
{
	for (size_t i = myRedoStack.size(); i > 0; i--)
	{
		delete myRedoStack.top();
		myRedoStack.pop();
	}


	myUndoStack.push(aCommand);
}

void CommandManager::Undo()
{
	if(myUndoStack.empty())
	{
		return;
	}

	auto command = myUndoStack.top();
	if(command == nullptr)
	{
		return;
	}

	command->Undo();


	myRedoStack.push(command);
	myUndoStack.pop();
}

void CommandManager::Redo()
{

	if (myRedoStack.empty())
	{
		return;
	}

	auto command = myRedoStack.top();
	if (command == nullptr)
	{
		return;
	}

	command->Execute();

	myUndoStack.push(command);
	myRedoStack.pop();

}
