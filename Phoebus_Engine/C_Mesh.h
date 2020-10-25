#ifndef __C_MESH__
#define __C_MESH__
#include "Component.h"
#include <vector>
#include "Mesh.h"

class C_Mesh:public Component
{
public:
	C_Mesh(GameObject* owner);
	~C_Mesh();

	void SetMesh(Mesh mesh);
	Mesh* GetMesh()const;

private:
	Mesh* m;

	//WILL store bounding box here in the future
};



#endif // !__C_MESH__

