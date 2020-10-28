#include "Application.h"
#include "ModuleEditor3D.h"
#include "Importer.h"
#include "GameObject.h"
#include "MathGeoLib/include/MathGeoLib.h"


ModuleEditor3D::ModuleEditor3D(bool start_enabled) :Module(start_enabled)
{
	mat4x4 transform;
	root = new GameObject(nullptr, "SceneRoot", transform);
	selectedGameObj = root;
	
	test1 = new GameObject(root, "Test1", transform);
	test2 = new GameObject(root, "Test2", transform);
	test2->isActive = false;
	test3 = new GameObject(root, "Test3", transform); 
	test4 = new GameObject(test3, "Test4", transform); 
	test4->isActive = false;
	test5 = new GameObject(test3, "Test5", transform); 
	test6 = new GameObject(test4, "Test6", transform); 
	test7 = new GameObject(test6, "Test7", transform);
	
	
	
}

ModuleEditor3D::~ModuleEditor3D()
{
	delete root;	
	root = nullptr;	

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

	App->fileSystem->LoadAsset("Assets/bakerHouse/BakerHouse.fbx");
	

	return ret;
}

update_status ModuleEditor3D::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleEditor3D::Update(float dt)
{
	if (root)
		root->Update(dt);

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

void ModuleEditor3D::DrawAllMeshes()
{
	for (int i = 0; i < drawMeshes.size(); i++)
	{
		drawMeshes[i].Draw();
	}

	drawMeshes.clear();
}

void ModuleEditor3D::AddMeshToDraw(C_Mesh* mesh,C_Material* material, mat4x4 gTransform, MeshDrawMode drawMode, NormalDrawMode normalMode)
{
	drawMeshes.push_back(RenderMesh(mesh,material, gTransform, drawMode, normalMode));
}

bool ModuleEditor3D::SetSelectedGameObject(GameObject* selected)
{
	bool ret = false;
	if (selected != nullptr) 
	{ 
		selectedGameObj = selected;
		ret = true;
	}	
	return ret;
}

GameObject* ModuleEditor3D::GetSelectedGameObject()
{
	return selectedGameObj;
}

bool ModuleEditor3D::UpdateInfoOnSelectedGameObject()
{
	bool ret = true;
	if (selectedGameObj != nullptr) 
	{ 
		ImGui::Text("My name is %s", selectedGameObj->GetName().c_str());
		selectedGameObj->DrawOnEditorAllComponents();
	}
	return ret;
}


