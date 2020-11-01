#include "C_Mesh.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?
#include "Mesh.h"
#include <string>

C_Mesh::C_Mesh(GameObject* owner) :Component(ComponentType::MESH, owner), m(nullptr),
normalVertexSize(1.0f),normalFaceSize(1.0f),normalDrawMode(0),meshDrawMode(0)
{
}

C_Mesh::~C_Mesh()
{
	if (m != nullptr)
	{
		delete m;
		m = nullptr;
	}
	normalVertexSize = 0;
	normalFaceSize = 0;
	normalDrawMode = 0;
	meshDrawMode = 0;
}



void C_Mesh::SetMesh(Mesh mesh)
{
	if (m != nullptr)
	{
		delete m;
		m = nullptr;
	}

	m = new Mesh(mesh);
}

Mesh* C_Mesh::GetMesh() const
{
	return m;
}

void C_Mesh::OnEditor()
{
	if (m == nullptr) return;
		bool activeAux = active;

	std::string headerName = "Mesh";
	if (!activeAux)headerName += " (not active)";

	/*ImGui::Checkbox(" ", &active);
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX()-12);*/
	ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

	if(!activeAux)ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

	if (ImGui::CollapsingHeader(headerName.c_str(), headerFlags))
	{
		if (!activeAux)ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 0.75f, 0.8f));
		/*ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 20);*/
		ImGui::Checkbox("IS ACTIVE##MeshCheckbox", &active);

		ImGui::Separator();
		ImGui::Indent();

		/*static int normalMode = 0;
		ImGui::Combo(" Show Vertex Normals", &normalMode, "NONE\0VERTEX NORMALS\0FACE NORMALS\0ALL NORMALS\0\0");
		static int drawMode = 0;
		ImGui::Combo("Draw Mode", &drawMode, "FILL\0WIREFRAME\0BOTH\0\0");*/

		//Normal & draw

		ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft;
		const char* normalModes[] = { "NONE","VERTEX NORMALS","FACE NORMALS","ALL NORMALS" };
		const char* normalLabel = normalModes[normalDrawMode];  // Label to preview before opening the combo (technically it could be anything)
		if (ImGui::BeginCombo("Normals", normalLabel, flags))
		{
			for (int n = 0; n < IM_ARRAYSIZE(normalModes); n++)
			{
				const bool is_selected = (normalDrawMode == n);
				if (ImGui::Selectable(normalModes[n], is_selected))
					normalDrawMode = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}


		if (normalDrawMode == 1 || normalDrawMode == 3)
			ImGui::SliderFloat("Vertex Normal Size", &normalVertexSize, 0.1f, 1.0f);
		if (normalDrawMode == 2 || normalDrawMode == 3)
			ImGui::SliderFloat("Face Normal Size", &normalFaceSize, 0.1f, 1.0f);

		
		const char* drawModes[] = { "BOTH","FILL","WIREFRAME" };           
		const char* drawLabel = drawModes[meshDrawMode];  // Label to preview before opening the combo (technically it could be anything)
		if (ImGui::BeginCombo("Draw Mode", drawLabel, flags))
		{
			for (int n = 0; n < IM_ARRAYSIZE(drawModes); n++)
			{
				const bool is_selected = (meshDrawMode == n);
				if (ImGui::Selectable(drawModes[n], is_selected))
					meshDrawMode = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		//End of normal & draw

		ImGui::Text("ID_Index: %i", m->idIndex);
		ImGui::Text("ID_Vertex: %i", m->idVertex);
		ImGui::Text("ID_Normals: %i", m->idNormals);									
		ImGui::Text("N of vertices: %i", m->vertices.size());

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text("Here I would show a bounding box... IF I HAD ONE!");
		ImGui::Separator();
		ImGui::Unindent();

		if (ImGui::BeginPopup("Delete Mesh", ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("you are about to delete\n this component");

			if (ImGui::Button("Go ahead"))
			{
				toDelete = true;
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		float maxWidth = ImGui::GetWindowContentRegionMax().x;
		ImGui::SetCursorPosX(maxWidth - 50);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.25f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

		if (ImGui::Button("Delete"))
		{
			ImGui::OpenPopup("Delete Mesh");


		}
		
		if (!activeAux)ImGui::PopStyleColor();
		ImGui::PopStyleColor(2);

	}

	if (!activeAux)ImGui::PopStyleColor();
}
