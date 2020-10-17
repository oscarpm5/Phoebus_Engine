#include<iostream>

#include "Mesh.h"

#include "Glew/include/glew.h" //order matters



Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices) :
drawMode(MeshDrawMode::DRAW_MODE_BOTH),
idVertex(0), numVertex(0),idIndex(0)
{
	this->vertices = vertices;
	this->indices = indices;

	
}

Mesh::~Mesh()
{
	FreeBuffers();

	indices.clear();
	vertices.clear();

	numVertex = 0;


	drawMode = MeshDrawMode::DRAW_MODE_FILL;
}


//TODO for oscar : this could be structured better, FIX IT
void Mesh::Draw()
{

	if(idVertex==0||idIndex==0)
		GenerateBuffers();

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
