#include "GraphicsEngine.pch.h"
#include "Renderer/Particles/ParticleSystem.h"

void ParticleSystem::Update()
{
	SceneObject::Update();

	for (ParticleEmitter& emitter : myEmitters)
	{
		//emitter.OnUpdate();
	}
}
