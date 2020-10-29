#include "C_Mesh.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?

C_Mesh::C_Mesh(GameObject* owner) :Component(ComponentType::MESH, owner), m(nullptr),
normalVertexSize(0.0f),normalFaceSize(0.0f),normalDrawMode(0),meshDrawMode(0)
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

	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
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
			ImGui::SliderFloat("Vertex Normal Size", &normalVertexSize, 0.0f, 1.0f);
		if (normalDrawMode == 2 || normalDrawMode == 3)
			ImGui::SliderFloat("Face Normal Size", &normalFaceSize, 0.0f, 1.0f);

		
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
		ImGui::Text("ID_TexCoords: %i", m->texCoords);
		ImGui::Text("N of vertices: %i", m->vertices.size());

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text("Here I would show a bounding box... IF I HAD ONE!");
		ImGui::Separator();
		ImGui::Unindent();


	}
}
