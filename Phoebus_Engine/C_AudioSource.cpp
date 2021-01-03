#include "C_AudioSource.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?
#include <string>
#include "Application.h"
#include "ModuleAudioManager.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "AudioEvent.h"

C_AudioSource::C_AudioSource(GameObject* owner, unsigned int ID) :Component(ComponentType::AUDIO_SOURCE, owner, ID), volume(50.0f)
{
	App->audioManager->RegisterNewAudioObj(this->ID);
}

C_AudioSource::~C_AudioSource()
{
	DeleteAllAudioEvents();
	
	if (App != nullptr && App->audioManager != nullptr)
	{
		App->audioManager->UnRegisterAudioObj(this->ID);
	}
}

void C_AudioSource::OnEditor()
{
	bool activeAux = active;

	std::string headerName = "AudioSource";
	std::string suffixLabel = "##AudioSource";
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
		//TODO actual Component code here
		actualname = "Audio Volume" + suffixLabel;
		if (ImGui::SliderFloat(actualname.c_str(), &volume, 0.0f, 100.0f))
		{
			App->audioManager->ChangeRTPCValue(this->ID, "SourceVolume", volume);
		}


		ImGui::Separator();
		ImGui::Unindent();

		actualname = "Delete AudioSource Component" + suffixLabel;
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
			actualname = "Delete AudioSource Component" + suffixLabel;
			ImGui::OpenPopup(actualname.c_str());


		}

		if (!activeAux)ImGui::PopStyleColor();
		ImGui::PopStyleColor(2);
	}
	if (!activeAux)ImGui::PopStyleColor();

}

bool C_AudioSource::GameUpdate(float gameDT)
{
	if (owner != nullptr)
	{
		C_Transform* transformComp = owner->GetComponent<C_Transform>();
		float4x4 transform = transformComp->GetGlobalTransform();

		App->audioManager->SetAudioObjTransform(this->ID, transform);


	}
	return true;
}

bool C_AudioSource::GameInit()
{
	//SAME AS GAME UPDATE FOR THE MOMENT
	if (owner != nullptr)
	{
		C_Transform* transformComp = owner->GetComponent<C_Transform>();
		float4x4 transform = transformComp->GetGlobalTransform();

		App->audioManager->SetAudioObjTransform(this->ID, transform);
	}

	//different
	App->audioManager->SendAudioObjEvent(this->ID, "Laser_Player");
	return true;
}

float C_AudioSource::GetVolume() const
{
	return volume;
}

void C_AudioSource::SetVolume(float volume)
{
	this->volume = volume;
	App->audioManager->ChangeRTPCValue(ID, "SourceVolume", this->volume);
}

void C_AudioSource::CreateAudioEvent(std::string name, AudioEventType type, float changeTracksTime)
{
	if (!DoesAudioEventExist(name))
	{
		AudioEvent* newEv = new AudioEvent(name, type, changeTracksTime);
		events.push_back(newEv);
	}
	else
	{
		LOG("[error] Could not create an event with name: %s. Reason:", name);
		LOG("[error] There is an event with the same name already created in this component");
	}
}

void C_AudioSource::DeleteAudioEvent(std::string name)
{
	for (int i = 0; i < events.size(); i++)
	{
		if (events[i]->GetEventName() == name)
		{
			AudioEvent* auxEv = events[i];
			events.erase(events.begin() + i);
			delete auxEv;
			auxEv = nullptr;
			break;
		}
	}
}

bool C_AudioSource::DoesAudioEventExist(std::string name)
{
	for (int i = 0; i < events.size(); i++)
	{
		if (events[i]->GetEventName() == name)
		{
			return true;
		}
	}
	return false;
}

void C_AudioSource::DeleteAllAudioEvents()
{
	while (!events.empty())
	{
		AudioEvent* auxEv = events.back();
		events.pop_back();
		delete auxEv;
		auxEv = nullptr;
	}
}

void C_AudioSource::ResetAllAudioEvents()
{
	for (int i = 0; i < events.size(); i++)
	{
		events[i]->ResetPlayed();
	}
}
