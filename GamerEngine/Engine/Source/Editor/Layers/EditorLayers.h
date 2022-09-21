#pragma once
#include <memory>
#include <vector>
#include <Layers/Layer.h>
#include <iostream>
#include <string>

class EditorLayers
{
public:

	void AddLayer(std::shared_ptr<Layer> aLayer)
	{
		myLayers.push_back(aLayer);
		aLayer->OnAttach();
	}

	void Update()
	{
		
	}

	void OnAttach()
	{
		
	}

	void OnImGuiRender()
	{
		for (int i = 0; i < myLayers.size(); i++)
		{
			if(!myLayers[i]->OnImGuiRender())
			{
				std::cout << "Removed a Layer" << std::endl;
				myLayers[i].swap(myLayers[myLayers.size() - 1]);
				myLayers.pop_back();
			}
		}
	}

	void OnDetach()
	{
		
	}

private:
	std::vector<std::shared_ptr<Layer>> myLayers;
};
