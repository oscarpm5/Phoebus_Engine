#ifndef __C_CAMERA__
#define __C_CAMERA__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class C_Camera:public Component
{
public:

	C_Camera(GameObject* owner);
	~C_Camera();

	bool Update(float dt);

	void OnEditor();//component drawing itself on the window

	void CalcCamPosFromTransformMat(float4x4 &gTransform);
	void CalcCamPosFromDirections(float3 pos, float3 front, float3 up);
	float4x4 GetProjMat()const;
	float4x4 GetViewMat()const;

	void SetNewAspectRatio(int width, int height);
	void SetNewAspectRatio(float aspectRatio);
	void SetNewFoV(float foV);
	void SetNearPlane(float dist);
	void SetFarPlane(float dist);

	float GetNearPlaneDist()const;
	float GetFarPlaneDist()const;
	float GetFoV()const;
	float GetFoVx() const;
	float GetFoVY() const;
	float GetAspectRatio()const;
	float GetInvAspectRatio()const;
	const Frustum& GetFrustum() const;

	void GetFrustumPoints(std::vector<float3>& emptyVector);

private:
	void UpdateProjectionMat();
private:

	float nearPlaneDist;
	float farPlaneDist;
	float FoVx;//This is the horizontal FoV which the user changes
	float FoVy;//Vertical FoV, the user cannot change this
	float invAspectRatio;

	Frustum frustum;

	float4x4 projectionMatrix;//this matrix is already transposed for use in opengl
};

#endif // !__C_CAMERA__
