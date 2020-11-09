#ifndef __RENDER_AABB__
#define __RENDER_AABB__

#include "MathGeoLib/include/MathGeoLib.h"
#include "Color.h"

class RenderAABB
{
public:
	RenderAABB(AABB renderBox, Color col = Color(1.0f, 1.0f, 1.0f));

	~RenderAABB();

	void Draw();

private:
	AABB renderBox;
	Color bbColor;
};



#endif // !__RENDER_AABB__
