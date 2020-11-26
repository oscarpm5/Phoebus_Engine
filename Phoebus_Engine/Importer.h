#include <string>
#include <vector>
struct aiMesh;
struct aiNode;
struct aiScene;
class GameObject;
class ResourceMesh;
class Resource;
class ResourceTexture;
class C_Material;
class C_Transform;
class C_Camera;
struct JSON_Object;
class Config;
class Component;

namespace Importer
{
	bool InitializeDevIL();

	namespace Model
	{
		//Take a saved buffer in memory and loads it into a model resource, also imports dependencies if needed (aka meshes / textures) TODO
		bool ImportModel(const char* Buffer, unsigned int Length, const char* relativePath);

		bool LoadModel(const char* libPath, GameObject* root);//revise if needed
		unsigned int SaveModel(GameObject* root, char* buffer);//returns buffer size
	}

	namespace Mesh
	{
		void ImportRMesh(aiMesh* fbxMesh, ResourceMesh& meshToFill);//TODO
		//Own file format
		bool LoadMesh(char* Buffer, unsigned int Length, ResourceMesh& meshToFill);
		unsigned int SaveMesh(ResourceMesh aux, char* buffer);//returns buffer 
	}

	namespace Texture
	{
		//path required for display in the inspector
		bool ImportImage(const char* Buffer, unsigned int Length, Resource& textureToFill);//TODO needs completion

		bool LoadNewImage(const char* libPath, Resource& textureToFill);

		//Used in GameObject serialization. Never call this "raw"
		void SaveComponentMaterial(Config& config, Component* Mat);

		//Testing own file format
		unsigned int SaveTexture(Resource& texture);//TODO all

	}

	namespace Camera
	{
		//Import camera will exist when we load Cameras from Fbx, NOT NOW
		//Own file format
		unsigned int SaveCamera(C_Camera* aux, char* buffer);
		//Used in GameObject serialization. Never call this "raw"
		void SaveComponentCamera(Config& config, Component* camera);
		//Own file format
		bool LoadCameraFromPho(char* buffer, unsigned int Lenght);

	}


	//Dont use this directly
	unsigned int LoadPureImageGL(const char* path);


	//returns size of buffer. Caution! this does not create file, it only fills buffer. Create file usinsg SavePHO from FliseSystem.
	unsigned int SerializeScene(GameObject* root, char** TrueBuffer);

	//takes a buffer from a JSON and creates all the GO hierarchy starting from root
	void LoadScene(char* buffer, GameObject* sceneRoot);

	//Dont use this directly: it's called from SerializeScene
	void SerializeGameObject(Config& config, GameObject* gameObject);
	//Fill a vector with a GO and the childs tree
	void SeekChildrenRecurvisely(GameObject* root, std::vector<GameObject*>& vectorToFill);
	//Dont use this directly: it's called from SerializeScene
	void SaveComponentRaw(Config& config, Component* component);


	//Importing Components once we have a loaded buffer
	//void ImportResourceMeshFromBuffer(char* buffer, unsigned int buffersize, Resource& meshToFill);
}