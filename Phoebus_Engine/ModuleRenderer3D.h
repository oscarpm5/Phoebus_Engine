#ifndef __MODULE_RENDERER__
#define __MODULE_RENDERER__


#include "Module.h"
//#include "Globals.h"
#include "Light.h" //incuded due to an array declared here
#include <vector>
#include "RenderMesh.h"



#define MAX_LIGHTS 8
#define INDEX_CUBE 36

class mat3x3;
class mat4x4;

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

	void ModuleRenderer3D::AddMeshToDraw(C_Mesh* mesh, C_Material* material, float4x4 gTransform);


private:
	void RenderMeshes();
	void DrawGrid();

	//sets all the config options(depth testing, cull faces,etc...) to their bool values
	void SetGLRenderingOptions();

private:
	unsigned int exampleMeshIdentifier;
	unsigned int indexBind;
	unsigned int vertexBind;
	int nVertex;
	int indexSize;
	std::vector<RenderMesh> drawMeshes;



public:
	Light lights[MAX_LIGHTS];

	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	unsigned int frameBuffer;
	unsigned int renderTex;
	unsigned int depthBuffer;

	float gridLength;

	//rendering config bools
	bool depthTesting;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool drawGrid;
};
#endif // !__MODULE_RENDERER__