#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "Application.h"
#include "imgui/imgui.h"
//#include "glmath.h"//new, we should include glMath here but C_Transform already has it and we cannot remove it from there


int GameObject::numberOfObjects = 0;


GameObject::GameObject(GameObject* parent, std::string name, mat4x4 transform) :name(name), transform(nullptr), focused(false)
{
	App->editor3d->AddObjName(this->name);

	this->parent = parent;
	isActive = true;

	if (parent)
	{
		parent->children.push_back(this);
	}

	//TODO add transform component here
	this->transform = new C_Transform(this, transform);
	components.push_back(this->transform);

	numberOfObjects++;

}

void GameObject::Update(float dt)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->toDelete)
		{
			delete components[i];
			components[i] = nullptr;
			components.erase(components.begin() + i);
			i--;
		}
	}

	if (isActive)
	{

		for (int i = 0; i < components.size(); i++)
		{
			if (components[i]->IsActive())
			{
				components[i]->Update(dt);
			}
		}



		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->isActive)
			{
				children[i]->Update(dt);
			}
		}

		DrawGameObject();
	}
}

GameObject::~GameObject()
{
	App->editor3d->RemoveName(name);

	numberOfObjects--;

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] != transform)
		{
			delete components[i];
			components[i] = nullptr;
		}
	}
	components.clear();

	if (transform != nullptr)//This wont be needed as transform is deleted from the component vector
		delete transform;

	transform = nullptr;

	while (!children.empty())
	{
		GameObject* child = children.back();
		children.pop_back();
		delete child;
	}
	children.clear();

	//This may cause some sort of cyclic behaviour??? TODO investigate if vector.erase just removes the element or also calls de destructor
	RemoveMyselfFromParent();

	if (App)
		App->editor3d->RemoveGameObjFromSelected(this);

}

void GameObject::RemoveChildren(GameObject* toRemove)
{
	std::vector<GameObject*>::iterator iterator = children.begin();

	for (iterator; iterator != children.end(); iterator++)
	{
		if (*iterator == toRemove)
		{

			children.erase(iterator);

			break;
		}

	}

}

void GameObject::RemoveMyselfFromParent()
{
	if (parent)
	{
		parent->RemoveChildren(this);
	}
}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* ret = nullptr;
	//TODO add diferent components here
	switch (type)
	{
	case ComponentType::MESH:
		ret = new C_Mesh(this);
		break;
	case ComponentType::MATERIAL:
		//only one instance of material for a certain gameObj
		if (GetComponent<C_Material>() == nullptr)
			ret = new C_Material(this);

		break;
	}


	if (ret)
	{
		components.push_back(ret);
	}

	return ret;
}

std::string GameObject::GetName()
{
	return this->name;
}

//recursive function that returns false if some game object from which this inherits is not active 
bool GameObject::IsParentActive()
{
	if (!isActive)
		return false;

	if (parent)
	{
		return parent->IsParentActive();
	}

	return isActive;
}

void GameObject::DrawOnEditorAllComponents()
{
	if (ImGui::BeginChild("Object name window", ImVec2(0.0f, 30.0f)))
	{
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Checkbox(" ", &isActive);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Active");
			ImGui::EndTooltip();
		}


		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX()-10);
		char* auxName = (char*)name.c_str();
		std::string oldName = name;
		if (ImGui::InputText("Object Name", auxName, 100))
		{
			name = auxName;
			name.shrink_to_fit();
			if (name.empty())
			{
				name = "Untitled";
			}
			App->editor3d->ChangeObjName(oldName, name);
		}
	}
		ImGui::EndChild();
	//ImGui::Text("My name is %s", name.c_str());
	ImGui::Separator();
	ImGui::Spacing();


	for (int i = 0; i < this->components.size(); i++)
	{
		components[i]->OnEditor();
	}
}

void GameObject::DrawGameObject()
{
	/*if (C_Mesh * m = GetComponent<C_Mesh>())
	{
		App->editor3d->AddMeshToDraw(m, transform->GetGlobalTransform(), MeshDrawMode::DRAW_MODE_BOTH, NormalDrawMode::NORMAL_MODE_NONE);
	}*/
	std::vector<C_Mesh*>meshes = GetComponents<C_Mesh>();

	for (int i = 0; i < meshes.size(); i++)
	{
		App->editor3d->AddMeshToDraw(meshes[i], GetComponent<C_Material>(), transform->GetGlobalTransform()); //TODO drawMode& normal Mode not needed anymore

	}

}
