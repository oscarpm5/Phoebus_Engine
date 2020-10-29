#include "C_Material.h"
#include "imgui/imgui.h"
#include "Glew/include/glew.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"

C_Material::C_Material(GameObject* owner) :Component(ComponentType::MATERIAL, owner),
idTexture(0), idCheckers(0), width(0), height(0), format(0), depth(0),
sizeInBytes(0), bpp(0), usingCkeckers(false)
{
	GenDefaultTexture();
}

C_Material::C_Material(GameObject* owner, unsigned int ilImageName, const char* path) :Component(ComponentType::MATERIAL, owner),
idTexture(0), idCheckers(0), width(0), height(0), format(0), depth(0),
sizeInBytes(0), bpp(0), usingCkeckers(false)
{
	GenTextureFromName(ilImageName);
	GenDefaultTexture();
}

C_Material::~C_Material()
{
	DestroyTexture();
	DestroyCheckers();
	width = 0;
	height = 0;
	format = 0;
	depth = 0;
	sizeInBytes = 0;
	bpp = 0;
	usingCkeckers = 0;
	path = "";
}

bool C_Material::HasTexture() const
{
	if (idTexture != 0)
		return true;
	return false;
}

bool C_Material::HasCheckers() const
{
	if (idCheckers != 0)
		return true;
	return false;
}

unsigned int C_Material::GetTextureID() const
{
	return idTexture;
}

unsigned int C_Material::GetCheckersID() const
{
	return idCheckers;
}

void C_Material::OnEditor()
{
	if (idTexture == 0) return;

	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		ImGui::Separator();

		ImGui::Text("ID: %i", this->GetTextureID()); 
		ImGui::Text("Size in Bytes: %i ", this->sizeInBytes);
		ImGui::Separator();
		ImGui::Checkbox("Checkers", &usingCkeckers);
		ImGui::Separator();
		if (ImGui::TreeNode("Texture Preview")) 
		{	
			ImGui::SliderInt("Size", &size, 1, 400);
			ImGui::Image((ImTextureID)this->GetTextureID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::TreePop();
		}
		ImGui::Separator();

		ImGui::Text("Path: %s", this->path.c_str());



		ImGui::Separator();
		ImGui::Unindent();
	}
}

void C_Material::GenTextureFromName(unsigned int ilImageName, std::string path)
{
	DestroyTexture();


	ilBindImage(ilImageName);

	//get properties
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);

	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}


	format = ImageInfo.Format;
	/*IL_COLOUR_INDEX
		IL_RGB
		IL_RGBA
		IL_BGR
		IL_BGRA
		IL_LUMINANCE*/

	depth = ImageInfo.Depth;
	bpp = ImageInfo.Bpp;//bytes per pixel
	sizeInBytes = ImageInfo.SizeOfData;//bytes
	width = ImageInfo.Width;
	height = ImageInfo.Height;

	//set name
	this->path = path;

	//gen texture

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	glBindTexture(GL_TEXTURE_2D, 0);

	ilBindImage(0);
}

void C_Material::DestroyTexture()
{
	if (idTexture != 0)
	{
		glDeleteTextures(1, &idTexture);
		idTexture = 0;
	}
}

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
	this->path = "Default texture - no path";
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
