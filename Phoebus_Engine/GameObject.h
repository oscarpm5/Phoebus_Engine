#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>
#include <string>
//#include "glmath.h"

#include "MathGeoLib/include/MathGeoLib.h"

class Component;
class C_Transform;
enum class ComponentType;

class GameObject
{
public:

	GameObject(GameObject* parent, std::string name, float4x4 transform, bool showAABB = true);

	void Update(float dt);

	~GameObject();

	void RemoveChildren(GameObject* toRemove);

	void RemoveMyselfFromParent();

	Component* CreateComponent(ComponentType type);

	std::string GetName();

	bool IsParentActive();

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


	void UpdateChildTransforms();
	void UpdateBoundingBox();


	void DrawOnEditorAllComponents();
private:
	void DrawGameObject();
private:
	std::vector<Component*> components;
	C_Transform* transform;



	std::string name;

	AABB globalAABB;
	OBB globalOBB;

public:
	bool isActive;
	bool focused;
	bool displayBoundingBox;
	bool bbHasToUpdate;
	std::vector<GameObject*> children; //we need them public for hierarchy

	static int numberOfObjects;
};



#endif // !__GAME_OBJECT_H__
