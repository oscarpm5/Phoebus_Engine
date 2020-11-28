#pragma once

#include "Module.h"
#include "Globals.h"

#include <string>
#include <vector>
#include <map>

class Resource;
enum class ResourceType;

struct ActiveResources
{
	std::vector<Resource*> meshes;
	std::vector<Resource*> textures;
	std::vector<Resource*> scenes;
	std::vector<Resource*> models;
};



class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool start_enabled = true);
	~ModuleResourceManager();
	bool Init();
	bool Start();
	update_status PreUpdate(float dt)override;
	//update_status Update(float dt)override;
	//update_status PostUpdate(float dt)override;
	bool CleanUp();

	Resource* ImportNewFile(const char* newAssetFile);
	Resource* ReImportExistingFile(const char* newAssetFile,unsigned int uid);
	//given a uid the system checks for any lib file with that uid
	void FindFileRecursively(std::string uid, std::string currDir, std::string& foundFile);//Deprecated??

	void GenerateMetaFile(Resource* res);

	Resource* RequestNewResource(unsigned int uid);
	Resource* RequestExistingResource(unsigned int uid);//This doesn't add to the resource count, we use it during the execution
	void StopUsingResource(unsigned int uid);
	//getAll=true will get you all resources imported, the ones active and the ones that are not active
	ActiveResources GetActiveResources(bool getAll = false);
	//if creatinga  new resource do not use the last parameter, it is only used when loading resources
	Resource* CreateNewResource(const char* assetsFile, ResourceType type,unsigned int existingID=0);//we need this for Import 
	//Manages importing of an asset file (whether it has to import, create lib file, update from meta, etc)
	Resource* ManageAssetUpdate(const char* newAssetFile);

	bool DeleteItemFromResourcesMap(unsigned int UID);

	Resource* FindResInMemory(unsigned int UID);

	void LoadResourceIntoMem(Resource* res);


	void GetAllResourcesOfType(ResourceType type, std::vector<Resource*>& ret);

private:
	void LoadAllAssets();

	
	void LoadAssetsRecursively(std::string dir);

	void LoadAssetsFromDir(std::string dir);




	std::string GenLibPath(Resource& res);
	//Returns resource type from assets path
	ResourceType ResourceTypeFromPath(std::string path);

	Resource* TryToLoadResource(unsigned int uid);//TODO deprecated?

	bool ReleaseSingleResource(unsigned int uid);

	void SaveResource(Resource& r);

private:
	float checkTimer;
public:
	bool haveToReload;

	std::map<unsigned int, Resource*> resources;
};

