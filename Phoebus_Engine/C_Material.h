#ifndef __C_MATERIAL__
#define __C_MATERIAL__


#include "Component.h"

class C_Material:public Component
{
public:
	C_Material(GameObject* owner);
	C_Material(GameObject* owner,unsigned int ilImageName);
	~C_Material();


	bool HasTexture() const;
	bool HasCheckers()const;
	unsigned int GetTextureID()const;
	unsigned int GetCheckersID()const;

	void OnEditor();

private:

	void GenTextureFromName(unsigned int ilImageName);
	void DestroyTexture();
	void DestroyCheckers();

	void GenDefaultTexture();

public:
	bool usingCkeckers;

	unsigned int width, height;
	int format;
	int depth;
	int sizeInBytes;
	unsigned int bpp;

private:
	unsigned int idCheckers;
	unsigned int idTexture;
};
#endif // !__C_MATERIAL__
