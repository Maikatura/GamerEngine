#pragma once
#include "Layers/Layer.h"

class ShaderToolLayer : public Layer
{
    public:
        ShaderToolLayer();

        void OnImGuiRender() override;

        void OnBeginFrame() override;
};
