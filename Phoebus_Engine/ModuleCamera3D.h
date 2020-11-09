#ifndef __CAMERA_3D__
#define __CAMERA_3D__


#include "Module.h"
#include "Globals.h"
//#include "glmath.h"

class vec3;
class mat4x4;

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

	//void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	//void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void MoveTo(const vec3& Destination,CamObjective toMove);
	void CamZoom(int addZoomAmount);
	//float* GetRawViewMatrix();
	//mat4x4 GetViewMatrix();

private:

	//void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	//float foV;
	//float nearPlaneDist;
	//float farPlaneDist;
	//bool debugcamera;

	float camSpeed;
	float camSpeedMult;

	//NEW
	C_Camera* editorCam;

private:
	float zoomLevel;
	mat4x4 ViewMatrix, ViewMatrixInverse;

};
#endif // !__CAMERA_3D__