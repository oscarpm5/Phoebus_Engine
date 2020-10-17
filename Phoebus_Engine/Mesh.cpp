#include<iostream>

#include "Mesh.h"

#include "Glew/include/glew.h" //order matters



Mesh::Mesh() :drawMode(MeshDrawMode::DRAW_MODE_FILL),
idVertex(0), numVertex(0), vertex(nullptr),
idIndex(0), numIndex(0), index(nullptr)
{
}

Mesh::~Mesh()
{
	FreeBuffers();


	if (vertex != nullptr)
	{
		delete[]vertex;
		vertex = nullptr;
	}

	if (index != nullptr)
	{
		delete[]index;
		index = nullptr;
	}


	numIndex = -1;
	numVertex = -1;

	drawMode = MeshDrawMode::DRAW_MODE_FILL;
}


//TODO for oscar : this could be structured better, FIX IT
void Mesh::Draw() const
{

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


		glDrawElements(GL_TRIANGLES, numIndex, GL_UNSIGNED_INT, NULL);	//End of "bind addition" here...
		glColor3f(1.0f, 1.0f, 1.0f);//TODO change this for the default mesh color

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	glBindBuffer(GL_ARRAY_BUFFER, idVertex);			//this is for printing the index
	glVertexPointer(3, GL_FLOAT, 0, NULL);				//Null => somehow OpenGL knows what you're talking about
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);	//Because this Bind is after the vertex bind, OpenGl knows these two are in order & connected. *Magic*	


	glDrawElements(GL_TRIANGLES, numIndex, GL_UNSIGNED_INT, NULL);	//End of "bind addition" here...



	glDisableClientState(GL_VERTEX_ARRAY); // ... TODO (2) Put this on end of render postupdate


	//clear buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::GenerateBuffers()
{
	bool ret = true;
	if (vertex == nullptr)
	{
		LOG("ERROR, cannot generate buffers: no vertex has been assigned to the mesh");
		ret = false;
	}

	if (index == nullptr)
	{
		LOG("ERROR, cannot generate buffers: no index has been assigned to the mesh");
		ret = false;
	}

	if (ret)
	{
		//delete buffers in case they were already assigned

		FreeBuffers();

		//gen buffers
		glGenBuffers(1, (GLuint*)&this->idVertex);
		glBindBuffer(GL_ARRAY_BUFFER, idVertex);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, vertex, GL_STATIC_DRAW);


		glGenBuffers(1, (GLuint*)& this->idIndex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndex * sizeof(unsigned int), index, GL_STATIC_DRAW);

		//clear buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

}

void Mesh::FreeBuffers()
{
	if (idVertex != 0)
	{
		glDeleteBuffers(1, &this->idVertex);
		idVertex = 0;
	}
	if (idIndex != 0)
	{
		glDeleteBuffers(1, &this->idIndex);
		idIndex = 0;
	}
}
