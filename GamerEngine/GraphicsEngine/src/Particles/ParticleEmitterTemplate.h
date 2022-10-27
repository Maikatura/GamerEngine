#pragma once
#include <string>

#include <CommonUtilities/Math/MathTypes.hpp>


struct EmitterSettingsData
{
	float SpawnRate = 1.0f;

	float SpawnAngle = 0.0f;

	float LifeTime;

	Vector3f StartVelocity;

	Vector3f EndVelocity;

	float GravityScale;

	float StartSize;

	float EndSize;

	Vector4f StartColor;

	Vector4f EndColor;
};

struct ParticleEmitterTemplate
{
	std::string Path;
	EmitterSettingsData EmitterSettings;
};

struct ParticleVertex
{
	bool operator<(const ParticleVertex& aParticle) const;

	float myLifeTime = 1.0f;
	float myAge = 0.0f;
	Vector3f myPosition = {0.0f,0.0f,0.0f };
	Vector3f myCurrentVelocity = { 0.0f,0.0f,0.0f };
	float myRotation = 0.0f;
	float myCurrentRotationalVelocity = 0.0f;
};

struct ParticleBehaviourTimePoint
{
	Vector3f myScale;
	Vector3f myVelocity;
	Vector3f myAcceleration;
	Vector4f myColor;
	float myDrag;
	float myRotationalDrag;
};

struct ParticleEmitterBehaviourTimePoint
{
	float myMinFrequency;
	float myMaxFrequency;
	float myMinSpeed;
	float myMaxSpeed;
	float myMinRotationSpeed;
	float myMaxRotationSpeed;
	float myMinLifeTime;
	float myMaxLifeTime;
	Vector3f myMinAngle;
	Vector3f myMaxAngle;
	Vector3f myPosition;
};