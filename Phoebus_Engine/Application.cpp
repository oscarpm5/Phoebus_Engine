#include "Application.h"

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	renderer3D = new ModuleRenderer3D();
	renderer2D = new ModuleRenderer2D();
	camera = new ModuleCamera3D();
	editor3d = new ModuleEditor3D();
	//fileSystem = new ModuleFileSystem();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	//AddModule(fileSystem);
	AddModule(input);
	AddModule(editor3d);
	// Renderer last!
	AddModule(renderer3D);
	AddModule(renderer2D);
}

Application::~Application()
{
	for (int i = list_modules.size() - 1; i >= 0; i--)
	{
		if (list_modules[i] != nullptr)
		{
			delete(list_modules[i]);
			list_modules[i] = nullptr;
		}
	}
	list_modules.clear();

}

bool Application::Init()
{
	bool ret = true;

	App = this;

	// Call Init() in all modules
	for (int i = 0; i < list_modules.size(); i++)
	{
		if (list_modules[i] != nullptr && ret == true)
			ret = list_modules[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("-------------- Application Start --------------");
	for (int i = 0; i < list_modules.size(); i++)
	{
		if (list_modules[i] != nullptr && ret == true)
			ret = list_modules[i]->Start();
	}

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;

	//FPS buffer for graph display
	fpsBuffer.push_back(1 / dt);
	while (fpsBuffer.size() >= MAXFPSDISPLAY)
	{
		fpsBuffer.erase(fpsBuffer.begin());
	}
	//Milliseconds buffer for graph display
	millisecondsBuffer.push_back((float)ms_timer.Read());
	while (millisecondsBuffer.size() >= MAXFPSDISPLAY)
	{
		millisecondsBuffer.erase(millisecondsBuffer.begin());
	}




	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (int i = 0; i < list_modules.size(); i++)
	{
		if (list_modules[i] != nullptr && ret == true)
			ret = list_modules[i]->PreUpdate(dt);
	}

	for (int i = 0; i < list_modules.size(); i++)
	{
		if (list_modules[i] != nullptr && ret == true)
			ret = list_modules[i]->Update(dt);
	}

	for (int i = 0; i < list_modules.size(); i++)
	{
		if (list_modules[i] != nullptr && ret == true)
			ret = list_modules[i]->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (int i = list_modules.size()-1; i >=0; i--)
	{
		if (list_modules[i] != nullptr && ret == true)
			ret = list_modules[i]->CleanUp();
	}

	App = nullptr;
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

Application* App = nullptr;
