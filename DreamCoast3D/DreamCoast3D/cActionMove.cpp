#include "stdafx.h"
#include "cActionMove.h"
#include "cTransform.h"
#include "cGameActionObject.h"

cActionMove::cActionMove()
{
	m_fActionTime = 1.0f;
}


cActionMove::~cActionMove()
{
}


void cActionMove::Start(){
	m_fPassedTime = 0.0f;
	if (m_pOwner)
	{
		cTransform* pTransform = m_pOwner->GetTransform();
		pTransform->SetPosition(m_vFrom);
	}
}

void cActionMove::Update(float delta){
	m_vPrevPosition = m_pOwner->GetTransform()->GetPosition();
	if (m_bNoMove == false){
		m_fPassedTime += delta;
	}
	else {
		m_fPassedTime = 0;
	}
	//
	float t = m_fPassedTime / m_fActionTime;
	if (t < 1.0f)
	{
		D3DXVECTOR3 v;
		//p = (1 - t) * p0 + t * p1
		D3DXVec3Lerp(&v, &m_vFrom, &m_vTo, t);
		cTransform* pTransform = m_pOwner->GetTransform();
		pTransform->SetPosition(v);
	}
	else
	{
		cTransform* pTransform = m_pOwner->GetTransform();
		pTransform->SetPosition(m_vTo);
		if (m_pDelegate){
			m_pDelegate->OnActionFinish(this);
			return;
		}
	}
	D3DXVECTOR3 vCurrPosition = m_pOwner->GetTransform()->GetPosition();
	D3DXVECTOR3 vDeltaPosition = vCurrPosition - m_vPrevPosition;
	//
	if (fabs(vDeltaPosition.x) > 0.0001f)
	{
		float fAngle;
		fAngle = -atan2(vDeltaPosition.z, vDeltaPosition.x) - D3DXToRadian(-270.0f);
		m_pOwner->GetTransform()->SetYAxisAngle(fAngle);
	}
	else{
		if (vDeltaPosition.z > 0.0f){
			m_pOwner->GetTransform()->SetYAxisAngle(D3DXToRadian(-180.0f));
		}
		else {
			m_pOwner->GetTransform()->SetYAxisAngle(D3DXToRadian(0.0f));
		}
	}
}