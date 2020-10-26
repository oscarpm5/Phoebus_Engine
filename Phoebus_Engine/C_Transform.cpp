#include "C_Transform.h"
#include "GameObject.h"
#include "glmath.h"

C_Transform::C_Transform(GameObject* owner, mat4x4 lTransform):Component(ComponentType::TRANSFORM,owner),
lTransformMat(lTransform),gTransformMat(IdentityMatrix)
{
	
	UpdateGlobalMat();
}

C_Transform::~C_Transform()
{
}

mat4x4 C_Transform::GetGlobalTransform() const
{
	return gTransformMat;
}

mat4x4 C_Transform::GetLocalTransform() const
{
	return lTransformMat;
}

void C_Transform::UpdateGlobalMat()
{
	if (owner->parent!=nullptr)
	{
		gTransformMat = owner->parent->GetComponent<C_Transform>()->GetGlobalTransform()*lTransformMat;
	}
	else
		gTransformMat = lTransformMat;
}


