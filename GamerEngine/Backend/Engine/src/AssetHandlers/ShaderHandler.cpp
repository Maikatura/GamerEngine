#include "GraphicsEngine.pch.h"
#include "ShaderHandler.h"
#include "Utilites/StringCast.h"

ShaderHandler& ShaderHandler::Get()
{
    static ShaderHandler instance;
    return instance;
}

ShaderHandler::ShaderHandler()
{

    if (!std::filesystem::exists("Shaders\\Custom"))
    {
        std::filesystem::create_directories("Shaders\\Custom");
    }

    for (const auto& entry : std::filesystem::directory_iterator("Shaders\\Custom"))
    {
        if (entry.is_regular_file())
        {
            std::string filePath = entry.path().string();
            std::string extension = entry.path().extension().string();

            if (extension == ".cso")
            {
                LoadPixelShader(filePath);
            }
        }
    }
}

Ref<PixelShader> ShaderHandler::LoadPixelShader(const std::string& aPath)
{
    myPixelShaders[Helpers::ToLowerCase(aPath)] = MakeRef<PixelShader>(aPath);
    return myPixelShaders[Helpers::ToLowerCase(aPath)];
}

Ref<VertexShader> ShaderHandler::LoadVertexShader(const std::string& aPath)
{
    myVertexShaders[Helpers::ToLowerCase(aPath)] = MakeRef<VertexShader>(aPath);
    return myVertexShaders[Helpers::ToLowerCase(aPath)];
}

Ref<PixelShader> ShaderHandler::GetPixelShader(const std::string& aPath)
{
    return myPixelShaders[Helpers::ToLowerCase(aPath)];
}

Ref<VertexShader> ShaderHandler::GetVertexShader(const std::string& aPath)
{
    return myVertexShaders[Helpers::ToLowerCase(aPath)];
}

ShaderHandler::PixelShaderMap& ShaderHandler::GetPixelShaderMap()
{
    return myPixelShaders;
}
