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
	return type;
}

unsigned int Resource::GetUID() const
{
	return uid;
}

void Resource::SetUID(int newUID)
{
	this->uid = newUID;
}

std::string Resource::GetAssetFile() const
{
	return assetsFile;
}

std::string Resource::GetLibraryFile() const
{
	return libraryFile;
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
