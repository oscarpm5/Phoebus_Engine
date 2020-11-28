#include "C_Mesh.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?
#include "Mesh.h"
#include <string>
#include "MathGeoLib/include/Geometry/AABB.h"
#include "Application.h"
#include "M_ResourceManager.h"

C_Mesh::C_Mesh(GameObject* owner, unsigned int ID) :Component(ComponentType::MESH, owner, ID), resourceID(0),
normalVertexSize(1.0f), normalFaceSize(1.0f), normalDrawMode(0), meshDrawMode(0), temporalRMesh(nullptr)
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

	ResourceMesh* r = (ResourceMesh*)App->rManager->RequestNewResource(resourceUID);
	if (r != nullptr)
	{

		resourceID = resourceUID;
		localAABB.SetNegativeInfinity();//this is like setting the AABB to null
		localAABB.Enclose((float3*)r->vertices.data(), r->vertices.size() / 3); //generates an AABB on local space from a set of vertices
	}
	else
	{
		resourceID = 0;
		LOG("[error] the resource with ID:%i, given to this component doesn't exist", resourceUID);
	}



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

unsigned int C_Mesh::GetResourceID()
{
	return resourceID;
}

void C_Mesh::OnEditor()
{
	//ResourceMesh* m= GetMesh();
	//if (m == nullptr) return; //TODO the component should be supported in the editor even without a mesh


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

		ResourceMesh* mesh = nullptr;
		mesh = GetMesh();

		//===========================================

		//get the items here

		std::string meshNameDisplay = "No Selected Mesh";
		unsigned int myResourceID = 0;
		if (mesh != nullptr)
		{
			meshNameDisplay = mesh->GetAssetFile();
			myResourceID = mesh->GetUID();
		}
		std::vector<Resource*> allLoadedMeshes;
		App->rManager->GetAllResourcesOfType(ResourceType::MESH, allLoadedMeshes);

		//const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
		//static int item_current_idx = 0;                    // Here our selection data is an index.
		//const char* combo_label = textNameDisplay.c_str();  // Label to preview before opening the combo (technically it could be anything)
		if (ImGui::BeginCombo("Used Mesh##mesh", meshNameDisplay.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			const bool noneSelected = (mesh == nullptr);
			if (ImGui::Selectable("NONE##mesh", noneSelected))
			{
				if (mesh != nullptr)
				{
					App->rManager->StopUsingResource(resourceID);
					resourceID = 0;
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (noneSelected)
				ImGui::SetItemDefaultFocus();

			//================================






			for (int n = 0; n < allLoadedMeshes.size(); n++)
			{
				std::string name = allLoadedMeshes[n]->GetAssetFile();
				name += "##";
				name += "meshList";
				name += std::to_string(n);
				const bool is_selected = (myResourceID == allLoadedMeshes[n]->GetUID());
				if (ImGui::Selectable(name.c_str(), is_selected))
				{
					SetNewResource(allLoadedMeshes[n]->GetUID());
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		//===========================================







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
		if (mesh != nullptr)
		{
			ImGui::Text("ID_Index: %i", mesh->idIndex);
			ImGui::Text("ID_Vertex: %i", mesh->idVertex);
			ImGui::Text("ID_Normals: %i", mesh->idNormals);
			ImGui::Text("N of vertices: %i", mesh->vertices.size() / 3);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Text("Here I would show a bounding box... IF I HAD ONE!");//TODO bruh we have one already
		}
		else
		{
			ImGui::Text("Select a Mesh to show its properties");
		}


		ImGui::Separator();
		ImGui::Unindent();

		if (ImGui::BeginPopup("Delete Mesh Component", ImGuiWindowFlags_AlwaysAutoResize))
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
			ImGui::OpenPopup("Delete Mesh Component");


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

ResourceMesh* C_Mesh::GetTemporalMesh()
{
	return temporalRMesh;
}

void C_Mesh::DeleteTemporalMesh()
{
	if (temporalRMesh != nullptr)
	{
		delete temporalRMesh;
		temporalRMesh = nullptr;
	}
}
