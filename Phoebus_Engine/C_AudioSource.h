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
	bool GameUpdate(float gameDT)override;
	bool GameInit()override;

	float GetVolume()const;
	void SetVolume(float volume);

	void CreateAudioEvent(std::string name, AudioEventType type, float changeTracksTime = 0.0f);
	void DeleteAudioEvent(std::string name);
	bool DoesAudioEventExist(std::string name);
	void DeleteAllAudioEvents();
	void ResetAllAudioEvents();

private:
	float volume;
	std::vector<AudioEvent*> events;
	char newEvName[250] = "";
};

#endif // !__C_AUDIO_SOURCE__

