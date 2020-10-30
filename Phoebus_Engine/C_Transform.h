#ifndef __C_TRANSFORM__
#define __C_TRANSFORM__
#include "Component.h"
#include "Assimp/include/matrix4x4.h" //we cant do forward declaration. Thanks, assimp
#include "glmath.h"//we cant do forward declaration.

class C_Transform : public Component
{
public:

	C_Transform(GameObject* owner, mat4x4 lTransform);
	~C_Transform();

	mat4x4 GetGlobalTransform()const;
	mat4x4 GetLocalTransform()const;
	void OnEditor();

	vec3 GetLocalPosition();
	vec3 GetGlobalPosition();

	void SetLocalPosition(vec3 newPos);

	void UpdateGlobalMat();//updates the global matrix according to the local matrix

private:
	aiMatrix4x4 lTraansIntoAssimpMatrix();
	vec3 GetEulerFromQuat(aiQuaterniont<float> rotation);
private:

	mat4x4 lTransformMat;//local transform mat
	mat4x4 gTransformMat;//global transform mat
};







#endif // !__C_TRANSFORM__