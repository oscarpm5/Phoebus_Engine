#include "ModuleAudioManager.h"
#include <assert.h> //TODO change asserts for logs
#include "wwise_libraries.h"
#include "Wwise/low_level_IO/Win32/AkFilePackageLowLevelIOBlocking.h"
//#include "AK/DefaultIO/Win32/AkFilePackageLowLevelIOBlocking.h"


//Testing Includes:
#include "Application.h"
#include "ModuleEditor3D.h"
#include "GameObject.h"

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

ModuleAudioManager::ModuleAudioManager(bool start_enabled) :Module(start_enabled)
{
	activeListener = nullptr;
}

ModuleAudioManager::~ModuleAudioManager()
{
	activeListener = nullptr;
}

bool ModuleAudioManager::Init()
{
	//Initialize Memory Manager
	AkMemSettings memSettings;

	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{

		assert(!"Could not create the memory manager.");

		return false;

	}

	//Initialize Stream Manager

	AkStreamMgrSettings stmSettings;

	AK::StreamMgr::GetDefaultSettings(stmSettings);



	// Customize the Stream Manager settings here.

	if (!AK::StreamMgr::Create(stmSettings))
	{

		assert(!"Could not create the Streaming Manager");

		return false;

	}


	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. 

	AkDeviceSettings deviceSettings;

	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);


	// Customize the streaming device settings here.
	// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
	// in the Stream Manager, and registers itself as the File Location Resolver.

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;

	}

	//Initialize Sound Engine
	// Create the Sound Engine
	// Using default initialization parameters

	AkInitSettings initSettings;

	AkPlatformInitSettings platformInitSettings;

	AK::SoundEngine::GetDefaultInitSettings(initSettings);

	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);


	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	// Initialize the music engine
	// Using default initialization parameters

	AkMusicSettings musicInit;

	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}

	//Initialize Spatial Audio
	// Using default initialization parameters

	AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

	if (AK::SpatialAudio::Init(settings) != AK_Success) //TODO settings was passed as a reference but triggered an error
	{
		assert(!"Could not initialize the Spatial Audio.");
		return false;
	}

	//Initialize Communications
#ifndef AK_OPTIMIZED
// Initialize communications (not in release build!)
	AkCommSettings commSettings;

	AK::Comm::GetDefaultInitSettings(commSettings);

	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}

#endif // AK_OPTIMIZED



	//=======================================================================================================================
	//LOAD SOUNDBANKS
	// Setup banks path

	g_lowLevelIO.SetBasePath(AKTEXT("Assets/wwise/PhoebusWwise/GeneratedSoundBanks/Windows/"));
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));
	// Load banks synchronously (from file name).

	AkBankID bankID; // Not used. These banks can be unloaded with their file name.

	AKRESULT eResult = AK::SoundEngine::LoadBank("Init.bnk", bankID);

	//assert(eResult != AK_Success);

	eResult = AK::SoundEngine::LoadBank("Main.bnk", bankID);

	//assert(eResult != AK_Success);





	return true;
}

bool ModuleAudioManager::GameInit()
{
	UpdateListener();
	AkGameObjectID id = App->editor3d->root->ID;
	if (activeListener != nullptr)
	{
		id = activeListener->ID;
	}
	AK::SoundEngine::PostEvent("Laser_Player", id); //plays Laser sound (from the listener for the moment)
	return true;
}

bool ModuleAudioManager::Start()
{
	//AkGameObjectID id = App->editor3d->root->ID;

	//// Register the main listener. //TODO this will change in the near future, also for now the main listener is also the player
	//AK::SoundEngine::RegisterGameObj(id);

	//UpdateListener();

	return true;
}

update_status ModuleAudioManager::Update(float dt)
{
	//UpdateListener();
	// Register the main listener. //TODO this will change in the near future, also for now the main listener is also the player
	//AK::SoundEngine::RegisterGameObj(id);

	return UPDATE_CONTINUE;
}

update_status ModuleAudioManager::GameUpdate(float dt)
{

	// Process bank requests, events, positions, RTPC, etc.
	AK::SoundEngine::RenderAudio();

	return UPDATE_CONTINUE;
}

void ModuleAudioManager::UpdateListener()
{
	AkGameObjectID id = App->editor3d->root->ID;
	//TODO trashy way to do it, we do not want to set the listener every frame
	if (activeListener != nullptr)
	{
		id = activeListener->ID;
	}

	// Set one listener as the default.
	AK::SoundEngine::SetDefaultListeners(&id, 1);
}

bool ModuleAudioManager::CleanUp()
{

	AK::SoundEngine::UnregisterAllGameObj();



	// Terminate Communication Services
#ifndef AK_OPTIMIZED
	AK::Comm::Term();

#endif // AK_OPTIMIZED

	// Terminate Spatial Audio
	//AK::SpatialAudio::Term();//TODO this method doesn't exist ???

	 // Terminate the music engine
	AK::MusicEngine::Term();

	// Terminate the sound engine
	AK::SoundEngine::Term();


	// Terminate the streaming device and streaming manager
   //CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
   // that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();


	// Terminate the Memory Manager
	AK::MemoryMgr::Term();
	enabled = false;
	return true;
}

void ModuleAudioManager::RegisterNewAudioObj(unsigned int componentID)
{
	if (enabled)
		AK::SoundEngine::RegisterGameObj(componentID);
}

void ModuleAudioManager::UnRegisterAudioObj(unsigned int componentID)
{
	if (enabled)
		AK::SoundEngine::UnregisterGameObj(componentID);
}

void ModuleAudioManager::UnRegisterAllAudioObjs() const
{
	if (activeListener != nullptr)
	{
		activeListener->SetAsListener(false);
	}
	AK::SoundEngine::UnregisterAllGameObj();
}
