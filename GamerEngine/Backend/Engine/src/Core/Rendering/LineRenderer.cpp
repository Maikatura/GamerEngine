#include "GraphicsEngine.pch.h"
#include "LineRenderer.h"
#include "GraphicsEngine.h"
#include "Core/Framework/DX11.h"
#include <d3d11.h>
#include <fstream>
#include "Core/Rendering/Renderer.h"
#include "Input/Input.h"


LineRenderer& LineRenderer::Get()
{
#ifdef _DISTRIBUTON
    GE_ASSERT("You should not use the linerenderer on a distribution build...")
#endif

    
    static LineRenderer instance;
    return instance;
}

bool LineRenderer::Init()
{
#if defined(_DEBUG) || defined(_RELEASE)
    HRESULT result;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = nullptr;


    D3D11_INPUT_ELEMENT_DESC layout[3] = {
        {
            "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"WIDTH", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    std::ifstream vsFile;
    vsFile.open("Shaders\\Lineshader_VS.cso", std::ios::binary);
    std::string vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};
    result = DX11::Get().GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr,
                                                         myLineVertexShader.GetAddressOf());
    if (FAILED(result))
    {
        return false;
    }
    vsFile.close();

    std::ifstream psFile;
    psFile.open("Shaders\\Lineshader_PS.cso", std::ios::binary);
    std::string psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};
    result = DX11::Get().GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr,
                                                        myLinePixelShader.GetAddressOf());
    if (FAILED(result))
    {
        return false;
    }
    psFile.close();

    std::ifstream gsFile;
    gsFile.open("Shaders\\Lineshader_GS.cso", std::ios::binary);
    std::string gsData = {std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>()};
    result = DX11::Get().GetDevice()->CreateGeometryShader(gsData.data(), gsData.size(), nullptr,
                                                           myLineGeometryShader.GetAddressOf());
    if (FAILED(result))
    {
        return false;
    }
    gsFile.close();

    result = DX11::Get().GetDevice()->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),
                                                        vsData.data(), vsData.size(), myInputLayout.GetAddressOf());
    if (FAILED(result))
    {
        return false;
    }

    D3D11_BUFFER_DESC bufferDescription;
    ZeroMemory(&bufferDescription, sizeof(bufferDescription));
    bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    bufferDescription.ByteWidth = sizeof(LineCBufferData);
    result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myLineCBuffer.GetAddressOf());
    if (FAILED(result))
    {
        return false;
    }

    bufferDescription.ByteWidth = sizeof(LineVertex) * 2;
    bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    result = DX11::Get().GetDevice()->CreateBuffer(&bufferDescription, nullptr, myBuffer.GetAddressOf());
    if (FAILED(result))
    {
        return false;
    }
#endif
    return true;
}

void LineRenderer::DrawPoint(Vector3f aPosition, Vector4f aColor)
{
#if defined(_DEBUG) || defined(_RELEASE)

    if (!myLineRendererOn)
    {
        return;
    }

    LineVertex lineOne = LineVertex({aPosition.x, aPosition.y, aPosition.z, 1.0f}, aColor);
    LineVertex lineTwo = LineVertex({aPosition.x, aPosition.y, aPosition.z, 1.0f}, aColor);
    
    std::array<LineVertex, 2> point = {
        lineOne,
        lineTwo
    };

    myLinesToRender.push_back(point);
#endif
}

void LineRenderer::DrawLine(const Vector3f& aStartPoint, const Vector3f& aEndPoint, const Vector4f& aColor,
                            float aWidth)
{
#if defined(_DEBUG) || defined(_RELEASE)

    if (!myLineRendererOn)
    {
        return;
    }

    LineVertex lineOne = LineVertex({aStartPoint.x, aStartPoint.y, aStartPoint.z, 1.0f}, aColor, aWidth);
    LineVertex lineTwo = LineVertex({aEndPoint.x, aEndPoint.y, aEndPoint.z, 1.0f}, aColor, aWidth);
    
    std::array<LineVertex, 2> lines = {
        lineOne,
        lineTwo
    };

    myLinesToRender.push_back(lines);
#endif
}

void LineRenderer::DrawCube(Vector3f aPosition, Vector3f aSize, Vector3f aRotation, Vector4f aColor)
{
#if defined(_DEBUG) || defined(_RELEASE)

    if (!myLineRendererOn)
    {
        return;
    }
    // Convert rotation from degrees to radians
    aRotation *= 3.1415f / 180.0f;

    // Create rotation matrix
    Matrix4x4f rotationMatrix = Matrix4x4f::CreateRotationAroundX(aRotation.x) *
        Matrix4x4f::CreateRotationAroundY(aRotation.y) * Matrix4x4f::CreateRotationAroundZ(aRotation.z);

    // Define the 8 points of the cube
    Vector3f halfSize = aSize * 0.5f;
    std::array<Vector3f, 8> points = {
        aPosition + Vector3f(-halfSize.x, -halfSize.y, -halfSize.z),
        aPosition + Vector3f(halfSize.x, -halfSize.y, -halfSize.z),
        aPosition + Vector3f(halfSize.x, -halfSize.y, halfSize.z),
        aPosition + Vector3f(-halfSize.x, -halfSize.y, halfSize.z),
        aPosition + Vector3f(-halfSize.x, halfSize.y, -halfSize.z),
        aPosition + Vector3f(halfSize.x, halfSize.y, -halfSize.z),
        aPosition + Vector3f(halfSize.x, halfSize.y, halfSize.z),
        aPosition + Vector3f(-halfSize.x, halfSize.y, halfSize.z)
    };

    // Apply rotation to each point
    for (Vector3f& point : points)
    {
        Vector3f relativePosition = point - aPosition;
        Vector3f rotatedPosition = rotationMatrix * relativePosition;
        point = aPosition + rotatedPosition;
    }

    // Draw the 12 edges of the cube
    for (int i = 0; i < 4; ++i)
    {
        DrawLine(points[i], points[(i + 1) % 4], aColor); // Bottom square
        DrawLine(points[i + 4], points[((i + 1) % 4) + 4], aColor); // Top square
        DrawLine(points[i], points[i + 4], aColor); // Vertical lines
    }

#endif
}

