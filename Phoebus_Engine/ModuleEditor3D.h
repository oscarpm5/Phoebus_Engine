#pragma once
#include "Module.h"
#include "Globals.h"

#include "Mesh.h"
#include "texture.h"

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


public:
	std::vector<Mesh> meshes;
	std::vector<NewTexture*> textures;
	GameObject* root;
};