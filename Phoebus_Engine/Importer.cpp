#include "Importer.h"
#include "Globals.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

//devil initialization
#include "DevIL/include/IL/il.h"  //loading-saving-converting iamges
#include "DevIL/include/IL/ilu.h" //middle level library for image manipulation
#include "DevIL/include/IL/ilut.h" //high level library for displaying images

//note that those 3 devil libraries are hierarchical and so to include one of them we have to include the ones before

#pragma comment(lib,"DevIL/lib/x86/Release/DevIL.lib")
#pragma comment(lib,"DevIL/lib/x86/Release/ILU.lib")
#pragma comment(lib,"DevIL/lib/x86/Release/ILUT.lib")

#include<deque>
#include"glmath.h"
#include"MathGeoLib/include/MathGeoLib.h"

#include "texture.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Mesh.h"
#include "C_Material.h"

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

bool Importer::LoadNewImageFromBuffer(const char* Buffer, unsigned int Length)
{
	ILuint newImage = 0;
	ilGenImages(1, &newImage);
	ilBindImage(newImage);

	//TODO this will need to accept more formats in the future
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
		//NewTexture* t = new NewTexture;
		//App->editor3d->textures.push_back(t);


		//ILinfo ImageInfo;
		//iluGetImageInfo(&ImageInfo);
		//if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		//{
		//	iluFlipImage();
		//}
		////TODO load this
		//ImageInfo.Format; 
		//int i=ilGetInteger(IL_IMAGE_FORMAT);
		///*IL_COLOUR_INDEX
		//	IL_RGB
		//	IL_RGBA
		//	IL_BGR
		//	IL_BGRA
		//	IL_LUMINANCE*/
		//
		//ImageInfo.Depth;
		//ImageInfo.Bpp;//bytes per pixel
		//ImageInfo.SizeOfData;//bytes
		////End of TODO
		//App->editor3d->textures.back()->width = ilGetInteger(IL_IMAGE_WIDTH);
		//App->editor3d->textures.back()->height = ilGetInteger(IL_IMAGE_HEIGHT);
		//App->editor3d->textures.back()->GenTextureFromName(newImage);

		//temporal code that puts the image in every mesh avaliable
		/*for (int i = 0; i < App->editor3d->meshes.size(); i++)
		{
			App->editor3d->meshes[i].GenerateTexturefromILUT();
		}*/

		//testing code
		/*if (App->editor3d->meshes.size() > 0)
		{
			if (App->editor3d->meshes.back().texture == nullptr)
			{
				App->editor3d->meshes.back().texture = App->editor3d->textures.back();
			}
		}*/


		if (App->editor3d->selectedGameObjs.size() > 0 && App->editor3d->selectedGameObjs.back() != App->editor3d->root)
		{

			C_Material* mat = App->editor3d->selectedGameObjs.back()->GetComponent<C_Material>();

			if (mat == nullptr)
			{
				App->editor3d->selectedGameObjs.back()->CreateComponent(ComponentType::MATERIAL);
				mat = App->editor3d->selectedGameObjs.back()->GetComponent<C_Material>();
			}

			mat->GenTextureFromName(newImage);

		}
		ilDeleteImages(1, &newImage);
	}
	return ret;
}

