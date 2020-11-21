#ifndef __MODULE_RENDERER__
#define __MODULE_RENDERER__


#include "Module.h"
//#include "Globals.h"
#include "Light.h" //incuded due to an array declared here
#include <vector>
#include "RenderMesh.h"
#include "RenderBox.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "C_Camera.h" //TODO forward declare this



#define MAX_LIGHTS 8
#define INDEX_CUBE 36

class mat3x3;
class mat4x4;
class Mesh;

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
	void DrawOutline();


	void AddMeshToDraw(C_Mesh* mesh, C_Material* material, float4x4 gTransform,bool isSelected);
	void AddMeshToStencil(C_Mesh* mesh, float4x4 gTransform,float3 color=float3(1.0f,1.0f,1.0f));
	void AddBoxToDraw(std::vector<float3> corners);

	bool IsInsideFrustum(std::vector<float3>& points);

private:
	void RenderMeshes();
	void RenderSelectedMeshes();
	void RenderStencil();
	void RenderAABBs();
	void DrawGrid();

	//sets all the config options(depth testing, cull faces,etc...) to their bool values
	void SetGLRenderingOptions();

	bool ExpandMeshVerticesByScale(Mesh& m, float newScale);//returns false if scaling cannot be done

private:
	unsigned int exampleMeshIdentifier;
	unsigned int indexBind;
	unsigned int vertexBind;
	int nVertex;
	int indexSize;
	std::vector<RenderMesh> drawMeshes;
	std::vector<RenderMesh> drawSelectedMeshes;
	std::vector<RenderMesh> drawStencil;
	std::vector<RenderBox> drawAABBs;
	std::vector<C_Mesh*> stencilMeshes;


public:
	Light lights[MAX_LIGHTS];

	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	unsigned int frameBuffer;
	unsigned int renderTex;
	unsigned int depthBuffer;

	float gridLength;
	float outlineScale;
	LCG seed;

	//rendering config bools
	bool depthTesting;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool drawGrid;
	bool showDepth;
	bool displayAABBs;
	C_Camera* activeCam;//culling camera
};
#endif // !__MODULE_RENDERER__