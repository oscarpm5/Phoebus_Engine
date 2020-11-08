#ifndef __C_TRANSFORM__
#define __C_TRANSFORM__
#include "Component.h"
#include "Assimp/include/matrix4x4.h" //we cant do forward declaration. Thanks, assimp
#include "glmath.h"//we cant do forward declaration.
#include "MathGeoLib/include/MathGeoLib.h"

class C_Transform : public Component
{
public:

	C_Transform(GameObject* owner, float4x4 lTransform);
	~C_Transform();

	float4x4 GetGlobalTransform()const;
	float4x4 GetLocalTransform()const;
	void OnEditor();

	float3 GetLocalPosition();
	float3 GetGlobalPosition();
	float3 GetLocalScale();

	void SetLocalPosition(float3 newPos);
	void SetLocalScale(float3 newScale);
	void SetLocalRot(float3x3 newRot);

	void UpdateGlobalMat();//updates the global matrix according to the local matrix

private:

	float4x4 lTransformMat;//local transform mat
	float4x4 gTransformMat;//global transform mat
};







#endif // !__C_TRANSFORM__