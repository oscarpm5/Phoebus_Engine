
//The order is relevant from here...
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL.h"

//...to here
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer2D.h"
#include "ModuleRenderer3D.h" //we need the projection matrix

//We're using pretty much all of it for cheks, so we're just including the whole thing
#include "MathGeoLib/include/MathGeoLib.h"

#include "Console.h"
#include "MathChecks.h"


ModuleRenderer2D::ModuleRenderer2D(bool start_enabled)
{
	console = new Cnsl();
}

ModuleRenderer2D::~ModuleRenderer2D()
{
	if (console != nullptr)
	{
		delete(console);
		console = nullptr;
	}

}

bool ModuleRenderer2D::Init()
{
	bool ret = true;



	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init("#version 140"); //TODO: this is hardcoded. Deal with it.

	//Start number generator seed
	seed = LCG::LCG();

	//fps goodness
	fps_log.resize(maxFPShown);

	return ret;
}

update_status ModuleRenderer2D::PreUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;

	//Fill fps_log with new info of past frame
	CheckFPS();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// Our state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	CreateDockingSpace();

	//Main Menu bar test
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("Menu", true))
		{
			////Sample Dropdown menu with options
			//if (ImGui::BeginMenu("Export", true))
			//{
			//	if (ImGui::MenuItem("As PNG")) {}
			//	if (ImGui::MenuItem("As JPG")) {}
			//	if (ImGui::MenuItem("As JPEG")) {}


			//	ImGui::EndMenu();
			//}
			////End of sample

			if (ImGui::MenuItem("Exit", "Esc")) { status = UPDATE_STOP; }
			if (ImGui::MenuItem("About", "...")) {
				ImGui::SetNextWindowSize(ImVec2(435, 800));
				showAboutWindowbool = true;
			}
			if (ImGui::MenuItem("Config")) {
				//ImGui::SetNextWindowSize(ImVec2(435, 800));
				showConfig = true;
			}


			ImGui::EndMenu();
		}

		//Math checks window
		if (ImGui::BeginMenu("Math checks", true))
		{
			//showing some random number to prove LGC works
			int randomNum = seed.Int(10, 99);
			ImGui::Text("Random Num : %i", randomNum);


			//Intersections between

			//Spheres
			if (ImGui::MenuItem("Sphere collision")) {
				sphereCol = TestColSphere(seed);
				showSphereWindow = true;
			}
			//Cylinders
			if (ImGui::MenuItem("Cylinders collision")) {
				cylCol = TestColCyl(seed);
				showCylWindow = true;
			}
			//Triangles
			if (ImGui::MenuItem("Triangles collision")) {
				triCol = TestColTri(seed);
				showTriWindow = true;
			}

			//AABB
			if (ImGui::MenuItem("AABB collision")) {
				AABBCol = TestColAABB(seed);
				showAABBWindow = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Example Window")) { showDemoWindow = true; }
		if (ImGui::MenuItem("Console")) { showConsoleWindow = true; }



		ImGui::EndMainMenuBar();

	}
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
	if (showAboutWindowbool)
	{ 
		showAboutWindow(); 
	}
	if (showConfig) 
	{ 
		showConfigFunc(); 
	}
	if (showConsoleWindow)
	{
		ShowExampleAppConsole(&showConsoleWindow);
	}
	if (showSphereWindow) {

		ShowSphereWindow(showSphereWindow, seed, sphereCol);
	}
	if (showCylWindow) {

		ShowCylWindow(showCylWindow, seed, cylCol);
	}
	if (showTriWindow) {

		ShowTriWindow(showTriWindow, seed, triCol);
	}
	if (showAABBWindow) {

		ShowAAABBWindow( showAABBWindow,  seed,  AABBCol);
	}
	return status;
}

update_status ModuleRenderer2D::PostUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;

	// Rendering
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui::Render(); ImGuiIO& io = ImGui::GetIO(); (void)io;
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
	SDL_GL_SwapWindow(App->window->window);

	return status;
}

bool ModuleRenderer2D::CleanUp()
{
	bool ret = true;

	//TODO: Void functions, no return, no check possible. FIX!
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return ret;
}

