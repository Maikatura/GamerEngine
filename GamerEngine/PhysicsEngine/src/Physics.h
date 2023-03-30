#pragma once
#include <memory>
#include <btBulletDynamicsCommon.h>
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"

class Physics
{
public:

	Physics();
	~Physics();

	static Physics& Get();

	void Init();

	void Update();

private:

	btDefaultCollisionConfiguration* myCollisionConfiguration;
	btCollisionDispatcher* myDispatcher;
	btBroadphaseInterface* myOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mySolver;
	btDiscreteDynamicsWorld* myDynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> myCollisionShapes;
	inline static std::shared_ptr<Physics> myInstance;
};

