#pragma once
#include<vector>
#include "texture.h"

enum class MeshDrawMode
{
	DRAW_MODE_FILL,
	DRAW_MODE_WIRE,
	DRAW_MODE_BOTH
};
enum class NormalDrawMode
{
	NORMAL_MODE_VERTEX,
	NORMAL_MODE_FACES,
	NORMAL_MODE_BOTH,
	NORMAL_MODE_NONE
};

class Mesh
{
public:

	Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> normals, std::vector<float> texCoords, NewTexture* texture = nullptr);
	Mesh(const Mesh& other);
	~Mesh();

	//void Draw();

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

	unsigned int idTexCoords; //texture coordinates in VRAM
	std::vector<float> texCoords;//texture coordinates array(note that texCoords are just 2 of the elements stored in this vector)

	//NewTexture* texture;
};