void ModuleRenderer2D::OnResize(int width, int height)
{
}

bool ModuleRenderer2D::showAboutWindow()
{
	//this can't be externalized because of ShellExecuteA command + SDL references

	if (!ImGui::Begin("About this Engine", &showAboutWindowbool))		//this is how you add the cross button to a window
	{
		ImGui::End();
		return false;
	}


	//name of engine
	const char* aux = "PHOEBUS ENGINE";
	ImGui::TextColored(ImVec4(0, 100, 130, 150), aux);
	ImGui::Spacing();
	ImGui::Spacing();

	//descrption
	ImGui::Text("Welcome to Phoebus Engine! This engine is made by two 3rd grade students of Design and Development of Videogames on CITM university");
	ImGui::Text("The intent is to create a 3D functional engine with a focus on sound design, performance and capability (hence the engine name)");
	ImGui::Text("We hope you'll enjoy using it as much as we enjoyed making it!");
	ImGui::Spacing();
	ImGui::Spacing();

	//autohors
	ImGui::Text("This Engine was made by these two brave souls: ");
	ImGui::Spacing();
	if (ImGui::MenuItem("Adria Serrano Lopez")) { ShellExecuteA(NULL, NULL, "https://github.com/adriaserrano97", NULL, NULL, SW_SHOWNORMAL); }
	if (ImGui::MenuItem("Oscar Perez martin")) { ShellExecuteA(NULL, NULL, "https://github.com/oscarpm5", NULL, NULL, SW_SHOWNORMAL); }
	ImGui::Spacing();
	ImGui::Spacing();

	//libraries version used with links
	if (ImGui::MenuItem("Libraries used")) { showLibs = true; }
	if (showLibs) { showLibsFunc(); }
	ImGui::Spacing();
	ImGui::Spacing();

	//Project License
	if (ImGui::MenuItem("Engine License")) { ShellExecuteA(NULL, NULL, "https://opensource.org/licenses/MIT", NULL, NULL, SW_SHOWNORMAL); }




	ImGui::End();

	return true;
}

bool ModuleRenderer2D::showLibsFunc()
{
	//this can't be externalized because of ShellExecuteA command + SDL references
	if (!ImGui::Begin("Libraries used:", &showLibs))		//this is how you add the cross button to a window
	{
		ImGui::End();
		return false;
	}

	ImGui::Text("MathGeoLib v1.5"); //TODO: MathGeoLib has no get version lel, stuck on 1.5 since 2004 or something
	ImGui::Spacing();

	ImGui::Text("ImGui");
	ImGui::SameLine();
	ImGui::Text(ImGui::GetVersion());
	ImGui::Spacing();

	ImGui::Text("SDL");
	ImGui::SameLine();
	SDL_version compiled;
	SDL_VERSION(&compiled);
	ImGui::Text("%d.%d.%d", compiled.major, compiled.minor, compiled.patch); //Because returning a string was too easy huh
	ImGui::Spacing();

	ImGui::Text("OpenGL v.");
	GLint aux1; (glGetIntegerv(GL_MAJOR_VERSION, &aux1));
	GLint aux2; (glGetIntegerv(GL_MINOR_VERSION, &aux2));
	ImGui::SameLine();
	ImGui::Text("%i.", int(aux1));
	ImGui::SameLine();
	ImGui::Text("%i.", int(aux2));
	ImGui::Spacing();

	ImGui::End();
	return true;
}

