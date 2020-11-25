#ifndef __RESOURCES_H__
#define __RESOURCES_H__
#include <string>

//#include "Config.h"

//class Config;

enum class ResourceType {
	TEXTURE,
	MESH,
	SCENE,
	MODEL,
	UNKNOWN
};

class Resource
{
public:
	Resource(unsigned int UID,ResourceType type);
	virtual ~Resource();

	ResourceType GetType()const;
	unsigned int GetUID()const;


	const char* GetAssetFile() const;
	const char* GetLibraryFile() const;

	void SetAssetPath(std::string newPath);
	void SetLibPath(std::string newPath);

	//TODO should we save/load here or in the importer?
	//virtual void Save(Config& config) const;
	//virtual void Load(const Config& config);
	//virtual bool LoadInMemory() = 0;

public:
	unsigned int referenceCount;

protected:
	unsigned int uid;
	ResourceType type;

	std::string assetsFile;
	std::string libraryFile;

};


#endif // !__RESOURCES_H__