void LineRenderer::DrawAABB3D(const CommonUtilities::AABB3D<float>& aAABB, Vector4f aColor)
{
#if defined(_DEBUG) || defined(_RELEASE)

    if (!myLineRendererOn)
    {
        return;
    }
    
    Vector3f min = aAABB.GetMin();
    Vector3f max = aAABB.GetMax();

    Vector3f p1 = {min.x, min.y, min.z};
    Vector3f p2 = {max.x, min.y, min.z};
    Vector3f p3 = {max.x, max.y, min.z};
    Vector3f p4 = {min.x, max.y, min.z};
    Vector3f p5 = {min.x, min.y, max.z};
    Vector3f p6 = {max.x, min.y, max.z};
    Vector3f p7 = {max.x, max.y, max.z};
    Vector3f p8 = {min.x, max.y, max.z};

    DrawLine(p1, p2, aColor);
    DrawLine(p2, p3, aColor);
    DrawLine(p3, p4, aColor);
    DrawLine(p4, p1, aColor);

    DrawLine(p5, p6, aColor);
    DrawLine(p6, p7, aColor);
    DrawLine(p7, p8, aColor);
    DrawLine(p8, p5, aColor);

    DrawLine(p1, p5, aColor);
    DrawLine(p2, p6, aColor);
    DrawLine(p3, p7, aColor);
    DrawLine(p4, p8, aColor);

#endif
}

void LineRenderer::DrawCircle(Vector3f aPosition, float aRadius, int aTesselation) const
{
#ifdef _DISTRIBUTION
    aPosition;
    aRadius;
    aTesselation;
#endif
#if defined(_DEBUG) || defined(_RELEASE)
    if (!myLineRendererOn)
    {
        return;
    }
    
    /*for(int i = 0; i < aTesselation; i++)
    {
        double angle = double(i) / double(aTesselation) * 3.14f * 2;
        Vector2f offset(sin(angle) * aRadius, cos(angle) * aRadius);
        DrawPoint({ aPosition.x + offset.x, aPosition.y + offset.y, 0.0f });
    }*/
#endif
}

void LineRenderer::Update()
{
#if defined(_DEBUG) || defined(_RELEASE)
    if (Input::IsKeyPressed(0x70))
    {
        myLineRendererOn = !myLineRendererOn;
    }
#endif
}

void LineRenderer::Render(const Matrix4x4f& aView, const Matrix4x4f& aProjection)
{
#if defined(_DEBUG) || defined(_RELEASE)

    if (!myLineRendererOn)
    {
        return;
    }
    
    D3D11_MAPPED_SUBRESOURCE bufferData;

    myLineCBufferData.View = Matrix4x4f::GetFastInverse(aView);
    myLineCBufferData.Projection = aProjection;
    myLineCBufferData.World = Matrix4x4f();

    ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
    HRESULT result = DX11::Get().GetContext()->Map(myLineCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
    if (FAILED(result))
    {
        // BOOM?
        return;
    }

    memcpy_s(bufferData.pData, sizeof(LineCBufferData), &myLineCBufferData, sizeof(LineCBufferData));
    DX11::Get().GetContext()->Unmap(myLineCBuffer.Get(), 0);

    DX11::Get().GetContext()->VSSetConstantBuffers(6, 1, myLineCBuffer.GetAddressOf());

    UINT stride = sizeof(LineVertex);
    UINT offset = 0;

    // TODO : Skip the lines that is outside the camera (NOTE do it for models and will will have it for this too) (Also if they are out side the Far Plane distance on the camera skip them too)

    for (auto line : myLinesToRender)
    {
        DX11::Get().GetContext()->IASetInputLayout(myInputLayout.Get());
        DX11::Get().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

        DX11::Get().GetContext()->VSSetShader(myLineVertexShader.Get(), nullptr, 0);
        //DX11::Get().GetContext()->GSSetShader(myLineGeometryShader.Get(), NULL, 0);
        DX11::Get().GetContext()->PSSetShader(myLinePixelShader.Get(), nullptr, 0);

        D3D11_MAPPED_SUBRESOURCE lineData;
        HRESULT hResult = DX11::Get().GetContext()->Map(myBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lineData);

        if (FAILED(hResult))
        {
            return;
        }

        memcpy(lineData.pData, line.data(), sizeof(LineVertex) * 2);
        DX11::Get().GetContext()->Unmap(myBuffer.Get(), 0);

        DX11::Get().GetContext()->IASetVertexBuffers(0, 1, myBuffer.GetAddressOf(), &stride, &offset);
        DX11::Get().GetContext()->Draw(2, 0);
    }

    Clear();
#endif
}

void LineRenderer::Clear()
{
#if defined(_DEBUG) || defined(_RELEASE)
    myLinesToRender.clear();
#endif
}
