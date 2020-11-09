#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "Application.h"
#include "imgui/imgui.h"

#include "MathGeoLib/include/MathGeoLib.h"
//#include "glmath.h"//new, we should include glMath here but C_Transform already has it and we cannot remove it from there


int GameObject::numberOfObjects = 0;


GameObject::GameObject(GameObject* parent, std::string name, float4x4 transform, bool showAABB) :name(name), transform(nullptr), focused(false), displayBoundingBox(showAABB)
{
	App->editor3d->AddObjName(this->name);
	this->parent = parent;
	isActive = true;
	bbHasToUpdate = true;
	if (parent)
	{
		parent->children.push_back(this);
	}
	this->transform = new C_Transform(this, transform);
	components.push_back(this->transform);

	numberOfObjects++;

	globalAABB.SetNegativeInfinity();
	globalOBB.SetNegativeInfinity();
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
		if (bbHasToUpdate)
		{
			UpdateBoundingBox();
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
	case ComponentType::CAMERA:
		//only one instance of camera for a certain gameObj
		if (GetComponent<C_Camera>() == nullptr)
			ret = new C_Camera(this);
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

void GameObject::UpdateChildTransforms()
{
	GetComponent<C_Transform>()->UpdateGlobalMat();
	bbHasToUpdate = true;
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->UpdateChildTransforms();
	}
}

void GameObject::UpdateBoundingBox()
{
	C_Mesh* mesh = GetComponent <C_Mesh>(); //TODO for now we will only make it that the first mesh draws the bounding box, add support for multiple boundingboxes (if more than 1 mesh)

	if (mesh != nullptr)
	{
		globalOBB = mesh->GetAABB();
		globalOBB.Transform(GetComponent< C_Transform>()->GetGlobalTransform());//TODO we need to fork with float4x4 and not mat4x4

		globalAABB.SetNegativeInfinity();
		globalAABB.Enclose(globalOBB);
	}
	else
	{
		globalAABB.SetNegativeInfinity();
		globalAABB.SetFromCenterAndSize((float3)transform->GetGlobalPosition(), float3(1, 1, 1));//todo we need to return float3 not vec3
		globalOBB = globalAABB;
	}

	bbHasToUpdate = false;
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
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);
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

	C_Material* mat = GetComponent<C_Material>();
	if (mat != nullptr && !mat->IsActive())
	{
		mat = nullptr;
	}

	for (int i = 0; i < meshes.size(); i++)
	{
		if (meshes[i]->IsActive())
		{
			App->renderer3D->AddMeshToDraw(meshes[i], mat, transform->GetGlobalTransform());
		}

	}

	if (displayBoundingBox&&focused)
	{
		App->renderer3D->AddAABBToDraw(globalAABB);
	}

}
