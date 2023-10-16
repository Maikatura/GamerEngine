#pragma once
#include "Components/ScriptableEntity.h"



struct RandomMoverData :  public Component
{

	RandomMoverData() = default;
	RandomMoverData(const RandomMoverData&) = default;

	float mySpeed;
	Vector3f myMinArea;
	Vector3f myMaxArea;
};

class RandomMoverComponent : public ScriptableEntity
{
	float mySendTimer;
	
public:

	RandomMoverComponent();

	void OnCreate() override;
	void OnUpdate() override;


	float GetSpeed();
	void SetSpeed(float aSpeed);

	void SetMinArea(Vector3f aMinArea);
	void SetMaxArea(Vector3f aMaxArea);

	Vector3f GetMinArea();
	Vector3f GetMaxArea();

	Vector3f myVelocity;

	RandomMoverData* myMoverData;

};