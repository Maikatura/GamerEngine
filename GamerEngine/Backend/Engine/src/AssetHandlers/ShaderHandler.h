#pragma once
#include "Core/Rendering/PixelShader.h"
#include "Core/Rendering/VertexShader.h"

class ShaderHandler
{
    public:
        static ShaderHandler& Get();

    ShaderHandler();
        

        typedef std::unordered_map<std::string, Ref<PixelShader>> PixelShaderMap;
        typedef std::unordered_map<std::string, Ref<VertexShader>> VertexShaderMap;
        
        Ref<PixelShader> LoadPixelShader(const std::string& aPath);
        Ref<VertexShader> LoadVertexShader(const std::string& aPath);
            
        Ref<PixelShader> GetPixelShader(const std::string& aPath);
        Ref<VertexShader> GetVertexShader(const std::string& aPath);

        PixelShaderMap& GetPixelShaderMap();
    
    private:

        PixelShaderMap myPixelShaders;
        VertexShaderMap myVertexShaders;
};
