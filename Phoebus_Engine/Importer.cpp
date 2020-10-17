#include "Importer.h"
#include "Globals.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

void Importer::LoadFBX(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			std::vector<float> normals;
			//creates a new mesh at scene editor

			aiMesh* mesh = scene->mMeshes[i];


			// copy vertices
			//newMesh.numVertex = mesh->mNumVertices;
			vertices.reserve(mesh->mNumVertices * 3);
			
			for (int i = 0; i < mesh->mNumVertices; i++)
			{
				vertices.push_back(mesh->mVertices[i].x);
				vertices.push_back(mesh->mVertices[i].y);
				vertices.push_back(mesh->mVertices[i].z);
			}
			//memcpy(newMesh.vertices, mesh->mVertices, sizeof(float) * newMesh.numVertex * 3);
			LOG("New mesh with %i vertices", (vertices.size()/3));

			// copy faces
			if (mesh->HasFaces())
			{
				//newMesh.numIndex = mesh->mNumFaces * 3;
				//newMesh.index = new uint[newMesh.numIndex]; // assume each face is a triangle
				indices.reserve(mesh->mNumFaces * 3);
				for (int i = 0; i < mesh->mNumFaces; i++)
				{

					if (mesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						indices.push_back(mesh->mFaces[i].mIndices[0]);
						indices.push_back(mesh->mFaces[i].mIndices[1]);
						indices.push_back(mesh->mFaces[i].mIndices[2]);

						//memcpy(&newMesh.index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			//copy normals
			if (mesh->HasNormals())
			{
				normals.reserve(mesh->mNumVertices * 3);
				for (int i = 0; i < mesh->mNumVertices; i++)
				{

						normals.push_back(mesh->mNormals[i].x);
						normals.push_back(mesh->mNormals[i].y);
						normals.push_back(mesh->mNormals[i].z);					
				}
			}


			App->editor3d->meshes.push_back(Mesh(vertices, indices,normals));
			vertices.clear();
			indices.clear();
			normals.clear();

			mesh = nullptr;
		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene % s", path);
}
