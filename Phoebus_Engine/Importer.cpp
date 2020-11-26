#include "Importer.h"
#include "Globals.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

//Saving things in own format
#include "PhysFS/include/physfs.h"

//devil initialization
#include "DevIL/include/IL/il.h"  //loading-saving-converting iamges
#include "DevIL/include/IL/ilu.h" //middle level library for image manipulation
#include "DevIL/include/IL/ilut.h" //high level library for displaying images

//note that those 3 devil libraries are hierarchical and so to include one of them we have to include the ones before

#pragma comment(lib,"DevIL/lib/x86/Release/DevIL.lib")
#pragma comment(lib,"DevIL/lib/x86/Release/ILU.lib")
#pragma comment(lib,"DevIL/lib/x86/Release/ILUT.lib")

#include<deque>
//#include"glmath.h"
#include"MathGeoLib/include/MathGeoLib.h"

#include "GameObject.h"
#include "Component.h"
#include "C_Mesh.h"
#include "Mesh.h"
#include "Texture.h"
#include "C_Material.h"
#include "C_Transform.h"
#include "C_Camera.h"
#include "Config.h"
#include <map>

bool Importer::InitializeDevIL()
{
	//initialize IL
	ilInit();
	//initialize ILU
	iluInit();
	//initialize ILUT with OpenGl Support
	ilutInit();
	ilutRenderer(ILUT_OPENGL); //call this before using any ilut function

	return true;
}

