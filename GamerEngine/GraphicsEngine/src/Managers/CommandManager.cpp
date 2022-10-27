#include "GraphicsEngine.pch.h"
#include <Managers/CommandManager.h>

void CommandManager::DoCommand(AbstractCommand* aCommand)
{
	if (myRedoStack.size() > 0)
	{
		for (int i = 0; i < myRedoStack.size(); i++)
		{
			myRedoStack.pop();
		}
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
