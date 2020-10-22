#include<iostream>

#include "Mesh.h"

#include "Glew/include/glew.h" //order matters
#include "glmath.h"

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

//TODO normal structure done, still haven't found a way to implement them into the buffers


Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> normals, std::vector<float> texCoords,NewTexture* texture) :
	drawMode(MeshDrawMode::DRAW_MODE_FILL), normalMode(NormalDrawMode::NORMAL_MODE_NONE),
	idVertex(0), idIndex(0), idNormals(0), idTexCoords(0), shadingFlat(true), texture(texture)
{
	this->vertices = vertices;
	this->indices = indices;
	this->normals = normals;
	this->texCoords = texCoords;
	GenerateBuffers();
}

Mesh::Mesh(const Mesh& other)
{
	this->vertices = other.vertices;
	this->indices = other.indices;
	this->normals = other.normals;
	this->texCoords = other.texCoords;
	this->drawMode = other.drawMode;
	this->normalMode = other.normalMode;
	this->shadingFlat = other.shadingFlat;
	this->texture = other.texture;
	
	GenerateBuffers();
}

Mesh::~Mesh()
{
	FreeBuffers();

	indices.clear();
	vertices.clear();
	normals.clear();
	texCoords.clear();

	texture = nullptr;//TODO tell the texture that this user is not using it, and delete accordingly

	drawMode = MeshDrawMode::DRAW_MODE_FILL;
	normalMode = NormalDrawMode::NORMAL_MODE_NONE;
}


//TODO for oscar : this could be structured better, FIX IT
void Mesh::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);	//... TODO (1) Put this on start of render postupdate
	if (shadingFlat)glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	/*if (idVertex == 0 || idIndex == 0 ||idNormals==0|idTexCoords==0)
		GenerateBuffers();*/

	if (normalMode == NormalDrawMode::NORMAL_MODE_VERTEX || normalMode == NormalDrawMode::NORMAL_MODE_BOTH)
		DrawVertexNormals();

	if (normalMode == NormalDrawMode::NORMAL_MODE_FACES || normalMode == NormalDrawMode::NORMAL_MODE_BOTH)
		DrawFacesNormals();


	glColor3f(1.0f, 1.0f, 1.0f);//TODO change this for the default mesh color

	if (drawMode == MeshDrawMode::DRAW_MODE_WIRE)
	{
		glColor3f(0.5f, 0.5f, 0.5f); //TODO change this for the default wireframe color
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (drawMode == MeshDrawMode::DRAW_MODE_FILL)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glColor3f(0.5f, 0.5f, 0.5f); //TODO change this for the default wireframe color

		DrawBuffers();
		
		glColor3f(1.0f, 1.0f, 1.0f);//TODO change this for the default mesh color

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	DrawBuffers();
	
	
	glDisableClientState(GL_VERTEX_ARRAY); // ... TODO (2) Put this on end of render postupdate
	if (shadingFlat)glDisableClientState(GL_NORMAL_ARRAY); // ... TODO (2) Put this on end of render postupdate
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//
	
	//clear buffers

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void Mesh::GenerateBuffers()
{

	//gen buffers=========================================
	 
	//vertex buffer
	glGenBuffers(1, &idVertex);
	glBindBuffer(GL_ARRAY_BUFFER, idVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	

	
	//index buffer
	glGenBuffers(1, &idIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	

	
	//normal buffer
	glGenBuffers(1, &idNormals);
	glBindBuffer(GL_ARRAY_BUFFER, idNormals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
	

	
	//texture coordinate buffer
	glGenBuffers(1, &idTexCoords);
	glBindBuffer(GL_ARRAY_BUFFER, idTexCoords);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), &texCoords[0], GL_STATIC_DRAW);
	

	//clear buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



}

void Mesh::FreeBuffers()
{
	if (idVertex != 0)
	{
		glDeleteBuffers(1, &idVertex);
		idVertex = 0;
	}
	if (idIndex != 0)
	{
		glDeleteBuffers(1, &idIndex);
		idIndex = 0;
	}
	if (idNormals != 0)
	{
		glDeleteBuffers(1, &idNormals);
		idNormals = 0;
	}
	if (idTexCoords != 0)
	{
		glDeleteBuffers(1, &idTexCoords);
		idTexCoords = 0;
	}

}

void Mesh::DrawVertexNormals()
{
	float magnitude = 2.0f;
	glColor3f(1.0f, 0.5f, 0.0f);
	glLineWidth(4.0f);
	glBegin(GL_LINES);


	for (int i = 0; i < normals.size() / 3; i++)
	{
		vec3 vertex0 = { vertices[i * 3], vertices[(i * 3) + 1], vertices[(i * 3) + 2] };
		vec3 vertex1 =
		{
			vertices[i * 3] + (normals[i * 3] * magnitude),
			vertices[(i * 3) + 1] + (normals[(i * 3) + 1] * magnitude),
			vertices[(i * 3) + 2] + (normals[(i * 3) + 2] * magnitude)
		};


		glVertex3f(vertex0.x, vertex0.y, vertex0.z);
		glVertex3f(vertex1.x, vertex1.y, vertex1.z);
	}



	glEnd();

	glLineWidth(2.0f);

}

void Mesh::DrawFacesNormals()
{


	float magnitude = 6.0f;
	glColor3f(0.0f, 0.25f, 1.0f);
	glLineWidth(4.0f);


	glBegin(GL_LINES);

	for (int i = 0; i < indices.size(); i += 3)
	{
		unsigned int vertex0id = indices[i];
		unsigned int vertex1id = indices[i + 1];
		unsigned int vertex2id = indices[i + 2];


		vec3 vertex0 = { vertices[vertex0id * 3], vertices[(vertex0id * 3) + 1], vertices[(vertex0id * 3) + 2] };
		vec3 vertex1 = { vertices[vertex1id * 3], vertices[(vertex1id * 3) + 1], vertices[(vertex1id * 3) + 2] };
		vec3 vertex2 = { vertices[vertex2id * 3], vertices[(vertex2id * 3) + 1], vertices[(vertex2id * 3) + 2] };

		vec3 vector01 = vertex1 - vertex0;//vector from point 0 to point 1
		vec3 vector02 = vertex2 - vertex0;//vector from point 0 to point 2
		vec3 normal = normalize(cross(vector01, vector02));
		normal *= magnitude;


		vec3 center = (vertex0 + vertex1 + vertex2) / 3;
		//Provisional placement (it displays the normal in the first vertex)
		glVertex3f(center.x, center.y, center.z);
		glVertex3f((center.x + normal.x), (center.y + normal.y), (center.z + normal.z));

	}

	glEnd();

	glLineWidth(2.0f);
}

void Mesh::DrawBuffers()
{

	glBindBuffer(GL_ARRAY_BUFFER, idVertex);			//this is for printing the index
	glVertexPointer(3, GL_FLOAT, 0, NULL);				//Null => somehow OpenGL knows what you're talking about
	
	
	if (shadingFlat)
	{
		glBindBuffer(GL_ARRAY_BUFFER, idNormals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}

	
	glBindBuffer(GL_ARRAY_BUFFER, idTexCoords);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);	//Because this Bind is after the vertex bind, OpenGl knows these two are in order & connected. *Magic*	

	if(texture!=nullptr&&texture->HasTexture())
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);	//End of "bind addition" here...
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
