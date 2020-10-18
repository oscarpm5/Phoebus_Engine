#ifndef __MODULEFILESYSTEModuleH__
#define __MODULEFILESYSTEModuleH__
#include "Module.h"
#include "Globals.h"
#include "Module.h"
#include <vector>

enum class FileFormats
{
	UNDEFINED,

	FBX,
	JSON,
	OBJ
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

	FileFormats CheckFileFormat(const char* path);
	bool LoadFile(const char* path);

	bool LoadFbx(const char* path);
	//bool LoadJson(const char* path);
};

#endif // __MODULEFILESYSTEModuleH__
