#ifndef __MODULEFILESYSTEModuleH__
#define __MODULEFILESYSTEModuleH__
#include "Module.h"
#include "Globals.h"
#include "Module.h"
#include "SDL/include/SDL_rwops.h"
#include <vector>

enum class FileFormats
{
	UNDEFINED,

	FBX,
	JSON,
	OBJ,
	PNG
};


class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	// Return the bytes of a PhysFS filehandle
	uint Load(const char* path, char** buffer) const;

	// Allows you to use pointers to memory instead of files or things such as images or samples
	SDL_RWops* Load(const char* path) const;


	void LoadAsset(char * path);

	FileFormats CheckFileFormat(const char* path);
};

#endif // __MODULEFILESYSTEModuleH__
