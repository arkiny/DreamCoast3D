#include "stdafx.h"
#include "cGameActionObject.h"
#include "cTransform.h"
#include "cAction.h"

cGameActionObject::cGameActionObject()
	:cGameObject(),
	//m_pAction(NULL),
	m_vPrevPosition(0,0,0),
	m_fMoveSpeed(10.0f)
{
}


cGameActionObject::~cGameActionObject()
{
	//SAFE_RELEASE(m_pAction);
}

void cGameActionObject::Update(float fDelta){
	cGameObject::Update(fDelta);

	//if (m_pAction){
	//	m_pAction->Update(fDelta);
	//}
}

void cGameActionObject::SetAction(cAction* pAction){
	//if (m_pAction != pAction)
	//{
	//	pAction->SetOwner(this);
	//	SAFE_ADD_REF(pAction);
	//	SAFE_RELEASE(m_pAction);
	//	m_pAction = pAction;
	//}
}