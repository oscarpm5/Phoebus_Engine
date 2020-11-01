#include "glmath.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?
#include "Assimp/include/matrix4x4.inl"
#include "Assimp/include/vector3.h"
#include "MathGeoLib/include/MathGeoLib.h"


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


		float v[3];
		//Show me the things
		v[0]= lTransformMat.translation().x;
		v[1] = lTransformMat.translation().y;
		v[2] = lTransformMat.translation().z;
		
		if (ImGui::DragFloat3("Position", v, 0.1f))
		{
			SetLocalPosition(vec3(v[0],v[1],v[2]));
		}
		ImGui::Separator();

		//de matriz a euler
		//display
		//de euler a matriz
		float3x3 rotMat = GetRotationMat();
		float3 rotEuler=rotMat.ToEulerXYZ();
		
		v[0] = RadToDeg(rotEuler.x);
		v[1] = RadToDeg(rotEuler.y);
		v[2] = RadToDeg(rotEuler.z);

		if (ImGui::DragFloat3("Rotation", v, 0.1f,-360.0f,360.0f))
		{

			rotEuler.x = DegToRad(v[0]);
			rotEuler.y = DegToRad(v[1]);
			rotEuler.z = DegToRad(v[2]);

			rotMat = float3x3::FromEulerXYZ(rotEuler.x, rotEuler.y, rotEuler.z);
			SetLocalRot(rotMat);

		}
		ImGui::Separator();

		v[0] = lTransformMat[0];
		v[1] = lTransformMat[5];
		v[2] = lTransformMat[10];
		if (ImGui::DragFloat3("Scale", v, 0.1f))
		{

			SetLocalScale(vec3(v[0], v[1], v[2]));
		}

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

vec3 C_Transform::GetLocalPosition()
{
	return vec3(lTransformMat.M[12], lTransformMat.M[13], lTransformMat.M[14]);
}

vec3 C_Transform::GetGlobalPosition()
{
	return vec3(gTransformMat.M[12], gTransformMat.M[13], gTransformMat.M[14]);
}

vec3 C_Transform::GetLocalScale()
{
	return vec3(lTransformMat.M[0], lTransformMat.M[5], lTransformMat.M[10]);
}

float3x3 C_Transform::GetRotationMat()
{
	float3x3 rotMat;
	rotMat.v[0][0] = lTransformMat[0];
	rotMat.v[0][1] = lTransformMat[1];
	rotMat.v[0][2] = lTransformMat[2];
	rotMat.v[1][0] = lTransformMat[4];
	rotMat.v[1][1] = lTransformMat[5];
	rotMat.v[1][2] = lTransformMat[6];
	rotMat.v[2][0] = lTransformMat[8];
	rotMat.v[2][1] = lTransformMat[9];
	rotMat.v[2][2] = lTransformMat[10];

	//rotMat.Transpose();
	//we suppose this matrix is normalized and affine

	return rotMat;
}

void C_Transform::SetLocalPosition(vec3 newPos)
{
	lTransformMat.translate(newPos.x, newPos.y, newPos.z);
	UpdateGlobalMat();
	owner->UpdateChildTransforms();
}

void C_Transform::SetLocalScale(vec3 newScale)
{
	lTransformMat.M[0] = newScale.x;
	lTransformMat.M[5] = newScale.y;
	lTransformMat.M[10] = newScale.z;

	UpdateGlobalMat();
	owner->UpdateChildTransforms();

}

void C_Transform::SetLocalRot(float3x3 newRot)
{
	lTransformMat[0]=newRot.v[0][0];
	lTransformMat[1]=newRot.v[0][1];
	lTransformMat[2]=newRot.v[0][2];

	lTransformMat[4]=newRot.v[1][0];
	lTransformMat[5]=newRot.v[1][1];
	lTransformMat[6]=newRot.v[1][2];

	lTransformMat[8]=newRot.v[2][0];
	lTransformMat[9]=newRot.v[2][1];
	lTransformMat[10]=newRot.v[2][2];

	UpdateGlobalMat();
	owner->UpdateChildTransforms();
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

	ret.Transpose();
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

mat4x4 C_Transform::RottoTrans(float3x3 rot)
{
	mat4x4 ret = mat4x4();

	ret[0] = rot.v[0][0];
	ret[1] = rot.v[0][1];
	ret[2] = rot.v[0][2];

	ret[4] = rot.v[1][0];
	ret[5] = rot.v[1][1];
	ret[6] = rot.v[1][2];

	ret[8] = rot.v[2][0];
	ret[9] = rot.v[2][1];
	ret[10] = rot.v[2][2];

	return ret;
}



