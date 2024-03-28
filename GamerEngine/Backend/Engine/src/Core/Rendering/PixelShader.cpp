#include "GraphicsEngine.pch.h"
#include "PixelShader.h"

#include <fstream>

#include "AssetHandlers/ShaderHandler.h"
#include "Compilers/ShaderCompiler.h"
#include "Core/Framework/DX11.h"
#include "Utilites/StringCast.h"

PixelShader::PixelShader() : PixelShader("Shaders\\Default_PS.cso")
{
    
}



PixelShader::PixelShader(const std::string& aPath)
{
    myPath = Helpers::ToLowerCase(aPath);
    if (myPath.empty())
    {
        myPath = Helpers::ToLowerCase("Shaders\\Default_PS.cso");
    }

    std::ifstream psFile;
    psFile.open(myPath.c_str(), std::ios::binary);
    std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
    auto result = DX11::Get().GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &myShader);
    if (FAILED(result))
    {
        
    }
    psFile.close();
}

const std::string& PixelShader::Path()
{
    return myPath;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader::Get()
{
    return myShader;
}

Ref<PixelShader> PixelShader::Compile(const std::string& sourceCode, const std::string& outputFilePath)
{
    std::string shaderCode = ReadFile(sourceCode);
    std::filesystem::path workingDirectory = "editor\\shaders\\";
    std::string preprocessedShaderCode = PreprocessShaderCode(shaderCode, workingDirectory.string());
    ID3DBlob* compiledShader = ShaderCompiler::CompileShader(preprocessedShaderCode, "main", "ps_5_0", outputFilePath);

    if (!compiledShader)
    {
        return nullptr;
    }
    
    return ShaderHandler::Get().LoadPixelShader(outputFilePath);
}
