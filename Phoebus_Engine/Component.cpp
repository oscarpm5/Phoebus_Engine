#include "Component.h"
#include "Application.h"
#include "ModuleRenderer3D.h"



Component::Component(ComponentType type, GameObject* owner) :type(type)
{
	this->owner = owner;
	active = true;
	toDelete = false;
	this->ID = -1;
}

Component::~Component()
{
}

bool Component::Update(float dt)
{
	if (ID == -1) {
		this->ID = App->renderer3D->seed.Int(); //TODO ADRI: absolute garbage, fix this
	}
	return true;
}

bool Component::GameUpdate(float dt)
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