bool ModuleRenderer2D::showConfigFunc()
{
	if (!ImGui::Begin("Configuration", &showConfig))		//this is how you add the cross button to a window
	{
		ImGui::End();
		return false;
	}


	if (ImGui::CollapsingHeader("FPS")) {

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", App->fpsBuffer[App->fpsBuffer.size() - 1]);
		ImGui::PlotHistogram("##framerate", &App->fpsBuffer[0], App->fpsBuffer.size(), 0, title, 0.0f, 120.0f, ImVec2(400, 100));


		sprintf_s(title, 25, "Milliseconds %.1f", App->millisecondsBuffer[App->millisecondsBuffer.size() - 1]);
		ImGui::PlotHistogram("##framerate", &App->millisecondsBuffer[0], App->millisecondsBuffer.size(), 0, title, 0.0f, 100.0f, ImVec2(400, 100));

		ImGui::PopStyleColor();
		
	}

	if (ImGui::CollapsingHeader("Window")) {

		if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0, 1))
		{
			SDL_SetWindowBrightness(App->window->window, App->window->brightness);

		}

		if (ImGui::SliderInt("Width", &(App->window->w), 1, 2800))
		{
			if (resizable) {
				SDL_SetWindowSize(App->window->window, App->window->w, App->window->h);
				OpenGLOnResize(App->window->w, App->window->h);
			}
		};
		if (ImGui::SliderInt("Height", &(App->window->h), 1, 2800))
		{
			if (resizable) {
				SDL_SetWindowSize(App->window->window, App->window->w, App->window->h);
				OpenGLOnResize(App->window->w, App->window->h);
			}
		};
		if (ImGui::Checkbox("Resizable", &resizable))
		{
			if (resizable) {
				LOG("Resizable mode toggled to TRUE");
			}
			else {
				LOG("Resizable mode toggled to FALSE");
			}
		}
		if (ImGui::Checkbox("Borderless", &borderless))
		{
			//SDL_SetWindowFullscreen(App->window->window, );
			LOG("TODO: this button is still not fully operational");
		}
		if (ImGui::Checkbox("Fullscreen", &fullscreen))
		{
			LOG("TODO: this button is still not fully operational");
		}
		if (ImGui::Checkbox("Full desktop", &fullDesktop))
		{
			LOG("TODO: this button is still not fully operational");
		}

		
	}
	if (ImGui::CollapsingHeader("Hardware specs")) {

		int lineSize = SDL_GetCPUCacheLineSize();
		ImGui::Text("GPU has size %i", lineSize);
		int GPUCount = SDL_GetCPUCount();
		ImGui::Text("PC has %i CPU's", GPUCount);
		int systemRAM = SDL_GetSystemRAM();
		ImGui::Text("Ram configured is %i", systemRAM);
		ImGui::Text("Caps:");
		ImGui::SameLine(); if (SDL_Has3DNow()) { ImGui::Text("3DNow "); }
		ImGui::SameLine(); if (SDL_HasAVX()) { ImGui::Text("AVX "); }
		ImGui::SameLine(); if (SDL_HasAltiVec()) { ImGui::Text("AltiVec "); }
		ImGui::SameLine(); if (SDL_HasMMX()) { ImGui::Text("MMX "); }
		ImGui::SameLine(); if (SDL_HasRDTSC()) { ImGui::Text("RDTSC "); }
		ImGui::SameLine(); if (SDL_HasSSE()) { ImGui::Text("SSE "); }
		ImGui::SameLine(); if (SDL_HasSSE2()) { ImGui::Text("SSE2 "); }
		ImGui::SameLine(); if (SDL_HasSSE3()) { ImGui::Text("SSE3 "); }
		ImGui::SameLine(); if (SDL_HasSSE41()) { ImGui::Text("SSE41 "); }
		ImGui::SameLine(); if (SDL_HasSSE42()) { ImGui::Text("SSE42 "); }

		
	}
	ImGui::End();
	return true;
}

void ModuleRenderer2D::OpenGLOnResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glScissor(0, 0, w, h);

	//We set the new screen height & width
	SDL_GetWindowSize(App->window->window, &App->window->w, &App->window->h);

	//Calculate OpenGl stuff-----------------
	float width = (float)App->window->w;
	float height = (float)App->window->h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	App->renderer3D->ProjectionMatrix = perspective(60.0f, (width / 2) / height, 0.225f, 512.0f);
	glLoadMatrixf(&App->renderer3D->ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer2D::ShowExampleAppConsole(bool* p_open)
{
	console->Draw("Example: Console", p_open);
}

void ModuleRenderer2D::CreateDockingSpace()
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", NULL, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
		ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
		ImGui::SameLine(0.0f, 0.0f);
		if (ImGui::SmallButton("click here"))
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	ImGui::End();
}
