#pragma once
#include <d3dcompiler.h>
#include <string>
#include <fstream>
#include "Debugger/ConsoleHelper.h"

class ShaderCompiler
{
    public:
        static ID3DBlob* CompileShader(const std::string& sourceCode, const std::string& entryPoint, const std::string& shaderModel, const std::string& outputFilePath);
};
