#ifndef __C_CONTROL__
#define __C_CONTROL__

#include "Component.h"

class C_Control :public Component
{
public:
	C_Control(GameObject* owner, unsigned int ID);
	~C_Control();
	void OnEditor();
	bool GameUpdate(float gameDT)override;
	bool GameInit()override;
	bool ManageMovement(float gameDT);
	bool Update(float dt);
private:
	float speed;
};

#endif // !__C_CONTROL__
