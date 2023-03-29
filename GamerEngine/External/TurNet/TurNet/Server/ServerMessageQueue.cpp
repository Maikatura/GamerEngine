#include "ServerMessageQueue.h"

TurNet::TurMessage TurNet::ServerMessageQueue::Top()
{
	auto top = myQueue.front();
	myQueue.pop();
	return top;
}

void TurNet::ServerMessageQueue::Push(TurNet::TurMessage aMessage)
{
	myQueue.push(aMessage);
}

bool TurNet::ServerMessageQueue::Empty()
{
	return myQueue.empty();
}
