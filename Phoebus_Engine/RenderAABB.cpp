#include "RenderAABB.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/include/MathGeoLib.h"

RenderAABB::RenderAABB(AABB renderBox, Color col):renderBox(renderBox),bbColor(col)
{
}

RenderAABB::~RenderAABB()
{
	renderBox.SetNegativeInfinity();
	bbColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
}

void RenderAABB::Draw()
{
	float3 minPoint = renderBox.minPoint;
	float3 maxPoint = renderBox.maxPoint;
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	glLineWidth(2.0f);
	glColor4f(bbColor.r, bbColor.g, bbColor.b, bbColor.a);
	

	//Min lines
	glVertex3f(minPoint.x,minPoint.y,minPoint.z);
	glVertex3f(minPoint.x, minPoint.y, maxPoint.z);

	glVertex3f(minPoint.x, minPoint.y, minPoint.z);
	glVertex3f(minPoint.x, maxPoint.y, minPoint.z);

	glVertex3f(minPoint.x, minPoint.y, minPoint.z);
	glVertex3f(maxPoint.x, minPoint.y, minPoint.z);


	//Max lines
	glVertex3f(maxPoint.x, maxPoint.y, maxPoint.z);
	glVertex3f(maxPoint.x, maxPoint.y, minPoint.z);

	glVertex3f(maxPoint.x, maxPoint.y, maxPoint.z);
	glVertex3f(maxPoint.x, minPoint.y, maxPoint.z);

	glVertex3f(maxPoint.x, maxPoint.y, maxPoint.z);
	glVertex3f(minPoint.x, maxPoint.y, maxPoint.z);

	//Other lines
	//1
	glVertex3f(minPoint.x, minPoint.y, maxPoint.z);
	glVertex3f(minPoint.x, maxPoint.y, maxPoint.z);
	
	glVertex3f(minPoint.x, minPoint.y, maxPoint.z);
	glVertex3f(maxPoint.x, minPoint.y, maxPoint.z);
	//2
	glVertex3f(maxPoint.x, minPoint.y, minPoint.z);
	glVertex3f(maxPoint.x, minPoint.y, maxPoint.z);
	
	glVertex3f(maxPoint.x, minPoint.y, minPoint.z);
	glVertex3f(maxPoint.x, maxPoint.y, minPoint.z);
	//3
	glVertex3f(minPoint.x, maxPoint.y, minPoint.z);
	glVertex3f(minPoint.x, maxPoint.y, maxPoint.z);
	
	glVertex3f(minPoint.x, maxPoint.y, minPoint.z);
	glVertex3f(maxPoint.x, maxPoint.y, minPoint.z);
	
	glEnd();
	glEnable(GL_LIGHTING);

}
