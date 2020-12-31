#include "C_AudioListener.h"
#include "imgui/imgui.h" //On Editor usage. TODO: cant this be done in another way to not have this here?
#include <string>
#include "Application.h"
#include "ModuleAudioManager.h"

C_AudioListener::C_AudioListener(GameObject* owner, unsigned int ID) :Component(ComponentType::AUDIO_LISTENER, owner, ID),isListener(false)
{
}

C_AudioListener::~C_AudioListener()
{
	if (App != nullptr && App->audioManager->activeListener == this)
	{
		App->audioManager->activeListener = nullptr;
	}
}

void C_AudioListener::SetAsListener(bool newState)
{
	if (newState != isListener)
	{

		if (newState)
		{
			if (App->audioManager->activeListener != nullptr)
			{
				App->audioManager->activeListener->SetAsListener(false);
			}
			App->audioManager->activeListener = this;
		}
		else if (App->audioManager->activeListener == this)
		{
			App->audioManager->activeListener = nullptr;
		}

		isListener = newState;
	}
}

bool C_AudioListener::GetIsListener() const
{
	return isListener;
}

void C_AudioListener::OnEditor()
{

	bool activeAux = active;

	std::string headerName = "AudioListener";
	std::string suffixLabel = "##AudioListener";
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

		bool listenerAux = isListener;
		if (ImGui::Checkbox("Is Current Listener##ListenerCheckbox", &listenerAux))
		{
			SetAsListener(listenerAux);
		}

		ImGui::Separator();
		ImGui::Unindent();

		actualname = "Delete AudioListener Component" + suffixLabel;
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
			actualname = "Delete AudioListener Component" + suffixLabel;
			ImGui::OpenPopup(actualname.c_str());


		}

		if (!activeAux)ImGui::PopStyleColor();
		ImGui::PopStyleColor(2);
	}
	if (!activeAux)ImGui::PopStyleColor();


}
