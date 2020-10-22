#ifndef __TEXTURE_H__
#define __TEXTURE_H__



class NewTexture
{
public:
	NewTexture();
	NewTexture(const NewTexture&);
	~NewTexture();
	void GenTextureFromName(unsigned int ilImageName);
	void GenDefaultTexture();

	void DestroyTexture();

	bool HasTexture() const;
	unsigned int GetTextureID()const;

public:
	unsigned int width,height;

private:
	unsigned int idTexture;
};

#endif // !__TEXTURE_H__