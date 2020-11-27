#include "M_ResourceManager.h"
#include "Globals.h"
#include "Application.h"
#include "Resources.h"
#include "Config.h"
#include "Texture.h"
#include "Mesh.h"
#include <string>
#include "Importer.h"

M_ResourceManager::M_ResourceManager(bool start_enabled) :Module(start_enabled), checkTimer(0.0f)
{
}

M_ResourceManager::~M_ResourceManager()
{
}

bool M_ResourceManager::Init()
{
	return true;
}

bool M_ResourceManager::Start()
{
	//TODO TESTING CODE
	/*char* buffer;
	unsigned int size = App->fileSystem->Load("Assets/bakerHouse/Baker_house.png",&buffer);
	Resource* r = CreateNewResource("Assets/bakerHouse/Baker_house.png", ResourceType::TEXTURE);

	Importer::Texture::ImportImage(buffer, size, *r);*/

	/*unsigned int id= ImportNewFile("Assets/bakerHouse/Baker_house.png");
	std::string path;
	FindFileRecursively(std::to_string(id), LIB_PATH, path);
	ResourceTexture res(id);
	Importer::Texture::LoadNewImage(path.c_str(),res);*/


	char* buffer;
	unsigned int size = App->fileSystem->Load("Assets/bakerHouse/BakerHouse.fbx", &buffer);
	ManageAssetUpdate("Assets/bakerHouse/BakerHouse.fbx");
	//Importer::Model::ImportModel(buffer, size, "Assets/bakerHouse/BakerHouse.fbx");

	return true;
}

update_status M_ResourceManager::PreUpdate(float dt)
{
	checkTimer += dt;
	if (App->GetGameState() == GameStateEnum::STOPPED && checkTimer >= 1.0f)//we only check assets when the engine is not in runtime
	{
		checkTimer = 0.0f;
		//TODO check all asset files here


		//Testing code
	/*	CreateNewResource("emptyMesh", ResourceType::MESH);
		CreateNewResource("emptyTexture", ResourceType::TEXTURE);

		ActiveResources act= GetActiveResources();*/


	}
	return update_status::UPDATE_CONTINUE;
}

//update_status M_ResourceManager::Update(float dt)
//{
//	return update_status::UPDATE_CONTINUE;
//}
//
//update_status M_ResourceManager::PostUpdate(float dt)
//{
//	return update_status::UPDATE_CONTINUE;
//}

bool M_ResourceManager::CleanUp()
{
	return true;
}

unsigned int M_ResourceManager::ImportNewFile(const char* newAssetFile)
{
	unsigned int ret = 0;
	//if it has no meta, import normally (create resource in lib + create . meta in assets)

	ResourceType resType = ResourceTypeFromPath(newAssetFile);
	if (resType != ResourceType::UNKNOWN)
	{

		Resource* res = CreateNewResource(newAssetFile, resType);

		char* buffer;
		uint size = App->fileSystem->Load(newAssetFile, &buffer);
		//Import here->TODO->  method is given a buffer + a resource and saves the imported resource into the given one
		switch (resType)
		{
		case ResourceType::TEXTURE:
			Importer::Texture::ImportImage(buffer, size, *res);
			break;
		case ResourceType::MESH:
			//how tf did you get here
			break;
		case ResourceType::SCENE:
			break;
		case ResourceType::MODEL:
			Importer::Model::ImportModel(buffer, size, newAssetFile, res);
			break;
		case ResourceType::UNKNOWN:
		default:
			break;
		}

		//We then save the resource (TODO) -> create resource in lib + create . meta in assets
		SaveResource(*res);

		GenerateMetaFile(res);

		res->referenceCount = 0;
		if (res->IsLoadedInMemory())
		{
			res->UnloadFromMemory();
		}

		ret = res->GetUID();
		RELEASE_ARRAY(buffer); //TODO ask adri if its correct, because for example texture uses double buffer to save itself, is this a mem leak?
	}


	//after we are done using it, we unload the resource TODO

	return ret;
}

void M_ResourceManager::ReImportExistingFile(const char* newAssetFile, unsigned int uid)
{

	//if it has no meta, import normally (create resource in lib + create . meta in assets)

	ResourceType resType = ResourceTypeFromPath(newAssetFile);
	if (resType != ResourceType::UNKNOWN)
	{

		Resource* res = CreateNewResource(newAssetFile, resType, uid);

		char* buffer;
		uint size = App->fileSystem->Load(newAssetFile, &buffer);
		//Import here->TODO->  method is given a buffer + a resource and saves the imported resource into the given one
		switch (resType)
		{
		case ResourceType::TEXTURE:
			Importer::Texture::ImportImage(buffer, size, *res);
			break;
		case ResourceType::MESH:
			//how tf did you get here
			break;
		case ResourceType::SCENE:
			break;
		case ResourceType::MODEL:
			Importer::Model::ImportModel(buffer, size, newAssetFile,res);
			break;
		case ResourceType::UNKNOWN:
		default:
			break;
		}

		//We then save the resource (TODO) -> create resource in lib + create . meta in assets
		SaveResource(*res);

		GenerateMetaFile(res);

		res->referenceCount = 0;
		if (res->IsLoadedInMemory())
		{
			res->UnloadFromMemory();
		}

		RELEASE_ARRAY(buffer); //TODO ask adri if its correct, because for example texture uses double buffer to save itself, is this a mem leak?
	}


	//after we are done using it, we unload the resource TODO


}

