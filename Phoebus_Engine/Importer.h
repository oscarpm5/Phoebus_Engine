#include <string>
struct aiMesh;
class GameObject;

namespace Importer
{
	//bool LoadFBX(const char* path);
	bool InitializeDevIL();
	//bool LoadNewImage(const char* path);
	bool LoadNewImageFromBuffer(const char* Buffer,unsigned int Length);

	//Take a saved buffer in memory and load it
	bool LoadFBXfromBuffer(const char* Buffer, unsigned int Length); //lenght of the buffer, in bytes

	GameObject* LoadGameObjFromAiMesh(aiMesh* _mesh,GameObject* parent,std::string optName="");//optName lets the node name to be sent to the gameobj
}