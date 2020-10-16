#pragma once

#include "Globals.h"
enum class MeshDrawMode
{
	DRAW_MODE_FILL,
	DRAW_MODE_WIRE,
	DRAW_MODE_BOTH
};

class Mesh
{
public:

	Mesh();
	~Mesh();

	void Draw() const;
	void GenerateBuffers();
	void FreeBuffers();



public:

	MeshDrawMode drawMode;

	uint idIndex; // index in VRAM
	uint numIndex;//number of indices
	uint* index;//index array

	uint idVertex; // unique vertex in VRAM
	uint numVertex;//number of vertex
	float* vertex;//vertex array
};