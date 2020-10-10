#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "Console.h"
#include "imgui/imgui.h"

#include <iostream> 
#include <vector> 



class ModuleRenderer2D : public Module
{
public:
	ModuleRenderer2D(bool start_enabled = true);
	~ModuleRenderer2D();

	bool Init();
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();
	void ShowExampleAppConsole(bool* p_open);
	void CreateDockingSpace();
	void OnResize(int width, int height);
	void Draw();


public:
	bool showDemoWindow = false;
	bool showConsoleWindow = false;

	bool Show3DWindow();
	bool show3DWindow = false;

	Cnsl* console = nullptr;

	bool showAboutWindow();
	bool showAboutWindowbool = false;
	bool showLibs = false;
	bool showLibsFunc();
	bool showConfig = false;
	bool showConfigFunc();

	
private:

	//math checks

		//rand
	LCG seed; int randomRad1; int randomRad2;
		//spheres
	bool showSphereWindow = false;		bool sphereCol = false;
		//cylinders
	bool showCylWindow = false;			bool cylCol = false;
		//triangles
	bool showTriWindow = false;			bool triCol = false;
		//AABB
	bool showAABBWindow = false;		bool AABBCol = false;

	//fps related business

	//ImVector<float> fps_log[60];
	std::vector<float> fps_log;
	int maxFPShown = 60;

	//Resize badness
	void OpenGLOnResize(int w, int h);

	//flags
	bool resizable = true; bool borderless = false; bool fullscreen = false; bool fullDesktop = false; //TODO: move to window?

	ImVec2 imgSize;

}; 
