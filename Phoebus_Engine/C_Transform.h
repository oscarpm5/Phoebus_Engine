#ifndef __C_TRANSFORM__
#define __C_TRANSFORM__
#include "Component.h"
#include "glmath.h"



class C_Transform : public Component
{
public:

	C_Transform(GameObject* owner, mat4x4 lTransform);
	~C_Transform();

	mat4x4 GetGlobalTransform()const;
	mat4x4 GetLocalTransform()const;
	void OnEditor();


	void UpdateGlobalMat();//updates the global matrix according to the local matrix

private:

	mat4x4 lTransformMat;//local transform mat
	mat4x4 gTransformMat;//global transform mat
};







#endif // !__C_TRANSFORM__