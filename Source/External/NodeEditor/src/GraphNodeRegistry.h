#pragma once
#include <typeinfo>

#include "GraphNodeBase.h"

class GraphNodeBase;

class GraphNodeRegistry
{
public:
	static void PopulateTypes();
	static GraphNodeBase* GetNodeTypeFromID(unsigned int aClassID)
	{
		return myTypes[aClassID]; // 1:1 to nodetype enum
	}
	template<class T>
	static int GetIdFromType()
	{
		for (auto& type : myTypes)
		{
			if (typeid(type) == typeid(T))
				return type->myID;
		}

		return -1;
	}
	template<class T>
	static GraphNodeBase* GetNodeTypePtrFromType()
	{
		int typeID = -1;

		const std::type_info& AType = typeid(T);
		
		for (auto& type : myTypes)
		{
			const std::type_info& BType = typeid(*type);
			if (AType == BType)
			{
				typeID = type->myID;
				break;
			}
		}

		if (typeID != -1)
		{
			return GetNodeTypeFromID(typeID);
		}
		return nullptr;
	}
	static GraphNodeBase** GetAllNodeTypes()
	{
		return myTypes; // 1:1 to nodetype enum
	}
	static unsigned short GetNodeTypeCount()
	{
		return 	myTypeCounter; // 1:1 to nodetype enum
	}
	template <class T>
	static void RegisterType()
	{
		myTypes[myTypeCounter] = new T;
		myTypes[myTypeCounter]->myID = myTypeCounter;
		myTypeCounter++;
	}

	static void Destroy()
	{
		do
		{
			delete myTypes[--myTypeCounter];
			myTypes[myTypeCounter] = nullptr;
		} while(myTypeCounter != 0);
	}

	static GraphNodeBase* myTypes[128];
	static unsigned short myTypeCounter;
};
