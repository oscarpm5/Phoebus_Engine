#include "C_Material.h"
#include "imgui/imgui.h"
#include "Glew/include/glew.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "Texture.h"
#include "Application.h"
#include "ModuleResourceManager.h"

C_Material::C_Material(GameObject* owner, unsigned int ID, Color color) :Component(ComponentType::MATERIAL, owner, ID),
idCheckers(0), //width(0), height(0), format(0), depth(0),idTexture(0),sizeInBytes(0), bpp(0), 
usingCkeckers(false), resourceID(0)
{
	matCol = color;
	GenDefaultTexture();
}

//TODO deprecated constructor???
//C_Material::C_Material(GameObject* owner, unsigned int ilImageName, const char* path) :Component(ComponentType::MATERIAL, owner),
//idCheckers(0),//idTexture(0),  width(0), height(0), format(0), depth(0),sizeInBytes(0), bpp(0), 
//usingCkeckers(false), resourceID(0)
//{
//	//GenTextureFromName(ilImageName);
//	GenDefaultTexture();
//}

C_Material::~C_Material()
{
	//DestroyTexture();
	DestroyCheckers();
	//width = 0;
	//height = 0;
	//format = 0;
	//depth = 0;
	//sizeInBytes = 0;
	//bpp = 0;
	usingCkeckers = 0;

	if (resourceID != 0)
	{
		App->rManager->StopUsingResource(resourceID);
		resourceID = 0;
	}
}

//DO NOT use RequestNewResource() method when calling this method, it does it for you
void C_Material::SetNewResource(unsigned int resourceUID)
{
	if (resourceID != 0)
	{
		App->rManager->StopUsingResource(resourceID);
	}

	Resource* r = App->rManager->RequestNewResource(resourceUID);
	if (r != nullptr)
	{
		resourceID = resourceUID;
	}
	else
	{
		resourceID = 0;
		LOG("[error] the resource with ID:%i, given to this component doesn't exist", resourceUID);
	}
}

unsigned int C_Material::GetResourceID()
{
	return resourceID;
}

bool C_Material::HasTexture()
{
	if (resourceID != 0)
	{
		ResourceTexture* t = (ResourceTexture*)App->rManager->RequestExistingResource(resourceID);
		if (t == nullptr)
		{
			resourceID = 0;
		}
		if (t != nullptr && t->idTexture != 0)
			return true;
	}
	return false;
}

bool C_Material::HasCheckers() const
{
	if (idCheckers != 0)
		return true;
	return false;
}

unsigned int C_Material::GetTextureID() //if 0 then failed to load resource (aka the component ahs no resource associated)
{
	if (resourceID != 0)
	{
		ResourceTexture* t = (ResourceTexture*)App->rManager->RequestExistingResource(resourceID);
		if (t == nullptr)
		{
			resourceID = 0;
		}
		else
		{
			return t->GetTextureID();
		}
	}

	return 0;
}

ResourceTexture* C_Material::GetTexture()
{
	if (resourceID != 0)
	{
		ResourceTexture* t = (ResourceTexture*)App->rManager->RequestExistingResource(resourceID);
		if (t == nullptr)
		{
			resourceID = 0;
		}
		else
		{
			return t;
		}
	}

	return nullptr;
}

unsigned int C_Material::GetCheckersID() const
{
	return idCheckers;
}

