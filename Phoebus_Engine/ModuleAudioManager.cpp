#include "ModuleAudioManager.h"
#include <assert.h>
#include "wwise_libraries.h"
#include "Wwise/low_level_IO/Win32/AkFilePackageLowLevelIOBlocking.h"
//#include "AK/DefaultIO/Win32/AkFilePackageLowLevelIOBlocking.h"

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

ModuleAudioManager::ModuleAudioManager(bool start_enabled) :Module(start_enabled)
{
}

ModuleAudioManager::~ModuleAudioManager()
{
}

bool ModuleAudioManager::Init()
{/*
	AkMemSettings memSettings;

	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{

		assert(!"Could not create the memory manager.");

		return false;

	}*/

	return true;
}

update_status ModuleAudioManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleAudioManager::CleanUp()
{
	return true;
}
