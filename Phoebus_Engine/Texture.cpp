#include "Texture.h"
#include "Glew/include/glew.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"

ResourceTexture::ResourceTexture(unsigned int UID): Resource(UID,ResourceType::TEXTURE),
idTexture(0),width(0),height(0),format(0),depth(0),sizeInBytes(0),bpp(0),ilImageID(-1)
{
}

ResourceTexture::~ResourceTexture()
{
	UnloadFromMemory();
}

void ResourceTexture::GenTextureFromName(unsigned int ilImageName)
{
	DestroyTexture();
	ilImageID = ilImageName;
	ilBindImage(ilImageName);

	//get properties
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);

	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}


	format = ImageInfo.Format;
	/*IL_COLOUR_INDEX
		IL_RGB
		IL_RGBA
		IL_BGR
		IL_BGRA
		IL_LUMINANCE*/

	depth = ImageInfo.Depth;
	bpp = ImageInfo.Bpp;//bytes per pixel
	sizeInBytes = ImageInfo.SizeOfData;//bytes
	width = ImageInfo.Width;
	height = ImageInfo.Height;

	//gen texture

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	glBindTexture(GL_TEXTURE_2D, 0);

	ilBindImage(0);
}

void ResourceTexture::DestroyTexture()
{
	if (idTexture != 0)
	{
		glDeleteTextures(1, &idTexture);
		idTexture = 0;
	}

	if (ilImageID != -1)
	{
		ilDeleteImages(1, (unsigned int*)&ilImageID);
		ilImageID = -1;
	}
}

unsigned int ResourceTexture::GetTextureID() const
{
	return idTexture;
}

bool ResourceTexture::UnloadFromMemory()
{
	DestroyTexture();
	width = 0;
	height = 0;
	format = 0;
	depth = 0;
	sizeInBytes = 0;
	bpp = 0;

	isLoaded = false;

	return true;
}
