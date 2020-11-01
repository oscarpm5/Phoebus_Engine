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

	virtual void OnEditor();//component drawing itself on the window
	
	ComponentType GetType()const;

	void SetActive(bool active);
	bool IsActive()const;

public:
	GameObject* owner;

<<<<<<< HEAD
=======
	bool toDelete;
>>>>>>> Development
private:
	ComponentType type;
protected:
	bool active;
};


#endif // !__COMPONENT_H__
