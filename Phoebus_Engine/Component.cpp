#include "Component.h"



Component::Component(ComponentType type, GameObject* owner) :type(type)
{
	this->owner = owner;
	active = true;
<<<<<<< HEAD
=======
	toDelete = false;
>>>>>>> Development
}

Component::~Component()
{
}

bool Component::Update(float dt)
{
	return true;
}

void Component::OnEditor()
{
}

ComponentType Component::GetType() const
{
	return type;
}

void Component::SetActive(bool active)
{
	this->active = active;
}

bool Component::IsActive() const
{
	return active;
}
