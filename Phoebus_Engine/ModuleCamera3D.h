#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

enum class CamObjective
{
	REFERENCE,
	CAMERA
};


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt) override;
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void MoveTo(const vec3& Destination,CamObjective toMove);
	void CamZoom(int addZoomAmount);
	float* GetRawViewMatrix();
	mat4x4 GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	float foV;
	float nearPlaneDist;
	float farPlaneDist;
	bool debugcamera;

private:
	float zoomLevel;
	mat4x4 ViewMatrix, ViewMatrixInverse;
};