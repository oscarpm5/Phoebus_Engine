#pragma once
#include<vector>
enum class MeshDrawMode
{
	DRAW_MODE_FILL,
	DRAW_MODE_WIRE,
	DRAW_MODE_BOTH
};

class Mesh
{
public:

	Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);
	Mesh(const Mesh& other);
	~Mesh();

	void Draw();
	void GenerateBuffers();
	void FreeBuffers();



public:

	MeshDrawMode drawMode;

	unsigned int idIndex; // index in VRAM
	std::vector<unsigned int> indices;//index array

	unsigned int idVertex; // unique vertex in VRAM
	std::vector<float> vertices;//vertex array (note that vertex are just 3 of the elements stored in this vector)
};