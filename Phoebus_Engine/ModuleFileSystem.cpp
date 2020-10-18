#include "Globals.h"
#include "ModuleFileSystem.h"
#include "PhysFS/include/physfs.h"
#include <fstream>
#include <filesystem>
#include "Application.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"
#include "Importer.h"
#include <string.h> //used in checking file type

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )


//linea 102 module input Importer:: load fbx -> llamaar el filesysatem y pasarle el path (dropfiledyr) para probarlo rapido

ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Init()
{
	bool ret = true;
	return ret;
}

bool ModuleFileSystem::Start()
{
	bool ret = true;
	return ret;
}

update_status ModuleFileSystem::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleFileSystem::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleFileSystem::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp()
{
	bool ret = true;
	return ret;
}

FileFormats ModuleFileSystem::CheckFileFormat(const char* path)
{
	FileFormats format;

	const char * ext = strrchr(path, '.'); //look for the last instance of a point. Format should be next


	//TODO: convert ext into a lowercase const char *


	if (!ext) 
	{
		// somehow no extension 
		format = FileFormats::UNDEFINED;
	}
	else 
	{
		format = FileFormats::UNDEFINED; //delete the uppercase scenarios once the prior TODO is done

		if (!strcmp(ext,".fbx") || !strcmp(ext, ".FBX"))
		{
			format = FileFormats::FBX;
		}
		if (!strcmp(ext, ".obj") || !strcmp(ext, ".OBJ"))
		{
			format = FileFormats::OBJ;
		}
		if (!strcmp(ext, ".json") || !strcmp(ext, ".JSON"))
		{
			format = FileFormats::JSON;
		}
	}
	return format;
}

bool ModuleFileSystem::LoadFile(const char* path)
{
	bool ret = false;

	FileFormats format = CheckFileFormat(path);

	switch (format)
	{
	case FileFormats::UNDEFINED:
		break;

	case FileFormats::FBX:
		ret = LoadFbx(path);
		break;

	case FileFormats::OBJ:
		ret = LoadFbx(path); //yeah, this works. Deal with it.
		break;

	case FileFormats::JSON:
		//ret = LoadJson(path);
		break;

	default:
		break;
	}

	return ret;
}

bool ModuleFileSystem::LoadFbx(const char* path)
{
	bool ret = Importer::LoadFBX(path);
	return ret;
}
