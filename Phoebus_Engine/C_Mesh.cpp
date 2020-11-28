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
	std::string suffixLabel = "##Mesh";
	std::string actualname;
	suffixLabel += std::to_string(ID);
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
		
		ImGui::Spacing();


		actualname = "IS ACTIVE" + suffixLabel + "Checkbox";
		ImGui::Checkbox(actualname.c_str(), &active);

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
		actualname = "Used Mesh" + suffixLabel;
		if (ImGui::BeginCombo(actualname.c_str(), meshNameDisplay.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			actualname = "NONE" + suffixLabel;
			const bool noneSelected = (mesh == nullptr);
			if (ImGui::Selectable(actualname.c_str(), noneSelected))
			{
				if (mesh != nullptr)
				{
					App->rManager->StopUsingResource(resourceID);
					resourceID = 0;
					myResourceID = 0;
					mesh == nullptr;
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (noneSelected)
				ImGui::SetItemDefaultFocus();

			//================================






			for (int n = 0; n < allLoadedMeshes.size(); n++)
			{
				std::string name = allLoadedMeshes[n]->GetAssetFile();
				name += suffixLabel;
				name += "List";
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



		//begin drag drop target
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("ResourceMesh##dragdropSource"))
			{
				IM_ASSERT(payload->DataSize == sizeof(unsigned int));
				unsigned int payloadID = *(const int*)payload->Data;


				if (payloadID != 0 && payloadID != resourceID)
				{
					SetNewResource(payloadID);
				}
				
			}
			ImGui::EndDragDropTarget();

		}
		//end drag drop target


		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Indent();
		ImGui::Spacing();

		/*static int normalMode = 0;
		ImGui::Combo(" Show Vertex Normals", &normalMode, "NONE\0VERTEX NORMALS\0FACE NORMALS\0ALL NORMALS\0\0");
		static int drawMode = 0;
		ImGui::Combo("Draw Mode", &drawMode, "FILL\0WIREFRAME\0BOTH\0\0");*/

		//Normal & draw

		ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft;
		const char* normalModes[] = { "NONE","VERTEX NORMALS","FACE NORMALS","ALL NORMALS" };
		const char* normalLabel = normalModes[normalDrawMode];  // Label to preview before opening the combo (technically it could be anything)
		actualname = "Normals" + suffixLabel;
		if (ImGui::BeginCombo(actualname.c_str(), normalLabel, flags))
		{
			for (int n = 0; n < IM_ARRAYSIZE(normalModes); n++)
			{
				const bool is_selected = (normalDrawMode == n);
				actualname = normalModes[n] + suffixLabel;
				if (ImGui::Selectable(actualname.c_str(), is_selected))
					normalDrawMode = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}


		if (normalDrawMode == 1 || normalDrawMode == 3)
		{
			actualname = "Vertex Normal Size" + suffixLabel;
			ImGui::SliderFloat(actualname.c_str(), &normalVertexSize, 0.1f, 1.0f);
		}
		if (normalDrawMode == 2 || normalDrawMode == 3)
		{
			actualname = "Face Normal Size" + suffixLabel;
			ImGui::SliderFloat(actualname.c_str(), &normalFaceSize, 0.1f, 1.0f);
		}

		const char* drawModes[] = { "BOTH","FILL","WIREFRAME" };
		const char* drawLabel = drawModes[meshDrawMode];  // Label to preview before opening the combo (technically it could be anything)
		actualname = "Draw Mode" + suffixLabel;
		if (ImGui::BeginCombo(actualname.c_str(), drawLabel, flags))
		{
			for (int n = 0; n < IM_ARRAYSIZE(drawModes); n++)
			{
				actualname = drawModes[n] + suffixLabel;
				const bool is_selected = (meshDrawMode == n);
				if (ImGui::Selectable(actualname.c_str(), is_selected))
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
			ImGui::Spacing();
			ImGui::Separator();

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("ID_Index: %i", mesh->idIndex);
			ImGui::Text("ID_Vertex: %i", mesh->idVertex);
			ImGui::Text("ID_Normals: %i", mesh->idNormals);
			ImGui::Text("N of vertices: %i", mesh->vertices.size() / 3);

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Separator();
			actualname = suffixLabel + "columnHeader";
			ImGui::Columns(1, actualname.c_str(), true);
			ImGui::Text("Mesh local AABB:");
			actualname = suffixLabel + "columns";

			ImGui::Columns(2,actualname.c_str(),true);
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Min Corner:");
			//ImGui::Indent();
			ImGui::Text("X:%f", localAABB.minPoint.x);
			ImGui::Text("Y:%f", localAABB.minPoint.y);
			ImGui::Text("Z:%f", localAABB.minPoint.z);
			//ImGui::Unindent();
			//ImGui::Separator();
			ImGui::Spacing();
			ImGui::NextColumn();
			ImGui::Spacing();
			ImGui::Text("Max Corner:");
			//ImGui::Indent();
			ImGui::Text("X:%f", localAABB.maxPoint.x);
			ImGui::Text("Y:%f", localAABB.maxPoint.y);
			ImGui::Text("Z:%f", localAABB.maxPoint.z);
			ImGui::Spacing();
			ImGui::Columns(1);
			ImGui::Separator();
			//ImGui::Unindent();
		}
		else
		{
			ImGui::Text("Select a Mesh to show its properties");
		}


		ImGui::Separator();
		ImGui::Unindent();

		actualname = "Delete Mesh Component" + suffixLabel;
		if (ImGui::BeginPopup(actualname.c_str(), ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("you are about to delete\n this component");
			actualname = "Go ahead" + suffixLabel;
			if (ImGui::Button(actualname.c_str()))
			{
				toDelete = true;
			}

			ImGui::SameLine();
			actualname = "Cancel" + suffixLabel;
			if (ImGui::Button(actualname.c_str()))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		float maxWidth = ImGui::GetWindowContentRegionMax().x;
		ImGui::SetCursorPosX(maxWidth - 50);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.25f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		actualname = "Delete" + suffixLabel+"component";
		if (ImGui::Button(actualname.c_str()))
		{
			actualname = "Delete Mesh Component" + suffixLabel;
			ImGui::OpenPopup(actualname.c_str());


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
