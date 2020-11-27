#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

	enum class ComponentType
	{
		TRANSFORM,
		MESH,
		MATERIAL,
		CAMERA
	};

class Component
{
public:

	Component(ComponentType type,GameObject* owner,unsigned int ID=0);
	virtual ~Component();
	virtual bool Update(float dt);
	virtual bool GameUpdate(float dt);

	virtual void OnEditor();//component drawing itself on the window
	
	ComponentType GetType()const;

	void SetActive(bool active);
	bool IsActive()const;

	virtual void SetNewResource(unsigned int resourceID);

public:
	GameObject* owner;
	bool toDelete;
	unsigned int ID;

private:
	ComponentType type;
protected:
	bool active;
};


#endif // !__COMPONENT_H__
