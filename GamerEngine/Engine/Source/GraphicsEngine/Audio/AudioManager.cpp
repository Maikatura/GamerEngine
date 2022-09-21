#include "GraphicsEngine.pch.h"
#include <Audio/AudioManager.h>
#include <soloud.h>
#include <soloud_speech.h>
#include <soloud_thread.h>
#include <soloud_wav.h>
#include <Audio/Audio.h>

void AudioManager::Init()
{
	mySoLoudInstance.init();
}


void AudioManager::PlayAudio(std::string aAudio)
{
	auto item = mySoundRegistry.find(aAudio);
	std::shared_ptr<SoLoud::Wav> audio;

	if (item == mySoundRegistry.end())
	{
		audio = std::make_shared<SoLoud::Wav>();
		audio->load(aAudio.c_str());

		mySoundRegistry.insert({ aAudio, audio });
	}
	else
	{
		audio = item->second;
	}

	mySoLoudInstance.stopAudioSource(*audio);
	mySoLoudInstance.play(*audio);
}

void AudioManager::StopAudio(std::string aAudio)
{
	auto item = mySoundRegistry.find(aAudio);
	std::shared_ptr<SoLoud::Wav> audio;


	if(item->second != nullptr)
	{
		audio = item->second;
		mySoLoudInstance.stopAudioSource(*audio);
	}
}

void AudioManager::StopAllAudio()
{
	mySoLoudInstance.stopAll();
}
