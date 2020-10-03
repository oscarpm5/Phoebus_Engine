
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

#include "Console.h"

/*
struct ExampleAppConsole
{
	char                  InputBuf[256];
	ImVector<char*>       Items;
	ImVector<const char*> Commands;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter       Filter;
	bool                  AutoScroll;
	bool                  ScrollToBottom;

	ExampleAppConsole()
	{
		ClearLog();
		memset(InputBuf, 0, sizeof(InputBuf));
		HistoryPos = -1;

		// "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
		Commands.push_back("HELP");
		Commands.push_back("HISTORY");
		Commands.push_back("CLEAR");
		Commands.push_back("CLASSIFY");
		AutoScroll = true;
		ScrollToBottom = false;
		AddLog("Welcome to Dear ImGui!");
	}
	~ExampleAppConsole()
	{
		ClearLog();
		for (int i = 0; i < History.Size; i++)
			free(History[i]);
	}

	// Portable helpers
	static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
	static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
	static char* Strdup(const char* s) { size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
	static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

	void    ClearLog()
	{
		for (int i = 0; i < Items.Size; i++)
			free(Items[i]);
		Items.clear();
	}

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		// FIXME-OPT
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(Strdup(buf));
	}

	void    Draw(const char* title, bool* p_open)
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		// As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
		// So e.g. IsItemHovered() will return true when hovering the title bar.
		// Here we create a context menu only available from the title bar.
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Close Console"))
				* p_open = false;
			ImGui::EndPopup();
		}

		ImGui::TextWrapped(
			"This example implements a console with basic coloring, completion (TAB key) and history (Up/Down keys). A more elaborate "
			"implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
		ImGui::TextWrapped("Enter 'HELP' for help.");

		// TODO: display items starting from the bottom

		if (ImGui::SmallButton("Add Debug Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); }
		ImGui::SameLine();
		if (ImGui::SmallButton("Add Debug Error")) { AddLog("[error] something went wrong"); }
		ImGui::SameLine();
		if (ImGui::SmallButton("Clear")) { ClearLog(); }
		ImGui::SameLine();
		bool copy_to_clipboard = ImGui::SmallButton("Copy");
		//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

		ImGui::Separator();

		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		// Options, Filter
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
		ImGui::Separator();

		// Reserve enough left-over height for 1 separator + 1 input text
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear")) ClearLog();
			ImGui::EndPopup();
		}

		// Display every line as a separate entry so we can change their color or add custom widgets.
		// If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
		// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
		// to only process visible items. The clipper will automatically measure the height of your first item and then
		// "seek" to display only items in the visible area.
		// To use the clipper we can replace your standard loop:
		//      for (int i = 0; i < Items.Size; i++)
		//   With:
		//      ImGuiListClipper clipper(Items.Size);
		//      while (clipper.Step())
		//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		// - That your items are evenly spaced (same height)
		// - That you have cheap random access to your elements (you can access them given their index,
		//   without processing all the ones before)
		// You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
		// We would need random-access on the post-filtered list.
		// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
		// or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
		// and appending newly elements as they are inserted. This is left as a task to the user until we can manage
		// to improve this example code!
		// If your items are of variable height:
		// - Split them into same height items would be simpler and facilitate random-seeking into your list.
		// - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		if (copy_to_clipboard)
			ImGui::LogToClipboard();
		for (int i = 0; i < Items.Size; i++)
		{
			const char* item = Items[i];
			if (!Filter.PassFilter(item))
				continue;

			// Normally you would store more information in your item than just a string.
			// (e.g. make Items[] an array of structure, store color/type etc.)
			ImVec4 color;
			bool has_color = false;
			if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
			else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
			if (has_color)
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(item);
			if (has_color)
				ImGui::PopStyleColor();
		}
		if (copy_to_clipboard)
			ImGui::LogFinish();

		if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;

		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		// Command-line
		bool reclaim_focus = false;
		ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
		{
			char* s = InputBuf;
			Strtrim(s);
			if (s[0])
				ExecCommand(s);
			strcpy(s, "");
			reclaim_focus = true;
		}

		// Auto-focus on window apparition
		ImGui::SetItemDefaultFocus();
		if (reclaim_focus)
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		ImGui::End();
	}

	void    ExecCommand(const char* command_line)
	{
		AddLog("# %s\n", command_line);

		// Insert into history. First find match and delete it so it can be pushed to the back.
		// This isn't trying to be smart or optimal.
		HistoryPos = -1;
		for (int i = History.Size - 1; i >= 0; i--)
			if (Stricmp(History[i], command_line) == 0)
			{
				free(History[i]);
				History.erase(History.begin() + i);
				break;
			}
		History.push_back(Strdup(command_line));

		// Process command
		if (Stricmp(command_line, "CLEAR") == 0)
		{
			ClearLog();
		}
		else if (Stricmp(command_line, "HELP") == 0)
		{
			AddLog("Commands:");
			for (int i = 0; i < Commands.Size; i++)
				AddLog("- %s", Commands[i]);
		}
		else if (Stricmp(command_line, "HISTORY") == 0)
		{
			int first = History.Size - 10;
			for (int i = first > 0 ? first : 0; i < History.Size; i++)
				AddLog("%3d: %s\n", i, History[i]);
		}
		else
		{
			AddLog("Unknown command: '%s'\n", command_line);
		}

		// On command input, we scroll to bottom even if AutoScroll==false
		ScrollToBottom = true;
	}

	// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{
		ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
		return console->TextEditCallback(data);
	}

	int     TextEditCallback(ImGuiInputTextCallbackData* data)
	{
		//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			// Example of TEXT COMPLETION

			// Locate beginning of current word
			const char* word_end = data->Buf + data->CursorPos;
			const char* word_start = word_end;
			while (word_start > data->Buf)
			{
				const char c = word_start[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;
				word_start--;
			}

			// Build a list of candidates
			ImVector<const char*> candidates;
			for (int i = 0; i < Commands.Size; i++)
				if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
					candidates.push_back(Commands[i]);

			if (candidates.Size == 0)
			{
				// No match
				AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
			}
			else if (candidates.Size == 1)
			{
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else
			{
				// Multiple matches. Complete as much as we can..
				// So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
				int match_len = (int)(word_end - word_start);
				for (;;)
				{
					int c = 0;
					bool all_candidates_matches = true;
					for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
						if (i == 0)
							c = toupper(candidates[i][match_len]);
						else if (c == 0 || c != toupper(candidates[i][match_len]))
							all_candidates_matches = false;
					if (!all_candidates_matches)
						break;
					match_len++;
				}

				if (match_len > 0)
				{
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
				}

				// List matches
				AddLog("Possible matches:\n");
				for (int i = 0; i < candidates.Size; i++)
					AddLog("- %s\n", candidates[i]);
			}

			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			// Example of HISTORY
			const int prev_history_pos = HistoryPos;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (HistoryPos == -1)
					HistoryPos = History.Size - 1;
				else if (HistoryPos > 0)
					HistoryPos--;
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (HistoryPos != -1)
					if (++HistoryPos >= History.Size)
						HistoryPos = -1;
			}

			// A better implementation would preserve the data on the current input line along with cursor position.
			if (prev_history_pos != HistoryPos)
			{
				const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);
			}
		}
		}
		return 0;
	}
};
*/



