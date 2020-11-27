#ifndef __C_MATERIAL__
#define __C_MATERIAL__


#include "Component.h"
#include <string>

class ResourceTexture;
class C_Material:public Component
{
public:
	C_Material(GameObject* owner,unsigned int ID);
	C_Material(GameObject* owner,unsigned int ilImageName, const char * name);
	~C_Material();

	void SetNewResource(unsigned int resourceUID)override;

	bool HasTexture();
	bool HasCheckers()const;
	unsigned int GetTextureID();
	ResourceTexture* GetTexture();
	unsigned int GetCheckersID()const;

	void OnEditor();

	//void GenTextureFromName(unsigned int ilImageName, std::string path = "");
private:

	//void DestroyTexture();
	void DestroyCheckers();
	void GenDefaultTexture();


public:
	bool usingCkeckers;

	//unsigned int width, height;
	//int format;
	//int depth;
	//int sizeInBytes;
	//unsigned int bpp;
	std::string path;

private:
	unsigned int idCheckers;
	//unsigned int idTexture;
	int size = 200; //preview
	unsigned int resourceID;

};
#endif // !__C_MATERIAL__
