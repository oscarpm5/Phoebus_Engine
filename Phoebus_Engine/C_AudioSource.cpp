#include "C_AudioSource.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?
#include "imgui/imgui_internal.h"
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
		ImGui::Spacing();
		ImGui::Spacing();

		//TODO actual Component code here

		bool disableButtons = true;
		if (App->GetGameState() == GameStateEnum::STOPPED)//makes it so the audio events can only be edited when in offline mode
		{
			disableButtons = false;
		}
		if (disableButtons)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.5f, 1.0f, 1.0f));
		}

		actualname = "New event name";
		ImGui::InputTextWithHint(suffixLabel.c_str(), actualname.c_str(), newEvName, IM_ARRAYSIZE(newEvName), ImGuiInputTextFlags_CharsNoBlank);
		actualname = "Create SFX" + suffixLabel;
		if (ImGui::Button(actualname.c_str()))
		{
			std::string evName = newEvName;
			if (!evName.empty())
				CreateAudioEvent(evName, AudioEventType::SFX);
			else
				LOG("[warning] Please enter a valid name");
		}
		ImGui::SameLine();
		actualname = "Create MusicBlend" + suffixLabel;
		if (ImGui::Button(actualname.c_str()))
		{
			std::string evName = newEvName;
			if (!evName.empty())
				CreateAudioEvent(evName, AudioEventType::MUSIC);
			else
				LOG("[warning] Please enter a valid name");
		}
		ImGui::Spacing();
		ImGui::Separator();


		for (int i = 0; i < events.size(); i++)
		{
			ImGui::Spacing();
			ImGui::Spacing();
			//Print all events here
			ImGui::Columns(2, 0, false);
			float maxWidth = ImGui::GetWindowContentRegionMax().x;
			ImGui::SetColumnWidth(-1, maxWidth - 125);
			actualname = "Event:  " + events[i]->GetEventName();
			if (ImGui::CollapsingHeader(actualname.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth))
			{
				if (events[i]->GetType() == AudioEventType::SFX)
				{
					actualname = "Type: Sound Effect";
					ImGui::Text(actualname.c_str());
					actualname = "Delete Event" + suffixLabel + events[i]->GetEventName();
					if (ImGui::Button(actualname.c_str()))
					{
						DeleteAudioEvent(events[i]->GetEventName());
					}
				}
				else if (events[i]->GetType() == AudioEventType::MUSIC)
				{
					actualname = "Type: Music";
					ImGui::Text(actualname.c_str());
					actualname = "Delete Event" + suffixLabel + events[i]->GetEventName();
					if (ImGui::Button(actualname.c_str()))
					{
						DeleteAudioEvent(events[i]->GetEventName());
					}
				}
			}
			ImGui::NextColumn();
			ImGui::SetColumnWidth(-1, 125);
			actualname = "Play" + suffixLabel+ events[i]->GetEventName();
			if (ImGui::Button(actualname.c_str()))
			{
				App->audioManager->SendStopEvent(this->ID, events[i]->GetEventName());
				App->audioManager->SendAudioObjEvent(this->ID, events[i]->GetEventName());
			}
			ImGui::SameLine();
			actualname = "Stop" + suffixLabel+ events[i]->GetEventName();
			if (ImGui::Button(actualname.c_str()))
			{
				App->audioManager->SendStopEvent(this->ID, events[i]->GetEventName());
			}
			ImGui::Columns();



		}
		if (disableButtons)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

		ImGui::Spacing();
		ImGui::Separator();

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

		float secs =60 * gameDT * App->GetTimeScale();//TODO adjust the formula THIS IS JSUT A PLACEHOLDER
		float overallPitch = (secs * 100);//TODO take into account user pitch too
		App->audioManager->ChangeRTPCValue(this->ID, "SoundPitch", overallPitch);

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



		for (int i = 0; i < events.size(); i++)
		{
			if (events[i]->GetPlayed() == false)
			{
				App->audioManager->SendAudioObjEvent(this->ID, events[i]->GetEventName());
				events[i]->StartPlaying();
			}
		}



	}

	//different
	//App->audioManager->SendAudioObjEvent(this->ID, "Laser_Player");
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
		LOG("[error] Could not create an event with name: %s. Reason:", name.c_str());
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

const std::vector<AudioEvent*> C_AudioSource::GetEvents() const
{
	return events;
}
