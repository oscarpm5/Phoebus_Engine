#include "Application.h"
#include "ModuleEditor3D.h"
#include "Primitive.h"


ModuleEditor3D::ModuleEditor3D(bool start_enabled):Module(start_enabled)
{
}

ModuleEditor3D::~ModuleEditor3D()
{
}

bool ModuleEditor3D::Init()
{
	bool ret = true;
	return ret;
}

bool ModuleEditor3D::Start()
{
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));



	SAux = PSphere(0.5,1);
	SAux.SetPos(0,1,1);
	 

	return ret;
}

update_status ModuleEditor3D::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleEditor3D::Update(float dt)
{
	PPlane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();
	return UPDATE_CONTINUE;
}

update_status ModuleEditor3D::PostUpdate(float dt)
{
	//Render
	//SAux.Render();
	return UPDATE_CONTINUE;
}

bool ModuleEditor3D::CleanUp()
{
	bool ret = true;
	return ret;
}
