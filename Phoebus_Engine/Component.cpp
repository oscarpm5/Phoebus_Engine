#include "Component.h"



Component::Component(ComponentType type, GameObject* owner) :type(type)
{
	this->owner = owner;
	active = true;
	toDelete = false;
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
	if (type != ComponentType::TRANSFORM)
		this->active = active;
}

bool Component::IsActive() const
{
	return active;
}
