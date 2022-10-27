#pragma once

class AbstractCommand
{
public:
	virtual bool Execute() = 0;
	virtual bool Undo() = 0;
	virtual ~AbstractCommand() = default;
};