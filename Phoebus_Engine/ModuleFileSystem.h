#ifndef __MODULEFILESYSTEModuleH__
#define __MODULEFILESYSTEModuleH__
#include "Module.h"
<<<<<<< HEAD
#include "Globals.h"
#include "Module.h"
=======
//#include "Globals.h"
>>>>>>> Development
#include "SDL/include/SDL_rwops.h"
#include <vector>
#include <string>

enum class FileFormats
{
	UNDEFINED,

	FBX,
	JSON,
	OBJ,
	PNG,
	JPG,
	JPEG,
	DDS
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
<<<<<<< HEAD
	uint Load(const char* path, char** buffer) const;
=======
	unsigned int Load(const char* path, char** buffer) const;
>>>>>>> Development

	// Allows you to use pointers to memory instead of files or things such as images or samples
	SDL_RWops* Load(const char* path);

	void TransformToRelPath(std::string& path);
	void TransformToLowerCase(std::string& lowerCase);

	void SeparatePath(std::string path, std::string* newPath, std::string* file);
	std::string NormalizePath(const char* path);
	void LoadAsset(char * path);

	FileFormats CheckFileFormat(const char* path);
};

#endif // __MODULEFILESYSTEModuleH__
