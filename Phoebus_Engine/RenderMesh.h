#ifndef __RENDER_MESH_H__
#define __RENDER_MESH_H__
#include "C_Mesh.h"
#include "C_Material.h"
#include "glmath.h"


class RenderMesh
{
public:
	RenderMesh(C_Mesh* mesh,C_Material*material, mat4x4 gTransform, MeshDrawMode drawMode, NormalDrawMode normalMode);
	~RenderMesh();
	void Draw();

private:
	void DrawVertexNormals();
	void DrawFacesNormals();
	void DrawBuffers();

private:
	C_Mesh* mesh;
	C_Material* material;


	MeshDrawMode drawMode;
	NormalDrawMode normalMode;
	mat4x4 transform;



};

#endif // !__RENDER_MESH_H__

