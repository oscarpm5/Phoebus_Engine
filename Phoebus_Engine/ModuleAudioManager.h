#ifndef __MODULE_AUDIO_MANAGER__
#define __MODULE_AUDIO_MANAGER__

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h> // Communication between Wwise and the game (excluded in release build)
#endif


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
