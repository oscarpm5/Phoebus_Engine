#ifndef __C_MESH__
#define __C_MESH__

#include "Component.h"
#include <vector>
//#include "Mesh.h"

#include "MathGeoLib/include/Geometry/AABB.h"

class Mesh;

class C_Mesh:public Component
{
public:
	C_Mesh(GameObject* owner);
	~C_Mesh();

	void SetMesh(Mesh mesh);
	Mesh* GetMesh()const;

	void OnEditor();

	AABB GetAABB()const;

public:
	float normalVertexSize;
	float normalFaceSize;
	int normalDrawMode;
	int meshDrawMode;
private:
	Mesh* m;
	AABB localAABB;
	//WILL store bounding box here in the future
};



#endif // !__C_MESH__

