#include "Resources.h"

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
	return 0;
}
