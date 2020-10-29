
#include "Glew/include/glew.h"
#include "DevIL/include/IL/il.h"
#include "texture.h"

NewTexture::NewTexture():width(0),height(0),idTexture(0)
{
}

NewTexture::NewTexture(const NewTexture&)
{
	width = 0;
	height = 0;
	idTexture = 0;
}

NewTexture::~NewTexture()
{
	DestroyTexture();
	width = 0;
	height = 0;
	
}

void NewTexture::GenTextureFromName(unsigned int ilImageName)
{
	DestroyTexture();

	ilBindImage(ilImageName);

	
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

void NewTexture::GenDefaultTexture()
{
	DestroyTexture();

	const int checkersHeight = 100;
	const int checkersWidth = 100;

	GLubyte checkerImage[checkersHeight][checkersWidth][4];
	for (int i = 0; i < checkersWidth; i++) {
		for (int j = 0; j < checkersWidth; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkersWidth, checkersHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void NewTexture::DestroyTexture()
{
	if (idTexture != 0)
	{
		glDeleteTextures(1, &idTexture);
		idTexture = 0;
	}
}

bool NewTexture::HasTexture() const
{
	if (idTexture != 0)
		return true;

	return false;
}

unsigned int NewTexture::GetTextureID() const
{
	return idTexture;
}
