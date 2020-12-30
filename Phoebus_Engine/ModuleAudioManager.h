#ifndef __MODULE_AUDIO_MANAGER__
#define __MODULE_AUDIO_MANAGER__

#include "Module.h"
#include "Globals.h"



class ModuleAudioManager:public Module
{
public:
	ModuleAudioManager(bool start_enabled = true);
	~ModuleAudioManager();

	bool Init();

	update_status Update(float dt) override;
	bool CleanUp();

private:

};




#endif // !__MODULE_AUDIO_MANAGER__
