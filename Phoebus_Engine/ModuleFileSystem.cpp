#include "Globals.h"
#include "ModuleFileSystem.h"
#include "PhysFS/include/physfs.h"
#include <fstream>
#include <filesystem>
#include "Application.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"
#include "Importer.h"
#include "PhysFS/include/physfs.h"
#include <string.h> //used in checking file type

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )


//linea 102 module input Importer:: load fbx -> llamaar el filesysatem y pasarle el path (dropfiledyr) para probarlo rapido



ModuleFileSystem::~ModuleFileSystem()
{
	PHYSFS_deinit();
	LOG("De-init PHYSFS");
}

bool ModuleFileSystem::Init()
{
	bool ret = true;
	return ret;
}

bool ModuleFileSystem::Start()
{
	bool ret = true;
	// Determine if the PhysicsFS library is initialized, we can check it for avoid errors.
	if (PHYSFS_isInit()) {
		LOG("Asset Manager is succefully loaded");
	}
	else
		LOG("[error]Failed loading Asset Manager");


	// Add an archive or directory to the search path.
	// If this is a duplicate, the entry is not added again, even though the function succeeds.
	// When you mount an archive, it is added to a virtual file system...
	// all files in all of the archives are interpolated into a single hierachical file tree.
	PHYSFS_mount("Assets", nullptr, 1);
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

uint ModuleFileSystem::Load(const char* path, char** buffer) const
{
	uint ret = 0;


	// The reading offset is set to the first byte of the file.
	// Returns a filehandle on success that we will need for the PHYSFS_fileLength
	PHYSFS_file* file = PHYSFS_openRead(path);
	if (file != nullptr)
	{
		// Check for end-of-file state on a PhysicsFS filehandle.
		if (!PHYSFS_eof(file))
		{
			// Get total length of a file in bytes
			uint lenght = PHYSFS_fileLength(file);
			*buffer = new char[lenght];
			LOG("Loading buffer from path: %s", path);
			// Read data from a PhysicsFS firehandle. Returns a number of bytes read.
			uint bytes = PHYSFS_readBytes(file, *buffer, lenght);

			if (bytes != lenght)
			{
				LOG("[error]%s", path, "ERROR: %s", PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
				ret = bytes;
		}
		else
			LOG("[error]%s", path, "ERROR: %s", PHYSFS_getLastError());
	}
	else
	{
		LOG("[error]: File '%s' doesn't exist", path);
	}
	// Close a PhysicsFS firehandle
	PHYSFS_close(file);

	return ret;
}

SDL_RWops* ModuleFileSystem::Load(const char* path)
{
	char* buffer;
	std::string newPath = NormalizePath(path);
	//std::string newPath=path;

	TransformToRelPath(newPath);

	uint bytes = Load(newPath.c_str(), &buffer);


	// Read-only memory buffer for use with RWops, retruns a pointer to a new SDL_RWops structure
	SDL_RWops* ret = SDL_RWFromConstMem(buffer, bytes);

	return ret;
}

//TODO provisional solution until we copy files into the directory
void ModuleFileSystem::TransformToRelPath(std::string& path)
{
	unsigned int splitPos = path.find("Assets"); //file must be inside Assets directory
	path = path.substr(splitPos, path.length());
}

void ModuleFileSystem::SeparatePath(std::string path, std::string* newPath, std::string* file)
{
	size_t filePos = path.find_last_of("\\/");

	if (filePos < path.size())
	{
		if(newPath)	*newPath = path.substr(0, filePos + 1);
		if(file)*file = path.substr(filePos + 1);
	}
	else if (path.size() > 0)
	{
		if(file)*file = path;
	}
}

//normalizes '//' paths
std::string ModuleFileSystem::NormalizePath(const char* path)
{
	std::string newPath(path);
	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	return newPath;
}

void ModuleFileSystem::LoadAsset(char* path)
{
	char* buffer;
	std::string newPath = NormalizePath(path);
	//std::string newPath=path;

	TransformToRelPath(newPath);
	//LOG("Loading Asset from path: %s", newPath.c_str());
	uint size = App->fileSystem->Load((char*)newPath.c_str(), &buffer);

	FileFormats thisFormat = CheckFileFormat(newPath.c_str());

	switch (thisFormat)
	{
	case FileFormats::FBX:
		Importer::LoadFBXfromBuffer(buffer, size, newPath.c_str());
		break;

	case FileFormats::OBJ:
		Importer::LoadFBXfromBuffer(buffer, size, newPath.c_str()); //this workas, deal with it
		break;

	case FileFormats::JSON:
		//to be
		break;

	case FileFormats::PNG:
		Importer::LoadNewImageFromBuffer(buffer, size);
		break;

	case FileFormats::UNDEFINED:
		LOG("[error]asset from %s has no recognizable format", path);
		break;
	default:
		break;
	}
	RELEASE_ARRAY(buffer);
}

FileFormats ModuleFileSystem::CheckFileFormat(const char* path)
{
	FileFormats format;

	const char* ext = strrchr(path, '.'); //look for the last instance of a point. Format should be next


	//TODO: convert ext into a lowercase const char *


	if (!ext)
	{
		// somehow no extension 
		format = FileFormats::UNDEFINED;
	}
	else
	{
		format = FileFormats::UNDEFINED; //delete the uppercase scenarios once the prior TODO is done

		if (!strcmp(ext, ".fbx") || !strcmp(ext, ".FBX"))
		{
			format = FileFormats::FBX;
		}
		if (!strcmp(ext, ".obj") || !strcmp(ext, ".OBJ"))
		{
			format = FileFormats::OBJ;
		}
		if (!strcmp(ext, ".png") || !strcmp(ext, ".PNG"))
		{
			format = FileFormats::PNG;
		}
		if (!strcmp(ext, ".json") || !strcmp(ext, ".JSON"))
		{
			format = FileFormats::JSON;
		}
	}
	return format;
}

ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
	// Initialize the PhysicsFS library
	// This must be called before any other PhysicsFS function
	// This should be called prior to any attempts to change your process's current working directory
	PHYSFS_init(nullptr);

	// We only need this when compiling in debug. In Release we don't need it.
	PHYSFS_mount(".", nullptr, 1);

}

