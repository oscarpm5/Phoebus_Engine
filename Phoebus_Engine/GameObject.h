#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>
#include <string>
<<<<<<< HEAD
#include "glmath.h"
=======
//#include "glmath.h"
>>>>>>> Development


class Component;
class C_Transform;
enum class ComponentType;

<<<<<<< HEAD
=======
class mat4x4;
>>>>>>> Development

class GameObject
{
public:

	GameObject(GameObject* parent, std::string name, mat4x4 transform);

	void Update(float dt);

	~GameObject();

	void RemoveChildren(GameObject* toRemove);

	void RemoveMyselfFromParent();

	Component* CreateComponent(ComponentType type);

	std::string GetName();

	bool IsParentActive(); //will need this when the hierarchy is implemented TODO

	GameObject* parent;

	template<typename Comp>
	Comp* GetComponent()
	{
		for (int i = 0; i < components.size(); i++) { Comp* c = dynamic_cast<Comp*>(components[i]);    if (c != nullptr)    return    c; }
		return nullptr;
	}

	template<typename Comp>
	std::vector<Comp*> GetComponents()
	{
		std::vector<Comp*> compVec;
		for (int i = 0; i < components.size(); i++) { Comp* c = dynamic_cast<Comp*>(components[i]);    if (c != nullptr)   compVec.push_back(c); }
		return compVec;
	}


<<<<<<< HEAD
=======
	void UpdateChildTransforms();


>>>>>>> Development

	void DrawOnEditorAllComponents();
private:
	void DrawGameObject();
private:
	std::vector<Component*> components;
	C_Transform* transform;



	std::string name;

public:
	bool isActive;
	bool focused;
	std::vector<GameObject*> children; //we need them public for hierarchy

	static int numberOfObjects;
};



#endif // !__GAME_OBJECT_H__
