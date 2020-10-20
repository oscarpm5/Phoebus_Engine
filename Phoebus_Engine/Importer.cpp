#include "Importer.h"
#include "Globals.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

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
			LOG("----------Mesh %i has been loaded----------",i);
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
