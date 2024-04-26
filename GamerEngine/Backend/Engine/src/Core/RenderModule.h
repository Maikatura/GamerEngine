#pragma once

class RenderModule
{
public:
    virtual ~RenderModule() = default;

    virtual bool OnAdd() = 0;


    virtual void OnRelease() = 0;
    
    virtual void OnRenderSetup() = 0;
    virtual void OnEnd() = 0;
    
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    
};
