
#include "Globals.h"
#include "Glew/include/glew.h" //order matters
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#include "Application.h"


// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), wire(false), type(PrimitiveTypes::Primitive_Point)
{
	indexBind = 0;
	vertexBind = 0;
	indexSize = 0;
}

Primitive::~Primitive()
{
	//reset all values & delete buffers
	glDeleteBuffers(1, &this->vertexBind);
	glDeleteBuffers(1, &this->indexBind);
	transform = IdentityMatrix;
	wire = false;
	type = PrimitiveTypes::Primitive_Unknown;
	indexSize = 0;

}

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
	this->SetPos(x, y, z);
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

	//delete buffers in case they were already assigned

	glDeleteBuffers(1, &vertexBind);
	glDeleteBuffers(1, &indexBind);

	//gen buffers
	glGenBuffers(1, &vertexBind);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBind);
	glBufferData(GL_ARRAY_BUFFER, VAsize, vertexArray, GL_STATIC_DRAW);


	glGenBuffers(1, &indexBind);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IAsize, indexArray, GL_STATIC_DRAW);

	this->indexSize = IAsize / sizeof(unsigned int);

	//clear buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ret;
}

void SphereFillVectorsVertexAndIndex(std::vector<float> &vertices, std::vector<unsigned int> &indices, float radius, uint sectors, uint stacks)
{
		float x, y, z, xy;
		int k1, k2;

		// vertex position

		float sectorStep = 2 * pi / sectors;
		float stackStep = pi / stacks;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stacks; ++i)
		{
			stackAngle = pi / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectors; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
		}


		// generate CCW index list of sphere triangles
		for (int i = 0; i < stacks; ++i)
		{
			k1 = i * (sectors + 1);     // beginning of current stack
			k2 = k1 + sectors + 1;      // beginning of next stack

			for (int j = 0; j < sectors; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stacks - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
}

void CylinderFillVectorsVertexAndIndex(std::vector<float>& vertices, std::vector<unsigned int>& indices, float rBase, float rTop, float height, uint sectorCount, uint stacks)
{
	std::vector<float> unitCircleVertices; // this will be used in filling the vertices for the base and top

	float sectorStep = 2 * pi / sectorCount;
	float stackStep = pi / stacks;
	float sectorAngle, stackAngle;
	float radius;                     // radius for each stack
	float x, y, z;                     // vertex position
	// generate vertices for a cylinder



	for (int i = 0; i <= sectorCount; ++i) //filling spaces in a unit cirlce
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(cos(sectorAngle)); // x
		unitCircleVertices.push_back(sin(sectorAngle)); // y
		unitCircleVertices.push_back(0);                // z
	}

	// put side vertices to arrays
	for (int i = 0; i <= stacks; ++i)
	{
		z = -(height * 0.5f) + (float)i / stacks * height;      // vertex position z
		radius = rBase + (float)i / stacks * (rTop - rBase);     // lerp
		float t = 1.0f - (float)i / stacks;   // top-to-bottom

		for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
		{
			x = unitCircleVertices[k];
			y = unitCircleVertices[k + 1];
			vertices.push_back(x * radius);
			vertices.push_back(y * radius);
			vertices.push_back(z);
		}
	}
	// remember where the base.top vertices start
	unsigned int baseCenterIndex = (unsigned int)vertices.size() / 3;

	// put vertices of base of cylinder
	z = -height * 0.5f;
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(z);

	for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
	{
		x = unitCircleVertices[j];
		y = unitCircleVertices[j + 1];
		vertices.push_back(x * rBase);
		vertices.push_back(y * rBase);
		vertices.push_back(z);
	}

	// remember where the base vertices start
	unsigned int topCenterIndex = (unsigned int)vertices.size() / 3;

	// put vertices of top of cylinder
	z = height * 0.5f;
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(z);

	for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
	{
		x = unitCircleVertices[j];
		y = unitCircleVertices[j + 1];
		vertices.push_back(x * rTop);
		vertices.push_back(y * rTop);
		vertices.push_back(z);
	}

	// generate CCW index list of cylinder triangles
	uint k1, k2;

	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (sectorCount + 1);     // bebinning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 trianles per sector

			indices.push_back(k1);
			indices.push_back(k1 + 1);
			indices.push_back(k2);

			indices.push_back(k2);
			indices.push_back(k1 + 1);
			indices.push_back(k2 + 1);

		}
	}


	// remember where the base indices start
	//uint baseIndex = (unsigned int)indices.size();

	// put indices for base
	for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < (sectorCount - 1))
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(k + 1);
			indices.push_back(k);
		}
		else // last triangle
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(baseCenterIndex + 1);
			indices.push_back(k);
		}
	}

	// remember where the base indices start

	for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < (sectorCount - 1)) {
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(k + 1);
		}

		else {
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(topCenterIndex + 1);
		}
	}
}

