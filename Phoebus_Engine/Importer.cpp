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
			Mesh &newMesh = App->editor3d->NewMesh(); //creates a new mesh at scene editor
			newMesh.drawMode = MeshDrawMode::DRAW_MODE_BOTH;
			aiMesh* mesh = scene->mMeshes[i];


			// copy vertices
			newMesh.numVertex = mesh->mNumVertices;
			newMesh.vertex = new float[newMesh.numVertex * 3];
			memcpy(newMesh.vertex, mesh->mVertices, sizeof(float) * newMesh.numVertex * 3);
			LOG("New mesh with %d vertices", newMesh.numVertex);

			// copy faces
			if (mesh->HasFaces())
			{
				newMesh.numIndex = mesh->mNumFaces * 3;
				newMesh.index = new uint[newMesh.numIndex]; // assume each face is a triangle
				for (uint i = 0; i < mesh->mNumFaces; ++i)
				{

					if (mesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&newMesh.index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			newMesh.GenerateBuffers();

		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene % s", path);
}
