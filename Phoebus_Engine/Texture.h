#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Resources.h"
#include <string>

class ResourceTexture:public Resource
{
public:
	ResourceTexture(unsigned int UID);
	~ResourceTexture();

	void GenTextureFromName(unsigned int ilImageName, std::string path = "");
	void DestroyTexture();
	unsigned int GetTextureID()const;

public:
	unsigned int width, height;
	int format;
	int depth;
	int sizeInBytes;
	unsigned int bpp;

	unsigned int idTexture;
};

#endif // !__TEXTURE_H__
