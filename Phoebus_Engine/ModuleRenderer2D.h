#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"

class ModuleRenderer2D : public Module
{
public:
	ModuleRenderer2D(bool start_enabled = true);
	~ModuleRenderer2D();

	bool Init();
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(int width, int height);

public:
	bool showDemoWindow = false;
	

private:

	//math checks

		//rand
	LCG seed; int randomRad1; int randomRad2;
		//spheres
	bool showSphereWindow = false;		bool sphereCol = false;
		//cylinders
	bool showCylWindow = false;			bool cylCol = false;
		//triangles
	bool showTriWindow = false;			bool triCol = false;
		//AABB
	bool showAABBWindow = false;		bool AABBCol = false;

}; 
