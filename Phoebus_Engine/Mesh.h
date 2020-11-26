#pragma once
#include<vector>
#include "Resources.h"

enum class MeshDrawMode
{
	DRAW_MODE_BOTH, //from more to less important
	DRAW_MODE_FILL,
	DRAW_MODE_WIRE
};
enum class NormalDrawMode
{
	NORMAL_MODE_NONE,
	NORMAL_MODE_VERTEX,
	NORMAL_MODE_FACES,
	NORMAL_MODE_BOTH
};

class ResourceMesh : public Resource
{
public:

	ResourceMesh(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> normals, std::vector<float> texCoords,unsigned int UID);
	ResourceMesh(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> normals, std::vector<float> smoothedNormals, std::vector<float> texCoords, unsigned int UID);
	ResourceMesh(unsigned int UID);
	ResourceMesh(const ResourceMesh& other);
	~ResourceMesh();

	//void Draw();
	void GenerateSmoothedNormals();
	void GenerateBuffers();
	void FreeBuffers();
private:
	//void DrawVertexNormals();
	//void DrawFacesNormals();
	//void DrawBuffers();

public:

	//bool shadingFlat;
	//MeshDrawMode drawMode;
	//NormalDrawMode normalMode;



	unsigned int idIndex; // index in VRAM
	std::vector<unsigned int> indices;//index array

	unsigned int idVertex; // unique vertex in VRAM
	std::vector<float> vertices;//vertex array (note that vertex are just 3 of the elements stored in this vector)

	unsigned int idNormals;// normals in VRAM
	std::vector<float> normals;//normals array(note that normals are just 3 of the elements stored in this vector)

	//ADRI: DO NOT IMPORT THIS; CALL THE FUNC GENERATESMOOTHNORMALS()
	std::vector<float> smoothedNormals;//average of all faces 
	unsigned int idTexCoords; //texture coordinates in VRAM
	std::vector<float> texCoords;//texture coordinates array(note that texCoords are just 2 of the elements stored in this vector)

	//NewTexture* texture;
};