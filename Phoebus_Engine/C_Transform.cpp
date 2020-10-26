#include "C_Transform.h"
#include "GameObject.h"
#include "glmath.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?

C_Transform::C_Transform(GameObject* owner, mat4x4 lTransform):Component(ComponentType::TRANSFORM,owner),
lTransformMat(lTransform),gTransformMat(IdentityMatrix)
{
	
	UpdateGlobalMat();
}

C_Transform::~C_Transform()
{
}

mat4x4 C_Transform::GetGlobalTransform() const
{
	return gTransformMat;
}

mat4x4 C_Transform::GetLocalTransform() const
{
	return lTransformMat;
}

void C_Transform::OnEditor()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		ImGui::Separator();

		ImGui::Text("Local Matrix");
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; j++)
			{
				float auxF = float(lTransformMat.M[i*j + j]);
				ImGui::Text("%f", auxF);
				if (j < 3) ImGui::SameLine();
			}
		}

		ImGui::PopStyleColor();
		ImGui::Separator();

		ImGui::Text("Global Matrix");
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; j++)
			{
				float auxF = float(gTransformMat.M[j * i + j]);
				ImGui::Text("%f", auxF);
				if (j < 3) ImGui::SameLine();
			}
		}
		ImGui::PopStyleColor();
		ImGui::Separator();

		ImGui::Unindent();
	}


}

void C_Transform::UpdateGlobalMat()
{
	if (owner->parent!=nullptr)
	{
		gTransformMat = owner->parent->GetComponent<C_Transform>()->GetGlobalTransform()*lTransformMat;
	}
	else
		gTransformMat = lTransformMat;
}