void C_Material::OnEditor()
{
	bool activeAux = active;

	std::string headerName = "Texture";
	if (!activeAux)headerName += " (not active)";

	if (!activeAux)ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

	if (ImGui::CollapsingHeader(headerName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!activeAux)ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 0.75f, 0.8f));
		ImGui::Checkbox("IS ACTIVE##MaterialCheckbox", &active); //##adds more to the label id without displaying so we can have 2 checkbox with the same text

		ResourceTexture* texture = nullptr;
		texture = GetTexture();

		//===========================================

		//get the items here

		std::string textNameDisplay = "No Selected Texture";
		unsigned int myResourceID = 0;
		if (texture != nullptr)
		{
			textNameDisplay = texture->GetAssetFile();
			myResourceID = texture->GetTextureID();
		}

		std::vector<Resource*> allLoadedTextures;
		App->rManager->GetAllResourcesOfType(ResourceType::TEXTURE, allLoadedTextures);
		//const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
		//static int item_current_idx = 0;                    // Here our selection data is an index.
		//const char* combo_label = textNameDisplay.c_str();  // Label to preview before opening the combo (technically it could be anything)
		if (ImGui::BeginCombo("Used Texture##texture", textNameDisplay.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{


			const bool noneSelected = (texture == nullptr);
			if (ImGui::Selectable("NONE##texture", noneSelected))
			{
				if (texture != nullptr)
				{
					App->rManager->StopUsingResource(resourceID);
					resourceID = 0;
					myResourceID = 0;
					texture == nullptr;

				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (noneSelected)
				ImGui::SetItemDefaultFocus();

			//================================
			for (int n = 0; n < allLoadedTextures.size(); n++)
			{
				std::string name = allLoadedTextures[n]->GetAssetFile();
				name += "##";
				name += "textureList";
				name += std::to_string(n);
				const bool is_selected = (myResourceID == allLoadedTextures[n]->GetUID());
				if (ImGui::Selectable(name.c_str(), is_selected))
				{
					SetNewResource(allLoadedTextures[n]->GetUID());
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		//===========================================

//begin drag drop target
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("ResourceTex##dragdropSource"))
			{
				IM_ASSERT(payload->DataSize == sizeof(unsigned int));
				unsigned int payloadID = *(const int*)payload->Data;


				if (payloadID != 0 && payloadID != resourceID)
				{
					SetNewResource(payloadID);
				}
			}
			ImGui::EndDragDropTarget();

		}
		//end drag drop target

		ImGui::Indent();
		ImGui::Separator();
		if (texture != nullptr)
		{
			ImGui::Text("ID: %i", this->GetTextureID());
			ImGui::Text("Size in Bytes: %i ", texture->sizeInBytes);
		}
		ImGui::Separator();
		ImGui::Checkbox("Checkers", &usingCkeckers);
		ImGui::Separator();

		if (usingCkeckers || texture != nullptr)
		{
			if (ImGui::TreeNode("Texture Preview"))
			{
				ImGui::SliderInt("Size", &size, 1, 400);
				if (usingCkeckers)
					ImGui::Image((ImTextureID)this->GetCheckersID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));
				else if (texture != nullptr)
					ImGui::Image((ImTextureID)this->GetTextureID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));

				ImGui::TreePop();
			}
		}
		ImGui::Separator();

		if (ImGui::TreeNode("Selet Material Color"))
		{
			ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview;
			ImGui::ColorPicker4("MyColor##4", (float*)& matCol, flags);
			ImGui::TreePop();
		}

		ImGui::Separator();
		ImGui::Unindent();

		if (ImGui::BeginPopup("Delete Material Component", ImGuiWindowFlags_AlwaysAutoResize))
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

		if (ImGui::Button("Delete##Material"))
		{
			ImGui::OpenPopup("Delete Material Component");
		}


		if (!activeAux)ImGui::PopStyleColor();
		ImGui::PopStyleColor(2);
	}

	if (!activeAux)ImGui::PopStyleColor();

}
//
//void C_Material::GenTextureFromName(unsigned int ilImageName, std::string path)
//{
//	DestroyTexture();
//
//
//	ilBindImage(ilImageName);
//
//	//get properties
//	ILinfo ImageInfo;
//	iluGetImageInfo(&ImageInfo);
//
//	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
//	{
//		iluFlipImage();
//	}
//
//
//	format = ImageInfo.Format;
//	/*IL_COLOUR_INDEX
//		IL_RGB
//		IL_RGBA
//		IL_BGR
//		IL_BGRA
//		IL_LUMINANCE*/
//
//	depth = ImageInfo.Depth;
//	bpp = ImageInfo.Bpp;//bytes per pixel
//	sizeInBytes = ImageInfo.SizeOfData;//bytes
//	width = ImageInfo.Width;
//	height = ImageInfo.Height;
//
//	//set name
//	this->path = path;
//
//	//gen texture
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	glGenTextures(1, &idTexture);
//	glBindTexture(GL_TEXTURE_2D, idTexture);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	ilBindImage(0);
//}
//
//void C_Material::DestroyTexture()
//{
//	if (idTexture != 0)
//	{
//		glDeleteTextures(1, &idTexture);
//		idTexture = 0;
//	}
//}

void C_Material::DestroyCheckers()
{
	if (idCheckers != 0)
	{
		glDeleteTextures(1, &idCheckers);
		idCheckers = 0;
	}
}

void C_Material::GenDefaultTexture()
{
	DestroyCheckers();
	//this->path = "Default texture - no path";
	const int checkersHeight = 100;
	const int checkersWidth = 100;

	GLubyte checkerImage[checkersHeight][checkersWidth][4];
	for (int i = 0; i < checkersWidth; i++) {
		for (int j = 0; j < checkersWidth; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &idCheckers);
	glBindTexture(GL_TEXTURE_2D, idCheckers);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkersWidth, checkersHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}
