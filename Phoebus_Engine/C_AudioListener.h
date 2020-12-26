#ifndef __C_AUDIO_LISTENER__
#define __C_AUDIO_LISTENER__

#include "Component.h"


class C_AudioListener :public Component
{
public:
	C_AudioListener(GameObject* owner, unsigned int ID);
	~C_AudioListener();

	void OnEditor();

private:

};

#endif // !__C_AUDIO_LISTENER__

