#include "GraphicsEngine.pch.h"
#include <Renderer/Particles/ParticleEmitterTemplate.h>
#include "Renderer/Render/Renderer.h"

bool ParticleVertex::operator<(const ParticleVertex& aParticle) const
{
	auto camPos = Renderer::GetViewMatrix().GetPosition();
	return (aParticle.myPosition - camPos).Length() < (myPosition - camPos).Length();
}
