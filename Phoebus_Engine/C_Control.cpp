#include "C_Control.h"
#include "Application.h"
#include "ModuleEditor3D.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "ModuleInput.h"

C_Control::C_Control(GameObject* owner, unsigned int ID) :Component(ComponentType::CONTROL, owner, ID), speed(1)
{
	//App->editor3d->selectedGameObjs.push_back(owner);
}

C_Control::~C_Control()
{
}

void C_Control::OnEditor()
{

	bool activeAux = active;

	std::string headerName = "Control";
	std::string suffixLabel = "##Control";
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

		actualname = "Speed" + suffixLabel;
		if (ImGui::SliderFloat(actualname.c_str(), &speed, 0.0f, 10.0f))
		{
			if (speed < 0) speed = 0;
		}

		ImGui::Separator();
		ImGui::Unindent();

		actualname = "Delete Control Component" + suffixLabel;
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
			actualname = "Delete Control Component" + suffixLabel;
			ImGui::OpenPopup(actualname.c_str());
		}

		if (!activeAux)ImGui::PopStyleColor();
		ImGui::PopStyleColor(2);
	}
	if (!activeAux)ImGui::PopStyleColor();
}

bool C_Control::GameUpdate(float gameDT)
{
	if (this->IsActive())
	{
		ManageMovement(gameDT);
	}
	return true;
}

bool C_Control::GameInit()
{
	//speed = 1;
	return true;
}

bool C_Control::ManageMovement(float dt)
{
	bool ret = false;
	float3 auxPos(0.0f,0.0f,0.0f);
	float auxSpeed = speed * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		auxSpeed *= 3;
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		auxPos.z -= auxSpeed;
		ret = true;
		//owner->GetComponent<C_Transform>()->SetGlobalPosition(float3(auxPos.x, auxPos.y, auxPos.z - speed * dt));
		//owner->UpdateChildTransforms();
	}
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		auxPos.z += auxSpeed;
		ret = true;
		//owner->GetComponent<C_Transform>()->SetGlobalPosition(float3(auxPos.x, auxPos.y, auxPos.z + speed * dt));
		//owner->UpdateChildTransforms();
	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		auxPos.x += auxSpeed;
		ret = true;
		//owner->GetComponent<C_Transform>()->SetGlobalPosition(float3(auxPos.x + speed * dt, auxPos.y, auxPos.z));
		//owner->UpdateChildTransforms();
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		auxPos.x -= auxSpeed;
		ret = true;
		//owner->GetComponent<C_Transform>()->SetGlobalPosition(float3(auxPos.x - speed * dt, auxPos.y, auxPos.z));
		//owner->UpdateChildTransforms();
	}

	if (ret == true) {
		auxPos = owner->GetComponent<C_Transform>()->GetGlobalPosition() + auxPos;
		owner->GetComponent<C_Transform>()->SetGlobalPosition(auxPos);
		//owner->UpdateChildTransforms();
	}

	return ret;
}

bool C_Control::Update(float dt)
{
	return true;
}

float C_Control::GetSpeed() const
{
	return speed;
}

void C_Control::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}
