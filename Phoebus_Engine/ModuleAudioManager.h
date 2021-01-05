#ifndef __MODULE_AUDIO_MANAGER__
#define __MODULE_AUDIO_MANAGER__

#include "Module.h"
#include "Globals.h"
#include "C_AudioListener.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"
#include "AK/SoundEngine/Common/AkTypes.h" // cant forward typedefs if extern, apparently
#include <string>
#include <vector>

class C_ReverbZone; // C_ReverbZone.h included in .cpp

class ModuleAudioManager:public Module
{
public:
	ModuleAudioManager(bool start_enabled = true);
	~ModuleAudioManager();

	bool Init();
	bool GameInit();
	bool Start();

	update_status Update(float dt) override;
	update_status PostUpdate(float dt)override;
	update_status GameUpdate(float dt) override;
	void UpdateListener();
	bool CleanUp();

	void RegisterNewAudioObj(unsigned int componentID);
	void UnRegisterAudioObj(unsigned int componentID);
	void UnRegisterAllAudioObjs()const;
	void SetAudioObjTransform(unsigned int componentID, float4x4 transform);
	void SendAudioObjEvent(unsigned int componentID, std::string eventName)const;
	void SendStopEvent(unsigned int componentID, std::string eventName)const;
	void ChangeRTPCValue(unsigned int componentID, std::string RPTCname, float value);

	void StopAllSounds()const;
	void PauseAllSounds()const;
	void ResumeAllSounds()const;

	void AddRevZone(C_ReverbZone* revZone);
	bool RemoveRevZone(C_ReverbZone* revZone);
	void AuxSendValues(AkReal32 value, const char* targetBus, unsigned long listenerID, unsigned long sourceID) const;

public:

	C_AudioListener* activeListener;
	std::vector <C_ReverbZone*> revAreas;
};




#endif // !__MODULE_AUDIO_MANAGER__
