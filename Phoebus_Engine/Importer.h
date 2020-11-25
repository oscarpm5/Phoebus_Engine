#include <string>
#include <vector>
struct aiMesh;
struct aiNode;
struct aiScene;
class GameObject;
class ResourceMesh;
class C_Material;
class C_Transform;
class C_Camera;
struct JSON_Object;
class Config;
class Component;

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
	char* SaveMesh(ResourceMesh aux);
	char* SaveMaterial(C_Material * aux);
	char* SaveTransform(C_Transform * aux);
	char* SaveCamera(C_Camera* aux);

	//Used in GameObject serialization. Never call this "raw"
	void SaveComponentCamera(Config& config, Component* camera);
	//Used in GameObject serialization. Never call this "raw"
	void SaveComponentMaterial(Config& config, Component* Mat);

	bool LoadMeshFromPho(char* Buffer, unsigned int Length, std::string path);
	bool LoadMaterialFromPho(char* buffer, unsigned int Lenght, std::string path);
	bool LoadTransformFromPho(char* buffer, unsigned int Lenght, std::string path);
	bool LoadCameraFromPho(char* buffer, unsigned int Lenght, std::string path);

	void SerializeGameObject(Config& config, GameObject* gameObject);
	unsigned int SerializeScene(GameObject* root, char** TrueBuffer); //returns size of buffer
	void SeekChildrenRecurvisely(GameObject* root, std::vector<GameObject*> & vectorToFill);
	void SaveComponentRaw(Config& config, Component* component);
}