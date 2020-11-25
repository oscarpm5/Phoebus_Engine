#include "Resources.h"
//#include "Config.h"

Resource::Resource(unsigned int UID, ResourceType type):uid(UID),type(type),referenceCount(0)
{
}

Resource::~Resource()
{
}

ResourceType Resource::GetType() const
{
	return ResourceType();
}

unsigned int Resource::GetUID() const
{
	return uid;
}

const char* Resource::GetAssetFile() const
{
	return assetsFile.c_str();
}

const char* Resource::GetLibraryFile() const
{
	return libraryFile.c_str();
}

void Resource::SetAssetPath(std::string newPath)
{
	assetsFile = newPath;
}

void Resource::SetLibPath(std::string newPath)
{
	libraryFile = newPath;
}

//void Resource::Save(Config& config) const
//{
//}
//
//void Resource::Load(const Config& config)
//{
//}
