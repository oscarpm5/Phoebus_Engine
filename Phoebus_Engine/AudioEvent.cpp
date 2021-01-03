#include "AudioEvent.h"
#include "Globals.h"

AudioEvent::AudioEvent(std::string eventName, AudioEventType type, float changeTrackTime):
	eventName(eventName),type(type),changeTrackTime(changeTrackTime),hasPlayed(false),currentTime(0.0f)
{
	if (this->type == AudioEventType::MUSIC)
	{
		float auxChangeTrackTime = max(changeTrackTime,0.0f);

		if (changeTrackTime != 0.0f)
			this->changeTrackTime = changeTrackTime;
		else
			this->changeTrackTime = 30.0f;
	}
}

AudioEvent::~AudioEvent()
{
	hasPlayed = false;
	eventName.clear();
	changeTrackTime = -1.0f;
	currentTime = -1.0f;
	type = AudioEventType::UNKNOWN;
}

std::string AudioEvent::GetEventName() const
{
	return eventName;
}

void AudioEvent::ResetPlayed()
{
	hasPlayed = false;
}

bool AudioEvent::GetPlayed() const
{
	return hasPlayed;
}

void AudioEvent::StartPlaying()
{
	hasPlayed = true;
}

AudioEventType AudioEvent::GetType() const
{
	return type;
}

float AudioEvent::GetTrackTimeChange() const
{
	return changeTrackTime;
}

void AudioEvent::SetTrackTimeChange(float newTime)
{
	changeTrackTime = max(newTime,0.0f);
}

void AudioEvent::ResetTimer()
{
	currentTime = 0.0f;
}
