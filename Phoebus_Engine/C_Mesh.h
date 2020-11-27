#ifndef __C_MESH__
#define __C_MESH__

#include "Component.h"
#include <vector>
//#include "Mesh.h"

#include "MathGeoLib/include/Geometry/AABB.h"

class ResourceMesh;

class C_Mesh:public Component
{
public:
	C_Mesh(GameObject* owner,unsigned int ID);
	~C_Mesh();
	void SetNewResource(unsigned int resourceUID)override;
	//void SetMesh(ResourceMesh mesh);
	ResourceMesh* GetMesh();
	unsigned int GetResourceID()override;
	void OnEditor();

	AABB GetAABB()const;

	//used only for temporalResources(aka outline)
	void SetTemporalMesh(ResourceMesh* newTempMesh);
	ResourceMesh* GetTemporalMesh();
	void DeleteTemporalMesh();

public:
	float normalVertexSize;
	float normalFaceSize;
	int normalDrawMode;
	int meshDrawMode;
private:
	unsigned int resourceID;
	AABB localAABB;
	//WILL store bounding box here in the future

	ResourceMesh* temporalRMesh;//used only for outline, do NOT use it for anythig else
};



#endif // !__C_MESH__

