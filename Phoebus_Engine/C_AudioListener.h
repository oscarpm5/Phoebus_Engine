#ifndef __C_AUDIO_LISTENER__
#define __C_AUDIO_LISTENER__

#include "Component.h"


class C_AudioListener :public Component
{
public:
	C_AudioListener(GameObject* owner, unsigned int ID);
	~C_AudioListener();
	void SetAsListener(bool newState);
	bool GetIsListener()const;
	void OnEditor();

private:
	bool isListener;

};

#endif // !__C_AUDIO_LISTENER__

