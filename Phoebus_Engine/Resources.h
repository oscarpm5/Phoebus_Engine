#ifndef __RESOURCES_H__
#define __RESOURCES_H__

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

protected:
	unsigned int uid;
	ResourceType type;
	unsigned int referenceCount;



};


#endif // !__RESOURCES_H__