ModuleRenderer2D::ModuleRenderer2D(bool start_enabled)
{
	console = new Cnsl();
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
				ImGui::SetNextWindowSize(ImVec2(435, 800));
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
		if (ImGui::MenuItem("Console")) { showConsoleWindow = true; }


		ImGui::EndMainMenuBar();

	}

	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);
	if (showAboutWindowbool) {showAboutWindow(); }
	if (showConfig) { showConfigFunc(); }

	if (showConsoleWindow)
	{
		ShowExampleAppConsole(&showConsoleWindow);
	}

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

	if (console != nullptr)
	{
		delete(console);
		console = nullptr;
	}
	
	return ret;
}

void ModuleRenderer2D::OnResize(int width, int height)
{
}

bool ModuleRenderer2D::showAboutWindow()
{
	

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
	

	if (ImGui::BeginMenu("FPS")) {
		/*
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log->size() - 1]);
		ImGui::PlotHistogram("##framerate", fps_log->begin() , fps_log->size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		*/
		ImGui::Text("This is where I would show a fps counter, if I had one");
		ImGui::End();
	}

	if (ImGui::BeginMenu("Window")) {
		ImGui::Text("brightness slider");
		ImGui::Text("width slider");
		ImGui::Text("height slider");
		ImGui::Text("Resizable bool");
		ImGui::Text("Borderless bool");
		ImGui::Text("Fullscreen bool");
		ImGui::Text("Full Desktop bool");
		ImGui::End();
	}
	if (ImGui::BeginMenu("Hardware specs")) {
		
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
	
		ImGui::End();
	}
	ImGui::End();
	return true;
}

void ModuleRenderer2D::CheckFPS()
{
	/*
	fps_log->Data[fps_log->size()-1] = seed.Int(40, 60); // placeholder
	

	for (int i = 0; i++;  fps_log->size() -2) {
		fps_log->Data[i] = fps_log->Data[i + 1];		//cursed tech
	}
	*/
}
