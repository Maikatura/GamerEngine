#include "GraphicsEngine.pch.h"
#include <Particles/ParticleEmitter.h>
#include "Framework/DX11.h"
#include "Render/Renderer.h"
#include <fstream>
#include <Render/Render2D.h>
#include <Sort/Sort.hpp>
#include <Utilites/UtilityFunctions.h>
#include <Model/Texture.h>
#include "Components/ScriptableEntity.h"

ParticleEmitter::ParticleEmitter()
{
	std::vector<ParticleBehaviourTimePoint> data;
	Init();
}

bool ParticleEmitter::Init()
{

	ParticleBehaviourTimePoint pbtp;
	pbtp.myScale = Vector3f{ 1,1,1 };
	pbtp.myVelocity = Vector3f{ 0,0,0 };
	pbtp.myColor = Vector4f{ 1,0,0,1 };
	pbtp.myAcceleration = Vector3f{ 0,10.0f,0 };
	pbtp.myDrag = 0.f;
	pbtp.myRotationalDrag = -.1f;
	myParticleTimePoints.push_back(pbtp);

	pbtp.myScale = Vector3f{ 0.0f,0.0f,0.0f };
	pbtp.myColor = { 1, 0, 1, 1 };
	pbtp.myAcceleration = { .0f , 20.0f, 0.0f };
	myParticleTimePoints.push_back(pbtp);

	ParticleEmitterBehaviourTimePoint psbtp;
	psbtp.myMaxAngle = { 45.f, 45.f, 45.f };
	psbtp.myMinAngle = { -45.f, -45.f, -45.f };
	psbtp.myMaxFrequency = .001f;
	psbtp.myMinFrequency = .0005f;
	psbtp.myMaxLifeTime = 4.f;
	psbtp.myMinLifeTime = 3.f;
	psbtp.myMaxSpeed = 4.f;
	psbtp.myMinSpeed = 1.f;
	psbtp.myMaxRotationSpeed = -20.f;
	psbtp.myMinRotationSpeed = 20.f;
	psbtp.myPosition = { 0.f, 0.f, 0.f };

	myEmitterTimePoints.push_back(psbtp);
	myEmitterTimePoints.push_back(psbtp);

	
	myBlendState = 1;
	myLifeTime = 3.f;
	myShouldLoop = true;
	myIsActive = true;
	myOffset = Vector3f{0,0,0};

	myAbsoluteMaxSimultaniousParticles = 4000;

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SCALE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"CENTER", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	std::ifstream vsFile;
	vsFile.open("Shaders\\SpriteVS.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator(vsFile), std::istreambuf_iterator<char>() };
	DX11::Device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, myVertexShader.GetAddressOf());
	vsFile.close();

	std::ifstream gsFile;
	gsFile.open("Shaders\\SpriteGS.cso", std::ios::binary);
	std::string gsData = { std::istreambuf_iterator(gsFile), std::istreambuf_iterator<char>() };
	DX11::Device->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, myGeometryShader.GetAddressOf());
	gsFile.close();

	std::ifstream psFile;
	psFile.open("Shaders\\SpritePS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator(psFile), std::istreambuf_iterator<char>() };
	DX11::Device->CreatePixelShader(psData.data(), psData.size(), nullptr, myPixelShader.GetAddressOf());
	psFile.close();

	HRESULT res = DX11::Device->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), myInputLayout.GetAddressOf());

	myVertexStride = sizeof(SpriteVertex);
	myVertexOffset = 0;
	myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	particleVertexData.resize(myAbsoluteMaxSimultaniousParticles);

	D3D11_BUFFER_DESC vertBufferDesc{};
	vertBufferDesc.ByteWidth = myAbsoluteMaxSimultaniousParticles * sizeof(SpriteVertex);
	vertBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertSubData{};
	vertSubData.pSysMem = &particleVertexData[0];

	DX11::Device->CreateBuffer(&vertBufferDesc, &vertSubData, myVertexBuffer.GetAddressOf());

	if(!TextureAssetHandler::LoadTexture(L"Assets\\Textures\\ParticleStar.dds"))
		myTexture = TextureAssetHandler::GetTexture(L"Assets\\Textures\\ParticleStar.dds");

	return true;
}

