#include "Globals.h"
#include "Application.h"
#include "ModuleFileSystem.h"

//#include <fstream>
//#include <filesystem>

#include <string.h> //used in checking file type

#include "Assimp/include/cfileio.h"
#include "Assimp/include/types.h"

#include "Importer.h"

#include "PhysFS/include/physfs.h"
#pragma comment( lib, "PhysFS/libx86/physfs.lib" )


//linea 102 module input Importer:: load fbx -> llamaar el filesysatem y pasarle el path (dropfiledyr) para probarlo rapido


ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
	// Initialize the PhysicsFS library
	// This must be called before any other PhysicsFS function
	// This should be called prior to any attempts to change your process's current working directory
	PHYSFS_init(nullptr);

	// We only need this when compiling in debug. In Release we don't need it.
	AddNewPath(".");
	PHYSFS_setWriteDir("."); //necessary to save in own file format (working directory aka Game)
	AddNewPath("Assets");

	CreateAllLibDirectories();
}

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


	//PHYSFS_mount("Assets", nullptr, 1); //TODO why is that here?
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

void ModuleFileSystem::TransformToLowerCase(std::string& lowerCase)
{
	std::string newString;
	for (int i = 0; i < lowerCase.size(); i++)
	{
		newString += tolower(lowerCase[i]);
	}
	lowerCase = newString;
}

void ModuleFileSystem::SeparatePath(std::string path, std::string* newPath, std::string* file)
{
	size_t filePos = path.find_last_of("\\/");

	if (filePos < path.size())
	{
		if (newPath)* newPath = path.substr(0, filePos + 1);
		if (file)* file = path.substr(filePos + 1);
	}
	else if (path.size() > 0)
	{
		if (file)* file = path;
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

	//TransformToRelPath(newPath);
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
	case FileFormats::JPG:
	case FileFormats::JPEG:
	case FileFormats::DDS:
		Importer::LoadNewImageFromBuffer(buffer, size, newPath);
		break;

	case FileFormats::PHO:
		Importer::LoadMeshFromPho(buffer, size, newPath);
		//Importer::LoadMaterialFromPho(buffer, size, newPath);
		//Importer::LoadTransformFromPho(buffer, size, newPath);
		//Importer::LoadCameraFromPho(buffer, size, newPath);
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
	std::string newPath = path;
	std::string strFormat;
	unsigned int index = newPath.find_last_of(".");

	if (index < newPath.size())
	{
		strFormat = newPath.substr(index); //look for the last instance of a point. Format should be next
		TransformToLowerCase(strFormat);
	}

	if (strFormat.size() == 0)
	{
		// somehow no extension 
		format = FileFormats::UNDEFINED;
	}
	else
	{
		format = FileFormats::UNDEFINED; 

		if (!strcmp(strFormat.c_str(), ".fbx"))
		{
			format = FileFormats::FBX;
		}
		else if (!strcmp(strFormat.c_str(), ".obj"))
		{
			format = FileFormats::OBJ;
		}
		else if (!strcmp(strFormat.c_str(), ".png"))
		{
			format = FileFormats::PNG;
		}
		else if (!strcmp(strFormat.c_str(), ".dds"))
		{
			format = FileFormats::DDS;
		}
		else if (!strcmp(strFormat.c_str(), ".jpg"))
		{
			format = FileFormats::JPG;
		}
		else if (!strcmp(strFormat.c_str(), ".jpeg"))
		{
			format = FileFormats::JPEG;
		}
		else if (!strcmp(strFormat.c_str(), ".json"))
		{
			format = FileFormats::JSON;
		}
		else if (!strcmp(strFormat.c_str(), ".pho"))
		{
			format = FileFormats::PHO;
		}
	}
	return format;
}


// Save a whole buffer to disk
unsigned int ModuleFileSystem::SavePHO(const char* file, const void* buffer, unsigned int size)
{
	unsigned int ret = 0;

	bool overwrite = PHYSFS_exists(file) != 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);

	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("[error] File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			if (overwrite == true)
			{
				LOG("File [%s] overwritten with %u bytes", file, size);
			}
			else
			{
				LOG("New file created [%s] of %u bytes", file, size);
			}
			ret = written;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("[error] File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("[error] File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}

bool ModuleFileSystem::IsFileDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;//TODO, deprecated funct? use PHYSFS_stat()?
}

bool ModuleFileSystem::CreateNewDirectory(const char* directory)
{
	bool ret = false;

	if (!IsFileDirectory(directory))
	{
		PHYSFS_mkdir(directory);
		ret = true;
	}

	return ret;
}

void ModuleFileSystem::CreateAllLibDirectories()
{
	CreateNewDirectory(LIB_PATH);
	CreateNewDirectory(MESH_PATH);
	CreateNewDirectory(MATERIAL_PATH);
	CreateNewDirectory(TEXTURE_PATH);
	CreateNewDirectory(SCENE_PATH);

}

bool ModuleFileSystem::AddNewPath(const char* newPath)
{
	bool ret = false;

	if (PHYSFS_mount(newPath, nullptr, 1) == 0)
	{
		LOG("[error] Path could not be added to the file system: %s", PHYSFS_getLastError());
	}
	else
	{
		ret = true;
	}

	return ret;
}
