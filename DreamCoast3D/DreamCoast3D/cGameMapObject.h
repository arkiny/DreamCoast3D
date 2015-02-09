#pragma once
#include "iMap.h"
//�ʿ��� ���� ������Ʈ�� �����̹Ƿ� ���ӿ�����Ʈ�� ����Ѵ�.
class cGameMapObject : public cGameObject, public iMap 
{
protected:
	SYNTHESIZE(int, m_nIndex, Index);
	int	m_arrAdjMapIndex[8]; // TODO : ���� �������ε��� ���� �����

public:
	cGameMapObject();
	virtual ~cGameMapObject();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	// iMap Override
	//�� ���� ���� ��� ���� �ٸ��Ƿ� ���� �������̵� ������Ѵ�.
	virtual float GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition) override;
};

