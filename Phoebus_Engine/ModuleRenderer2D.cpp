
//The order is relevant from here...
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL/include/SDL.h"
#include "Glew/include/glew.h"
//...to here
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer2D.h"

//We're using pretty much all of it for cheks, so we're just including the whole thing
#include "MathGeoLib/include/MathGeoLib.h"







ModuleRenderer2D::ModuleRenderer2D(bool start_enabled)
{
}

ModuleRenderer2D::~ModuleRenderer2D()
{
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
	ImGui_ImplOpenGL3_Init("#version 130"); //TODO: this is hardcoded. Deal with it.

	//Start number generator seed
	seed = LCG::LCG();

	return ret;
}

update_status ModuleRenderer2D::PreUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// Our state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

				randomRad1 = seed.Int(0, 5);
				Sphere aux1 = Sphere(float3(0, 0, 0), randomRad1);
				randomRad2 = seed.Int(0, 5);
				Sphere aux2 = Sphere(float3(3, 3, 3), randomRad2);

				sphereCol = aux1.Intersects(aux2);
				showSphereWindow = true;

			}
			//Cylinders
			if (ImGui::MenuItem("Cylinders collision")) {

				randomRad1 = seed.Int(0, 5);
				Cylinder aux1 = Cylinder(float3(0, 0, 0), float3(0, 0, 0), randomRad1);
				randomRad2 = seed.Int(0, 5);
				Cylinder aux2 = Cylinder(float3(4, 0, 0), float3(4, 0, 0), randomRad2);

				if(randomRad1 + randomRad2 >= 4) //TODO: cheap AF
					cylCol = true;
				else 
					cylCol = false;

				showCylWindow = true;
			}
			//Triangles
			if (ImGui::MenuItem("Triangles collision")) {

				randomRad1 = seed.Int(0, 5);
				Triangle aux1 = Triangle(float3(0,0,0), float3(randomRad1,0,0), float3(randomRad1/2,2,0));
				randomRad2 = seed.Int(0, 5);
				Triangle aux2 = Triangle(float3(3, 0, 0), float3(3+ randomRad2, 0, 0), float3(3+ randomRad2 / 2, 2, 0));

				if (randomRad1 + randomRad2 >= 3) //TODO: cheap AF
					triCol = true;
				else
					triCol = false;

				showTriWindow = true;
			}

			//AABB
			if (ImGui::MenuItem("AABB collision")) {
				randomRad1 = seed.Int(0, 5); Sphere s1(float3(0, 0, 0), randomRad1);
				randomRad2 = seed.Int(0, 5); Sphere s2(float3(4, 0, 0), randomRad2);

				AABB aux1(s1); AABB aux2(s2);
				AABB auxSol = aux1.Intersection(aux2);

				if (auxSol.Volume() != 0) { 
					
					AABBCol = true;
				}
				else { 
					AABBCol = false; 
				}

				showAABBWindow = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Example Window")) { showDemoWindow = true; }


		ImGui::EndMainMenuBar();

	}

	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);

	if (showSphereWindow) {
		
		ImGui::Begin("Sphere checks");
		ImGui::Text("Sphere 1: center on [0 ,0 , 0] and rad %i", randomRad1);
		ImGui::Text("Sphere 2: center on [3 ,3 , 3] and rad %i", randomRad2);

		if (sphereCol) {
			ImGui::Text("Intersection succesful!");
		}
		else {
			ImGui::Text("Failed to intersect!");
		}
		(ImGui::Text(" "));
		if (ImGui::MenuItem("Close window")) { showSphereWindow = false; }
		if (ImGui::MenuItem("Another one!")) {
			randomRad1 = seed.Int(0, 5);
			Sphere aux1 = Sphere(float3(0, 0, 0), randomRad1);
			randomRad2 = seed.Int(0, 5);
			Sphere aux2 = Sphere(float3(3, 3, 3), randomRad2);
			sphereCol = aux1.Intersects(aux2);
		}

		ImGui::End();
	}
	if (showCylWindow) {

		ImGui::Begin("Cylinder checks");
		ImGui::Text("Cylinder 1: center on [0 ,0 , 0] and rad %i", randomRad1);
		ImGui::Text("Cylinder 2: center on [4 ,0 , 0] and rad %i", randomRad2);

		if (cylCol) {
			ImGui::Text("Intersection succesful!");
		}
		else {
			ImGui::Text("Failed to intersect!");
		}
		(ImGui::Text(" "));
		if (ImGui::MenuItem("Close window")) { showCylWindow = false; }
		if (ImGui::MenuItem("Another one!")) {
			randomRad1 = seed.Int(0, 5);
			Cylinder aux1 = Cylinder(float3(0, 0, 0), float3(0, 0, 0), randomRad1);
			randomRad2 = seed.Int(0, 5);
			Cylinder aux2 = Cylinder(float3(4, 0, 0), float3(4, 0, 0), randomRad2);

			if (randomRad1 + randomRad2 >= 4) //TODO: cheap AF
				cylCol = true;
			else
				cylCol = false;
		}

		ImGui::End();
	}
	if (showTriWindow) {

		ImGui::Begin("Triangle checks");
		ImGui::Text("Triangle 1: flat side has %i", randomRad1);
		ImGui::Text("Triangle 2: Is fixed 3 units away", randomRad2);

		if (triCol) {
			ImGui::Text("Intersection succesful!");
		}
		else {
			ImGui::Text("Failed to intersect!");
		}
		(ImGui::Text(" "));
		if (ImGui::MenuItem("Close window")) { showTriWindow = false; }
		if (ImGui::MenuItem("Another one!")) {
			randomRad1 = seed.Int(0, 5);
			Triangle aux1 = Triangle(float3(0, 0, 0), float3(randomRad1, 0, 0), float3(randomRad1 / 2, 2, 0));
			randomRad2 = seed.Int(0, 5);
			Triangle aux2 = Triangle(float3(3, 0, 0), float3(3 + randomRad2, 0, 0), float3(3 + randomRad2 / 2, 2, 0));

			if (randomRad1 >= 3) //TODO: cheap AF
				triCol = true;
			else
				triCol = false;
		}

		ImGui::End();
	}
	if (showAABBWindow) {

		ImGui::Begin("AABB checks");
		ImGui::Text("AABB 1: contains sphere with center 0,0,0 with rad %i", randomRad1);
		ImGui::Text("AABB 2: contains sphere with center 4,0,0 with rad %i", randomRad2);

		if (AABBCol) {
			ImGui::Text("Intersection succesful!");
		}
		else {
			ImGui::Text("Failed to intersect!");
		}

		if (ImGui::MenuItem("Close window")) { 
			
			showAABBWindow = false; 
		}
		ImGui::End();
	}
	return status;
}

update_status ModuleRenderer2D::PostUpdate(float dt)
{
	update_status status = UPDATE_CONTINUE;

	// Rendering
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui::Render(); ImGuiIO& io = ImGui::GetIO(); (void)io;
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
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
