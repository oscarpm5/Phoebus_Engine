<<<<<<< HEAD

#pragma once
#include "glmath.h"
#include "Color.h"
#include "Glew/include/glew.h"
#include <vector>

//#include "PhysBody3D.h"
=======
#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

//#include "glmath.h" //it has to be incuded here because of the Identity Matrix
//#include "Color.h"
//#include "Glew/include/glew.h"
#include <vector>

>>>>>>> Development

enum class PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,

	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Cone, //cyl
	Primitive_Box,	//cyl
	Primitive_Unknown
};

<<<<<<< HEAD
class Primitive
{
public:
	Primitive();
	virtual ~Primitive();

    void			Draw() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	PrimitiveTypes	GetType() const;
	mat4x4			GetTransform() const;
	void			SetTransform(mat4x4 newTrans);
	void			SetTransform(float x, float y, float z, float angle, const vec3& u);
	vec3			GetBuffers();
public:
	
	bool wire;

protected:
	PrimitiveTypes type;
	mat4x4 transform;

	GLuint indexBind;  //bind that stores the index (that codify for (x,y,z) vertices)
	GLuint vertexBind; //bind that stores the vertex (3 floats for each one)
	GLuint indexSize;  //number of vertex that make up the primitive. Automatically setted inside each primitive.

protected:
	bool PrimitiveGenerateBuffers(float vertexArray[], uint indexArray[], uint vertexArraySize, uint indexArraySize);

};

//Related functions to primitives, but we don't want them to be part of the class
//=============================================

void SphereFillVectorsVertexAndIndex(std::vector<float> &vertices, std::vector<unsigned int> &index, float radius = 1.f, uint sectors = 36, uint stacks = 18);
void CylinderFillVectorsVertexAndIndex(std::vector<float>& vertices, std::vector<unsigned int>& index, float rBase = 1, float rTop = 1, float height = 2, uint sectors = 36, uint stacks = 8);
void ConeFillVectorsVertexAndIndex(std::vector<float>& vertices, std::vector<unsigned int>& indices, float rBase = 1, float height = 2, uint sectors = 36, uint stacks = 8);
// ============================================
class PCube : public Primitive
{
public :
	PCube(mat4x4 transform = IdentityMatrix, const vec3& size = vec3(1.f,1.f,1.f));

};

// ============================================
class PSphere : public Primitive
{
public:
	//sectors refer to polygon rows & stacks refer to polygon columns
	PSphere(mat4x4 transform= IdentityMatrix, float radius = 1.f, uint sectors=36,uint stacks=18);

	float GetRadius() const;

private:
	float radius;
};

// ============================================
class PCylinder : public Primitive
{
public:
	PCylinder(float rBase = 1, float rTop = 1, float height = 2, uint sectors = 36,uint stacks = 8,bool smooth = true);

	float GetTopRadius() const;
	float GetHeight() const;
	float GetBaseRadius() const;

	std::vector<float> getUnitCircleVertices(uint sectorcount);
private:
	float rBase;
	float rTop;
	float height;
};

// ============================================
class PLine : public Primitive
{
public:
	PLine();
	PLine(const vec3& A, const vec3& B);

	vec3 GetOrigin() const;
	vec3 GetDestination() const;

public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class PPlane : public Primitive
{
public:
	PPlane(const vec3& normal = vec3(0,1,0));

	vec3 GetNormal() const;

private:
	vec3 normal;
};
=======
//Related functions to primitives, but we don't want them to be part of the class
//=============================================

void SphereFillVectorsVertexAndIndex(std::vector<float> &vertices, std::vector<unsigned int> &index, float radius = 1.f, unsigned int sectors = 36, unsigned int stacks = 18);
void CylinderFillVectorsVertexAndIndex(std::vector<float>& vertices, std::vector<unsigned int>& index, float rBase = 1, float rTop = 1, float height = 2, unsigned int sectors = 36, unsigned int stacks = 8);
void ConeFillVectorsVertexAndIndex(std::vector<float>& vertices, std::vector<unsigned int>& indices, float rBase = 1, float height = 2, unsigned int sectors = 36, unsigned int stacks = 8);
// ============================================

#endif // !__PRIMITIVES_H__
>>>>>>> Development

