#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "Primitive.h"


#define MAX_LIGHTS 8
#define INDEX_CUBE 36

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
	void GenerateBuffers(int width, int height);
	void Draw3D();

private:
	uint exampleMeshIdentifier = 0;
	uint indexBind = 0;
	uint vertexBind = 0;
	int nVertex = 0;
	int indexSize = 0;




public:
	PSphere SAux;
	Light lights[MAX_LIGHTS];

	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	uint frameBuffer = -1;
	uint renderTex = -1;
	uint depthBuffer = -1;


	//rendering config bools

	bool depthTesting = true;
	bool cullFace = true;
	bool lighting = true;
	bool colorMaterial = true;
	bool texture2D = true;
	bool wireframe = false;
};