#include "Application.h"
#include "ModuleEditor3D.h"
#include "Importer.h"
#include "GameObject.h"
#include "MathGeoLib/include/MathGeoLib.h"


ModuleEditor3D::ModuleEditor3D(bool start_enabled) :Module(start_enabled)
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
	
	mat4x4 transform;
	root = new GameObject(nullptr, "SceneRoot", transform);

	test1 = new GameObject(root, "Test1", transform);
	test2 = new GameObject(root, "Test2", transform);
	test2->isActive = false;
	test3 = new GameObject(root, "Test3", transform);
	transform.scale(5, 15, 25);
	transform.translate(10, 20, 30);
	test4 = new GameObject(test3, "Test4", transform);
	test4->isActive = false;
	test5 = new GameObject(test3, "Test5", transform);
	test6 = new GameObject(test4, "Test6", transform);
	test7 = new GameObject(test6, "Test7", transform);


	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	App->fileSystem->LoadAsset("Assets/bakerHouse/BakerHouse.fbx");


	return ret;
}

update_status ModuleEditor3D::PreUpdate(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		if (selectedGameObjs.size() > 0)
		{
			GameObject* aux = selectedGameObjs.back();
			selectedGameObjs.pop_back();
			delete aux;
		}
	}
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
	selectedGameObjs.clear();

	if (root)
		delete root;

	root = nullptr;

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

void ModuleEditor3D::AddMeshToDraw(C_Mesh* mesh, C_Material* material, mat4x4 gTransform, MeshDrawMode drawMode, NormalDrawMode normalMode)
{
	drawMeshes.push_back(RenderMesh(mesh, material, gTransform, drawMode, normalMode));
}

bool ModuleEditor3D::SetSelectedGameObject(GameObject* selected, bool addMode)
{
	bool ret = false;
	if (selected != root)
	{

		//TODO Add mode not working properly
		if (!addMode)
			selectedGameObjs.clear();

		if (selected != nullptr)
		{
			RemoveGameObjFromSelected(selected);

			selectedGameObjs.push_back(selected);
			ret = true;
		}
	}
	return ret;
}

//if the item exists in the Game Object vector, this method erases it from the list
bool ModuleEditor3D::RemoveGameObjFromSelected(GameObject* toRemove)
{
	bool ret = false;
	for (int i = 0; i < selectedGameObjs.size(); i++)
	{
		if (selectedGameObjs[i] == toRemove)
		{
			selectedGameObjs[i] = nullptr;
			selectedGameObjs.erase(selectedGameObjs.begin() + i);
			ret = true;
			break;
		}
	}
	return ret;
}

std::vector<GameObject*> ModuleEditor3D::GetSelectedGameObject()
{
	return selectedGameObjs;
}

bool ModuleEditor3D::UpdateInfoOnSelectedGameObject()
{
	bool ret = true;
	if (selectedGameObjs.size() > 0)
	{
		ImGui::Text("My name is %s", selectedGameObjs.back()->GetName().c_str());
		ImGui::Separator();
		ImGui::Spacing();
		selectedGameObjs.back()->DrawOnEditorAllComponents();
	}
	return ret;
}


