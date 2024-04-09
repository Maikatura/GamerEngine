#pragma once
#include <d3dcompiler.h>
#include <string>
#include <fstream>

class ShaderCompiler
{
    public:
        static ID3DBlob* CompileShader(const std::string& sourceCode, const std::string& entryPoint, const std::string& shaderModel, const std::string& outputFilePath);
};
