//#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
//#include "Light.h"

#include "ModuleEditor3D.h"
#include "GameObject.h"
#include "RenderMesh.h"
#include "Mesh.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Transform.h"
//include & lib of glew

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")


#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


//#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include"Color.h"

#include <math.h>
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */



//TODO make a bool that tells whether we are on editor mode or play mode and changes camera accordingly


float vertexArray[] = {
	0.f, 0.f, 0.f,
	10.f, 0.f, 0.f,
	10.f, 0.f, -10.f,
	0, 0, -10.f,

	0.f, 10.f, 0.f,
	10.f, 10.f, 0.f,
	10.f, 10.f, -10.f,
	0.f, 10.f, -10.f
};


uint indexArray[] = {
	4, 0, 1,
	1, 5, 4,

	4, 7, 3,
	3, 0, 4,

	2, 3, 7,
	7, 6, 2,

	7, 4, 5,
	5, 6, 7,

	5, 1, 2,
	2, 6, 5,

	0, 3, 2,
	2, 1, 0
};


ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled), context()
{
	frameBuffer = -1;
	renderTex = -1;
	depthBuffer = -1;


	exampleMeshIdentifier = 0;
	indexBind = 0;
	vertexBind = 0;
	nVertex = 0;
	indexSize = 0;


	//TODO load this from config in the future
	depthTesting = true;
	cullFace = true;
	lighting = true;
	colorMaterial = true;
	texture2D = true;
	drawGrid = true;
	drawDebugRay = false;
	showDepth = false;
	activeCam = nullptr;
	displayAABBs = false;
	//Just making sure this is initialized
	gridLength = 500.f;
	outlineScale = 1.1f;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
	activeCam = nullptr;
}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Start number generator seed
	seed = LCG::LCG();

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("[error]OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		GLenum glewError = glewInit();

		if (glewError != GLEW_OK)
		{
			printf("[error] Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}
	}

	if (ret == true)
	{


		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("[error]Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("[error]Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//TODO ADDED from class this enables alpha clip at 0.5 alpha
		//glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, 0.5f);
		//TODO ADDED from class this enables alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("[error]Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		SetGLRenderingOptions();
		lights[0].Active(true);


		//Use Vsync
		if (VSYNC &&
			SDL_GL_SetSwapInterval(1) < 0)
			LOG("[warning] Unable to set VSync! SDL Error: %s\n", SDL_GetError());


	}
	LOG("-------------- OpenGL Info --------------")
		LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//TestingRenderAtStart();



	//SAux = PSphere(0.5, 1);
	//SAux.SetPos(0, 1, 1);


	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	//glLoadMatrixf(App->camera->GetRawViewMatrix());
	glLoadMatrixf(&App->camera->editorCam->GetViewMat().v[0][0]);

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (unsigned int i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//Render here


	Draw3D();
	App->renderer2D->Draw();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");


	glDeleteRenderbuffers(1, &depthBuffer);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &renderTex);

	SDL_GL_DeleteContext(context);

	drawMeshes.clear();
	drawSelectedMeshes.clear();
	drawStencil.clear();
	drawAABBs.clear();
	while (!stencilMeshes.empty())//clear stencil meshes
	{

		C_Mesh* currMesh = stencilMeshes.back();
		delete currMesh;
		currMesh = nullptr;

		stencilMeshes.pop_back();
	}
	stencilMeshes.clear();
	stencilMeshes.shrink_to_fit();

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//ProjectionMatrix = perspective(App->camera->foV, (float)width / (float)height, App->camera->nearPlaneDist, App->camera->farPlaneDist);
	//glLoadMatrixf(&ProjectionMatrix);

	App->camera->editorCam->SetNewAspectRatio(width, height);
	glLoadMatrixf(&App->camera->editorCam->GetProjMat().v[0][0]);

	glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(App->camera->GetRawViewMatrix());
	glLoadMatrixf(&App->camera->editorCam->GetViewMat().v[0][0]);

	GenerateBuffers(width, height);
}

void ModuleRenderer3D::TestingRender()
{
	glEnableClientState(GL_VERTEX_ARRAY);


	/*
	glBindBuffer(GL_ARRAY_BUFFER, exampleMeshIdentifier);
	glVertexPointer(3, GL_FLOAT, 0, NULL);			//this is for printing the vertices
	// … bind and use other buffers
	glDrawArrays(GL_TRIANGLES, 0, nVertex);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, vertexBind);			//this is for printing the index
	glVertexPointer(3, GL_FLOAT, 0, NULL);				//Null => somehow OpenGL knows what you're talking about
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBind);	//Because this Bind is after the vertex bind, OpenGl knows these two are in order & connected. *Magic*	

	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, NULL);	//End of "bind addition" here...



	glDisableClientState(GL_VERTEX_ARRAY);		//... or here



}

//For now it uses Inmediate mode (testing purposes)
void ModuleRenderer3D::TestingRenderAtStart()
{
	// no index cube
	static const GLfloat g_vertex_buffer[] = {
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
	};
	nVertex = sizeof(g_vertex_buffer) / sizeof(float);
	glGenBuffers(1, (GLuint*) & (exampleMeshIdentifier));
	glBindBuffer(GL_ARRAY_BUFFER, exampleMeshIdentifier);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer), g_vertex_buffer, GL_STATIC_DRAW);


	////index cube
	//
	//glGenBuffers(1, (GLuint*) & (vertexBind));
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBind);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);


	//glGenBuffers(1, (GLuint*) & (indexBind));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBind);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

	//indexSize = sizeof(indexArray) / sizeof(unsigned int);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, exampleMeshIdentifier);
	glVertexPointer(3, GL_FLOAT, 0, NULL);			//this is for printing the vertices
	// … bind and use other buffers
	glDrawArrays(GL_TRIANGLES, 0, nVertex);
	glDisableClientState(GL_VERTEX_ARRAY);		//... or here

	glDeleteBuffers(1, &exampleMeshIdentifier);
}

void ModuleRenderer3D::GenerateBuffers(int width, int height)
{
	//Delete buffers
	glDeleteRenderbuffers(1, &depthBuffer);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &renderTex);

	//Create Frame Buffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//Generate the texture
	glGenTextures(1, &renderTex);
	glBindTexture(GL_TEXTURE_2D, renderTex);

	if (showDepth)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

	//Generate the depth buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);//unbind renderbuffer

	//attaching the image to the frame buffer
	if (showDepth)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderTex, 0);
	else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);//unbind framebuffer
}

void ModuleRenderer3D::Draw3D()
{


	//TODO Set a color here  from the camera ?? on editor mode take it from the editor cam on game take it from the main cam
	Color c = Color(0.05f, 0.05f, 0.1f);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	DrawOutline();
	//RenderMeshes();
	RenderAABBs();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(c.r, c.g, c.b, c.a);



}

void ModuleRenderer3D::DrawOutline()
{
	const float3 outlineColorMain = float3(1.0f, 0.5f, 0.0f);
	const float3 outlineColorSecond = float3(0.75f, 0.25f, 0.0f);

	for (int i = 0; i < App->editor3d->selectedGameObjs.size(); i++)
	{
		C_Transform* transf = App->editor3d->selectedGameObjs[i]->GetComponent<C_Transform>();
		std::vector<C_Mesh*> meshes = App->editor3d->selectedGameObjs[i]->GetComponents<C_Mesh>();


		float4x4 transfMat = transf->GetGlobalTransform();
		for (int j = 0; j < meshes.size(); j++)
		{
			Mesh m = Mesh(*meshes[i]->GetMesh());
			ExpandMeshVerticesByScale(m, outlineScale);//TODO make the user adjust this from the config panel
			m.FreeBuffers();
			m.GenerateBuffers();

			C_Mesh* currMesh = new C_Mesh(nullptr);
			currMesh->SetMesh(m);
			stencilMeshes.push_back(currMesh);

			float3 currentOutlineCol = outlineColorMain;
			if (j != meshes.size() - 1)//makes outline color different for selected & selected->focused objects (will test once we have multiselection)
			{
				currentOutlineCol = outlineColorSecond;
			}

			AddMeshToStencil(currMesh, transfMat, currentOutlineCol);
		}

	}


	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glStencilMask(0xFF);
	if (drawGrid)
	{
		DrawGrid();
	}
	if (drawDebugRay)
	{
		DrawDebugRay();
	}
	RenderMeshes();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	RenderSelectedMeshes();

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);

	if (depthTesting)
		glDisable(GL_DEPTH_TEST);
	if (lighting)
		glDisable(GL_LIGHTING);
	//glDisable(GL_CULL_FACE);
	RenderStencil();
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);

	if (depthTesting)
		glEnable(GL_DEPTH_TEST);
	if (lighting)
		glEnable(GL_LIGHTING);
	//glEnable(GL_CULL_FACE);

	//TODO for the moment we harcode enable/ disable of the depth/lighting here and that causes depth 6 lightning display to have stopped working
	//TODO make objects scale from their bounding box center when on outline mode or ideally scale from vertex normal

	glDisable(GL_STENCIL_TEST);
}

void ModuleRenderer3D::RenderMeshes()
{
	for (int i = 0; i < drawMeshes.size(); i++)
	{
		drawMeshes[i].Draw(App->editor3d->maxSceneDrawMode);
	}
	drawMeshes.clear();
	drawMeshes.shrink_to_fit();
}

void ModuleRenderer3D::RenderSelectedMeshes()
{
	for (int i = 0; i < drawSelectedMeshes.size(); i++)
	{
		drawSelectedMeshes[i].Draw(App->editor3d->maxSceneDrawMode);
	}
	drawSelectedMeshes.clear();
	drawSelectedMeshes.shrink_to_fit();
}

void ModuleRenderer3D::RenderStencil()
{
	for (int i = 0; i < drawStencil.size(); i++)
	{
		drawStencil[i].Draw(MeshDrawMode::DRAW_MODE_BOTH);
	}
	drawStencil.clear();
	drawStencil.shrink_to_fit();

	while (!stencilMeshes.empty())//clear stencil meshes
	{

		C_Mesh* currMesh = stencilMeshes.back();
		delete currMesh;
		currMesh = nullptr;

		stencilMeshes.pop_back();
	}
	stencilMeshes.clear();
	stencilMeshes.shrink_to_fit();

}

void ModuleRenderer3D::RenderAABBs()
{
	if (lighting)
		glDisable(GL_LIGHTING);
	for (int i = 0; i < drawAABBs.size(); i++)
	{
		drawAABBs[i].Draw();
	}
	drawAABBs.clear();
	drawAABBs.shrink_to_fit();

	if(lighting)
		glEnable(GL_LIGHTING);
}

void ModuleRenderer3D::DrawGrid()
{
	int nQuadsInAQuad = 4; //ex. 4 quads: 4 lines + 1 extra line for the next quad aka: number of small quads in a giant quad

	float cameraHeight = abs(App->camera->Position.y - fmod(App->camera->Position.y, 1));

	float sepLvl = logf(cameraHeight) / logf(nQuadsInAQuad);//log(x)/log(b)=log_baseb(x)

	float transparency = 1 - fmod(sepLvl, 1.0f); //value between 1 and 0 being 0 the most transparent

	sepLvl = max(sepLvl, 0);


	float newSep = pow(nQuadsInAQuad, (int)floor(sepLvl)); //what is the new separation compared to the original one (4 times bigger?,..)
	//float realGridLength = gridLength- (fmod(gridLength, newSep));
	//float lineCount = realGridLength / newSep;


	float realGridLength = gridLength / (newSep * nQuadsInAQuad);
	realGridLength = ceil(realGridLength);
	realGridLength *= (newSep * nQuadsInAQuad);

	glBegin(GL_LINES);

	for (float i = -realGridLength; i <= realGridLength; i += newSep)
	{
		float greatLines = fmod(i + realGridLength, newSep * nQuadsInAQuad);//i+realgridlength makes the number positive

		bool isCenterLine = false;
		bool isGreatLine = false;
		if (i >= -newSep * 0.5f && i <= newSep * 0.5f)isCenterLine = true;
		if (greatLines == 0)isGreatLine = true;

		float4 color1 = float4(transparency, transparency, transparency, transparency);
		float4 color2 = float4(transparency, transparency, transparency, transparency);;

		if (isGreatLine)
		{
			glLineWidth(1.0f);
			color1 = float4(1.0f, 1.0f, 1.0f, 1.0f);
			color2 = float4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (isCenterLine)
		{
			glLineWidth(1.0f);
			color1 = float4(0.0f, 0.0f, 1.0f, 1.0f);
			color2 = float4(1.0f, 0.0f, 0.0f, 1.0f);
		}

		glLineWidth(1.0f);

		glColor4f(color1.x, color1.y, color1.z, color1.w);

		glVertex3f(i, 0.0f, -gridLength);
		glVertex3f(i, 0.0f, gridLength);

		glColor4f(color2.x, color2.y, color2.z, color2.w);

		glVertex3f(-gridLength, 0.0f, i);
		glVertex3f(gridLength, 0.0f, i);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


	}

	glEnd();

}

void ModuleRenderer3D::DrawDebugRay()
{
	float4 rayColor = float4(0.5f, 0.0f, 1.0f, 1.0f);

	if (lighting)
	{
		glDisable(GL_LIGHTING);
	}

	glBegin(GL_LINES);
	glColor4f(rayColor.x, rayColor.y, rayColor.z, rayColor.w);

	glVertex3f(rayLine.a.x, rayLine.a.y, rayLine.a.z);
	glVertex3f(rayLine.b.x, rayLine.b.y, rayLine.b.z);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnd();

	if (lighting)
	{
		glEnable(GL_LIGHTING);
	}
}

void ModuleRenderer3D::SetGLRenderingOptions()
{
	if (depthTesting)glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
	if (cullFace)glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);
	if (lighting)glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);
	if (colorMaterial)glEnable(GL_COLOR_MATERIAL);
	else glDisable(GL_COLOR_MATERIAL);
	if (texture2D)glEnable(GL_TEXTURE_2D);
	else glDisable(GL_TEXTURE_2D);

	//wireframe here too?
}

bool ModuleRenderer3D::ExpandMeshVerticesByScale(Mesh& m, float newScale)//TODO consider creating an expanded mesh when the mesh is created instead of doing it every frame
{
	if (m.normals.empty())//TODO in the future if scaling cannot be done by vertex normals, use generated face normals or normal scaling instead
		return false;


	for (int i = 0; i < m.vertices.size(); i += 3)
	{
		float3 currVertex = float3(m.vertices[i], m.vertices[i + 1], m.vertices[i + 2]);

		//if newScale is 1 it means the scale should remain the same
		currVertex.x += (m.smoothedNormals[i] * (newScale - 1.0f));
		currVertex.y += (m.smoothedNormals[i + 1] * (newScale - 1.0f));
		currVertex.z += (m.smoothedNormals[i + 2] * (newScale - 1.0f));
		m.vertices[i] = currVertex.x;
		m.vertices[i + 1] = currVertex.y;
		m.vertices[i + 2] = currVertex.z;

	}


}


void ModuleRenderer3D::AddMeshToDraw(C_Mesh* mesh, C_Material* material, float4x4 gTransform, bool isSelected)
{
	if (isSelected)
		drawSelectedMeshes.push_back(RenderMesh(mesh, material, gTransform));
	else
		drawMeshes.push_back(RenderMesh(mesh, material, gTransform));
}

void ModuleRenderer3D::AddMeshToStencil(C_Mesh* mesh, float4x4 gTransform, float3 color)
{
	float scale = 1.05f;
	drawStencil.push_back(RenderMesh(mesh, nullptr, gTransform /** float4x4::Scale(float3(scale, scale, scale))*/, color));
}

void ModuleRenderer3D::AddBoxToDraw(std::vector<float3> corners)
{
	drawAABBs.push_back(RenderBox(corners));//TODO change Box color here (global config var?)
}

bool ModuleRenderer3D::IsInsideFrustum(std::vector<float3>& points)
{
	//if inside frustum ret true

	int iTotalIn = 0;

	//for each camera plane
	for (int p = 0; p < 6; p++) {
		int iInCount = 8;
		int iPtIn = 1;
		//for each corner of the AABB box
		for (int i = 0; i < 8; i++) {
			// test this point against the planes

			Frustum f;
			Plane planes[6];
			if (activeCam != nullptr && activeCam->IsActive())
			{
				f = activeCam->GetFrustum();//TODO active cam goes kabbom if deleted
			}
			else
			{
				f = App->camera->editorCam->GetFrustum();
			}
			f.GetPlanes(planes);

			if (planes[p].IsOnPositiveSide(points[i])) //<-- “IsOnPositiveSide” from MathGeoLib
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if (iInCount == 0)
			return false;
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn > 0)
		return true;
}

void ModuleRenderer3D::SetCamRay(LineSegment line)
{
	rayLine = line;
}
