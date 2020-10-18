#include<iostream>

#include "Mesh.h"

#include "Glew/include/glew.h" //order matters

//TODO normal structure done, still haven't found a way to implement them into the buffers


Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> normals) :
	drawMode(MeshDrawMode::DRAW_MODE_BOTH), normalMode(NormalDrawMode::NORMAL_MODE_NONE),
	idVertex(0), idIndex(0)
{
	this->vertices = vertices;
	this->indices = indices;
	this->normals = normals;
	GenerateBuffers();
}

Mesh::Mesh(const Mesh& other)
{
	this->vertices = other.vertices;
	this->indices = other.indices;
	this->normals = other.normals;
	this->drawMode = other.drawMode;
	this->normalMode = other.normalMode;
	GenerateBuffers();
}

Mesh::~Mesh()
{
	FreeBuffers();

	indices.clear();
	vertices.clear();
	normals.clear();

	drawMode = MeshDrawMode::DRAW_MODE_FILL;
	normalMode = NormalDrawMode::NORMAL_MODE_NONE;
}


//TODO for oscar : this could be structured better, FIX IT
void Mesh::Draw()
{

	if (idVertex == 0 || idIndex == 0)
		GenerateBuffers();

	if (normalMode == NormalDrawMode::NORMAL_MODE_VERTEX || normalMode == NormalDrawMode::NORMAL_MODE_BOTH)
		DrawVertexNormals();

	if (normalMode == NormalDrawMode::NORMAL_MODE_FACES || normalMode == NormalDrawMode::NORMAL_MODE_BOTH)
		DrawFacesNormals();


	glEnableClientState(GL_VERTEX_ARRAY);	//... TODO (1) Put this on start of render postupdate

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

		glBindBuffer(GL_ARRAY_BUFFER, idVertex);			//this is for printing the index
		glVertexPointer(3, GL_FLOAT, 0, NULL);				//Null => somehow OpenGL knows what you're talking about
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);	//Because this Bind is after the vertex bind, OpenGl knows these two are in order & connected. *Magic*	


		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);	//End of "bind addition" here...
		glColor3f(1.0f, 1.0f, 1.0f);//TODO change this for the default mesh color

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	glBindBuffer(GL_ARRAY_BUFFER, idVertex);			//this is for printing the index
	glVertexPointer(3, GL_FLOAT, 0, NULL);				//Null => somehow OpenGL knows what you're talking about
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);	//Because this Bind is after the vertex bind, OpenGl knows these two are in order & connected. *Magic*	


	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);	//End of "bind addition" here...


	glDisableClientState(GL_VERTEX_ARRAY); // ... TODO (2) Put this on end of render postupdate

	//clear buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::GenerateBuffers()
{

	//gen buffers
	glGenBuffers(1, &idVertex);
	glBindBuffer(GL_ARRAY_BUFFER, idVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &idIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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
}

void Mesh::DrawVertexNormals()
{
	glLineWidth(4.0f);
	glColor3f(1.0f, 0.25f, 0.0f);
	glBegin(GL_LINES);

	float magnitude = 10.0f;

	for (int i = 0; i < normals.size() / 3; i++)
	{
		glVertex3f(vertices[i * 3], vertices[(i * 3) + 1], vertices[(i * 3) + 2]);
		glVertex3f(vertices[i * 3] + normals[i * 3], vertices[(i * 3) + 1] + normals[(i * 3) + 1], vertices[(i * 3) + 2] + normals[(i * 3) + 2]);
	}



	glEnd();

	glLineWidth(2.0f);

}

void Mesh::DrawFacesNormals()
{

}
