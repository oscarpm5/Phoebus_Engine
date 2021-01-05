#ifndef __C_AUDIO_SOURCE__
#define __C_AUDIO_SOURCE__

#include "Component.h"
#include <string>
#include <vector>
#include "AudioEvent.h"

class C_AudioSource:public Component
{
public:
	C_AudioSource(GameObject* owner, unsigned int ID);
	~C_AudioSource();

	void OnEditor();
	bool Update(float dt)override;
	bool GameUpdate(float gameDT)override;
	bool GameInit()override;

	float GetVolume()const;
	void SetVolume(float volume);

	void CreateAudioEvent(std::string name, AudioEventType type, float changeTracksTime = 0.0f);
	void DeleteAudioEvent(std::string name);
	bool DoesAudioEventExist(std::string name);
	void DeleteAllAudioEvents();
	void ResetAllAudioEvents();
	const std::vector<AudioEvent*> GetEvents()const;
	void SetSecondsToChangeMusic(float newSeconds);
	float GetSecondsToChangeMusic()const;
	void SetUserPitch(float newPitch);
	float GetUserPitch()const;
	float PitchCalculationFromDT(float gamedt)const;
	bool CheckReverbZones();
	void ApplyReverb(float revValue, const char* targetBus);

private:

	float musicChangeTime;//user set variable, it controls when the music has to change
	float musicTimeCounter;//just a timer
	float volume;
	float userPitch;
	float scalingMod; //used to scale pitch modification
	char newEvName[250] = "";
	std::vector<AudioEvent*> events;
};

#endif // !__C_AUDIO_SOURCE__