bool Importer::LoadNewImageFromObj(const char* Buffer, unsigned int Length, GameObject* target)
{
	ILuint newImage = 0;
	ilGenImages(1, &newImage);
	ilBindImage(newImage);

	//TODO this will need to accept more formats in the future
	bool ret = ilLoadL(IL_TYPE_UNKNOWN, Buffer, Length);

	if (!ret)
	{
		ILenum error;
		error = ilGetError();
		LOG("\n[error]Could not load an image from buffer");
		LOG("[error]Error %d :\n %s", error, iluErrorString(error));
		ilDeleteImages(1, &newImage);
	}
	else if (ret = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
	{
		if (target != nullptr)
		{

			C_Material* mat = target->GetComponent<C_Material>();

			if (mat == nullptr)
			{
				target->CreateComponent(ComponentType::MATERIAL);
				mat = target->GetComponent<C_Material>();
			}

			mat->GenTextureFromName(newImage);

		}
		ilDeleteImages(1, &newImage);
	}

	if (Length != 0)
		RELEASE_ARRAY(Buffer);

	return ret;

}


bool Importer::LoadFBXfromBuffer(const char* Buffer, unsigned int Length, const char* relativePath)
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
			gameObjParents.push_back(nullptr);//first node is root and doesn't have mesh nor game object

			//create obj for root and pushes it to gameObjParents TODO

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
						gameObjParents.push_back(LoadGameObjFromAiMesh(newMesh, scene, parents.back(), currObjParent, pathWithoutFile));

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

GameObject* Importer::LoadGameObjFromAiMesh(aiMesh* _mesh, const aiScene* scene, aiNode* currNode, GameObject* parent, std::string relPath)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> normals;
	std::vector<float>texCoords;

	//creates a new mesh
	aiMesh* mesh = _mesh;
	//assigns game object name
	std::string name = "Untitled";

	if (currNode->mName.C_Str() != "")
	{
		name = currNode->mName.C_Str();
	}
	else if (mesh != nullptr)
		name = mesh->mName.C_Str();

	//assigns game object parent
	GameObject* newParent = parent;
	if (parent == nullptr)
		newParent = App->editor3d->root;

	//Transform importing TODO can be optimized
	aiVector3D translation, scaling;
	aiQuaternion rotation;
	currNode->mTransformation.Decompose(scaling, rotation, translation);

	mat4x4 transformMat = IdentityMatrix;
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 vec;
	float angle;

	rot.ToAxisAngle(vec, angle);
	angle = RadToDeg(angle);

	transformMat.scale(scaling.x, scaling.y, scaling.z);
	mat4x4 auxTransform = transformMat;

	transformMat = auxTransform.rotate(angle, { vec.x,vec.y,vec.z }) * transformMat;
	transformMat.translate(translation.x, translation.y, translation.z);


	//creates new game object
	GameObject* newObj = new GameObject(newParent, name, transformMat);

	LOG("----------Importing mesh '%s'----------", (char*)name.c_str());


	// copy vertices
	//newMesh.numVertex = mesh->mNumVertices;

	if (mesh != nullptr && mesh->mNumVertices > 0)
	{
		vertices.reserve(mesh->mNumVertices * 3);


		for (int j = 0; j < mesh->mNumVertices; j++)
		{
			vertices.push_back(mesh->mVertices[j].x);
			vertices.push_back(mesh->mVertices[j].y);
			vertices.push_back(mesh->mVertices[j].z);
		}
		//memcpy(newMesh.vertices, mesh->mVertices, sizeof(float) * newMesh.numVertex * 3);
		LOG("New mesh with %i vertices", (vertices.size() / 3));
		// copy faces
		if (mesh->HasFaces())
		{
			//newMesh.numIndex = mesh->mNumFaces * 3;
			//newMesh.index = new uint[newMesh.numIndex]; // assume each face is a triangle
			indices.reserve(mesh->mNumFaces * 3);
			indices.resize(mesh->mNumFaces * 3);

			for (int j = 0; j < mesh->mNumFaces; j++)
			{

				if (mesh->mFaces[j].mNumIndices != 3)
				{
					LOG("[waring] geometry face with != 3 indices!");
				}
				else
				{
					//indices.push_back(mesh->mFaces[j].mIndices[0]);
					//indices.push_back(mesh->mFaces[j].mIndices[1]);
					//indices.push_back(mesh->mFaces[j].mIndices[2]);

					memcpy(&indices[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(unsigned int));


				}

			}
			LOG("New mesh with %i indices", (indices.size()));
		}


		texCoords.reserve(mesh->mNumVertices * 2); //there are 2 floats for every index
		LOG("Importing mesh texture coordinates");
		for (int j = 0; j < mesh->mNumVertices; j++)
		{
			//copy TextureCoords
			if (mesh->mTextureCoords[0])
			{

				texCoords.push_back(mesh->mTextureCoords[0][j].x);
				texCoords.push_back(mesh->mTextureCoords[0][j].y);
			}
			else
			{
				LOG("[warning]No texture coordinates found");
				texCoords.push_back(0.0f);
				texCoords.push_back(0.0f);
			}
		}
		LOG("%i texture coordinates have been loaded", texCoords.size() / 2);

		//copy normals
		if (mesh->HasNormals())
		{
			LOG("Importing normals");
			normals.reserve(mesh->mNumVertices * 3);
			for (int j = 0; j < mesh->mNumVertices; j++)
			{

				normals.push_back(mesh->mNormals[j].x);
				normals.push_back(mesh->mNormals[j].y);
				normals.push_back(mesh->mNormals[j].z);
			}
			LOG("%i normals have been loaded", normals.size() / 3);
		}
		else
			LOG("[warning]Mesh has no normals!");



		newObj->CreateComponent(ComponentType::MESH);
		newObj->GetComponent<C_Mesh>()->SetMesh(Mesh(vertices, indices, normals, texCoords));


		if (scene->HasMaterials())
		{

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			unsigned int numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);

			if (numTextures > 0)
			{
				aiString path;
				char* c = (char*)path.C_Str();
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

				path = App->fileSystem->NormalizePath(path.C_Str());
				path = relPath + path.C_Str();
				char* buffer;
				unsigned int buffLength = App->fileSystem->Load(path.C_Str(), &buffer);
				//TODO if path not found try to get the texture from the fbx path, if not found try to get the texture from the library/textures folder (not created yet)
				LoadNewImageFromObj(buffer, buffLength, newObj);
				//App->fileSystem->LoadAsset(c); //TODO make path relative to the folder we want to load from
				//use also the LoadNewImageFromObj() function when the file system only returns buffers

			}
		}
		//App->editor3d->meshes.push_back(Mesh(vertices, indices, normals, texCoords));
		LOG("----------Mesh '%s' has been loaded----------", (char*)name.c_str());
		vertices.clear();
		indices.clear();
		normals.clear();
		mesh = nullptr;
	}
	return newObj;
}
