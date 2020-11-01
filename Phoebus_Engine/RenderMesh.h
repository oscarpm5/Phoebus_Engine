#ifndef __RENDER_MESH_H__
#define __RENDER_MESH_H__


#include "glmath.h" // cannot forward declare mat4x4

class C_Mesh;
class C_Material;
enum class MeshDrawMode;

class RenderMesh
{
public:
	RenderMesh(C_Mesh* mesh,C_Material*material, mat4x4 gTransform);
	~RenderMesh();
	void Draw(MeshDrawMode sceneMaxDrawMode);

private:
	void DrawVertexNormals();
	void DrawFacesNormals();
	void DrawBuffers();

private:
	C_Mesh* mesh;
	C_Material* material;

	mat4x4 transform;



};

#endif // !__RENDER_MESH_H__

