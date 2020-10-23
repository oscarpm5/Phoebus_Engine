#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "Console.h"
#include "imgui/imgui.h"
#include "Primitive.h"
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
	bool showQuitPopup();
	bool showQuit = false;
	bool quitAlreadyOpened = false;
	
private:

		
	bool CreateBasicForm(PrimitiveTypes type, float ar1 = 0, float ar2 = 0, float ar3 = 0, float ar4 = 0); //arX meaning depends on type. This saves massive amounts of code. Deal with it.
	float ar1 = 0; float ar2 = 0; float ar3 = 0; float ar4 = 0;
	void CreateMeshfromPrimAndSendToScene(std::vector<float> vertices, std::vector<unsigned int> indices);

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
