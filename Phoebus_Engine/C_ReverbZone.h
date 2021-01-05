#ifndef __C_REVERBZONE__
#define __C_REVERBZONE__

#include "Component.h"

class C_ReverbZone :public Component
{
public:
	C_ReverbZone(GameObject* owner, unsigned int ID);
	~C_ReverbZone();
	void OnEditor();
	bool GameUpdate(float gameDT)override;
	bool GameInit()override;
	bool Update(float dt);

	void SetReverbZone(float3 dimensions);
	void UpdateReverbZoneDimension();
	void GetAABBPoints(AABB& aabb, std::vector<float3>& emptyVector);
	bool DoesReverbZoneContainPoint(float3 point) const;
	float3 GetDimensions()const;
	std::string targetBus; //these 2 need to be in public. Do not move. -Adri
	float revValue;

private:
	float3 dimensions;
	bool dirtyUpdate;
	AABB revZone;
};

#endif // !__C_REVERBZONE__
