#include "C_Transform.h"
#include "GameObject.h"
#include "glmath.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?
#include "Assimp/include/matrix4x4.inl"
#include "Assimp/include/vector3.h"



C_Transform::C_Transform(GameObject* owner, mat4x4 lTransform) :Component(ComponentType::TRANSFORM, owner),
lTransformMat(lTransform), gTransformMat(IdentityMatrix)
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


		//Calculate all the stuff
		aiVector3t<float> scaling; aiQuaterniont<float> rotation; aiVector3t<float> position;
		lTraansIntoAssimpMatrix().Decompose(scaling, rotation, position);
		vec3 auxvec3 = GetEulerFromQuat(rotation);
		float v[3];
		//Show me the things
		v[0] = position.x; v[1] = position.y; v[2] = position.z;
		ImGui::InputFloat3("Position", v);
		ImGui::Separator();

		v[0] = auxvec3.x; v[1] = auxvec3.y; v[2] = auxvec3.z;
		ImGui::InputFloat3("Rotation", v);
		ImGui::Separator();

		v[0] = scaling.x; v[1] = scaling.y; v[2] = scaling.z;
		ImGui::InputFloat3("Scale", v);
		ImGui::Separator();

		//Additional info

		if (ImGui::TreeNode("Local Matrix")) {

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; j++)
				{
					float auxF = float(lTransformMat.M[j * i + j]);
					ImGui::Text("%f", auxF);
					if (j < 3) ImGui::SameLine();
				}
			}
			ImGui::PopStyleColor();
			ImGui::TreePop();
		}


		ImGui::Separator();
		if (ImGui::TreeNode("Global Matrix")) {
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
			ImGui::TreePop();
		}

		ImGui::Separator();
		ImGui::Unindent();
	}
}

void C_Transform::UpdateGlobalMat()
{
	if (owner->parent != nullptr)
	{
		gTransformMat = owner->parent->GetComponent<C_Transform>()->GetGlobalTransform() * lTransformMat;
	}
	else
		gTransformMat = lTransformMat;
}

aiMatrix4x4 C_Transform::lTraansIntoAssimpMatrix()
{
	aiMatrix4x4 ret(lTransformMat.M[0], lTransformMat.M[1], lTransformMat.M[2], lTransformMat.M[3],
		lTransformMat.M[4], lTransformMat.M[5], lTransformMat.M[6], lTransformMat.M[7],
		lTransformMat.M[8], lTransformMat.M[9], lTransformMat.M[10], lTransformMat.M[11],
		lTransformMat.M[12], lTransformMat.M[13], lTransformMat.M[14], lTransformMat.M[15]);

	return ret;
}

vec3 C_Transform::GetEulerFromQuat(aiQuaterniont<float> rotation)
{
	float qx = rotation.x; float qy = rotation.y; float qz = rotation.z; float qw = rotation.w; float heading; float attitude; float bank;

	attitude = asin(2 * qx * qy + 2 * qz * qw);
	heading = atan2(2 * qy * qw - 2 * qx * qz, 1 - 2 * qy * qy - 2 * qz * qz);
	bank = atan2(2 * qx * qw - 2 * qy * qz, 1 - 2 * qx * qx - 2 * qz * qz);

	if (qx * qy + qz * qw == 0.5)
	{
		heading = 2 * atan2(qx, qw);
		bank = 0;
	}
	if (qx * qy + qz * qw == -0.5)
	{
		heading = -2 * atan2(qx, qw);
		bank = 0;
	}

	return vec3(heading, attitude, bank);
}