void M_ResourceManager::FindFileRecursively(std::string uid, std::string currDir, std::string& foundFile)
{
	std::vector<std::string>files;
	std::vector<std::string>dirs;

	if (!App->fileSystem->GetDirFiles(currDir.c_str(), files, dirs))
	{
		LOG("[error] Trying to acces an invalid directory: %s", currDir.c_str());
		return; //if directory doesn't exist return, just for safety
	}

	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
	{
		const std::string& str = *it;
		std::string absPath = currDir + str;

		std::string extension;

		App->fileSystem->SeparateExtension(str, &extension);

		if (str == (uid + extension))
		{
			foundFile = absPath;
			return;
		}
	}

	//for every directory in the current directory, search
	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		const std::string& str = *it;
		FindFileRecursively(uid, currDir + str + "/", foundFile);
		if (foundFile != "")
		{
			return;
		}
	}
}

void M_ResourceManager::GenerateMetaFile(Resource* res)
{
	char* bufferToFill;
	std::string assetPathStr = res->GetAssetFile();
	const char* assetPath = assetPathStr.c_str();

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

Resource* M_ResourceManager::RequestNewResource(unsigned int uid)
{
	//Find if the resource is already loaded or imported
	std::map<unsigned int, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
	{
		if (!it->second->IsLoadedInMemory())//if it is not loaded
		{
			//Find the library file and load the custom file format
			LoadResourceIntoMem(it->second);

		}
		it->second->referenceCount++;
		return it->second;
	}
	return nullptr;//the file doesn't exist in lib :c
}

void M_ResourceManager::StopUsingResource(unsigned int uid)
{
	//Find if the resource is already loaded or imported
	std::map<unsigned int, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end() && it->second->IsLoadedInMemory())
	{
		it->second->referenceCount--;
		if (it->second <= 0)
		{
			it->second = 0;
			it->second->UnloadFromMemory();
		}

	}
}

ActiveResources M_ResourceManager::GetActiveResources()
{
	ActiveResources res;

	std::map<unsigned int, Resource*>::iterator it = resources.begin();

	for (it; it != resources.end(); it++)
	{
		if (it->second->IsLoadedInMemory())
		{

			ResourceType type = it->second->GetType();
			switch (type)
			{
			case ResourceType::TEXTURE:
				res.textures.push_back(it->second);
				break;
			case ResourceType::MESH:
				res.meshes.push_back(it->second);
				break;
			case ResourceType::SCENE:
				res.scenes.push_back(it->second);
				break;
			case ResourceType::MODEL:
				res.models.push_back(it->second);
				break;
			case ResourceType::UNKNOWN:
			default:
				LOG("Unknown active resource with id: %i", it->second->GetUID());
				break;
			}
		}
	}


	return res;
}

void M_ResourceManager::LoadAllAssets()
{
	LoadAssetsRecursively("Assets/");
}

void M_ResourceManager::LoadAssetsRecursively(std::string dir)
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
		//std::string extension;
		//unsigned int index = str.find_last_of(".");

		//if (index < str.size())
		//{
		//	extension = str.substr(index); //look for the last instance of a point. Format should be next
		//}

		//if (extension != ".meta")
		//{
		//	ManageAssetUpdate(absPath.c_str()); //manages the file
		//}
		ManageAssetUpdate(absPath.c_str());//manages the file
	}

	//for every directory in the current directory, load their assets
	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		const std::string& str = *it;
		LoadAssetsRecursively(str + "/");
	}
}

void M_ResourceManager::LoadAssetsFromDir(std::string dir)
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
			ImportNewFile(str.c_str());
		}
	}
}

void M_ResourceManager::LoadResourceIntoMem(Resource* res)
{
	ResourceType type = res->GetType();

	char* buffer;
	unsigned int size = App->fileSystem->Load(res->GetLibraryFile().c_str(), &buffer);


	switch (type)
	{
	case ResourceType::TEXTURE:
		Importer::Texture::LoadNewImage(res->GetLibraryFile().c_str(), *res);
		break;
	case ResourceType::MESH:
		//Importer::Mesh::LoadMesh(buffer,size,) //TODO meshes should load general resource
		break;
	case ResourceType::SCENE:
		break;
	case ResourceType::MODEL:
		break;
	case ResourceType::UNKNOWN:
	default:
		LOG("[error] Trying to load resource with unknown type");
		break;
	}

	RELEASE_ARRAY(buffer);

}

