#include "ModuleAudioManager.h"
#include "wwise_libraries.h"
#include "Wwise/include/Win32/AkFilePackageLowLevelIOBlocking.h"

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

ModuleAudioManager::ModuleAudioManager(bool start_enabled) :Module(start_enabled)
{
}

ModuleAudioManager::~ModuleAudioManager()
{
}

bool ModuleAudioManager::Init()
{
	AkMemSettings memSettings;

	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)

	{
		LOG("[error]Could not create the memory manager.");

		return false;

	}
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	// Customize the Stream Manager settings here.
	if (!AK::StreamMgr::Create(stmSettings))
	{
		LOG("[error]Could not create the Streaming Manager");
		return false;
	}
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		LOG("[error]Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		LOG("[error]Could not initialize the Sound Engine.");
		return false;
	}
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);
	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		LOG("[error]Could not initialize the Music Engine.");
		return false;
	}
#ifndef AK_OPTIMIZED
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		LOG("[error]Could not initialize communication.");
		return false;
	}
#endif // AK_OPTIMIZED


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
