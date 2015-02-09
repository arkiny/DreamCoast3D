#pragma once
#include "cGameBillBoardingObject.h"

class cBillingExample : public cGameBillBoardingObject
{
private:
	LPD3DXMESH							m_pMesh;
	D3DMATERIAL9						m_stFontMtl;

public:
	cBillingExample();
	~cBillingExample();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
};

