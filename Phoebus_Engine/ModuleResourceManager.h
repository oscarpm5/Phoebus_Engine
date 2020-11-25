#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "Module.h"
#include "Globals.h"
#include "Resources.h" //NOTE! Resources with capital R and final s is the good one, change nomenclature as to not get confused with resource & Resource
#include <map>

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool start_enabled = true);
	~ModuleResourceManager();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	//returns a resource ID given an assets file
	unsigned int Find(const char* file_in_assets) const;
	//Imports a file in assets into a new resource
	unsigned int ImportFile(const char* newAssetFile);	//TODO Generate IDs here
private:
	//creates a new resource given a file path from assets and a type (Factory method)
	Resource* CreateNewResource(const char* assetsFile, ResourceType type);

	void LoadAllAssets();
	void LoadAssetsRecursively(std::string dir);
	void LoadAssetsFromDir(std::string dir);

	//checks if the asset has been already imported and its up to date, if it is not, acts accordingly (generates lib files + meta)
	void ManageAssetUpdate(const char* newAssetFile);

	//Generates a lib path from a given resource
	std::string GenLibraryPath(Resource& res);

	ResourceType ResourceTypeFromPath(std::string path);

private:
	//stores all resources that are loaded in memory
	std::map<unsigned int, Resource*> resources; 

	float checkTimer;
};






#endif // !__RESOURCE_MANAGER_H__

