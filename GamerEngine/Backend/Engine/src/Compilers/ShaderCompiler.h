#pragma once
#include <d3dcompiler.h>
#include <string>
#include <fstream>

#include "Debugger/ConsoleHelper.h"

class ShaderCompiler
{
    public:
    static ID3DBlob* CompileShader(const std::string& sourceCode, const std::string& entryPoint, const std::string& shaderModel, const std::string& outputFilePath)
    {
        ID3DBlob* compiledShader = nullptr;
        ID3DBlob* errorMessage = nullptr;

        HRESULT result = D3DCompile(
            sourceCode.c_str(),
            sourceCode.size(),
            nullptr,
            nullptr,
            nullptr,
            entryPoint.c_str(),
            shaderModel.c_str(),
            D3DCOMPILE_ENABLE_STRICTNESS,
            0,
            &compiledShader,
            &errorMessage
        );

        if (FAILED(result))
        {
            if (errorMessage)
            {
                // Output the shader compilation error message
                ConsoleHelper::Log(LogType::Error, (char*)errorMessage->GetBufferPointer());
                errorMessage->Release();
            }
            else
            {
                // Shader file not found
            }

            return nullptr;
        }

        // Save the compiled shader to a .cso file
        std::ofstream outputFile(outputFilePath, std::ios::binary);
        outputFile.write((char*)compiledShader->GetBufferPointer(), compiledShader->GetBufferSize());
        outputFile.close();

        return compiledShader;
    }
};