void ConeFillVectorsVertexAndIndex(std::vector<float>& vertices, std::vector<unsigned int>& indices, float rBase, float height, uint sectors, uint stacks)
{
	CylinderFillVectorsVertexAndIndex(vertices, indices, rBase, 0, height, sectors, stacks); //Megamind
}


// ------------------------------------------------------------
void Primitive::Draw() const
{
	glPushMatrix();

	glMultMatrixf(transform.M);

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);	//... TODO (1) Put this on startt of render postupdate

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBind);			//this is for printing the index
	glVertexPointer(3, GL_FLOAT, 0, NULL);				//Null => somehow OpenGL knows what you're talking about
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBind);	//Because this Bind is after the vertex bind, OpenGl knows these two are in order & connected. *Magic*	
	glDrawElements(GL_TRIANGLES, this->indexSize, GL_UNSIGNED_INT, NULL);	//End of "bind addition" here...

	glDisableClientState(GL_VERTEX_ARRAY); // ... TODO (2) Put this on end of render postupdate


	//clear buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPopMatrix();
}

// ------------------------------------------------------------

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);

}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3& u)
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
	size.x, 0.f, 0.f,
	size.x, 0.f, -size.z,
	0, 0, -size.z,

	0.f, size.y, 0.f,
	size.x, size.y, 0.f,
	size.x, size.y, -size.z,
	0.f, size.y, -size.z
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

	PrimitiveGenerateBuffers(vertexArrayCube, indexArrayCube, sizeof(vertexArrayCube), sizeof(indexArrayCube));


	//TODO: Add this to some array of shit on scene
}


// SPHERE ============================================

PSphere::PSphere(mat4x4 transform, float _radius, uint sectors, uint stacks) : Primitive()
{
	radius = _radius;
	type = PrimitiveTypes::Primitive_Sphere;
	SetTransform(transform);


	std::vector<float> vertices;
	std::vector<uint> indices;
	float x, y, z, xy;
	int k1, k2;

	// vertex position

	float sectorStep = 2 * pi / sectors;
	float stackStep = pi / stacks;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stacks; ++i)
	{
		stackAngle = pi / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}


	// generate CCW index list of sphere triangles
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (sectors + 1);     // beginning of current stack
		k2 = k1 + sectors + 1;      // beginning of next stack

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}




	PrimitiveGenerateBuffers(vertices.data(), indices.data(), vertices.size() * sizeof(float), indices.size() * sizeof(unsigned int));

	vertices.clear();
	indices.clear();
}

float PSphere::GetRadius() const
{
	return radius;
}




