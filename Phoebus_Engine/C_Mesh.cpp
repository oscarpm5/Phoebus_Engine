#include "C_Mesh.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?
#include "Mesh.h"
#include <string>
#include "MathGeoLib/include/Geometry/AABB.h"
#include "Application.h"
#include "M_ResourceManager.h"

C_Mesh::C_Mesh(GameObject* owner,unsigned int ID) :Component(ComponentType::MESH, owner,ID),resourceID(0),
normalVertexSize(1.0f), normalFaceSize(1.0f), normalDrawMode(0), meshDrawMode(0),temporalRMesh(nullptr)
{
}

C_Mesh::~C_Mesh()
{

	if (resourceID != 0)
	{
		App->rManager->StopUsingResource(resourceID);
		resourceID = 0;
	}

	DeleteTemporalMesh();

	normalVertexSize = 0;
	normalFaceSize = 0;
	normalDrawMode = 0;
	meshDrawMode = 0;

	localAABB.SetNegativeInfinity();
}

void C_Mesh::SetNewResource(unsigned int resourceUID)
{

	if (resourceID != 0)
	{
		App->rManager->StopUsingResource(resourceID);
	}

	ResourceMesh* r = (ResourceMesh*)App->rManager->RequestNewResource(resourceID);
	if (r != nullptr)
		resourceID = resourceUID;
	else
		resourceID = 0;

	localAABB.SetNegativeInfinity();//this is like setting the AABB to null
	localAABB.Enclose((float3*)r->vertices.data(), r->vertices.size() / 3); //generates an AABB on local space from a set of vertices


}

ResourceMesh* C_Mesh::GetMesh()
{
	if (resourceID != 0)
	{
		ResourceMesh* m = (ResourceMesh*)App->rManager->RequestExistingResource(resourceID);
		if (m == nullptr)
		{
			resourceID = 0;
		}
		else
		{
			return m;
		}
	}

	return nullptr;
}

void C_Mesh::OnEditor()
{
	ResourceMesh* m= GetMesh();
	if (m == nullptr) return; //TODO the component should be supported in the editor even without a mesh


	bool activeAux = active;

	std::string headerName = "Mesh";
	if (!activeAux)headerName += " (not active)";

	/*ImGui::Checkbox(" ", &active);
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX()-12);*/
	ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

	if (!activeAux)ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

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
		ImGui::Text("N of vertices: %i", m->vertices.size() / 3);

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

		if (ImGui::Button("Delete##Mesh"))
		{
			ImGui::OpenPopup("Delete Mesh");


		}

		if (!activeAux)ImGui::PopStyleColor();
		ImGui::PopStyleColor(2);

	}

	if (!activeAux)ImGui::PopStyleColor();
}

AABB C_Mesh::GetAABB() const
{
	return localAABB;
}

void C_Mesh::SetTemporalMesh(ResourceMesh* newTempMesh)
{
	temporalRMesh = newTempMesh;
}

void C_Mesh::DeleteTemporalMesh()
{
	if (temporalRMesh != nullptr)
	{
		delete temporalRMesh;
		temporalRMesh = nullptr;
	}
}
