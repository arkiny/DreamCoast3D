#pragma once

class cAction;

class cGameActionObject : public cGameObject
{
protected:
	D3DXVECTOR3		m_vPrevPosition;
	cAction*		m_pAction;
	float			m_fMoveSpeed;

	//SYNTHESIZE(float, m_fMoveSpeed, MoveSpeed);
	
public:
	cGameActionObject();
	virtual ~cGameActionObject();

	virtual void Update(float fDelta);
	virtual void SetAction(cAction* pAction);
	virtual void SetMoveSpeed(float fMoveSpeed){ m_fMoveSpeed = fMoveSpeed; }
	virtual float GetMoveSpeed() override { return m_fMoveSpeed; }
};