void ParticleEmitter::OnUpdate(TransformComponent& aTransform)
{
	myTransform = &aTransform;

	myAge += Time::GetDeltaTime();
	if(myAge < 0)
	{
		myAge = 0;
	}
	if(myAge > myLifeTime)
	{
		if(myShouldLoop)
		{
			myAge = 0;
		}
		else
		{
			myAge = myLifeTime;
			myIsActive = false;
		}
	}
	for(int i = 0; i < myParticles.size(); i++)
	{
		float particlePoint;
		particlePoint = (myParticles[i].myAge / myParticles[i].myLifeTime) * (myParticleTimePoints.size() - 1);
		if(particlePoint >= myParticleTimePoints.size() - 1) particlePoint = 0;
		float t = particlePoint - (int)particlePoint;
		myParticles[i].myAge += Time::GetDeltaTime();
		myParticles[i].myCurrentRotationalVelocity -= myParticles[i].myCurrentRotationalVelocity * ((CommonUtilities::LerpMinMax(myParticleTimePoints[(int)particlePoint].myRotationalDrag, myParticleTimePoints[(int)particlePoint + 1].myRotationalDrag, t)) * Time::GetDeltaTime());
		myParticles[i].myRotation += myParticles[i].myCurrentRotationalVelocity;
		myParticles[i].myCurrentVelocity -= myParticles[i].myCurrentVelocity * ((CommonUtilities::LerpMinMax(myParticleTimePoints[(int)particlePoint].myDrag, myParticleTimePoints[(int)particlePoint + 1].myDrag, t)) * Time::GetDeltaTime());
		myParticles[i].myCurrentVelocity += ((CommonUtilities::LerpMinMax(myParticleTimePoints[(int)particlePoint].myAcceleration, myParticleTimePoints[(int)particlePoint + 1].myAcceleration, t)) * Time::GetDeltaTime());
		myParticles[i].myPosition += ((CommonUtilities::LerpMinMax(myParticleTimePoints[(int)particlePoint].myVelocity, myParticleTimePoints[(int)particlePoint + 1].myVelocity, t)) * Time::GetDeltaTime()) + myParticles[i].myCurrentVelocity * Time::GetDeltaTime();
		if(myParticles[i].myAge >= myParticles[i].myLifeTime)
		{
			myParticles.erase(myParticles.begin() + i--);
		}
	}

	if(myTimeToNextSpawn < 0 && myIsActive)
	{
		float systemPoint;
		systemPoint = (myAge / myLifeTime) * (myEmitterTimePoints.size() - 1);
		if(systemPoint >= myEmitterTimePoints.size() - 1) systemPoint = 0;

		const int systemPointInt = static_cast<int>(systemPoint);

		myTimeToNextSpawn = CommonUtilities::LerpMinMax(
			CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMinFrequency,
			                            myEmitterTimePoints[systemPointInt + 1].myMinFrequency,
				systemPoint- systemPoint),
			CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMaxFrequency,
			                            myEmitterTimePoints[systemPointInt + 1].myMaxFrequency,
			                            systemPoint - systemPoint), (rand() % 1000) * .001f);


		float particlesPerFrame = Time::GetDeltaTime() / myTimeToNextSpawn;

		do
		{
			ParticleVertex p;
			p.myAge = 0;

			int systemPointInt = static_cast<int>(systemPoint);

			p.myLifeTime = CommonUtilities::LerpMinMax(CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMinLifeTime, myEmitterTimePoints[systemPointInt + 1].myMinLifeTime, systemPoint - systemPoint), CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMaxLifeTime, myEmitterTimePoints[systemPointInt + 1].myMaxLifeTime, systemPoint - systemPointInt), (rand() % 1000) * .001f);
			p.myPosition = aTransform.Translation + myOffset + CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myPosition, myEmitterTimePoints[systemPointInt + 1].myPosition, systemPoint - systemPoint);
			p.myRotation = 0;
			Vector3f angle;
			angle.x += CommonUtilities::LerpMinMax(CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMinAngle.x, myEmitterTimePoints[systemPointInt + 1].myMinAngle.x, systemPoint - systemPoint), CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMaxAngle.x, myEmitterTimePoints[systemPointInt + 1].myMaxAngle.x, systemPoint - systemPointInt), (rand() % 1000) * .001f);
			angle.y += CommonUtilities::LerpMinMax(CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMinAngle.y, myEmitterTimePoints[systemPointInt + 1].myMinAngle.y, systemPoint - systemPoint), CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMaxAngle.y, myEmitterTimePoints[systemPointInt + 1].myMaxAngle.y, systemPoint - systemPointInt), (rand() % 1000) * .001f);
			angle.z += CommonUtilities::LerpMinMax(CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMinAngle.z, myEmitterTimePoints[systemPointInt + 1].myMinAngle.z, systemPoint - systemPoint), CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMaxAngle.z, myEmitterTimePoints[systemPointInt + 1].myMaxAngle.z, systemPoint - systemPointInt), (rand() % 1000) * .001f);
			angle *= 3.14159f / 180;

			auto anglev4 = Matrix4x4f::CreateRotationAroundX(angle.x) * Matrix4x4f::CreateRotationAroundY(angle.y) * Matrix4x4f::CreateRotationAroundZ(angle.z) * Vector4f(0, 1, 0, 0);
			angle.x = anglev4.x;
			angle.y = anglev4.y;
			angle.z = anglev4.z;

			float speed = CommonUtilities::LerpMinMax(CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMinSpeed, myEmitterTimePoints[systemPointInt + 1].myMinSpeed, systemPoint - systemPoint), CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMaxSpeed, myEmitterTimePoints[systemPointInt + 1].myMaxSpeed, systemPoint - systemPointInt), (rand() % 1000) * .001f);
			p.myCurrentVelocity = angle * speed;
			p.myCurrentRotationalVelocity = CommonUtilities::LerpMinMax(CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMinRotationSpeed, myEmitterTimePoints[systemPointInt + 1].myMinRotationSpeed, systemPoint - systemPoint), CommonUtilities::LerpMinMax(myEmitterTimePoints[systemPointInt].myMaxRotationSpeed, myEmitterTimePoints[systemPointInt + 1].myMaxRotationSpeed, systemPoint - systemPointInt), (rand() % 1000) * .001f);
			myParticles.push_back(p);
			particlesPerFrame--;
		} while(particlesPerFrame > 0);

		CommonUtilities::MergeSort(myParticles);
	}
	else
	{
		myTimeToNextSpawn -= Time::GetDeltaTime();
	}
}

