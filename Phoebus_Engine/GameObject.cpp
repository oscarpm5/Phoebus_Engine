#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"

GameObject::GameObject(GameObject* parent, std::string name, mat4x4 transform) :name(name),transform(nullptr)
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
	if (parent)
	{
		parent->RemoveChildren(this);
	}

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

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* ret = nullptr;
	//TODO add diferent components here
	switch (type)
	{
	case ComponentType::MESH:

		break;
	case ComponentType::MATERIAL:

		break;
	}


	if (ret)
	{
		components.push_back(ret);
	}

	return ret;
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