// CYLINDER ============================================
PCylinder::PCylinder(float rBase, float rTop, float height, uint sectorCount, uint stacks, bool smooth) : Primitive(), height(height), rBase(rBase), rTop(rTop)
{
	//Disclaimer: this "cylinder" is also a cube, a box, a cone, a pyramid, a frustrum, and whatever you want it to be honestly (except spheres)
	
	type = PrimitiveTypes::Primitive_Cylinder;

	std::vector<float> vertices;
	std::vector<uint> indices;
	std::vector<float> unitCircleVertices; // this will be used in filling the vertices for the base and top

	float sectorStep = 2 * pi / sectorCount;
	float stackStep = pi / stacks;
	float sectorAngle, stackAngle;
	float radius;                     // radius for each stack
	float x, y, z;                     // vertex position
	// generate vertices for a cylinder



	for (int i = 0; i <= sectorCount; ++i) //filling spaces in a unit cirlce
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(cos(sectorAngle)); // x
		unitCircleVertices.push_back(sin(sectorAngle)); // y
		unitCircleVertices.push_back(0);                // z
	}

	// put side vertices to arrays
	for (int i = 0; i <= stacks; ++i)
	{
		z = -(height * 0.5f) + (float)i / stacks * height;      // vertex position z
		radius = rBase + (float)i / stacks * (rTop - rBase);     // lerp
		float t = 1.0f - (float)i / stacks;   // top-to-bottom

		for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
		{
			x = unitCircleVertices[k];
			y = unitCircleVertices[k + 1];
			vertices.push_back(x * radius);
			vertices.push_back(y * radius);
			vertices.push_back(z);
		}
	}
	// remember where the base.top vertices start
	unsigned int baseCenterIndex = (unsigned int)vertices.size() / 3;

	// put vertices of base of cylinder
	z = -height * 0.5f;
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(z);
	
	for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
	{
		x = unitCircleVertices[j];
		y = unitCircleVertices[j + 1];
		vertices.push_back(x * rBase);
		vertices.push_back(y * rBase);
		vertices.push_back(z);
	}

	// remember where the base vertices start
	unsigned int topCenterIndex = (unsigned int)vertices.size() / 3;

	// put vertices of top of cylinder
	z = height * 0.5f;
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(z);
	
	for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
	{
		x = unitCircleVertices[j];
		y = unitCircleVertices[j + 1];
		vertices.push_back(x * rTop);
		vertices.push_back(y * rTop);
		vertices.push_back(z);
	}

	// generate CCW index list of cylinder triangles
	uint k1, k2;

	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (sectorCount + 1);     // bebinning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 trianles per sector
			
			indices.push_back(k1);
			indices.push_back(k1 + 1);
			indices.push_back(k2);
			
			indices.push_back(k2);
			indices.push_back(k1 + 1);
			indices.push_back(k2 + 1);
			
		}
	}


	// remember where the base indices start
	//uint baseIndex = (unsigned int)indices.size();

	// put indices for base
	for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < (sectorCount - 1))
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(k + 1);
			indices.push_back(k);
		}
		else // last triangle
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(baseCenterIndex + 1);
			indices.push_back(k);
		}
	}

	// remember where the base indices start

	for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < (sectorCount - 1)) {
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(k+1);
		}
			
		else {
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(topCenterIndex + 1);
		}
	}

	PrimitiveGenerateBuffers(vertices.data(), indices.data(), vertices.size() * sizeof(float), indices.size() * sizeof(unsigned int));

	vertices.clear();
	indices.clear();
}

float PCylinder::GetTopRadius() const
{
	return rTop;
}

float PCylinder::GetBaseRadius() const
{
	return rBase;
}

std::vector<float> PCylinder::getUnitCircleVertices(uint sectorcount)
{
	const float PI = 3.1415926f;
	float sectorStep = 2 * PI / sectorcount;
	float sectorAngle;  // radian

	std::vector<float> unitCircleVertices;
	for (int i = 0; i <= sectorcount; ++i)
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(cos(sectorAngle)); // x
		unitCircleVertices.push_back(sin(sectorAngle)); // y
		unitCircleVertices.push_back(0);                // z
	}
	return unitCircleVertices;
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


