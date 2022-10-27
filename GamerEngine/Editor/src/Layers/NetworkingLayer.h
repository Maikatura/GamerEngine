#pragma once
#include <Layers/Layer.h>

class NetworkingLayer : public Layer
{
public:
	bool OnImGuiRender() override;
	void Listen();

private:

	void StartNetworking();
};