void M_ResourceManager::ManageAssetUpdate(const char* newAssetFile)
{
	std::string newPath = App->fileSystem->NormalizePath(newAssetFile);
	std::string metaPath = newPath + ".meta";

	LOG("Loading Asset from path: %s", newPath.c_str());

	//First, we check if the asset has a .meta associated with it
	if (App->fileSystem->DoesFileExist(metaPath.c_str()))
	{
		//if it exists we check if the file has changed recently

		unsigned long ModNew = App->fileSystem->GetLastModTimeFromPath(newAssetFile);

		char* metaBuffer;
		App->fileSystem->Load(metaPath.c_str(), &metaBuffer);

		Config metaFile(metaBuffer);

		unsigned long ModOld = metaFile.GetNumber("ModDate");
		unsigned int AssID = metaFile.GetNumber("ID");



		if (ModNew != ModOld)//if it has changed re-import, else try load the resource
		{

			ReImportExistingFile(newPath.c_str(), AssID);
			//ImportNewFile(newPath.c_str());
		}
		else
		{
			//if asset exists in lib but is not loaded in the database, load it
			std::string found = "";
			FindFileRecursively(std::to_string(AssID), LIB_PATH, found);
			if (found != "")//found in lib?
			{
				std::map<unsigned int, Resource*>::iterator it = resources.find(AssID);
				if (it == resources.end())//if not loaded
				{
					//Add resource into the map
					//aka load it in memory
					ResourceType resType = ResourceTypeFromPath(newAssetFile);
					if (resType != ResourceType::UNKNOWN)
					{
						Resource* res = CreateNewResource(newAssetFile, resType);
					}
				}
			}
			else
			{
				//else if asset doesn't exist in lib regenerate it
				ReImportExistingFile(newPath.c_str(), AssID);

			}


		}

	}
	else
	{
		ImportNewFile(newPath.c_str());
	}
}

Resource* M_ResourceManager::CreateNewResource(const char* assetsFile, ResourceType type, unsigned int existingID)
{
	Resource* ret = nullptr;
	unsigned int newUID = existingID;
	if (newUID == 0)
		newUID = App->renderer3D->seed.Int();

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
		ret = new Resource(newUID, ResourceType::MODEL); //model is a bundle of resources, we save it a json
		break;
	}

	//adds the resource to the map
	if (ret != nullptr)
	{
		std::map<unsigned int, Resource*>::iterator it = resources.find(ret->GetUID());
		if (it != resources.end())//if already found in mem delete it and regenerate it
		{
			delete it->second;
			it->second = ret;
		}
		else
		{
			resources[newUID] = ret;
		}

		ret->SetAssetPath(assetsFile);
		ret->SetLibPath(GenLibPath(*ret));
	}

	return ret;
}

std::string M_ResourceManager::GenLibPath(Resource& res)
{
	ResourceType restType = res.GetType();
	std::string path = "";
	std::string extension = "";

	switch (restType)
	{
	case ResourceType::TEXTURE:
		path = TEXTURE_PATH;
		extension = ".dds";
		break;
	case ResourceType::MESH:
		path = MESH_PATH;
		extension = ".mesh";
		break;
	case ResourceType::SCENE:
		path = SCENE_PATH;
		extension = ".pho";
		break;
	case ResourceType::MODEL:
		path = MODEL_PATH;
		extension = ".model";
		break;
	case ResourceType::UNKNOWN:
	default:
		LOG("[error] Error when Generating lib path for a resource: the resource has unknown type");
		break;
	}

	if (path != "")
	{
		path += std::to_string(res.GetUID());
		path += extension;
	}

	return path;
}

ResourceType M_ResourceManager::ResourceTypeFromPath(std::string path)
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
	case FileFormats::UNDEFINED:
	default:
		LOG("[error]asset from %s has no recognizable format", path);
		ret = ResourceType::UNKNOWN;
		break;
	}


	return ret;
}

Resource* M_ResourceManager::TryToLoadResource(unsigned int uid)
{
	Resource* res = nullptr;
	//find resource path and load resource into the engine here
	std::string result = "";
	FindFileRecursively(std::to_string(uid), LIB_PATH, result);
	if (result != "")
	{
		//TODO Load resource with path from lib (result) and assign it to "res" aka Importer::Load (not Import!!!)


	}



	return res;
}

bool M_ResourceManager::ReleaseSingleResource(unsigned int uid)
{
	bool ret = false;
	//Find if the resource is loaded
	std::map<unsigned int, Resource*>::iterator it = resources.find(uid);

	if (it != resources.end())
	{

		it->second->UnloadFromMemory();
		//release resource here TODO

		ret = true;
	}	return ret;
}

void M_ResourceManager::SaveResource(Resource& r)
{
	ResourceType type = r.GetType();

	switch (type)
	{
	case ResourceType::TEXTURE:
		Importer::Texture::SaveTexture(r);
		break;
	case ResourceType::SCENE:
		break;
	case ResourceType::MODEL:
		break;
	case ResourceType::UNKNOWN:
		break;
	default:
		break;
	}

	//save pho

}

