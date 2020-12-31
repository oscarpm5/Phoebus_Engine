#ifndef __MODULE_AUDIO_MANAGER__
#define __MODULE_AUDIO_MANAGER__

#include "Module.h"
#include "Globals.h"
#include "C_AudioListener.h"



class ModuleAudioManager:public Module
{
public:
	ModuleAudioManager(bool start_enabled = true);
	~ModuleAudioManager();

	bool Init();
	bool Start();

	update_status Update(float dt) override;
	update_status GameUpdate(float dt) override;
	bool CleanUp();

public:

	C_AudioListener* activeListener;

};




#endif // !__MODULE_AUDIO_MANAGER__
