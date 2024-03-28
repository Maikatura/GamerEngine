#include "GraphicsEngine.pch.h"
#include <Particles/ParticleEmitterTemplate.h>
#include "Core/Rendering/Renderer.h"

bool ParticleVertex::operator<(const ParticleVertex& aParticle) const
{
	auto camPos = Renderer::GetViewMatrix().GetPosition();
	return (aParticle.myPosition - camPos).Length() < (myPosition - camPos).Length();
}
