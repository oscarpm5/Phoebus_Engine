#pragma once
#include "Module.h"
#include "Globals.h"

#include "Mesh.h"
#include "texture.h"
#include "RenderMesh.h"

#define PRIMITIVES_IN_SCENE 10

class GameObject;

class ModuleEditor3D : public Module
{
public:
	ModuleEditor3D(bool start_enabled = true);
	~ModuleEditor3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void DrawAllMeshes();
	void AddMeshToDraw(C_Mesh* mesh, C_Material* material,mat4x4 gTransform);

	bool SetSelectedGameObject(GameObject* selected, bool addMode = false);
	bool RemoveGameObjFromSelected(GameObject* toRemove);

	std::vector<GameObject*> GetSelectedGameObject();
	bool UpdateInfoOnSelectedGameObject();

	void AddObjName(std::string& name); //add a name to the name vector
	void RemoveName(std::string name);//removes a name from the name vector
	void ChangeObjName(std::string oldName, std::string& newName); //modifies a name from the vector
private:
	int DoesNameExist(std::string name); //returns an index of where the current name is, defaults in -1
	void MakeNameUnique(std::string& name); //if this object is going to have the same name as another, make it unique
public:
	std::vector<Mesh> meshes;
	std::vector<NewTexture*> textures;
	GameObject* root;

	std::vector<RenderMesh> drawMeshes;
	GameObject* test1; GameObject* test2; GameObject* test3; GameObject* test4; GameObject* test5; GameObject* test6; GameObject* test7;
	

	std::vector<GameObject*> selectedGameObjs;


	MeshDrawMode maxSceneDrawMode;

	bool mouseActive;//checks whether the 3d scene can be interacted with the mouse (when hovering or mantaining click after hover)

private:
	std::vector<std::string> objNames;
};