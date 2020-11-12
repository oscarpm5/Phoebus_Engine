#include "Application.h"
#include "ModuleEditor3D.h"
#include "Importer.h"
#include "GameObject.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Mesh.h"


ModuleEditor3D::ModuleEditor3D(bool start_enabled) :Module(start_enabled)
{
	maxSceneDrawMode = MeshDrawMode::DRAW_MODE_BOTH;
	mouseActive = true;
	root = nullptr;
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

	root = new GameObject(nullptr, "SceneRoot", float4x4::identity,false);
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(vec3(0, 0, 0));

	//TODO this is a c camera game object for testing purposes, delete when finished
	GameObject* provisionalCam= new GameObject(root, "camera", float4x4::identity, true);
	provisionalCam->CreateComponent(ComponentType::CAMERA);



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
	objNames.clear();
	
	selectedGameObjs.clear();

	if (root)
		delete root;

	root = nullptr;

	return ret;
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


