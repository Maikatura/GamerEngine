#pragma once
#include <memory>
#include <soloud.h>
#include <unordered_map>
#include <xstring>

#include "soloud_wav.h"

class Audio;

class AudioManager
{

public:

	static void Init();
	static void PlayAudio(std::string aAudio);
	static void StopAudio(std::string aAudio);
	static void StopAllAudio();

private:

	inline static std::unordered_map<std::string, std::shared_ptr<SoLoud::Wav>> mySoundRegistry;
	inline static SoLoud::Soloud mySoLoudInstance;
};