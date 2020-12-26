#ifndef __C_AUDIO_SOURCE__
#define __C_AUDIO_SOURCE__

#include "Component.h"


class C_AudioSource:public Component
{
public:
	C_AudioSource(GameObject* owner, unsigned int ID);
	~C_AudioSource();

	void OnEditor();

private:

};

#endif // !__C_AUDIO_SOURCE__

