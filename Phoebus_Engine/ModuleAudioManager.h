#ifndef __MODULE_AUDIO_MANAGER__
#define __MODULE_AUDIO_MANAGER__

#include "Module.h"
#include "Globals.h"
#include "C_AudioListener.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"
#include <string>


class ModuleAudioManager:public Module
{
public:
	ModuleAudioManager(bool start_enabled = true);
	~ModuleAudioManager();

	bool Init();
	bool GameInit();
	bool Start();

	update_status Update(float dt) override;
	update_status GameUpdate(float dt) override;
	void UpdateListener();
	bool CleanUp();

	void RegisterNewAudioObj(unsigned int componentID);
	void UnRegisterAudioObj(unsigned int componentID);
	void UnRegisterAllAudioObjs()const;
	void SetAudioObjTransform(unsigned int componentID, float4x4 transform);
	void SendAudioObjEvent(unsigned int componentID, std::string eventName);
	void ChangeRTPCValue(unsigned int componentID, std::string RPTCname, float value);

public:

	C_AudioListener* activeListener;

};




#endif // !__MODULE_AUDIO_MANAGER__
