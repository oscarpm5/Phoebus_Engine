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

	if (myself->children.size() == 0)
		TreeNodeEx_flags = ImGuiTreeNodeFlags_Leaf;

	if (myself == App->editor3d->root)
	{
		for (int i = 0; i < myself->children.size(); i++)
		{
			SeekMyChildren(myself->children[i]);
		}
	}
	else
	{


		ImGui::PushStyleColor(ImGuiCol_Text, ChooseMyColor(myself));

		bool open = ImGui::TreeNodeEx(myself->GetName().c_str(), TreeNodeEx_flags);




		//==========================================================================================
		// Our buttons are both drag sources and drag targets here!
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set payload to carry the object pointer
			ImGui::SetDragDropPayload("OBJ_ID", &myself->ID, sizeof(unsigned int));

			// Display preview (could be anything, e.g. when dragging an image we could decide to display
			// the filename and a small preview of the image, etc.)
			
			ImGui::Text("%s",myself->GetName().c_str());
			
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("OBJ_ID"))
			{
				IM_ASSERT(payload->DataSize == sizeof(unsigned int));
				//int payload_n = *(const int*)payload->Data;
			
				GameObject* newObj = nullptr;
				App->editor3d->root->GetChildWithID(*(const unsigned int*)payload->Data,newObj);
				GameObject* newParent = nullptr;
				App->editor3d->root->GetChildWithID(myself->ID,newParent);
				
				newObj->ChangeParent(newParent);

				
				/*newParent->children.push_back(newObj);
				newObj->parent = newParent;*/

				/*if (mode == Mode_Move)
				{
					names[n] = names[payload_n];
					names[payload_n] = "";
				}*/
			}
			ImGui::EndDragDropTarget();
		}

		//==========================================================================================



		if (ImGui::IsItemClicked())
		{
			if (!App->editor3d->GetSelectedGameObject().empty())
			{
				App->editor3d->GetSelectedGameObject().back()->focused = false;
			}
			App->editor3d->SetSelectedGameObject(myself);
			App->editor3d->GetSelectedGameObject().back()->focused = true;

		}

		if (open == true)
		{

			for (int i = 0; i < myself->children.size(); i++)
			{
				SeekMyChildren(myself->children[i]);
			}
			ImGui::TreePop();
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


		ImGui::PopStyleColor();
	}
}

//Quick func that return the color vector based on IsActive
ImVec4 ChooseMyColor(GameObject* myself)
{
	ImVec4 activeColor ACTIVE_COLOR;

	if (!myself->isActive || !myself->IsParentActive()) activeColor = ImVec4 PASIVE_COLOR;
	if (myself->focused) activeColor = ImVec4 FOCUSED_COLOR;
	return activeColor;
}
