#include <string>
struct aiMesh;
struct aiNode;
struct aiScene;
class GameObject;
class Mesh;

namespace Importer
{
	//bool LoadFBX(const char* path);
	bool InitializeDevIL();
	//bool LoadNewImage(const char* path);
	bool LoadNewImageFromBuffer(const char* Buffer, unsigned int Length, std::string path = ""); //path for display
	bool LoadNewImageFromObj(const char* Buffer, unsigned int Length, GameObject* target, std::string path = ""); //path for display

	//Take a saved buffer in memory and load it
	bool LoadFBXfromBuffer(const char* Buffer, unsigned int Length, const char* relativePath); //lenght of the buffer, in bytes

	GameObject* LoadGameObjFromAiMesh(aiMesh* _mesh, const aiScene* scene, aiNode* currNode, GameObject* parent, std::string relPath);//optName lets the node name to be sent to the gameobj

	unsigned int LoadPureImageGL(const char* path);


	//Testing own file format
	char* SaveMesh(Mesh aux);
	bool LoadMeshFromPho(char* Buffer, unsigned int Length, std::string path);
	

}