#include "C_Mesh.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?

C_Mesh::C_Mesh(GameObject* owner) :Component(ComponentType::MESH, owner), m(nullptr)
{
}

C_Mesh::~C_Mesh()
{
	if (m != nullptr)
	{
		delete m;
		m = nullptr;
	}
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
