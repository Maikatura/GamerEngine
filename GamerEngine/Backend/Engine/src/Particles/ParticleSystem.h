#pragma once
#include "ParticleEmitter.h"
#include "Core/Model/SceneObject.h"

struct ParticleSystemEmitter
{
	ParticleEmitter Emitter;
	EmitterSettingsData Settings;
	TransformOld Transform;
};

class ParticleSystem : public SceneObject
{
	friend class ParticleAssetHandler;
	std::vector<ParticleEmitter> myEmitters;

public:

	void Update();

	FORCEINLINE const std::vector<ParticleEmitter>& GetEmitters() const { return myEmitters; }
};