bool Importer::Texture::ImportImage(const char* Buffer, unsigned int Length, Resource& textureToFill)
{
	ResourceTexture* t = (ResourceTexture*)& textureToFill;

	ILuint newImage = 0;
	ilGenImages(1, &newImage);
	ilBindImage(newImage);

	bool ret = ilLoadL(IL_TYPE_UNKNOWN, Buffer, Length);


	if (!ret)
	{
		ILenum error;
		error = ilGetError();
		LOG("\n[error]Could not load an miage from buffer: %s", Buffer);
		LOG("[error] %d :\n %s", error, iluErrorString(error));
		ilDeleteImages(1, &newImage);
	}
	else if (ret = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
	{
		t->GenTextureFromName(newImage);
		//ilDeleteImages(1, &newImage);
	}
	return ret;
}

bool Importer::Texture::LoadNewImage(const char* libPath, Resource& textureToFill)
{

	//generate buffer from lib path before using

	char* buffer;//TODO

	char* cursor = buffer; //where in memory does the file start (pointer to first memory access)

	// path
	unsigned int values[1]; //necessarily hardcoded
	unsigned int bytes = sizeof(values);
	memcpy(values, cursor, bytes);
	cursor += bytes;

	// Load path;
	bytes = values[0];
	std::string NewPath;
	NewPath.resize(bytes);
	memcpy(&NewPath.at(0), cursor, bytes); //&indices[0] since we only need to point where he needs to start writing. bytes will tell it when to stop
	cursor += bytes;

	//Remake the Material
	// TODO: we have the path to the texture, now do all the Ilbind image stuff

	return true;
}
//
//bool Importer::LoadNewImageFromObj(const char* Buffer, unsigned int Length, GameObject* target, std::string path)
//{
//	ILuint newImage = 0;
//	ilGenImages(1, &newImage);
//	ilBindImage(newImage);
//
//	bool ret = ilLoadL(IL_TYPE_UNKNOWN, Buffer, Length);
//
//	if (!ret)
//	{
//		ILenum error;
//		error = ilGetError();
//		LOG("\n[error]Could not load an image from buffer");
//		LOG("[error]Error %d :\n %s", error, iluErrorString(error));
//		ilDeleteImages(1, &newImage);
//	}
//	else if (ret = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
//	{
//		if (target != nullptr)
//		{
//
//			C_Material* mat = target->GetComponent<C_Material>();
//
//			if (mat == nullptr)
//			{
//				target->CreateComponent(ComponentType::MATERIAL);
//				mat = target->GetComponent<C_Material>();
//			}
//
//			//mat->GenTextureFromName(newImage, path);
//
//		}
//		ilDeleteImages(1, &newImage);
//	}
//
//	if (Length != 0)
//		RELEASE_ARRAY(Buffer);
//
//	return ret;
//
//}


bool Importer::Model::ImportModel(const char* Buffer, unsigned int Length, const char* relativePath)
{
	bool ret = false;

	if (Length > 0)
	{

		const aiScene* scene = aiImportFileFromMemory(Buffer, Length, aiProcessPreset_TargetRealtime_MaxQuality, nullptr); //nullptr as we need no external libs to hepl import

											//aiImportFileFromMemory										
		LOG("Importing 3D asset from buffer: %s", Buffer);

		if (scene != nullptr && scene->HasMeshes())
		{
			std::string pathWithoutFile;
			App->fileSystem->SeparatePath(relativePath, &pathWithoutFile, nullptr);
			// Use scene->mNumMeshes to iterate on scene->mMeshes array

			aiNode* node = scene->mRootNode;

			std::deque<aiNode*>parents;
			std::deque<GameObject*>gameObjParents;

			parents.push_back(node);

			std::string name = "Untitled";
			std::string fbxName;

			App->fileSystem->SeparatePath(relativePath, nullptr, &fbxName);
			int dotIndex = fbxName.find_last_of(".");
			if (dotIndex >= 0 && dotIndex < fbxName.length())
			{
				name = fbxName.substr(0, dotIndex);
				name += " File";
			}
			else if (node->mName.C_Str() != "")
			{
				name = "Default ";
				name += node->mName.C_Str();
			}

			GameObject* pObj = new GameObject(App->editor3d->root, name, float4x4::identity);
			gameObjParents.push_back(pObj);//first node is root and doesn't have mesh

			while (parents.size() > 0)
			{
				std::deque<aiNode*> parentsCopy = parents;

				for (int i = 0; i < parentsCopy.size(); i++)
				{
					aiNode* currentParent = parents[0];
					parents.pop_front();

					GameObject* currObjParent = gameObjParents[0];
					gameObjParents.pop_front();


					for (int j = 0; j < currentParent->mNumChildren; j++)
					{
						parents.push_back(currentParent->mChildren[j]);

						aiMesh* newMesh = nullptr;
						if (parents.back()->mNumMeshes > 0)
							newMesh = scene->mMeshes[parents.back()->mMeshes[0]];//loads a mesh from index
							//create game object and save it into gameObjParents (its parent is currObjParent)
						//gameObjParents.push_back(LoadGameObjFromAiMesh(newMesh, scene, parents.back(), currObjParent, pathWithoutFile));

					}
				}

			}

			aiReleaseImport(scene);
		}
		else
		{
			LOG("[error]Error loading scene % s", Buffer);
			ret = false;
		}
	}
	else
	{
		LOG("[error]Error scene with path '%s' doesnt exist", relativePath);
	}
	return ret;
}

//GameObject* Importer::LoadGameObjFromAiMesh(aiMesh* _mesh, const aiScene* scene, aiNode* currNode, GameObject* parent, std::string relPath)
//{
//	std::vector<float> vertices;
//	std::vector<unsigned int> indices;
//	std::vector<float> normals;
//	std::vector<float>texCoords;
//
//	//creates a new mesh
//	aiMesh* mesh = _mesh;
//	//assigns game object name
//	std::string name = "Untitled";
//
//	if (currNode->mName.C_Str() != "")
//	{
//		name = currNode->mName.C_Str();
//	}
//	else if (mesh != nullptr)
//		name = mesh->mName.C_Str();
//
//	//assigns game object parent
//	GameObject* newParent = parent;
//	if (parent == nullptr)
//		newParent = App->editor3d->root;
//
//	//Transform importing TODO can be optimized ??
//	aiVector3D translation, scaling;
//	aiQuaternion rotation;
//	currNode->mTransformation.Decompose(scaling, rotation, translation);
//
//	/*mat4x4 transformMat = IdentityMatrix;
//	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
//	float3 vec;
//	float angle;
//
//	rot.ToAxisAngle(vec, angle);
//	angle = RadToDeg(angle);
//
//	transformMat.scale(scaling.x, scaling.y, scaling.z);
//	mat4x4 auxTransform = transformMat;
//
//	transformMat = auxTransform.rotate(angle, { vec.x,vec.y,vec.z }) * transformMat;
//	transformMat.translate(translation.x, translation.y, translation.z);*/
//
//
//
//	float3 newTranslation = float3(translation.x, translation.y, translation.z);
//
//	Quat newRot = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
//
//	float4x4 newTransform = float4x4::FromTRS(newTranslation, newRot.ToFloat4x4(), float3(scaling.x, scaling.y, scaling.z));
//
//	//creates new game object
//	GameObject* newObj = new GameObject(newParent, name, newTransform);
//
//	LOG("----------Importing mesh '%s'----------", (char*)name.c_str());
//
//
//	// copy vertices
//	//newMesh.numVertex = mesh->mNumVertices;
//
//	if (mesh != nullptr && mesh->mNumVertices > 0)
//	{
//		vertices.reserve(mesh->mNumVertices * 3);
//
//
//		for (int j = 0; j < mesh->mNumVertices; j++)
//		{
//			vertices.push_back(mesh->mVertices[j].x);
//			vertices.push_back(mesh->mVertices[j].y);
//			vertices.push_back(mesh->mVertices[j].z);
//		}
//		//memcpy(newMesh.vertices, mesh->mVertices, sizeof(float) * newMesh.numVertex * 3);
//		LOG("New mesh with %i vertices", (vertices.size() / 3));
//		// copy faces
//		if (mesh->HasFaces())
//		{
//			//newMesh.numIndex = mesh->mNumFaces * 3;
//			//newMesh.index = new unsigned int[newMesh.numIndex]; // assume each face is a triangle
//			indices.reserve(mesh->mNumFaces * 3);
//			indices.resize(mesh->mNumFaces * 3);
//
//			for (int j = 0; j < mesh->mNumFaces; j++)
//			{
//
//				if (mesh->mFaces[j].mNumIndices != 3)
//				{
//					LOG("[waring] geometry face with != 3 indices!");
//				}
//				else
//				{
//					//indices.push_back(mesh->mFaces[j].mIndices[0]);
//					//indices.push_back(mesh->mFaces[j].mIndices[1]);
//					//indices.push_back(mesh->mFaces[j].mIndices[2]);
//
//					memcpy(&indices[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(unsigned int));
//
//
//				}
//
//			}
//			LOG("New mesh with %i indices", (indices.size()));
//		}
//
//
//		texCoords.reserve(mesh->mNumVertices * 2); //there are 2 floats for every index
//		LOG("Importing mesh texture coordinates");
//		for (int j = 0; j < mesh->mNumVertices; j++)
//		{
//			//copy TextureCoords
//			if (mesh->mTextureCoords[0])
//			{
//
//				texCoords.push_back(mesh->mTextureCoords[0][j].x);
//				texCoords.push_back(mesh->mTextureCoords[0][j].y);
//			}
//			else
//			{
//				LOG("[warning]No texture coordinates found");
//				texCoords.push_back(0.0f);
//				texCoords.push_back(0.0f);
//			}
//		}
//		LOG("%i texture coordinates have been loaded", texCoords.size() / 2);
//
//		//copy normals
//		if (mesh->HasNormals())
//		{
//			LOG("Importing normals");
//			normals.reserve(mesh->mNumVertices * 3);
//			for (int j = 0; j < mesh->mNumVertices; j++)
//			{
//
//				normals.push_back(mesh->mNormals[j].x);
//				normals.push_back(mesh->mNormals[j].y);
//				normals.push_back(mesh->mNormals[j].z);
//			}
//			LOG("%i normals have been loaded", normals.size() / 3);
//		}
//		else
//			LOG("[warning]Mesh has no normals!");
//
//
//
//		newObj->CreateComponent(ComponentType::MESH);
//		newObj->GetComponent<C_Mesh>()->SetMesh(ResourceMesh(vertices, indices, normals, texCoords,0));//TODO for the moment we pass id 0 to the mesh
//
//
//		if (scene->HasMaterials())
//		{
//
//			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//			unsigned int numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
//
//			if (numTextures > 0)
//			{
//				aiString path;
//				char* c = (char*)path.C_Str();
//				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
//
//				path = App->fileSystem->NormalizePath(path.C_Str());
//				path = relPath + path.C_Str();
//				char* buffer;
//				unsigned int buffLength = App->fileSystem->Load(path.C_Str(), &buffer);
//				//TODO if path not found try to get the texture from the fbx path, if not found try to get the texture from the library/textures folder (not created yet)
//				
//				//LoadNewImageFromObj(buffer, buffLength, newObj, path.C_Str());
//
//
//			}
//		}
//		//App->editor3d->meshes.push_back(Mesh(vertices, indices, normals, texCoords));
//		LOG("----------Mesh '%s' has been loaded----------", (char*)name.c_str());
//		vertices.clear();
//		indices.clear();
//		normals.clear();
//		mesh = nullptr;
//	}
//	return newObj;
//}

//returns 0 by default if something failed
unsigned int Importer::LoadPureImageGL(const char* path)
{
	unsigned int ID = 0;
	char* new_buffer;
	unsigned int length = App->fileSystem->Load(path, &new_buffer);

	ilGenImages(1, &ID);
	ilBindImage(ID);

	bool ret = ilLoadL(IL_TYPE_UNKNOWN, new_buffer, length);

	if (!ret)
	{
		ILenum error;
		error = ilGetError();
		LOG("\n[error]Could not load an miage from buffer: %s", new_buffer);
		LOG("[error] %d :\n %s", error, iluErrorString(error));
		RELEASE_ARRAY(new_buffer);
		ilDeleteImages(1, &ID);
	}
	else if (ret = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
	{
		ilBindImage(ID);

		//get properties
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}
		//gen texture
		unsigned int openGlId;
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenTextures(1, &openGlId);
		glBindTexture(GL_TEXTURE_2D, openGlId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		glBindTexture(GL_TEXTURE_2D, 0);

		ilBindImage(0);

		ilDeleteImages(1, &ID);
		RELEASE_ARRAY(new_buffer);
		return openGlId;
	}
	return 0;
}

unsigned int Importer::Mesh::SaveMesh(ResourceMesh mesh, char* buffer)
{
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	//uint ranges[2] = { mesh.num_indices, mesh.num_vertices };
	uint ranges[5] = { mesh.indices.size(), mesh.vertices.size(), mesh.normals.size(),mesh.smoothedNormals.size(), mesh.texCoords.size() };
	uint size =
		sizeof(ranges)
		+ sizeof(uint) * mesh.indices.size()
		+ sizeof(float) * mesh.vertices.size()
		+ sizeof(float) * mesh.normals.size()
		+ sizeof(float) * mesh.smoothedNormals.size()
		+ sizeof(float) * mesh.texCoords.size();

	char* fileBuffer = new char[size]; // Allocate
	char* cursor = fileBuffer;

	unsigned int bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(unsigned int) * mesh.indices.size();
	memcpy(cursor, &mesh.indices[0], bytes);
	cursor += bytes;

	// Store vertex
	bytes = sizeof(float) * mesh.vertices.size();
	memcpy(cursor, &mesh.vertices[0], bytes);
	cursor += bytes;

	// Store normals
	bytes = sizeof(float) * mesh.normals.size();
	memcpy(cursor, &mesh.normals[0], bytes);
	cursor += bytes;

	// Store smooth normals
	bytes = sizeof(float) * mesh.smoothedNormals.size();
	memcpy(cursor, &mesh.smoothedNormals[0], bytes);
	cursor += bytes;

	// Store tex coords
	bytes = sizeof(float) * mesh.texCoords.size();
	memcpy(cursor, &mesh.texCoords[0], bytes);
	cursor += bytes;

	//aqui directorio X

	std::string fileName = MESH_PATH;
	fileName += "testing.pho";
	App->fileSystem->SavePHO(fileName.c_str(), fileBuffer, size);
	buffer = fileBuffer;

	return size;
}

unsigned int Importer::Texture::SaveTexture(Resource& texture)
{
	ResourceTexture* t = (ResourceTexture*)& texture;
	if (t->ilImageID != -1)
	{
		ILuint image = t->ilImageID;
		ILubyte* data = nullptr;
		ilBindImage(image);

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		unsigned int size = ilSaveL(IL_DDS, nullptr, 0);//get size

		if (size > 0)
		{
			data = new ILubyte[size];
			if (ilSaveL(IL_DDS, data, size) <= 0)
			{
				return 0;
			}

		}
		else
		{
			LOG("[error] saving the texture, size is 0");
		}

		//unsigned int values[1] = { t->GetAssetFile().length() };
		//std::string auxPath = t->GetAssetFile();


		//unsigned int size = sizeof(values) + auxPath.length();
		//char* fileBuffer = new char[size]; // Allocate
		//char* cursor = fileBuffer;

		//// First store values
		//unsigned int bytes = sizeof(values);
		//memcpy(cursor, values, bytes);
		//cursor += bytes;

		//// Store path
		//bytes = auxPath.length();
		//memcpy(cursor, auxPath.c_str(), bytes);
		//cursor += bytes;

		if (data != nullptr)
		{
			App->fileSystem->SavePHO(t->GetLibraryFile().c_str(), data, size);
			RELEASE_ARRAY(data);
		}
		ilDeleteImages(1, (unsigned int*)& t->ilImageID);
		t->ilImageID = -1;

		return size;
	}
	LOG("[error] saving the texture, this texture IL ID doesn't exist");

	return 0;
}
//
//char* Importer::SaveTransform(C_Transform* aux) //TODO DEPRECATED? should we use game object to store the transform?
//{
//	float4x4 values[1] = { aux->GetGlobalTransform() };
//
//	unsigned int size = sizeof(values);
//	char* fileBuffer = new char[size]; // Allocate
//	char* cursor = fileBuffer;
//
//	// First store values
//	unsigned int bytes = sizeof(values);
//	memcpy(cursor, values, bytes);
//	cursor += bytes;
//
//	App->fileSystem->SavePHO("testingTransform.pho", fileBuffer, size);
//
//	return fileBuffer;
//}

unsigned int Importer::Camera::SaveCamera(C_Camera* aux, char* buffer)
{
	//float nearPlaneDist;
	//float farPlaneDist;
	//float FoV;
	float values[4] = { aux->GetNearPlaneDist(), aux->GetFarPlaneDist(), aux->GetFoV(),aux->GetAspectRatio() };

	unsigned int size = sizeof(values);
	char* fileBuffer = new char[size]; // Allocate
	char* cursor = fileBuffer;

	// First store values
	unsigned int bytes = sizeof(values);
	memcpy(cursor, values, bytes);
	cursor += bytes;

	std::string fileName = SCENE_PATH;
	fileName += "testingCamera.pho";
	App->fileSystem->SavePHO(fileName.c_str(), fileBuffer, size);
	buffer = fileBuffer;
	return size;
}

void Importer::Camera::SaveComponentCamera(Config& config, Component* cam)
{
	C_Camera* camera = (C_Camera*)cam;

	config.SetNumber("FOV", camera->GetFoV()); //this is the x, y is calculated afterwards
	config.SetNumber("NearPlane", camera->GetNearPlaneDist());
	config.SetNumber("FarPlane", camera->GetFarPlaneDist());

	//config.SetBool("MainCamera",camera->main);
}

void Importer::Texture::SaveComponentMaterial(Config& config, Component* auxMat)
{
	C_Material* mat = (C_Material*)auxMat;
	config.SetString("Path", mat->path.c_str());
}

bool Importer::Mesh::LoadMesh(char* buffer, unsigned int Length, ResourceMesh& meshToFill)
{
	std::vector<float> vertices; std::vector<unsigned int> indices; std::vector<float> normals; std::vector<float> smoothedNormals; std::vector<float> texCoords;
	char* cursor = buffer; //where in memory does the file start (pointer to first memory access)

	// amount of indices / vertices / smoothed vertices / colors / normals / texture_coords
	uint ranges[5]; //necessarily hardcoded
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;
	int num_indices = ranges[0];
	int num_vertices = ranges[1];
	int num_normals = ranges[2];
	int num_smoothedNormals = ranges[3];
	int num_tex = ranges[4];

	// Load indices
	bytes = sizeof(uint) * num_indices;
	indices.resize(num_indices); //TODO: THIS IS THE LESS GHETTO WAY TO DO IT
	//for (int i = 0; i < num_indices; i++)
	//{
	//	indices.push_back(1); //TODO: ask Oscar a less ghetto way to do this
	//}
	memcpy(&indices[0], cursor, bytes); //&indices[0] since we only need to point where he needs to start writing. bytes will tell it when to stop
	cursor += bytes;


	// Load vertex
	bytes = sizeof(float) * num_vertices;
	vertices.resize(num_vertices);
	//for (int i = 0; i < num_vertices; i++)
	//{
	//	vertices.push_back(1.0f); //TODO: ask Oscar a less ghetto way to do this
	//}
	//ret.indices = new uint[num_indices];
	memcpy(&vertices[0], cursor, bytes);
	cursor += bytes;


	// Load normals
	bytes = sizeof(float) * num_normals;
	normals.resize(num_normals);
	//for (int i = 0; i < num_normals; i++)
	//{
	//	normals.push_back(1.0f); //TODO: ask Oscar a less ghetto way to do this
	//}
	//ret.indices = new uint[num_indices];
	memcpy(&normals[0], cursor, bytes);
	cursor += bytes;

	// Load smoothed normals
	bytes = sizeof(float) * num_smoothedNormals;
	smoothedNormals.resize(num_smoothedNormals);
	//for (int i = 0; i < num_smoothedNormals; i++)
	//{
	//	smoothedNormals.push_back(1.0f); //TODO: ask Oscar a less ghetto way to do this
	//}
	//ret.indices = new uint[num_indices];
	memcpy(&smoothedNormals[0], cursor, bytes);
	cursor += bytes;


	// Load texcoords
	bytes = sizeof(float) * num_tex;
	texCoords.resize(num_tex);
	//for (int i = 0; i < num_tex; i++)
	//{
	//	texCoords.push_back(1.0f); //TODO: ask Oscar a less ghetto way to do this
	//}
	//ret.indices = new uint[num_indices];
	memcpy(&texCoords[0], cursor, bytes);
	cursor += bytes;


	//Remake the mesh
	ResourceMesh ret(vertices, indices, normals, smoothedNormals, texCoords, 0);//TODO for the moment we pass id 0 to the mesh
	if (!ret.vertices.empty() && !ret.indices.empty()) { return true; }
	else {
		LOG("Malformed mesh loaded from PHO");
		return false;
	};

}
//
//bool Importer::LoadMaterialFromPho(char* buffer, unsigned int Lenght, std::string path)
//{
//
//	char* cursor = buffer; //where in memory does the file start (pointer to first memory access)
//
//	// path
//	unsigned int values[1]; //necessarily hardcoded
//	unsigned int bytes = sizeof(values);
//	memcpy(values, cursor, bytes);
//	cursor += bytes;
//
//	// Load path;
//	bytes = values[0];
//	std::string NewPath;
//	NewPath.resize(bytes);
//	memcpy(&NewPath.at(0), cursor, bytes); //&indices[0] since we only need to point where he needs to start writing. bytes will tell it when to stop
//	cursor += bytes;
//
//	//Remake the Material
//	// TODO: we have the path to the texture, now do all the Ilbind image stuff
//
//	return true;
//}

bool Importer::Camera::LoadCameraFromPho(char* buffer, unsigned int Lenght)
{
	//float nearPlaneDist; float farPlaneDist; float FoV; float aspectRatio;

	char* cursor = buffer; //where in memory does the file start (pointer to first memory access)

	// values stored
	float values[4]; //necessarily hardcoded
	unsigned int bytes = sizeof(values);
	memcpy(values, cursor, bytes);
	cursor += bytes;

	// Load global mat (float4x4);
	float nearPlaneDist = values[0];
	float farPlaneDist = values[1];
	float FoV = values[2];
	float aspectRatio = values[3];

	return true;
}

void Importer::SerializeGameObject(Config& config, GameObject* gameObject) //serialize GO
{
	config.SetNumber("ID", gameObject->ID);
	config.SetNumber("ParentUID", gameObject->parent ? gameObject->parent->ID : 0);
	config.SetString("Name", gameObject->GetName().c_str());
	config.SetBool("Active", gameObject->isActive);
	config.SetBool("Focused", gameObject->focused);

	//Global transform
	const C_Transform* transform = gameObject->GetComponent<C_Transform>();
	config.SetArray("Transform").Add4x4Mat(transform->GetGlobalTransform()); //yeah, we save it raw. Deal with it, Marc


	Config_Array compConfig = config.SetArray("Components");
	std::vector<Component*> components = gameObject->GetAllComponents();

	for (unsigned int i = 0; i < components.size(); i++)
	{
		SaveComponentRaw(compConfig.AddNode(), components[i]);
	}
}

unsigned int Importer::SerializeScene(GameObject* root, char** TrueBuffer)	 //serialize scene
{
	/*
	TODO: Look at Marc Json functions?? -> class config + class configarray
										config has own destructor, no worries there
	*/
	/*
	1: search for children
	2. save gameobject -> UID,UID of parent (0 if root, ghetto Alex), save components as children
	3. save its components -> type, active, switch to know that extra info to save
	4. recurr
	*/

	Config file;
	Config_Array ArrayGameObjects = file.SetArray("GameObjects");
	std::vector<GameObject*> gameObjects;
	SeekChildrenRecurvisely(root, gameObjects);
	//We don't want to save root
	gameObjects.erase(gameObjects.begin());
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		SerializeGameObject(ArrayGameObjects.AddNode(), gameObjects[i]);
	}

	unsigned int size = file.Serialize(TrueBuffer);
	return size;
}

void Importer::LoadScene(char* buffer, GameObject* sceneRoot)
{
	//Open the bufer you are going to be reading
	Config file(buffer);
	//set the root of the new scene
	App->editor3d->root = sceneRoot;
	//the map is a correlation between ID nad GO. It comes useful later. Thanks Marc!
	std::map<int, GameObject*> createdGameObjects;
	Config_Array gameObjects_array = file.GetArray("GameObjects");
	for (uint i = 0; i < gameObjects_array.GetSize(); ++i)
	{
		//Pinpoint the GO 
		Config gameObject_node = gameObjects_array.GetNode(i);

		//Get its Global mat needed to call constructor
		float4x4 auxGlobalMat = gameObject_node.GetArray("Transform").GetMatTransform(0); //they shouldnt have more than 1 Global transform...

		//Parent setup
		GameObject* parent = nullptr;
		std::map<int, GameObject*>::iterator it = createdGameObjects.find(gameObject_node.GetNumber("ParentUID"));
		if (it != createdGameObjects.end())
			parent = it->second;
		//Create the GO
		GameObject* gameObject = new GameObject(parent ? parent : sceneRoot, gameObject_node.GetString("Name").c_str(), auxGlobalMat);

		//Set properties of GO
		gameObject->ID = gameObject_node.GetNumber("ID");
		createdGameObjects[gameObject->ID] = gameObject;
		gameObject->isActive = gameObject_node.GetBool("Active");
		gameObject->focused = gameObject_node.GetBool("Focused");

		//get the components
		Config_Array components = gameObject_node.GetArray("Components");

		for (uint i = 0; i < components.GetSize(); i++)
		{
			Config comp = components.GetNode(i);
			ComponentType type = (ComponentType)((int)comp.GetNumber("ComponentType"));

			if (Component * component = gameObject->CreateComponent(type))
			{
				component->ID = comp.GetNumber("ID");

				switch (type)
				{
				case ComponentType::CAMERA:
					break;
				case ComponentType::MESH:
					break;
				case ComponentType::MATERIAL:
					break;
				case ComponentType::TRANSFORM:
					//Nothing: this is already done in constructor
					break;
				default:
					LOG("[error] Tried to load a scene, but the material with ID %i of Game Object %s had an unexpected type", component->ID, component->owner->GetName());
					break;
				}


				//LoadComponent(comp, component);  //marc uses this for animations and cameras? tf is up witht that? 

			}

		}
		//Marc calls some Update funcs here; we don't need to since we set it up on GO constructor
	}

}

void Importer::SeekChildrenRecurvisely(GameObject* root, std::vector<GameObject*>& vectorToFill)
{
	vectorToFill.push_back(root);

	for (unsigned int i = 0; i < root->children.size(); i++)
	{
		SeekChildrenRecurvisely(root->children[i], vectorToFill);
	}
}

void Importer::SaveComponentRaw(Config& config, Component* component)
{
	config.SetNumber("ComponentType", (int)component->GetType());
	config.SetNumber("ID", (int)component->ID);


	switch (component->GetType())
	{
	case ComponentType::CAMERA:
		Camera::SaveComponentCamera(config, component);
		break;
	case ComponentType::MESH:
		//all you need is component type and ID, and you already have that
		break;
	case ComponentType::MATERIAL:
		Texture::SaveComponentMaterial(config, component);
		break;
	case ComponentType::TRANSFORM:
		//we're already saving it as an array
		break;

	default:
		//how did you even get here smh
		LOG("[error] Trying to save component with ID %i from Game Object %s but the type is invalid", component->ID, component->owner->GetName());
		break;
	}

}

void Importer::Mesh::ImportRMesh(aiMesh* fbxMesh, ResourceMesh& resToFill)
{
}


