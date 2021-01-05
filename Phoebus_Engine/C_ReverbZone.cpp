//#include "C_AudioSource.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?
#include "imgui/imgui_internal.h"
#include <string>
#include "Application.h"
#include "ModuleAudioManager.h"
#include "GameObject.h"
#include "C_ReverbZone.h"
#include "C_Transform.h"
#include "ModuleRenderer3D.h"

C_ReverbZone::C_ReverbZone(GameObject* owner, unsigned int ID) : Component(ComponentType::REVERB_ZONE, owner, ID),
dimensions(1.0f, 1.0f, 1.0f), dirtyUpdate(false), targetBus("ReverbBus"), revValue(1)
{
	UpdateReverbZoneDimension();
	App->audioManager->AddRevZone(this);
}

C_ReverbZone::~C_ReverbZone()
{
	App->audioManager->RemoveRevZone(this);
}

void C_ReverbZone::OnEditor()
{
	bool activeAux = active;

	std::string headerName = "ReverbZone";
	std::string suffixLabel = "##ReverbZone";
	std::string actualname;
	suffixLabel += std::to_string(ID);
	if (!activeAux)headerName += " (not active)";

	ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

	if (!activeAux)ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

	if (ImGui::CollapsingHeader(headerName.c_str(), headerFlags))
	{
		if (!activeAux)ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 0.75f, 0.8f));
		ImGui::Spacing();

		actualname = "IS ACTIVE" + suffixLabel + "Checkbox";
		ImGui::Checkbox(actualname.c_str(), &active);

		ImGui::Separator();
		ImGui::Indent();
		ImGui::Spacing();
		ImGui::Spacing();
		//TODO actual Component code here

		actualname = "Dimensions" + suffixLabel;
		float auxDimensions[3] = { dimensions.x,dimensions.y,dimensions.z };

		if (ImGui::DragFloat3(actualname.c_str(), auxDimensions, 0.1f, 0.0f, 1000.0f))
		{
			SetReverbZone(float3(auxDimensions[0], auxDimensions[1], auxDimensions[2]));
			dirtyUpdate = true;//Dirty update not needed??
		}
		/*if (ImGui::SliderFloat(actualname.c_str(), &radius, 0.0f, 100.0f))
		{
			dirtyUpdate = true;
		}*/

		//Thanks to Carlos Cabreira for clarifying the "targetBus" logic
		char* bus_name = new char[41];
		std::copy(targetBus.begin(), targetBus.end(), bus_name);
		bus_name[targetBus.length()] = '\0';
		actualname = "Target Bus" + suffixLabel;
		ImGui::InputText(actualname.c_str(), bus_name, 40);
		targetBus = bus_name;
		delete[] bus_name;

		actualname = "Reverb Intensity" + suffixLabel;

		ImGui::DragFloat(actualname.c_str(), &revValue, 0.1, 0.0, 12.0, "%.1f");

		ImGui::Separator();
		ImGui::Unindent();

		actualname = "Delete ReverbZone Component" + suffixLabel;
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
		actualname = "Delete" + suffixLabel + "component";
		if (ImGui::Button(actualname.c_str()))
		{
			actualname = "Delete ReverbZone Component" + suffixLabel;
			ImGui::OpenPopup(actualname.c_str());
			dirtyUpdate = true;
		}

		if (!activeAux)ImGui::PopStyleColor();
		ImGui::PopStyleColor(2);
	}
	if (!activeAux)ImGui::PopStyleColor();
}

bool C_ReverbZone::GameUpdate(float gameDT)
{
	return true;
}

bool C_ReverbZone::GameInit()
{
	return true;
}

bool C_ReverbZone::Update(float dt)
{
	/*if (dirtyUpdate == true) {
		dirtyUpdate = false;
		UpdateReverbZoneDimension();
	}*/
	UpdateReverbZoneDimension();//we will update its position every frame to make it easy for now


	if (active)
	{
		std::vector<float3> aabbVec;
		GetAABBPoints(revZone, aabbVec);
		App->renderer3D->AddBoxToDraw(aabbVec, Color(Red));
	}

	return true;
}

void C_ReverbZone::SetReverbZone(float3 dimensions)
{
	this->dimensions = dimensions;
}

void C_ReverbZone::UpdateReverbZoneDimension()
{
	float3 auxPos = float3::zero;
	if (this->owner && this->owner->GetComponent<C_Transform>())
	{
		auxPos = this->owner->GetComponent<C_Transform>()->GetGlobalPosition();
	}

		//should we clear memory of previous AABB or since we're rewriting over it it's not necessary? TODO -> (OSCAR) Not necessary if we do not use Enclose()
		//revZone = AABB(Sphere(this->owner->GetComponent<C_Transform>()->GetGlobalPosition(), radius));
	revZone = AABB::FromCenterAndSize(auxPos, dimensions);
}

void C_ReverbZone::GetAABBPoints(AABB& aabb, std::vector<float3>& emptyVector)//TODO This code is copied from somewhere else in the code??? consider making a single method
{
	float3* frustrumPoints = new float3[8];
	memset(frustrumPoints, NULL, sizeof(float3) * 8);
	aabb.GetCornerPoints(frustrumPoints);

	emptyVector.clear();

	for (int i = 0; i < 8; i++)
	{
		emptyVector.push_back(frustrumPoints[i]);
	}
	delete[]frustrumPoints;
	frustrumPoints = nullptr;
}

bool C_ReverbZone::DoesReverbZoneContainPoint(float3 point) const
{
	return revZone.Contains(point);
}

float3 C_ReverbZone::GetDimensions() const
{
	return dimensions;
}