void ParticleEmitter::InitParticle(size_t aParticleIndex)
{
	
}

void ParticleEmitter::SetAsResource()
{
	if(myParticles.empty()) return;
	float particlePoint;
	int i = 0;
	for(; i < myParticles.size(); i++)
	{
		if(particleVertexData.size() < myParticles.size()) particleVertexData.push_back({});
		particlePoint = (myParticles[i].myAge / myParticles[i].myLifeTime) * (myParticleTimePoints.size() - 1);
		if(particlePoint >= myParticleTimePoints.size() - 1) particlePoint = 0;
		float t = particlePoint - (int)particlePoint;
		particleVertexData[i].myPosition = myParticles[i].myPosition;
		particleVertexData[i].myZRotation = myParticles[i].myRotation;
		particleVertexData[i].myColor = CommonUtilities::LerpMinMax(myParticleTimePoints[(int)particlePoint].myColor, myParticleTimePoints[(int)particlePoint + 1].myColor, particlePoint - (int)particlePoint) * myParticles[i].myAge;
		auto scale = CommonUtilities::LerpMinMax(myParticleTimePoints[(int)particlePoint].myScale, myParticleTimePoints[(int)particlePoint + 1].myScale, particlePoint - (int)particlePoint) * myParticles[i].myAge;

		particleVertexData[i].myScale = { (scale.x * myTransform->Scale.x) * 0.1f, (scale.y * myTransform->Scale.y) * 0.1f };
		particleVertexData[i].myPivot = { .5f, .5f };
	}
	for(; i < particleVertexData.size(); i++)
	{
		particleVertexData[i].myScale = Vector2f{0,0};
	}

	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT result = DX11::Context->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	memcpy(bufferData.pData, &particleVertexData[0], sizeof(SpriteVertex) * myAbsoluteMaxSimultaniousParticles);
	DX11::Context->Unmap(myVertexBuffer.Get(), 0);

	DX11::Context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &myVertexStride, &myVertexOffset);
	DX11::Context->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)myPrimitiveTopology);
	DX11::Context->IASetInputLayout(myInputLayout.Get());
	DX11::Context->VSSetShader(myVertexShader.Get(), nullptr, 0);
	DX11::Context->GSSetShader(myGeometryShader.Get(), nullptr, 0);
	DX11::Context->PSSetShader(myPixelShader.Get(), nullptr, 0);

	if(myTexture)
	{
		myTexture->SetAsResource(0);
	}
}

void ParticleEmitter::Draw() const
{
	if(myParticles.empty()) return;

	DX11::Context->Draw((UINT)particleVertexData.size(), 0);
}
