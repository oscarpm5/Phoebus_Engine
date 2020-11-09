#include "C_Camera.h"
#include "GameObject.h"
#include "C_Transform.h"
C_Camera::C_Camera(GameObject* owner):Component(ComponentType::CAMERA,owner), 
nearPlaneDist(0.1f),farPlaneDist(500.0f),FoVx(70.0f),FoVy(0.0f),invAspectRatio(0),
projectionMatrix(float4x4::identity)
{
	frustrum.type = math::FrustumType::PerspectiveFrustum;
	SetNewAspectRatio(1920.0f, 1080.0f);//TODO default
}

C_Camera::~C_Camera()
{
}

bool C_Camera::Update(float dt)
{

	if (owner != nullptr)
	{
		CalcCamPosFromTransformMat(owner->GetComponent<C_Transform>()->GetGlobalTransform());
	}

	return true;
}

void C_Camera::OnEditor()
{
}

void C_Camera::CalcCamPosFromTransformMat(float4x4& gTransform)
{
	//TODO this global mat might have to be transposed?
	frustrum.pos = gTransform.TranslatePart();
	frustrum.front = gTransform.Col3(2); // The camera looks towards +Z axis of the given transform.
	frustrum.up = gTransform.Col3(1); // The camera up points towards +Y of the given transform.
	assume(pos.IsFinite());
	assume(front.IsNormalized());
	assume(up.IsNormalized());
	assume(gTransform.IsColOrthogonal3()); // Front and up must be orthogonal to each other.
	assume(EqualAbs(gTransform.Determinant(), 1.f)); // The matrix cannot contain mirroring.
}

void C_Camera::CalcCamPosFromDirections(float3 pos, float3 front, float3 up)
{
	float3 normFront = front.Normalized();
	float3 normUP = up.Normalized();
	//TODO this global mat might have to be transposed?
	frustrum.pos =pos;
	frustrum.front = -normFront; // The camera looks towards -Z axis of the given transform. TODO Z+ invert cam
	frustrum.up = normUP; // The camera up points towards +Y of the given transform.
	assume(pos.IsFinite());
	assume(front.IsNormalized());
	assume(up.IsNormalized());
}

float4x4 C_Camera::GetProjMat() const
{
	return projectionMatrix;
}

float4x4 C_Camera::GetViewMat() const
{
	float4x4 viewMat = frustrum.ViewMatrix();
	return viewMat.Transposed();
}

void C_Camera::SetNewAspectRatio(int width, int height)
{
	float newW = max(width, 0.01f);
	float newH = max(height, 0.01f);

	invAspectRatio = newH / newW;
	SetNewFoV(FoVx);
}

void C_Camera::SetNewFoV(float foV)
{

	FoVx = max(foV, 1.0f);
	FoVx = min(foV, 180.0f);

	float aux = Tan(DegToRad(FoVx / 2));
	aux *= invAspectRatio;

	FoVy = RadToDeg(2 * Atan(aux));//vertical aspect ratio calculates from horizontal one

	UpdateProjectionMat();

}

void C_Camera::SetNearPlane(float dist)
{
	nearPlaneDist = max(dist, 0.0f);

	if (nearPlaneDist > farPlaneDist)
	{
		SetFarPlane(nearPlaneDist);
	}
	else
	{
		UpdateProjectionMat();
	}

}

void C_Camera::SetFarPlane(float dist)
{
	farPlaneDist = max(dist, 0.0f);

	if (farPlaneDist < nearPlaneDist)
	{
		SetNearPlane(farPlaneDist);
	}
	else
	{
		UpdateProjectionMat();
	}
}

float C_Camera::GetNearPlaneDist() const
{
	return frustrum.nearPlaneDistance;
}

float C_Camera::GetFarPlaneDist() const
{
	return frustrum.farPlaneDistance;
}

float C_Camera::GetFoV() const
{
	return RadToDeg(frustrum.horizontalFov);
}

void C_Camera::UpdateProjectionMat()
{
	frustrum.nearPlaneDistance = nearPlaneDist;
	frustrum.farPlaneDistance = farPlaneDist;
	frustrum.horizontalFov = DegToRad(FoVx);
	frustrum.verticalFov = DegToRad(FoVy);
	projectionMatrix = frustrum.ProjectionMatrix().Transposed();
}
