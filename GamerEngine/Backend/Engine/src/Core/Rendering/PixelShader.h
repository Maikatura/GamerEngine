#pragma once
#include <d3d11.h>
#include <filesystem>
#include <fstream>
#include <regex>
#include <wrl/client.h>


class PixelShader
{
    public:
        PixelShader();
        PixelShader(const std::string& aPath);

        const std::string& Path();
    
         Microsoft::WRL::ComPtr<ID3D11PixelShader> Get();

         

         static Ref<PixelShader> Compile(const std::string& sourceCode, const std::string& outputFilePath);

    private:

        static std::string ReadFile(const std::string& filePath)
        {
            std::ifstream fileStream(filePath);
            if (!fileStream)
            {
                std::cerr << "Failed to open file: " << filePath << std::endl;
                if (!std::filesystem::exists(filePath))
                {
                    std::cerr << "File does not exist." << std::endl;
                }
                else if (!(std::filesystem::status(filePath).permissions() == std::filesystem::perms::owner_read))
                {
                    std::cerr << "No read permissions for file." << std::endl;
                }
                else
                {
                    std::cerr << "File is being used by another process." << std::endl;
                }
                return "";
            }

            std::stringstream buffer;
            buffer << fileStream.rdbuf();
            return buffer.str();
        }

        static std::string PreprocessShaderCode(const std::string& shaderCode, const std::string& currentFilePath)
        {
            std::string preprocessedCode = shaderCode;
            std::regex includeRegex("#include [<\"]([^>\"]+)[>\"]");

            std::smatch match;
            while (std::regex_search(preprocessedCode, match, includeRegex))
            {
                std::string includeDirective = match[0];
                std::string includedFilePath = match[1];

                // If the included file path is relative, combine it with the path of the current file
                if (includedFilePath[0] != '/' && includedFilePath.substr(0, 3) != "../")
                {
                    std::filesystem::path currentPath(currentFilePath);
                    currentPath.remove_filename();
                    includedFilePath = (currentPath / includedFilePath).string();
                }

                std::string includedFileContents = ReadFile(includedFilePath);

                // Preprocess the included file
                includedFileContents = PreprocessShaderCode(includedFileContents, includedFilePath);

                // Replace the include directive with the contents of the included file
                size_t pos = preprocessedCode.find(includeDirective);
                if (pos != std::string::npos)
                {
                    preprocessedCode.replace(pos, includeDirective.length(), includedFileContents);
                }
            }

            return preprocessedCode;
        }

        Microsoft::WRL::ComPtr<ID3D11PixelShader> myShader;

        std::string myPath;
};
