#include "Application.h"
#include "ModuleEditor3D.h"
#include "Importer.h"
#include "GameObject.h"


ModuleEditor3D::ModuleEditor3D(bool start_enabled) :Module(start_enabled)
{
	root = new GameObject(nullptr, "SceneRoot");

	
	test1 = new GameObject(root, "Test1"); testingrounds.push_back(test1);
	test2 = new GameObject(root, "Test2"); testingrounds.push_back(test2);
	test3 = new GameObject(root, "Test3"); testingrounds.push_back(test3);
	test4 = new GameObject(test3, "Test4"); testingrounds.push_back(test4);
	test5 = new GameObject(test3, "Test5"); testingrounds.push_back(test5);
	test6 = new GameObject(test4, "Test6"); testingrounds.push_back(test6);
	test7 = new GameObject(test6, "Test7"); testingrounds.push_back(test7);
	
}

ModuleEditor3D::~ModuleEditor3D()
{
	delete root;
	root = nullptr;
	for (int i = 0; i < testingrounds.size(); i++) 
	{
		delete testingrounds[i];
		testingrounds[i] = nullptr;
	}
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

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_UP)
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
	for (int i = textures.size() - 1; i >= 0; i--)
	{
		delete textures[i];
		textures[i] = nullptr;
	}
	textures.clear();
	return ret;
}
