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
/*
bool Importer::LoadFBX(const char* path)
{
	bool ret = false;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality); //assimp has a function to import from binary buffer
	LOG("");										//aiImportFileFromMemory										
	LOG("Importing 3D asset from: %s", path);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array

		for (int i = 0; i < scene->mNumMeshes; i++)
		{

			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			std::vector<float> normals;
			std::vector<float>texCoords;
			//creates a new mesh at scene editor

			aiMesh* mesh = scene->mMeshes[i];
			LOG("----------Importing mesh %i----------", i);


			// copy vertices
			//newMesh.numVertex = mesh->mNumVertices;
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

				for (int j = 0; j < mesh->mNumFaces; j++)
				{

					if (mesh->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						indices.push_back(mesh->mFaces[j].mIndices[0]);
						indices.push_back(mesh->mFaces[j].mIndices[1]);
						indices.push_back(mesh->mFaces[j].mIndices[2]);

						//memcpy(&newMesh.index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));


					}

				}
				LOG("New mesh with %i indices", (indices.size()));
			}


			texCoords.reserve(indices.size() * 2); //there are 2 floats for every index
			LOG("Importing mesh texture coordinates");
			for (int j = 0; j < indices.size(); j++)
			{
				//copy TextureCoords
				if (mesh->mTextureCoords[0])
				{
					texCoords.push_back(mesh->mTextureCoords[0][j].x);
					texCoords.push_back(mesh->mTextureCoords[0][j].y);
				}
				else
				{
					LOG("No texture coordinates found");
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
				LOG("Mesh has no normals!");




			App->editor3d->meshes.push_back(Mesh(vertices, indices, normals, texCoords));
			LOG("----------Mesh %i has been loaded----------", i);
			vertices.clear();
			indices.clear();
			normals.clear();
			ret = true;
			mesh = nullptr;
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene % s", path);
		ret = false;
	}
	return ret;
}
*/

#include "texture.h"


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
	bool ret = ilLoadL(IL_PNG,Buffer, Length);


	if (!ret)
	{
		ILenum error;
		error = ilGetError();
		LOG("\nCould not load an miage from buffer: %s", Buffer);
		LOG("Error %d :\n %s", error, iluErrorString(error));
		ilDeleteImages(1, &newImage);
	}
	else if (ret = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
	{
		NewTexture* t=new NewTexture;
		App->editor3d->textures.push_back(t);
		

		ILinfo ImageInfo; 
		iluGetImageInfo(&ImageInfo); 
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) 
		{ 
			iluFlipImage(); 
		}
		App->editor3d->textures.back()->width = ilGetInteger(IL_IMAGE_WIDTH);
		App->editor3d->textures.back()->height = ilGetInteger(IL_IMAGE_HEIGHT);
		App->editor3d->textures.back()->GenTextureFromName(newImage);

		//temporal code that puts the image in every mesh avaliable
		/*for (int i = 0; i < App->editor3d->meshes.size(); i++)
		{
			App->editor3d->meshes[i].GenerateTexturefromILUT();
		}*/

		//testing code
		if (App->editor3d->meshes.size() > 0)
		{
			if (App->editor3d->meshes.back().texture == nullptr)
			{
				App->editor3d->meshes.back().texture = App->editor3d->textures.back();
			}
		}




		ilDeleteImages(1, &newImage);
	}
	return ret;
}

/*
bool Importer::LoadNewImage(const char* path)
{
	ILuint newImage = 0;
	ilGenImages(1, &newImage);
	ilBindImage(newImage);

	bool ret = ilLoadImage(path);//TODO this will be loaded from buffer in the near future -> ilLoadL()


	if (!ret)
	{
		ILenum error;
		error = ilGetError();
		LOG("\nCould not load the file: %s", path);
		LOG("Error %d :\n %s", error, iluErrorString(error));
		ilDeleteImages(1, &newImage);
	}
	else if (ret = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
	{

		//temporal code that puts the image in every mesh avaliable
		for (int i = 0; i < App->editor3d->meshes.size(); i++)
		{
			App->editor3d->meshes[i].GenerateTexturefromILUT();
		}


		


		ilDeleteImages(1, &newImage);
	}
	return ret;
}
*/

bool Importer::LoadFBXfromBuffer(const char* Buffer, unsigned int Length)
{
	bool ret = false;

	const aiScene* scene = aiImportFileFromMemory(Buffer, Length, aiProcessPreset_TargetRealtime_MaxQuality, nullptr); //nullptr as we need no external libs to hepl import

										//aiImportFileFromMemory										
	LOG("Importing 3D asset from buffer: %s", Buffer);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array

		for (int i = 0; i < scene->mNumMeshes; i++)
		{

			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			std::vector<float> normals;
			std::vector<float>texCoords;
			//creates a new mesh at scene editor

			aiMesh* mesh = scene->mMeshes[i];
			LOG("----------Importing mesh %i----------", i);


			// copy vertices
			//newMesh.numVertex = mesh->mNumVertices;
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

				for (int j = 0; j < mesh->mNumFaces; j++)
				{

					if (mesh->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						indices.push_back(mesh->mFaces[j].mIndices[0]);
						indices.push_back(mesh->mFaces[j].mIndices[1]);
						indices.push_back(mesh->mFaces[j].mIndices[2]);

						//memcpy(&newMesh.index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));


					}

				}
				LOG("New mesh with %i indices", (indices.size()));
			}


			texCoords.reserve(indices.size() * 2); //there are 2 floats for every index
			LOG("Importing mesh texture coordinates");
			for (int j = 0; j < indices.size(); j++)
			{
				//copy TextureCoords
				if (mesh->mTextureCoords[0])
				{
					texCoords.push_back(mesh->mTextureCoords[0][j].x);
					texCoords.push_back(mesh->mTextureCoords[0][j].y);
				}
				else
				{
					LOG("No texture coordinates found");
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
				LOG("Mesh has no normals!");




			App->editor3d->meshes.push_back(Mesh(vertices, indices, normals, texCoords));
			LOG("----------Mesh %i has been loaded----------", i);
			vertices.clear();
			indices.clear();
			normals.clear();
			ret = true;
			mesh = nullptr;
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene % s", Buffer);
		ret = false;
	}
	return ret;
}
