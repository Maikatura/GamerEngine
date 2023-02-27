#pragma once
#include <unordered_map>

#include "Renderer/Particles/ParticleEmitterTemplate.h"


class ParticleSystem;

class ParticleAssetHandler
{
	typedef std::unordered_map<std::wstring, ParticleEmitterTemplate> EmitterTemplateMap;
	//typedef std::unordered_map<std::wstring, ParticleSystemTemplate> SystemTemplateMap;


	inline static EmitterTemplateMap myEmitterTemplates{};
	//inline static SystemTemplateMap mySystemTemplates{};

public:

	static bool LoadSystemTemplate(const std::wstring& someFilePath);
	static bool LoadEmitterTemplate(const std::wstring& someFilePath);
	static std::shared_ptr<ParticleSystem> GetParticleSystem(const std::wstring& aSystemName);
};
