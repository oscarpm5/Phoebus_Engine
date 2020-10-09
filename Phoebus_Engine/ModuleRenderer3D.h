#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(int width, int height);
	void TestingRender();
	void TestingRenderAtStart();

private:
	uint exampleMeshIdentifier = 0;
	uint exampleIndexBind = 0;
	uint exampleIndexData = 0;
	int nVertex = 0;
	int nIndex = 0;


public:

	Light lights[MAX_LIGHTS];

	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;
	
	
	//rendering config bools

	bool depthTesting = true;
	bool cullFace = true;
	bool lighting = true;
	bool colorMaterial = true;
	bool texture2D = true;
	bool wireframe = false;
};