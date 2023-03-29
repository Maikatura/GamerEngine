#pragma once
#include <stack>

#include <Renderer/Managers/Commands/AbstractCommand.h>



class CommandManager
{
public:
	static void DoCommand(AbstractCommand* aCommand);
	static void Undo();
	static void Redo();

private:
	inline static std::stack<AbstractCommand*> myUndoStack;
	inline static std::stack<AbstractCommand*> myRedoStack;
	
};

