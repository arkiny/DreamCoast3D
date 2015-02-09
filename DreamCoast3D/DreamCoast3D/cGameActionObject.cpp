#include "stdafx.h"
#include "cGameActionObject.h"
#include "cTransform.h"
#include "cAction.h"

cGameActionObject::cGameActionObject()
	:cGameObject(),
	m_pAction(NULL),
	m_vPrevPosition(0,0,0),
	m_fMoveSpeed(1.0f)
{
}


cGameActionObject::~cGameActionObject()
{
	SAFE_RELEASE(m_pAction);
}

void cGameActionObject::Update(float fDelta){
	cGameObject::Update(fDelta);

	if (m_pAction){
		m_vPrevPosition = m_pTransform->GetPosition();
		m_pAction->Update(fDelta);
		D3DXVECTOR3 vCurrPosition = m_pTransform->GetPosition();
		D3DXVECTOR3 vDeltaPosition = vCurrPosition - m_vPrevPosition;

		if (fabs(vDeltaPosition.x) > 0.0001f)
		{
			float fAngle;
			fAngle = -atan2(vDeltaPosition.z, vDeltaPosition.x) - D3DXToRadian(-270.0f);
			m_pTransform->SetYAxisAngle(fAngle);
		}
		else{
			if (vDeltaPosition.z > 0.0f){
				m_pTransform->SetYAxisAngle(D3DXToRadian(-180.0f));
			}
			else {
				m_pTransform->SetYAxisAngle(D3DXToRadian(0.0f));
			}
		}
	}

}

void cGameActionObject::SetAction(cAction* pAction){
	if (m_pAction != pAction)
	{
		pAction->SetOwner(this);
		SAFE_ADD_REF(pAction);
		SAFE_RELEASE(m_pAction);
		m_pAction = pAction;
	}
}