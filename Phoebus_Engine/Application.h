#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleRenderer2D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor3D.h"
#include "ModuleFileSystem.h"


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleRenderer2D* renderer2D;
	ModuleCamera3D* camera;
	ModuleEditor3D* editor3d;
	//ModuleFileSystem* fileSystem;	linea 102 module input Importer:: load fbx -> llamaar el filesysatem y pasarle el path (dropfiledyr) para probarlo rapido

	bool debug;
	bool renderPrimitives;
	std::vector<float> fpsBuffer;
	std::vector<float> millisecondsBuffer;

private:

	Timer	ms_timer;
	float	dt;
	
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};