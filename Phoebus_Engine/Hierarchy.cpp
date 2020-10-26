#include "Hierarchy.h"
#include "imgui/imgui.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleEditor3D.h"
#include "GameObject.h"

//value ret tied to SHow Hierarchy
bool ShowHierarchyTab()
{
	bool ret = true;

	if (App->editor3d->root != nullptr)
	{
		SeekMyChildren(App->editor3d->root);
	}
	return ret;
}

//This is the recursive func that makes the hierarchy
void SeekMyChildren(GameObject* myself)
{
	ImGuiTreeNodeFlags TreeNodeEx_flags = ImGuiTreeNodeFlags_OpenOnArrow; //yeah, we have to set it each iteration. ImGui Badness
	

	ImGui::PushStyleColor(ImGuiCol_Text, ChooseMyColor(myself));

	if (ImGui::TreeNodeEx(myself->GetName().c_str(), TreeNodeEx_flags))
	{
		if(ImGui::IsItemClicked)
		App->editor3d->SetSelectedGameObject(myself);

		for (int i = 0; i < myself->children.size(); i++)
		{
			SeekMyChildren(myself->children[i]);
		}

		
		

		//if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		//{
		//	//We are carrying the pointer to the Game Object as our payload
		//	ImGui::SetDragDropPayload("PointerToGameObject", myself, sizeof(GameObject*)); //We know any pointer is 4 bytes, but this way of putting it makes it clearer to understand what are we doing
		//	ImGui::EndDragDropSource();
		//}

		//if (ImGui::BeginDragDropTarget())
		//{
		//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PointerToGameObject"))
		//	{
		//		IM_ASSERT(payload->DataSize == sizeof(GameObject*)); //just making sure we received what we're looking for
		//		//myself->children.push_back((GameObject*)payload->Data);//cast required since payload is void*. This makes Adri scared
		//	}
		//	ImGui::EndDragDropTarget();
		//}

		ImGui::TreePop();
	}
	ImGui::PopStyleColor();
}

//Quick func that return the color vector based on IsActive
ImVec4 ChooseMyColor(GameObject* myself)
{
	ImVec4 activeColor ACTIVE_COLOR;

	if (!myself->isActive || !myself->IsParentActive()) activeColor = ImVec4 PASIVE_COLOR;

	return activeColor;
}
