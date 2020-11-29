#include "C_Camera.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "imgui/imgui.h"

#include "Application.h"

C_Camera::C_Camera(GameObject* owner, unsigned int ID) :Component(ComponentType::CAMERA, owner, ID), isCulling(false),
nearPlaneDist(0.1f), farPlaneDist(500.0f), FoVx(70.0f), FoVy(0.0f), invAspectRatio(0),
projectionMatrix(float4x4::identity)
{
	frustum.type = math::FrustumType::PerspectiveFrustum;

	float2 screenSize;

	if (App != nullptr)
		App->renderer2D->GetViewportRectUI(float2(), screenSize);
	else
		screenSize = float2(1920, 1080);

	SetNewAspectRatio(screenSize.x, screenSize.y);
}

C_Camera::C_Camera(GameObject* owner, float nPlaneDist, float fPlaneDist, float foV, float aspectRatio) :Component(ComponentType::CAMERA, owner),
nearPlaneDist(nPlaneDist), farPlaneDist(fPlaneDist), FoVx(foV), FoVy(0.0f), invAspectRatio(0), projectionMatrix(float4x4::identity),isCulling(false)
{
	frustum.type = math::FrustumType::PerspectiveFrustum;

	float2 screenSize;

	if (App != nullptr)
		App->renderer2D->GetViewportRectUI(float2(), screenSize);
	else
		screenSize = float2(1920, 1080);

	if (aspectRatio != 0.0f)
	{
		SetNewAspectRatio(aspectRatio);
	}
	else
	{
		SetNewAspectRatio(screenSize.x, screenSize.y);
	}
}

C_Camera::~C_Camera()
{
	if (App != nullptr && App->renderer3D->activeCam == this)//TODO this seems not ok... we dont want camera to be dependant on renderer 3d
	{
		App->renderer3D->activeCam = nullptr;
	}
}

bool C_Camera::Update(float dt)
{

	if (owner != nullptr)
	{
		CalcCamPosFromTransformMat(owner->GetComponent<C_Transform>()->GetGlobalTransform());
	}

	return true;
}

void C_Camera::OnEditor()
{
	bool activeAux = active;

	std::string headerName = "Camera";
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
		ImGui::Checkbox("IS ACTIVE##CamCheckbox", &active);

		ImGui::Separator();
		ImGui::Indent();
		ImGui::Spacing();
		ImGui::Spacing();

		//TODO camera things here
		bool cullingAux = isCulling;
		if (ImGui::Checkbox("Culling Cam##CamCheckbox", &cullingAux))
		{
			SetAsCullingCam(cullingAux);
		}

		float auxf = GetFoV();
		if (ImGui::DragFloat("FoV##CamFoVx", &auxf, 0.1f, 1.0f, 180.0f))
		{
			SetNewFoV(auxf);
		}
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		//Near plane
		auxf = GetNearPlaneDist();
		int flags = ImGuiSliderFlags_Logarithmic;

		if (ImGui::DragFloat("NearPlaneDist##CamNearDist", &auxf, 0.0f, 0.01f, GetFarPlaneDist(), "%.3f", flags))
		{
			SetNearPlane(auxf);
		}
		//flags = ImGuiSliderFlags_None;
		//Far plane
		auxf = GetFarPlaneDist();
		if (ImGui::DragFloat("FarPlaneDist##CamFarDist", &auxf, 0.0f, 0.1f, 20000.0f, "%.3f", flags))
		{
			SetFarPlane(auxf);
		}
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		//aspectRatio
		auxf = GetAspectRatio();
		if (ImGui::DragFloat("AspectRatio##CamAspectRatio", &auxf, 0.1f, 0.01f, 10.0f, "%.3f", flags))
		{
			SetNewAspectRatio(auxf);
		}


		//camera things end here
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Unindent();

		if (ImGui::BeginPopup("Delete Camera", ImGuiWindowFlags_AlwaysAutoResize))
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

		if (ImGui::Button("Delete##Camera"))
		{
			ImGui::OpenPopup("Delete Camera");
		}

		if (!activeAux)ImGui::PopStyleColor();
		ImGui::PopStyleColor(2);

	}

	if (!activeAux)ImGui::PopStyleColor();
}

void C_Camera::CalcCamPosFromTransformMat(float4x4& gTransform)
{
	//TODO this global mat might have to be transposed?
	frustum.pos = gTransform.TranslatePart();
	frustum.front = gTransform.Col3(2); // The camera looks towards +Z axis of the given transform.
	frustum.up = gTransform.Col3(1); // The camera up points towards +Y of the given transform.
	assume(pos.IsFinite());
	assume(front.IsNormalized());
	assume(up.IsNormalized());
	assume(gTransform.IsColOrthogonal3()); // Front and up must be orthogonal to each other.
	assume(EqualAbs(gTransform.Determinant(), 1.f)); // The matrix cannot contain mirroring.
}

