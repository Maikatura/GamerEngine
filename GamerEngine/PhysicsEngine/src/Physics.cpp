#include "Physics.h"


Physics::Physics()
{
}

Physics::~Physics()
{
	//for(int i = myDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	//{
	//	btCollisionObject* obj = myDynamicsWorld->getCollisionObjectArray()[i];
	//	btRigidBody* body = btRigidBody::upcast(obj);
	//	if(body && body->getMotionState())
	//	{
	//		delete body->getMotionState();
	//	}
	//	myDynamicsWorld->removeCollisionObject(obj);
	//	delete obj;
	//}

	//for(int j = 0; j < myCollisionShapes.size(); j++)
	//{
	//	btCollisionShape* shape = myCollisionShapes[j];
	//	myCollisionShapes[j] = 0;
	//	delete shape;
	//}

	////delete dynamics world
	//delete myDynamicsWorld;
	////delete solver
	//delete mySolver;
	////delete broadphase
	//delete myOverlappingPairCache;
	////delete dispatcher
	//delete myDispatcher;
	//delete myCollisionConfiguration;
	//myCollisionShapes.clear();
}

Physics& Physics::Get()
{
	if(myInstance == nullptr)
	{
		myInstance = std::make_shared<Physics>();
	}

	return *myInstance;
}

void Physics::Init()
{
	//btDefaultCollisionConfiguration collisionConfig;
	//btCollisionDispatcher dispatcher(&collisionConfig);
	//btDbvtBroadphase overlappingPairCache;
	//btSequentialImpulseConstraintSolver solver;
	//btDiscreteDynamicsWorld dynamicsWorld(&dispatcher, &overlappingPairCache, &solver, &collisionConfig);
	//dynamicsWorld.setGravity(btVector3(0, -9.81f, 0)); // Set the gravity
}

void Physics::Update()
{
	//myDynamicsWorld->debugDrawWorld();
	//
	//myDynamicsWorld->stepSimulation(1.f / 60.f, 10);
	//
	//for(int j = myDynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	//{
	//	btCollisionObject* obj = myDynamicsWorld->getCollisionObjectArray()[j];
	//	btRigidBody* body = btRigidBody::upcast(obj);
	//	btTransform trans;
	//	if(body && body->getMotionState())
	//	{
	//		body->getMotionState()->getWorldTransform(trans);
	//	}
	//	else
	//	{
	//		trans = obj->getWorldTransform();
	//	}
	//	printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	//}
}
