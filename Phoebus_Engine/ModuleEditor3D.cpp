#include "Application.h"
#include "ModuleEditor3D.h"
#include "Importer.h"


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
	Importer::InitializeDevIL();

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	
	 
	//Importer::LoadFBX("./Assets/warrior.FBX");
	//Importer::LoadFBX("./Assets/warrior.FBX");
	
	/*
	Importer::LoadFBX("./Assets/suzzane.FBX");
	Importer::LoadNewImage("./Assets/flamethrower.png");

	Importer::LoadFBX("./Assets/cube.fbx");
	Importer::LoadNewImage("./Assets/flamethrower.png");

	Importer::LoadFBX("./Assets/warrior.fbx");
	Importer::LoadNewImage("./Assets/lenna.png");*/

	return ret;
}

update_status ModuleEditor3D::PreUpdate(float dt)
{

	if(App->input->GetKey(SDL_SCANCODE_K) == KEY_UP) 
	{
		App->fileSystem->LoadAsset("Assets/bakerHouse/BakerHouse.fbx"); 
		App->fileSystem->LoadAsset("Assets/bakerHouse/Baker_house.png");
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_UP)
	{
		App->fileSystem->LoadAsset("Assets/Models/cube.FBX");
		App->fileSystem->LoadAsset("Assets/Models/Flamethrower.png");

	}
	return UPDATE_CONTINUE;
}

update_status ModuleEditor3D::Update(float dt)
{
	
	return UPDATE_CONTINUE;
}

update_status ModuleEditor3D::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleEditor3D::CleanUp()
{
	bool ret = true;
	meshes.clear();
	return ret;
}
