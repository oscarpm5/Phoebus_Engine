#include <string>
#include <vector>
struct aiMesh;
struct aiNode;
struct aiScene;
class GameObject;
class ResourceMesh;
class Resource;
class C_Material;
class C_Transform;
class C_Camera;
struct JSON_Object;
class Config;
class Component;

namespace Importer
{
	bool InitializeDevIL();

	//path required for display in the inspector
	bool LoadNewImageFromBuffer(const char* Buffer, unsigned int Length, std::string path = "");
	//path required for display in the inspector
	bool LoadNewImageFromObj(const char* Buffer, unsigned int Length, GameObject* target, std::string path = "");

	//Take a saved buffer in memory and load it. lenght of the buffer, in bytes
	bool LoadFBXfromBuffer(const char* Buffer, unsigned int Length, const char* relativePath); 

	//optName lets the node name to be sent to the gameobj
	GameObject* LoadGameObjFromAiMesh(aiMesh* _mesh, const aiScene* scene, aiNode* currNode, GameObject* parent, std::string relPath);

	//Dont use this directly
	unsigned int LoadPureImageGL(const char* path);


	//Testing own file format
	char* SaveMesh(ResourceMesh aux);
	char* SaveMaterial(C_Material * aux);
	//Own file format
	char* SaveTransform(C_Transform * aux);
	//Own file format
	char* SaveCamera(C_Camera* aux);

	//Used in GameObject serialization. Never call this "raw"
	void SaveComponentCamera(Config& config, Component* camera);
	//Used in GameObject serialization. Never call this "raw"
	void SaveComponentMaterial(Config& config, Component* Mat);

	//Own file format
	bool LoadMeshFromPho(char* Buffer, unsigned int Length, std::string path);
	//Own file format
	bool LoadMaterialFromPho(char* buffer, unsigned int Lenght, std::string path);
	//Own file format
	bool LoadTransformFromPho(char* buffer, unsigned int Lenght, std::string path);
	//Own file format
	bool LoadCameraFromPho(char* buffer, unsigned int Lenght, std::string path);

	//returns size of buffer. Caution! this does not create file, it only fills buffer. Create file usinsg SavePHO from FliseSystem.
	unsigned int SerializeScene(GameObject* root, char** TrueBuffer); 
	
	//takes a buffer from a JSON and creates all the GO hierarchy starting from root
	void LoadScene(char* buffer, GameObject* sceneRoot);

	//Dont use this directly: it's called from SerializeScene
	void SerializeGameObject(Config& config, GameObject* gameObject);
	//Fill a vector with a GO and the childs tree
	void SeekChildrenRecurvisely(GameObject* root, std::vector<GameObject*> & vectorToFill);
	//Dont use this directly: it's called from SerializeScene
	void SaveComponentRaw(Config& config, Component* component);


	//Importing Components once we have a loaded buffer
	void ImportResourceMeshFromBuffer(char * buffer, unsigned int buffersize, Resource& meshToFill);
}