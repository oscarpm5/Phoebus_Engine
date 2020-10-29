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

	maxSceneDrawMode = MeshDrawMode::DRAW_MODE_FILL;

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
	objNames.clear();
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
		drawMeshes[i].Draw(maxSceneDrawMode);
	}

	drawMeshes.clear();
	drawMeshes.shrink_to_fit();
}

void ModuleEditor3D::AddMeshToDraw(C_Mesh* mesh, C_Material* material, mat4x4 gTransform)
{
	drawMeshes.push_back(RenderMesh(mesh, material, gTransform));
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
		selectedGameObjs.back()->DrawOnEditorAllComponents();
	}
	return ret;
}

void ModuleEditor3D::AddObjName(std::string& name)
{

	int index = DoesNameExist(name);
	if (index == -1)
	{
		objNames.push_back(name);
	}
	else
	{
		MakeNameUnique(name);
		objNames.push_back(name);
	}
}

void ModuleEditor3D::RemoveName(std::string name)
{
	int index = DoesNameExist(name);
	if (index != -1)
	{
		objNames.erase(objNames.begin() + index);
	}
}

void ModuleEditor3D::ChangeObjName(std::string oldName, std::string& newName)
{
	int index = DoesNameExist(oldName);
	if(index==-1)//if oldName doesn't exist push it to the vector
	{
		AddObjName(newName);
	}
	else
	{
		RemoveName(oldName);
		AddObjName(newName);
	}
}

int ModuleEditor3D::DoesNameExist(std::string name)
{
	int ret = -1;
	for (int i = 0; i < objNames.size(); i++)
	{
		if (objNames[i] == name)
		{
			ret = i;
			break;
		}
	}
	return ret;
}

//makes a unique name for an object adding '.' + 3 digits when necessary
void ModuleEditor3D::MakeNameUnique(std::string& name)
{
	std::string currSuffix;
	std::string baseName;
	std::string auxName = name;

	int dotPos = auxName.find_last_of(".");

	if (dotPos >= 0 && dotPos < auxName.size())//if dot exists
	{
		currSuffix = auxName.substr(dotPos + 1);

		int letterPos = currSuffix.find_first_not_of("0123456789");

		if (letterPos < currSuffix.size())//if there are letters after the dot default first .num
		{
			auxName += ".001";
		}
		else//if after the dot there are only numbers
		{
			baseName = auxName.substr(0, dotPos);

			//1.find the current number
			int suffixNum = atoi(currSuffix.c_str());

			//2.take the number and add it one
			suffixNum++;

			if (suffixNum < 1000)
			{

				char newNums[16];
				sprintf_s(newNums, 16, ".%03i", suffixNum);

				auxName = baseName + newNums;
			}
			else
			{
				auxName += ".001";//default first .num
			}



			//3.find if it exists
			//repeat from 2 until no num exists

		}
	}
	else //default first .num
	{
		auxName += ".001";
	}


	int where = DoesNameExist(auxName);
	if (where == -1) //if name isn't found in the lsit assign it
	{
		name = auxName;
	}
	else //else repeat the process
	{
		MakeNameUnique(auxName);
		name = auxName;
	}

	currSuffix.clear();
	baseName.clear();
	auxName.clear();

}


