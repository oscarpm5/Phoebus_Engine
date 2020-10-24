#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

	enum class ComponentType
	{
		TRANSFORM,
		MESH,
		MATERIAL
	};

class Component
{
public:

	Component(ComponentType type,GameObject* owner);
	virtual ~Component();
	virtual bool Update(float dt);
	virtual void Enable();
	virtual void Disable();
	
	
	ComponentType GetType()const;

	void SetActive(bool active);
	bool IsActive()const;

public:
	GameObject* owner;
private:
	ComponentType type;
	bool active;
};


#endif // !__COMPONENT_H__
