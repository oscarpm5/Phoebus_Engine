#include "C_Mesh.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?

C_Mesh::C_Mesh(GameObject* owner):Component(ComponentType::MESH,owner),m(nullptr)
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
}
