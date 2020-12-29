#include "ModuleAudioManager.h"

#include <AK/SoundEngine/Common/AkTypes.h>

#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager interface
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory manager


#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
//#include <AK/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h>// Sample low-level I/O implementation
#include <AK/SoundEngine/Win32/AkDefaultIOHookBlocking.h>
#include <AK/SoundEngine/Win32/AkFileHelpers.h>

//TODO FOR OSCAR those are included as extra by the quick integration
#include <AK/SoundEngine/Common/AkDefaultLowLevelIODispatcher.h>
#include <AK/SoundEngine/Common/AkFileLocationBase.h>
#include <AK/SoundEngine/Common/AkFilePackage.h>
#include <AK/SoundEngine/Common/AkFilePackageLowLevelIO.h>
#include <AK/SoundEngine/Common/AkFilePackageLUT.h>
#include <AK/SoundEngine/Common/AkMultipleFileLocation.h>
//End of TODO

#include <AK/SoundEngine/Common/AkSoundEngine.h>                // Sound engine

#include <AK/MusicEngine/Common/AkMusicEngine.h>                // Music Engine

#include <AK/SpatialAudio/Common/AkSpatialAudio.h>              // Spatial Audio



// Include for communication between Wwiseand the game -- Not needed in the release version
#ifndef AK_OPTIMIZED

#include <AK/Comm/AkCommunication.h>

#endif // AK_OPTIMIZED

#pragma comment(lib,"Wwise/Profile(StaticCRT)/lib/AkSoundEngine.lib")			// Sound Engine Core
#pragma comment(lib,"Wwise/Profile(StaticCRT)/lib/AkMemoryMgr.lib")				// Memory Manager
#pragma comment(lib,"Wwise/Profile(StaticCRT)/lib/AkStreamMgr.lib")				// I/O Manager
#pragma comment(lib,"Wwise/Profile(StaticCRT)/lib/AkMusicEngine.lib")			// Music Engine
#pragma comment(lib,"Wwise/Profile(StaticCRT)/lib/AkSpatialAudio.lib")			// Built-in spatial audio features
#pragma comment(lib,"Wwise/Profile(StaticCRT)/lib/CommunicationCentral.lib")	// Wwise Communication: Not needed for the release configuration.




ModuleAudioManager::ModuleAudioManager(bool start_enabled) :Module(start_enabled)
{
}

ModuleAudioManager::~ModuleAudioManager()
{
}

bool ModuleAudioManager::Init()
{
	CAkDefaultIOHookBlocking gLowLevelIO;
	
	//TODO REMEMBER TO FREE ALL INITS SOMEHOW


	//Initializing the Memory Manager===========================================

	AkMemSettings memSettings;

	AK::MemoryMgr::GetDefaultSettings(memSettings);
	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)

	{

		LOG("[error] Could not create the memory manager.");
		return false;
	}


	//Initializing the Streaming Manager=======================================

	// Create and initialize an instance of the default streaming manager. Note
	// that you can override the default streaming manager with your own. 

	AkStreamMgrSettings stmSettings;

	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.
	if (!AK::StreamMgr::Create(stmSettings))
	{

		LOG("[error] Could not create the Streaming Manager");

		return false;
	}

	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. 
	AkDeviceSettings deviceSettings;

	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	// Customize the streaming device settings here.
	// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
	// in the Stream Manager, and registers itself as the File Location Resolver.

	if (gLowLevelIO.Init(deviceSettings) != AK_Success)
	{

		LOG("[error] Could not create the streaming device and Low-Level I/O system");

		return false;

	}

	//Initializing the Sound Engine===========================================

	// Create the Sound Engine using default initialization parameters

	AkInitSettings initSettings;

	AkPlatformInitSettings platformInitSettings;

	AK::SoundEngine::GetDefaultInitSettings(initSettings);

	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);



	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		LOG("[error] Could not initialize the Sound Engine.");

		return false;
	}

	//Initializing the Music Engine===========================================
	// Initialize the music engine using default initialization parameters

	AkMusicSettings musicInit;

	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		LOG("[error] Could not initialize the Music Engine.");

		return false;
	}

	//Initializing Spatial Audio=============================================
	// Initialize Spatial Audio using default initialization parameters

	AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

	if (AK::SpatialAudio::Init(settings) != AK_Success) //TODO in the example it uses &settings
	{
		LOG("[error] Could not initialize the Spatial Audio.");

		return false;
	}


#ifndef AK_OPTIMIZED

	// Initialize communications (not in release build!)=====================


	AkCommSettings commSettings;

	AK::Comm::GetDefaultInitSettings(commSettings);

	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		LOG("[error] Could not initialize communication.");

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
