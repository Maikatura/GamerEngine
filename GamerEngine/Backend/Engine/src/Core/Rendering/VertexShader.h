#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#ifndef InputLayoutSize
#define InputLayoutSize 18
#endif

class VertexShader
{
    
    
    public:
        VertexShader();
        VertexShader(const std::string& aPath);

        const std::string& Path();

        Microsoft::WRL::ComPtr<ID3D11VertexShader> Get();
        Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout();

        inline static std::array<D3D11_INPUT_ELEMENT_DESC, InputLayoutSize> CreateLayout();

    private:

        std::string myPath;
        
        Microsoft::WRL::ComPtr<ID3D11VertexShader> myShader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> myInputLayout;
};
