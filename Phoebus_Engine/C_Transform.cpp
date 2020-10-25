#include "C_Transform.h"
#include "GameObject.h"
#include "glmath.h"

C_Transform::C_Transform(GameObject* owner, mat4x4 lTransform):Component(ComponentType::TRANSFORM,owner),
lTransformMat(lTransform),gTransformMat()
{
	
	UpdateGlobalMat();
}

C_Transform::~C_Transform()
{
}

void C_Transform::UpdateGlobalMat()
{
	if (owner->parent)
	{
		gTransformMat = lTransformMat * owner->parent->GetComponent<C_Transform>()->gTransformMat;
	}
	else
		gTransformMat = lTransformMat;
}


