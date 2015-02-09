#pragma once

class cAction;

class cGameActionObject : public cGameObject
{
protected:
	D3DXVECTOR3		m_vPrevPosition;
	cAction*		m_pAction;
	SYNTHESIZE(float, m_fMoveSpeed, MoveSpeed);
	
public:
	cGameActionObject();
	virtual ~cGameActionObject();

	virtual void Update(float fDelta);
	virtual void SetAction(cAction* pAction);
};

