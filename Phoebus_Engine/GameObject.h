#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>
#include <string>

class Component;
enum class ComponentType;


class GameObject
{
public:

	GameObject(GameObject* parent,std::string name);

	void Update(float dt);

	~GameObject();

	void RemoveChildren(GameObject* toRemove);

	Component* CreateComponent(ComponentType type);

	std::string GetName();

	bool IsParentActive(); //will need this when the hierarchy is implemented TODO


private:

	GameObject* parent;
	std::vector<Component*> components; 
	std::string name;

public:
	bool isActive;
	std::vector<GameObject*> children; //we need them public for hierarchy


};



#endif // !__GAME_OBJECT_H__
