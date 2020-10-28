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
				//ImGui::Text("%f", auxF);
				static char buf1[64] = "%f"; 
				ImGui::InputText(" ", buf1, 64);
				if (j < 3) ImGui::SameLine();
			}
		}

		vec3 auxP = GetLocalPosition();	vec3 auxR = GetLocalRotation();	vec3 auxS = GetLocalScale();

		static float vec4a[4] = { auxP.x,  auxP.y,  auxP.z,  auxP.t };
		ImGui::InputFloat3("Position", vec4a);
		static float vec4b[4] = { auxR.x,  auxR.y,  auxR.z,  auxR.t };
		ImGui::InputFloat3("Rotation", vec4b);
		static float vec4c[4] = { auxS.x,  auxS.y,  auxS.z,  auxS.t };
		ImGui::InputFloat3("Scale", vec4c);

		
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

vec3 C_Transform::GetLocalPosition()
{
	return vec3(gTransformMat.M[3], gTransformMat.M[7], gTransformMat.M[11]);
}

vec3 C_Transform::GetLocalScale()
{
	float a = std::vector<float>{ gTransformMat.M[0], gTransformMat.M[4], gTransformMat.M[8]}.size();
	float b = std::vector<float>{ gTransformMat.M[1], gTransformMat.M[5], gTransformMat.M[9]}.size();
	float c = std::vector<float>{ gTransformMat.M[2], gTransformMat.M[6], gTransformMat.M[10]}.size();
	return vec3(a,b,c);
}

vec3 C_Transform::GetLocalRotation()
{
//https://math.stackexchange.com/questions/237369/given-this-transformation-matrix-how-do-i-decompose-it-into-translation-rotati	
	return vec3();
}


