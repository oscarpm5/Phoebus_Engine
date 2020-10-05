
#pragma once
#include "glmath.h"
#include "Color.h"
//#include "PhysBody3D.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:
	Primitive();

	void Update();
	virtual void	Render() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;


	Color color;
	mat4x4 transform;
	bool axis,wire;
	//PhysBody3D body;

protected:
	virtual void InnerRender() const;
	PrimitiveTypes type;
};

// ============================================
class PCube : public Primitive
{
public :
	PCube(const vec3& size = vec3(1.f,1.f,1.f), float mass = 1.f);

	vec3 GetSize() const;
protected:
	void InnerRender() const;
private:
	vec3 size;
};

// ============================================
class PSphere : public Primitive
{
public:
	PSphere(float radius = 1.f, float mass = 1.f);

	float GetRadius() const;
protected:
	void InnerRender() const;
private:
	float radius;
};

// ============================================
class PCylinder : public Primitive
{
public:
	PCylinder(float radius = 1.f, float height = 2.f, float mass = 1.f);

	float GetRadius() const;
	float GetHeight() const;
protected:
	void InnerRender() const;
private:
	float radius;
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

protected:
	void InnerRender() const;
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
protected:
	void InnerRender() const;
private:
	vec3 normal;
};