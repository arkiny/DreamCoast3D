#pragma once
#include "iMap.h"
//맵역시 게임 오브젝트의 일종이므로 게임오브젝트를 상속한다.
class cGameMapObject : public cGameObject, public iMap 
{
protected:
	SYNTHESIZE(int, m_nIndex, Index);
	int	m_arrAdjMapIndex[8]; // TODO : 차후 인접맵인덱스 정보 저장용

public:
	cGameMapObject();
	virtual ~cGameMapObject();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	// iMap Override
	//각 맵은 높이 재는 법이 다르므로 따로 오버라이드 해줘야한다.
	virtual float GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition) override;
};

