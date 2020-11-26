#include "Application.h"
#include "ModuleResourceManager.h"
#include "Globals.h"
#include "Importer.h"
#include "Mesh.h"
#include "Texture.h"
#include "Config.h"

ModuleResourceManager::ModuleResourceManager(bool start_enabled) :Module(start_enabled)
{
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Init()
{
	Importer::InitializeDevIL();
	return true;
}

bool ModuleResourceManager::Start()
{
	//LoadAllAssets();
	checkTimer = 0.0f;
	return true;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	checkTimer += dt;
	if (checkTimer >= 1.0f)
	{
		checkTimer = 0.0f;
		//TODO check all asset files here


		//Testing code

	}
	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{
	int i = 0;//need it to be different than module, THIS IS A PLACEHOLDER
	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::PostUpdate(float dt)
{
	int i = 0;//need it to be different than module, THIS IS A PLACEHOLDER
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	return true;
}

//TODO THIS ISNT WORKING YET
unsigned int ModuleResourceManager::ImportFile(const char* newAssetFile)
{
	unsigned int ret = 0;
	//if it has no meta, import normally (create resource in lib + create . meta in assets)

	ResourceType resType = ResourceTypeFromPath(newAssetFile);
	Resource* res = CreateNewResource(newAssetFile, resType);

	char* buffer;
	uint size = App->fileSystem->Load(newAssetFile, &buffer);
	//Import here->TODO->  method is given a buffer + a resource and saves the imported resource into the given one
	switch (resType)
	{
	case ResourceType::TEXTURE:
		//Import texture
		break;
	case ResourceType::MESH:
		break;
	case ResourceType::SCENE:
		break;
	case ResourceType::MODEL:
		//Import model
		break;
	case ResourceType::UNKNOWN:
		break;
	default:
		break;
	}

	//We then save the resource (TODO) -> create resource in lib + create . meta in assets

	ret = res->GetUID();
	RELEASE_ARRAY(buffer);
	//after we are done using it, we unload the resource TODO

	return ret;
}

Resource* ModuleResourceManager::CreateNewResource(const char* assetsFile, ResourceType type)
{
	Resource* ret = nullptr;
	unsigned int newUID = App->renderer3D->seed.Int();

	//TODO create the different types of resource
	switch (type)
	{
	case ResourceType::TEXTURE:
		ret = (Resource*) new ResourceTexture(newUID);
		break;
	case ResourceType::MESH:
		ret = (Resource*) new ResourceMesh(newUID);
		break;
	case ResourceType::SCENE:
		//TODO
		break;
	case ResourceType::MODEL:
		//ret = (Resource*) new ResourceModel(newUID);
		break;
	}

	//adds the resource to the map
	if (ret != nullptr)
	{
		resources[newUID] = ret;
		ret->SetAssetPath(assetsFile);
		ret->SetLibPath(GenLibraryPath(*ret));
	}

	return ret;
}

void ModuleResourceManager::LoadAllAssets()
{
	//for every file in assets, if not already in lbrary, imports them
	LoadAssetsRecursively("Assets/");

}

void ModuleResourceManager::LoadAssetsRecursively(std::string dir)
{
	std::vector<std::string>files;
	std::vector<std::string>dirs;

	if (!App->fileSystem->GetDirFiles(dir.c_str(), files, dirs))
	{
		LOG("[error] Trying to acces an invalid directory: %s", dir.c_str());
		return; //if directory doesn't exist return, just for safety
	}

	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
	{
		const std::string& str = *it;
		std::string absPath = dir + str;
		std::string extension;
		unsigned int index = str.find_last_of(".");

		if (index < str.size())
		{
			extension = str.substr(index); //look for the last instance of a point. Format should be next
		}

		if (extension != ".meta")
		{
			ManageAssetUpdate(absPath.c_str()); //manages the file
		}
	}

	//for every directory in the current directory, load their assets
	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		const std::string& str = *it;
		LoadAssetsRecursively(str + "/");
	}
}

void ModuleResourceManager::LoadAssetsFromDir(std::string dir)
{
	std::vector<std::string>files;
	std::vector<std::string>dirs;

	if (!App->fileSystem->GetDirFiles(dir.c_str(), files, dirs))
	{
		return; //if directory doesn't exist return
	}

	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
	{
		const std::string& str = *it;
		std::string extension;
		unsigned int index = str.find_last_of(".");

		if (index < str.size())
		{
			extension = str.substr(index); //look for the last instance of a point. Format should be next
		}

		if (extension != ".meta")
		{
			ImportFile(str.c_str());
		}
	}

}

void ModuleResourceManager::ManageAssetUpdate(const char* newAssetFile)
{
	std::string newPath = App->fileSystem->NormalizePath(newAssetFile);
	std::string metaPath = newPath + ".meta";

	LOG("Loading Asset from path: %s", newPath.c_str());

	//First, we check if the asset has a .meta associated with it
	if (App->fileSystem->DoesFileExist(metaPath.c_str()))
	{
		//if it exists we check if the file has changed recently

		//TODO code to check if file has changed 

		//if it has not, do nothing, else re-import the resource
	}
	else
	{
		ImportFile(newPath.c_str());
	}
}

std::string ModuleResourceManager::GenLibraryPath(Resource& res)
{
	ResourceType restType = res.GetType();
	std::string path = "";

	switch (restType)
	{
	case ResourceType::TEXTURE:
		path = TEXTURE_PATH;
		break;
	case ResourceType::MESH:
		path = MESH_PATH;
		break;
	case ResourceType::SCENE:
		path = SCENE_PATH;
		break;
	case ResourceType::MODEL:
		path = MODEL_PATH;
		break;
	case ResourceType::UNKNOWN:
	default:
		LOG("[error] Error when Generating lib path for a resource: the resource has unknown type");
		break;
	}

	if (path != "")
	{
		path += std::to_string(res.GetUID());
		path += ".pho";
	}

	return path;
}

ResourceType ModuleResourceManager::ResourceTypeFromPath(std::string path)
{
	ResourceType ret = ResourceType::UNKNOWN;
	FileFormats thisFormat = App->fileSystem->CheckFileFormat(path.c_str());

	switch (thisFormat)
	{
	case FileFormats::FBX:
	case FileFormats::OBJ:
		ret = ResourceType::MODEL;
		break;
	case FileFormats::JSON:
		break;

	case FileFormats::PNG:
	case FileFormats::JPG:
	case FileFormats::JPEG:
	case FileFormats::DDS:
		ret = ResourceType::TEXTURE;
		break;

	case FileFormats::PHO:
		ret = ResourceType::SCENE;
		break;
	case FileFormats::UNDEFINED:
	default:
		LOG("[error]asset from %s has no recognizable format", path);
		break;
	}


	return ret;
}

void ModuleResourceManager::GenerateMetaFile(Resource* res)
{
	char* bufferToFill;
	const char* assetPath = res->GetAssetFile();
	const char* metaExtension = ".meta";
	
	//fill the buffer with the necessary info
	Config file;
	file.SetNumber("ID", res->GetUID());
	unsigned long lastModDate = App->fileSystem->GetLastModTimeFromPath(assetPath);
	file.SetNumber("ModDate", lastModDate);
	//TODO: import configs go here, if we're adding that into the future
	unsigned int size = file.Serialize(&bufferToFill);
	
	//TODO: less ghetto way to do this? -Adri
	char* AuxPath = (char*)malloc(1 + strlen(assetPath) + strlen(metaExtension));
	strcpy(AuxPath, assetPath);
	strcat(AuxPath, metaExtension);

	//make physfs save the file
	App->fileSystem->SavePHO(AuxPath, bufferToFill, size);

	//clear buffer to avoid leaks (maybe it's automatic due to scope? Better safe than sorry)
	delete bufferToFill;
	bufferToFill = nullptr;
}
