<<<<<<< HEAD
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

=======
#ifndef __MODULE_RENDER_2D__
#define __MODULE_RENDER_2D__


#include "Module.h"
//#include "Globals.h"
//#include "glmath.h"
//#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "imgui/imgui.h" //ImVec2 cannot be forward declared
//#include "Primitive.h" //wtf
//#include <iostream> 
#include <vector> 
#include <string>

class Cnsl;
enum class PrimitiveTypes;
>>>>>>> Development


class ModuleRenderer2D : public Module
{
public:
	ModuleRenderer2D(bool start_enabled = true);
	~ModuleRenderer2D();

	bool Init();
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();
<<<<<<< HEAD
	void ShowExampleAppConsole(bool* p_open);
=======
>>>>>>> Development
	void CreateDockingSpace();
	void OnResize(int width, int height);
	void Draw();

<<<<<<< HEAD

public:
	bool showDemoWindow = false;
	bool showConsoleWindow = true;
	bool showHierarchy = true;
	bool showInspector = true;

	bool Show3DWindow();
	bool show3DWindow = true;

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
=======
	void ShowExampleAppConsole(bool* p_open);
	bool Show3DWindow();
	bool showAboutWindow();
	bool showConfigFunc();
	bool showQuitPopup();



>>>>>>> Development
	
private:

		
	bool CreateBasicForm(PrimitiveTypes type, float ar1 = 0, float ar2 = 0, float ar3 = 0, float ar4 = 0, float ar = 5); //arX meaning depends on type. This saves massive amounts of code. Deal with it.
<<<<<<< HEAD
	float ar1 = 0; float ar2 = 0; float ar3 = 0; float ar4 = 0; float ar5 = 0;
	void CreateMeshfromPrimAndSendToScene(std::vector<float> vertices, std::vector<unsigned int> indices,std::string name="");

	//fps related business

	//ImVector<float> fps_log[60];
	std::vector<float> fps_log;
	int maxFPShown = 60;

	//Resize badness
	void OpenGLOnResize(int w, int h);

	//flags
	bool resizable = true; bool borderless = false; bool fullscreen = false; bool fullDesktop = false; //TODO: move to window?

	bool Vsync = VSYNC;

	ImVec2 imgSize;

}; 
=======
	void CreateMeshfromPrimAndSendToScene(std::vector<float> vertices, std::vector<unsigned int> indices,std::string name="");
	void OpenGLOnResize(int w, int h);

public:
	bool showDemoWindow;
	bool showConsoleWindow;
	bool showHierarchy;
	bool showInspector;
	bool show3DWindow;
	bool showAboutWindowbool;
	bool showConfig;
	bool showQuit;
	bool quitAlreadyOpened;

	Cnsl* console;

private:
	//arguments for the primitive functions (TODO this could be changed in the future, find a way to have multiple args)
	float ar1; 
	float ar2; 
	float ar3; 
	float ar4; 
	float ar5;

	//fps related business
	//ImVector<float> fps_log[60];
	std::vector<float> fps_log;
	int maxFPShown;
	
	//flags
	bool Vsync;
	bool resizable; 
	bool borderless; 
	bool fullscreen; 
	bool fullDesktop; //TODO: move to window?
	ImVec2 imgSize;

	//this is for displaying our portraits in the About
private:
	unsigned int AdriID;
	unsigned int OscarID;
	unsigned int PhoebusIcon;
	



}; 
#endif // !__MODULE_RENDER_2D__
>>>>>>> Development
