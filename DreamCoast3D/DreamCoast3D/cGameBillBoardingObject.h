#pragma once

// 항상 플레이어 화면을 정면으로 바라보는 오브젝트
// 해당 오브젝트는 따로 액션이 없다는 걸로 치고 만들었음

class cGameBillBoardingObject : public cGameObject
{
protected:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vLocalPosition, LocalPosition);

public:
	cGameBillBoardingObject();
	virtual ~cGameBillBoardingObject();

	//virtual void Update(float fDelta);
	virtual void Render();
};

