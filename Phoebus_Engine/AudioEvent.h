#ifndef __AUDIO_EVENT_H__
#define __AUDIO_EVENT_H__
#include <string>
enum class AudioEventType
{
	SFX,
	MUSIC,
	UNKNOWN
};

class AudioEvent
{
public:
	AudioEvent(std::string eventName, AudioEventType type,float changeTrackTime=0.0f);
	~AudioEvent();
	std::string GetEventName()const;
	void ResetPlayed();
	bool GetPlayed()const;
	void StartPlaying();
	AudioEventType GetType()const;
	float GetTrackTimeChange()const;
	void SetTrackTimeChange(float newTime);
	void ResetTimer();

private:
	bool hasPlayed; //this bool shows whether the audio has been played or not

	AudioEventType type;
	float changeTrackTime;
	float currentTime;
	std::string eventName;
public:
	bool toDelete;
};


#endif // !__AUDIO_EVENT_H__

