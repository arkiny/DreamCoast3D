#pragma once
#include "cAction.h"

// AI�� ��� �׼��� �߽�
// ��� AI�� ��� �����ϰ� �����鼭
// �ֺ��� �����Ѵ�.

class cAcitionThink : public cAction
{
public:
	cAcitionThink();
	virtual ~cAcitionThink();

	virtual void Start(){}
	virtual void Update(float fDelta) {}
	virtual void Exit(){}
};

