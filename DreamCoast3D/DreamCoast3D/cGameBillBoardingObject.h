#pragma once

// �׻� �÷��̾� ȭ���� �������� �ٶ󺸴� ������Ʈ
// �ش� ������Ʈ�� ���� �׼��� ���ٴ� �ɷ� ġ�� �������

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

