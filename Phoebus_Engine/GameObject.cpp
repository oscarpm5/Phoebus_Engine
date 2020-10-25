#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(GameObject* parent, std::string name):name(name)
{
	this->parent = parent;
	isActive = true;

	if (parent)
	{
		parent->children.push_back(this);
	}

	//TODO add transform component here
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
		children[i] = nullptr;
	}
	children.clear();

	//This may cause some sort of cyclic behaviour??? TODO investigate if vector.erase just removes the element or also calls de destructor
	if (parent)
	{
		parent->RemoveChildren(this);
	}
}

void GameObject::RemoveChildren(GameObject* toRemove)
{
	std::vector<GameObject*>::iterator iterator = children.begin();

	for(iterator; iterator!= children.end(); iterator++)
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
	case ComponentType::TRANSFORM:
		//if it hasn't got any component of type T, create a new one and assign it to ret
		break;
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
