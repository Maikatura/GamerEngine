#include "GraphicsEngine.pch.h"
#include "Core/Rendering/RenderBuffer.h"

#include "Renderer.h"

bool GamerEngine::RenderBuffer2D::operator<(RenderBuffer2D& aRenderBuffer2D)
{
	Vector3f camPos = Renderer::GetViewMatrix().GetPosition();
	return (aRenderBuffer2D.myTransform.GetPosition() - camPos).Length() < (myTransform.GetPosition() - camPos).Length();
}
