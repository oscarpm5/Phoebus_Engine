#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "Application.h"

GameObject::GameObject(GameObject* parent, std::string name, mat4x4 transform) :name(name), transform(nullptr)
{
	this->parent = parent;
	isActive = true;

	if (parent)
	{
		parent->children.push_back(this);
	}

	//TODO add transform component here
	this->transform = new C_Transform(this, transform);
	components.push_back(this->transform);

}

void GameObject::Update(float dt)
{
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
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}

	children.clear();

	if (transform != nullptr)//This wont be needed as transform is deleted from the component vector
		delete transform;

	transform = nullptr;

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
		App->editor3d->AddMeshToDraw(meshes[i], GetComponent<C_Material>(), transform->GetGlobalTransform(), MeshDrawMode::DRAW_MODE_BOTH, NormalDrawMode::NORMAL_MODE_NONE);

	}

}
