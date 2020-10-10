#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor3D.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")


#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>



#include "glmath.h"
#include"Color.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled), context()
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		GLenum glewError = glewInit();

		if (glewError != GLEW_OK)
		{
			printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}
	}

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
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

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}
	LOG("-------------- OpenGL Info --------------")
		LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	TestingRenderAtStart();


	SAux = PSphere(0.5, 1);
	SAux.SetPos(0, 1, 1);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->GetRawViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	
	for (uint i = 0; i < MAX_LIGHTS; ++i)
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

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GenerateBuffers();
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, exampleIndexBind);		//this is for printing the index
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, nIndex, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);





}

void ModuleRenderer3D::TestingRenderAtStart()
{

	static const GLfloat g_vertex_buffer_data[] = {
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
	nVertex = sizeof(g_vertex_buffer_data) / sizeof(float);
	glGenBuffers(1, (GLuint*) & (exampleMeshIdentifier));
	glBindBuffer(GL_ARRAY_BUFFER, exampleMeshIdentifier);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


	static const GLfloat g_vertex_index_buffer_data[] = {
   -1.0, -1.0,  1.0,
	1.0, -1.0,  1.0,
	-1.0,  1.0,  1.0,
	1.0,  1.0,  1.0,
	-1.0, -1.0, -1.0,
	1.0, -1.0, -1.0,
	-1.0,  1.0, -1.0,
	1.0,  1.0, -1.0,
	};

	glGenBuffers(1, (GLuint*) & (exampleIndexData));
	glBindBuffer(GL_ARRAY_BUFFER, exampleIndexData);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_index_buffer_data), g_vertex_index_buffer_data, GL_STATIC_DRAW);


	static const GLuint g_index_buffer[] = {
	 0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
	};

	nIndex = sizeof(g_index_buffer) / sizeof(GLuint);
	glGenBuffers(1, (GLuint*) & (exampleIndexBind));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, exampleIndexBind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * nIndex, g_vertex_index_buffer_data, GL_STATIC_DRAW);

}

void ModuleRenderer3D::GenerateBuffers()
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->window->Width(), App->window->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

	//Generate the depth buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, App->window->Width(), App->window->Height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);//unbind renderbuffer

	//attaching the image to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);//unbind framebuffer
}

void ModuleRenderer3D::Draw3D()
{


	//Set a color here TODO from the camera
	Color c = Color(0.05f, 0.05f, 0.1f);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClearColor(c.r, c.g, c.b, c.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//In the future render all objects here (iterate all objects and call its draw function?)
	PPlane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	SAux.Render();
	//Testing ground
	TestingRender();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(c.r, c.g, c.b, c.a);
}
