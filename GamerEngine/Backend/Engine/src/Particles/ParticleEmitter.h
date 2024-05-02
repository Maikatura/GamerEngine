#pragma once
#include <vector>
#include <wrl/client.h>
#include <d3d11.h>
#include <Particles/ParticleEmitterTemplate.h>

#include <Core/Model/Render2D.h>

#include "Utilites/Pointers.h"

namespace GamerEngine
{
	class TransformComponent;
}

class ScriptableEntity;
class Render2D;
class Texture;

class ParticleEmitter
{

	int myBlendState;
	float myLifeTime;
	bool myShouldLoop;
	int myAbsoluteMaxSimultaniousParticles;
	Vector3f myOffset;

	float myAge;
	float myTimeToNextSpawn;
	bool myIsActive;


	friend class ParticleAssetHandler;

	std::vector<ParticleVertex> myParticles;
	std::vector<ParticleBehaviourTimePoint> myParticleTimePoints;
	std::vector<ParticleEmitterBehaviourTimePoint> myEmitterTimePoints;

	Microsoft::WRL::ComPtr<ID3D11Buffer> myVertexBuffer;
	UINT myVertexStride;
	UINT myVertexOffset;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> myVertexShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> myGeometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> myPixelShader;
	UINT myPrimitiveTopology;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> myInputLayout;

	Ref<Texture> myTexture;

	EmitterSettingsData myEmittorSettings;

	GamerEngine::TransformComponent* myTransform;

	void InitParticle(size_t aParticleIndex);

public:

	ParticleEmitter();
	virtual ~ParticleEmitter() = default;

	virtual bool Init();
	void OnUpdate(GamerEngine::TransformComponent& aTransform);

	std::vector<SpriteVertex> particleVertexData;

	virtual void SetAsResource();
	virtual void Draw() const;

	FORCEINLINE std::vector<ParticleEmitterBehaviourTimePoint>& GetEmitterSettings() { return myEmitterTimePoints; }
	FORCEINLINE std::vector<ParticleBehaviourTimePoint>& GetSettings() { return myParticleTimePoints; };
};
