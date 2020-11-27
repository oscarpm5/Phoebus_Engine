#ifndef __RENDER_MESH_H__
#define __RENDER_MESH_H__


//#include "glmath.h" // cannot forward declare mat4x4
#include "MathGeoLib/include/MathGeoLib.h"

class C_Mesh;
class C_Material;
class ResourceMesh;
enum class MeshDrawMode;

class RenderMesh
{
public:
	RenderMesh(C_Mesh* mesh,C_Material*material, float4x4 gTransform,float3 color=float3(1.0f,1.0f,1.0f));
	~RenderMesh();
	void Draw(MeshDrawMode sceneMaxDrawMode);

private:
	void DrawVertexNormals(ResourceMesh*m);
	void DrawFacesNormals(ResourceMesh* m);
	void DrawBuffers(ResourceMesh* m);

private:
	C_Mesh* mesh;
	C_Material* material;

	float4x4 transform;
	float3 color;


};

#endif // !__RENDER_MESH_H__

