#include "GraphicsEngine.pch.h"
#include "Particles/ParticleSystem.h"

void ParticleSystem::Update()
{
	SceneObject::Update();

	for (ParticleEmitter& emitter : myEmitters)
	{
		//emitter.OnUpdate();
	}
}
