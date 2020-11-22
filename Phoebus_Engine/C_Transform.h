#ifndef __C_TRANSFORM__
#define __C_TRANSFORM__
#include "Component.h"
#include "Assimp/include/matrix4x4.h" //we cant do forward declaration. Thanks, assimp
//#include "glmath.h"//we cant do forward declaration.
#include "MathGeoLib/include/MathGeoLib.h"

class C_Transform : public Component
{
public:

	C_Transform(GameObject* owner, float4x4 lTransform);
	~C_Transform();

	float4x4 GetGlobalTransform()const;
	float4x4 GetLocalTransform()const;
	void SetGlobalTransform(float4x4 newGTransform);
	void SetLocalTransform(float4x4 newLTransform);

	void OnEditor();

	float3 GetLocalPosition();
	float3 GetGlobalPosition();
	float3 GetLocalScale();
	float3 GetGlobalScale();


	void SetLocalPosition(float3 newPos);
	void SetLocalScale(float3 newScale);
	void SetLocalRot(Quat newRot);
	void SetGlobalPosition(float3 newPos);
	void SetGlobalScale(float3 newScale);
	void SetGlobalRot(Quat newRot);


	void UpdateGlobalMat();//updates the global matrix according to the local matrix
	void UpdateLocalMat();//updates the local matrix according to the global matrix

public:
	bool localMode;
private:

	float4x4 lTransformMat;//local transform mat
	float4x4 gTransformMat;//global transform mat
};







#endif // !__C_TRANSFORM__