void C_Camera::CalcCamPosFromDirections(float3 pos, float3 front, float3 up)
{
	float3 normFront = front.Normalized();
	float3 normUP = up.Normalized();
	//TODO this global mat might have to be transposed?
	frustum.pos = pos;
	frustum.front = -normFront; // The camera looks towards -Z axis of the given transform. TODO Z+ invert cam
	frustum.up = normUP; // The camera up points towards +Y of the given transform.
	assume(pos.IsFinite());
	assume(front.IsNormalized());
	assume(up.IsNormalized());
}

float4x4 C_Camera::GetProjMat() const
{
	return projectionMatrix;
}

float4x4 C_Camera::GetViewMat() const
{
	float4x4 viewMat = frustum.ViewMatrix();
	return viewMat.Transposed();
}

void C_Camera::SetNewAspectRatio(int width, int height)
{
	float newW = max(width, 0.01f);
	float newH = max(height, 0.01f);

	invAspectRatio = newH / newW;
	SetNewFoV(FoVx);
}

void C_Camera::SetNewAspectRatio(float aspectRatio)
{
	aspectRatio = max(aspectRatio, 0.1f);
	aspectRatio = min(aspectRatio, 10.0f);

	invAspectRatio = 1 / aspectRatio;
	SetNewFoV(FoVx);
}

void C_Camera::SetNewFoV(float foV)
{

	FoVx = max(foV, 1.0f);
	FoVx = min(FoVx, 180.0f);

	float aux = Tan(DegToRad(FoVx / 2));
	aux *= invAspectRatio;

	FoVy = RadToDeg(2 * Atan(aux));//vertical aspect ratio calculates from horizontal one

	UpdateProjectionMat();

}

void C_Camera::SetNearPlane(float dist)
{
	nearPlaneDist = max(dist, 0.0f);

	if (nearPlaneDist > farPlaneDist)
	{
		SetFarPlane(nearPlaneDist);
	}
	else
	{
		UpdateProjectionMat();
	}

}

void C_Camera::SetFarPlane(float dist)
{
	farPlaneDist = max(dist, 0.0f);

	if (farPlaneDist < nearPlaneDist)
	{
		SetNearPlane(farPlaneDist);
	}
	else
	{
		UpdateProjectionMat();
	}
}

float C_Camera::GetNearPlaneDist() const
{
	return frustum.nearPlaneDistance;
}

float C_Camera::GetFarPlaneDist() const
{
	return frustum.farPlaneDistance;
}

float C_Camera::GetFoV() const
{
	return RadToDeg(frustum.horizontalFov);
}

float C_Camera::GetAspectRatio() const
{

	return 1 / invAspectRatio;
}

float C_Camera::GetInvAspectRatio() const
{
	return invAspectRatio;
}

const Frustum& C_Camera::GetFrustum() const
{
	return frustum;
}

void C_Camera::GetFrustumPoints(std::vector<float3>& emptyVector)
{
	float3* frustrumPoints = new float3[8];
	memset(frustrumPoints, NULL, sizeof(float3) * 8);
	frustum.GetCornerPoints(frustrumPoints);

	emptyVector.clear();

	for (int i = 0; i < 8; i++)
	{
		emptyVector.push_back(frustrumPoints[i]);
	}
	delete[]frustrumPoints;
	frustrumPoints = nullptr;

}

bool C_Camera::GetIsCulling() const
{
	return isCulling;
}
//TODO add things to this
void C_Camera::SetAsCullingCam(bool newState)
{
	if (newState != isCulling)
	{

		if (newState)
		{
			if (App->renderer3D->activeCam != nullptr)
			{
				App->renderer3D->activeCam->SetAsCullingCam(false);
			}
			App->renderer3D->activeCam = this;
		}
		else if(App->renderer3D->activeCam==this)
		{
			App->renderer3D->activeCam = nullptr;
		}

		isCulling = newState;
	}
}

void C_Camera::UpdateProjectionMat()
{
	frustum.nearPlaneDistance = nearPlaneDist;
	frustum.farPlaneDistance = farPlaneDist;
	frustum.horizontalFov = DegToRad(FoVx);
	frustum.verticalFov = DegToRad(FoVy);
	projectionMatrix = frustum.ProjectionMatrix().Transposed();

}
