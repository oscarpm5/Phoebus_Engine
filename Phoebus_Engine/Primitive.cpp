
#include "Globals.h"
#include "Glew/include/glew.h" //order matters
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#include "Globals.h"
#include "Application.h"


// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), wire(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

mat4x4 Primitive::GetTransform() const
{
	return this->transform;
}

void Primitive::SetTransform(mat4x4 newTrans)
{
	this->transform = newTrans;
}

void Primitive::SetTransform(float x, float y, float z, float angle, const vec3& u)
{
	this->SetPos(x,y,z);
	this->SetRotation(angle, u);
}

// x = vertex , y = index, z = index count
vec3 Primitive::GetBuffers()
{
	return vec3(this->vertexBind, this->indexBind, this->indexSize);
}

bool Primitive::PrimitiveGenerateBuffers(float vertexArray[], uint indexArray[], uint VAsize, uint IAsize)
{
	bool ret = true;
	
	glGenBuffers(1, (GLuint*)&(this->vertexBind));
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBind);
	glBufferData(GL_ARRAY_BUFFER, VAsize, vertexArray, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)&(this->indexBind));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IAsize, indexArray, GL_STATIC_DRAW);

	this->indexSize = IAsize / sizeof(unsigned int);
							

	return ret;
}


// ------------------------------------------------------------
void Primitive::Draw() const
{

	glEnableClientState(GL_VERTEX_ARRAY);	//... TODO (1) Put this on startt of render postupdate

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBind);			//this is for printing the index
	glVertexPointer(3, GL_FLOAT, 0, NULL);				//Null => somehow OpenGL knows what you're talking about
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBind);	//Because this Bind is after the vertex bind, OpenGl knows these two are in order & connected. *Magic*	
	glDrawElements(GL_TRIANGLES, this->indexSize, GL_UNSIGNED_INT, NULL);	//End of "bind addition" here...

	glDisableClientState(GL_VERTEX_ARRAY); // ... TODO (2) Put this on end of render postupdate

}

// ------------------------------------------------------------

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
	
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
	
}

// ------------------------------------------------------------







// CUBE ============================================
PCube::PCube(mat4x4 transform, const vec3& size) : Primitive()
{
	type = PrimitiveTypes::Primitive_Cube;
	SetTransform(transform);

	float vertexArrayCube[] = {
	0.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, -1.f,
	0, 0, -1.f,

	0.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, -1.f,
	0.f, 1.f, -1.f
	};

	uint indexArrayCube[] = {
		4, 0, 1,
		1, 5, 4,

		4, 7, 3,
		3, 0, 4,

		2, 3, 7,
		7, 6, 2,

		7, 4, 5,
		5, 6, 7,

		5, 1, 2,
		2, 6, 5,

		0, 3, 2,
		2, 1, 0
	};

	PrimitiveGenerateBuffers(vertexArrayCube, indexArrayCube, sizeof(vertexArrayCube),sizeof(indexArrayCube));

	//TODO: Add this to some array of shit on scene

	//TODO: SIZE!!!
}


// SPHERE ============================================

PSphere::PSphere(float _radius, float mass) : Primitive(), radius(_radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
	//body.SetBody(this, mass);
}

float PSphere::GetRadius() const
{
	return radius;
}




// CYLINDER ============================================
PCylinder::PCylinder(float radius, float height, float mass) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

float PCylinder::GetRadius() const
{
	return radius;
}

float PCylinder::GetHeight() const
{
	return height;
}


// LINE ==================================================
PLine::PLine() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

PLine::PLine(const vec3& A, const vec3& B) : Primitive(), origin(A), destination(B)
{
	type = PrimitiveTypes::Primitive_Line;
}

vec3 PLine::GetOrigin() const
{
	return origin;
}

vec3 PLine::GetDestination() const
{
	return destination;
}


// PLANE ==================================================
PPlane::PPlane(const vec3& _normal) : Primitive(), normal(_normal)
{
	type = PrimitiveTypes::Primitive_Plane;
}

vec3 PPlane::GetNormal() const
{
	return normal;
}


