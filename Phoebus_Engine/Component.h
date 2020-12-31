#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

	enum class ComponentType
	{
		TRANSFORM,
		MESH,
		MATERIAL,
		CAMERA,
		AUDIO_SOURCE,
		AUDIO_LISTENER
	};

class Component
{
public:

	Component(ComponentType type,GameObject* owner,unsigned int ID=0);
	virtual ~Component();
	virtual bool Update(float dt);
	virtual bool GameUpdate(float dt);
	virtual bool GameInit();

	virtual void OnEditor();//component drawing itself on the window
	
	ComponentType GetType()const;

	void SetActive(bool active);
	bool IsActive()const;

	virtual void SetNewResource(unsigned int resourceID);
	//gets resource ID for saving: WARNING use the dedicated component methods to get the ID for everythign else as this function doesn't reset the ID
	virtual unsigned int GetResourceID();

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
