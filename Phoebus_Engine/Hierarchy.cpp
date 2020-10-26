#include "Hierarchy.h"
#include "imgui/imgui.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleEditor3D.h"
#include "GameObject.h"

bool ShowHierarchyTab()
{
	bool ret = true;
	
	if (App->editor3d->root != nullptr)
	{
		SeekMyChildren(App->editor3d->root);
	}
	return ret;
}


void SeekMyChildren(GameObject* myself)
{
	ImGuiTreeNodeFlags TreeNodeEx_flags = ImGuiTreeNodeFlags_None; //yeah, we have to set it each iteration. ImGui Badness

	ImGui::PushStyleColor(ImGuiCol_Text, ChooseMyColor(myself));

	if (ImGui::TreeNodeEx(myself->GetName().c_str(), TreeNodeEx_flags))
	{
		for (int i = 0; i < myself->children.size(); i++)
		{
			SeekMyChildren(myself->children[i]);
		}
		ImGui::TreePop();
	}
	ImGui::PopStyleColor();
}

ImVec4 ChooseMyColor(GameObject* myself)
{
	ImVec4 activeColor ACTIVE_COLOR;

	if ( !myself->isActive || !myself->IsParentActive()) activeColor = ImVec4 PASIVE_COLOR;
	
	return activeColor;
}
