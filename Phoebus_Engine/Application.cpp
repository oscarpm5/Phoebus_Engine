#include "Application.h"

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	renderer3D = new ModuleRenderer3D();
	renderer2D = new ModuleRenderer2D();
	camera = new ModuleCamera3D();


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);

	AddModule(input);
	
	// Renderer last!
	AddModule(renderer3D);
	AddModule(renderer2D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while (item != NULL)
	{
		delete item->data;
		item = item->prev;
	}

}

bool Application::Init()
{
	bool ret = true;

	App = this;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while (item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("-------------- Application Start --------------");
	item = list_modules.getFirst();

	while (item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	
	//FPS buffer for graph display
	fpsBuffer.push_back(1/dt);
	while (fpsBuffer.size()>=MAXFPSDISPLAY)
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

	p2List_item<Module*>* item = list_modules.getFirst();

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	App = nullptr;
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

Application* App = nullptr;
