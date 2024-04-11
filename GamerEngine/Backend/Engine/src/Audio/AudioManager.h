#pragma once
#include <memory>
#include <unordered_map>
#include <xstring>


class Audio;

class AudioManager
{

public:
	static void Init();
	static void PlayAudio(std::string aAudio);
	static void StopAudio(std::string aAudio);
	static void StopAllAudio();

private:

	//inline static std::unordered_map<std::string, Ref<SoLoud::Wav>> mySoundRegistry;
	//inline static SoLoud::Soloud mySoLoudInstance;
};