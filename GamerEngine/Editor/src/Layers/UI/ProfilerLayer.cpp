#include "Editor.pch.h"
#include "ProfilerLayer.h"
//#include <Profiler/Profiler.h>


ProfilerLayer::ProfilerLayer(bool& shouldUseProfiler) : Layer("Profiler", true, true), myUseProfiler(shouldUseProfiler)
{
	//gCPUProfiler.Initialize(10, 512);
}

void ProfilerLayer::OnImGuiRender()
{
	Layer::OnImGuiRender();

	myUseProfiler = myIsOpen;

	BeginMenu();

	//DrawProfilerHUD();

	EndMenu();
}

void ProfilerLayer::OnBeginFrame()
{
	myUseProfiler = myIsOpen;
}
