#include "Application.h"
#include "ModuleResourceManager.h"
#include "Globals.h"
#include "Importer.h"

ModuleResourceManager::ModuleResourceManager(bool start_enabled) :Module(start_enabled)
{
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Init()
{
	return true;
}

bool ModuleResourceManager::Start()
{
	return true;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	int i = 0;//need it to be different than module, THIS IS A PLACEHOLDER
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
	char* buffer;
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
		//if it has no meta, import normally (create resource in lib + create . meta in assets)


		uint size = App->fileSystem->Load((char*)newPath.c_str(), &buffer);

		FileFormats thisFormat = App->fileSystem->CheckFileFormat(newPath.c_str());

		switch (thisFormat)
		{
		case FileFormats::FBX:
		case FileFormats::OBJ:
			//Importer::LoadFBXfromBuffer(buffer, size, newPath.c_str()); //this workas, deal with it //TODO special case, serialize as model
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
			LOG("[error]asset from %s has no recognizable format", newPath);
			break;
		default:
			break;
		}
		RELEASE_ARRAY(buffer);
	}
	return 0;
}

Resource* ModuleResourceManager::CreateNewResource(const char* assetsFile)
{
	//First we get the type of the new resource from the assets extension
	std::string assetPath = assetsFile;
	FileFormats thisFormat = App->fileSystem->CheckFileFormat(assetPath.c_str());
	ResourceType resourceType = ResourceType::UNKNOWN;

	switch (thisFormat)
	{
	case FileFormats::FBX:
	case FileFormats::OBJ:
		resourceType = ResourceType::MODEL;
		break;

	case FileFormats::JSON:
		//to be
		break;

	case FileFormats::PNG:
	case FileFormats::JPG:
	case FileFormats::JPEG:
	case FileFormats::DDS:
		resourceType = ResourceType::TEXTURE;
		break;

	case FileFormats::UNDEFINED:
	default:
		LOG("[error]asset from %s has no recognizable format", assetPath);
		break;
	}

	//Creates the new resource from type
	Resource* ret = nullptr;
	unsigned int newUID = App->renderer3D->seed.Int();

	//TODO create the different types of resource
	switch (resourceType)
	{
	case ResourceType::TEXTURE:
		//ret = (Resource*) new ResourceTexture(newUID);
		break;
	case ResourceType::MESH:
		//ret = (Resource*) new ResourceMesh(newUID);
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
		//still TODO
		resources[newUID] = ret;
	}

	return ret;
}

void ModuleResourceManager::LoadAllAssets()
{
	//for every file in assets, if not already in lbrary, imports them
	LoadAssetsRecursively("Assets");

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
		std::string extension;
		unsigned int index = str.find_last_of(".");

		if (index < str.size())
		{
			extension = str.substr(index); //look for the last instance of a point. Format should be next
		}

		if (extension != ".meta")
		{
			ImportFile(str.c_str()); //import the files
		}
	}

	//for every directory in the current directory, load their assets
	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		const std::string& str = *it;
		LoadAssetsRecursively(str);
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
