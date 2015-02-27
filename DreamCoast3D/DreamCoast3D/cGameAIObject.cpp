#include "stdafx.h"
#include "cGameAIObject.h"
#include "cAIFSM.h"
#include "cActionMove.h"

cGameAIObject::cGameAIObject()
	:m_pCurrentState(NULL),
	m_pPrevState(NULL),
	m_fPassedTime(0),
	m_pTargetGameObject(NULL)
{
}


cGameAIObject::~cGameAIObject()
{
	for (auto p : m_vecPatterns){
		delete p;
	}
}

void cGameAIObject::Setup(std::string sFolder, std::string sFile){
	cGameActionSkinnedMeshObj::Setup(sFolder, sFile);
	m_vecPatterns.resize(EAIOBJECTSTATE::eAISTATE_MAX);
	m_vecPatterns[eAISTATE_IDLE] = new cAIIdle;
	m_vecPatterns[eAISTATE_MOVE] = new cAIMove;
	m_vecPatterns[eAISTATE_ATTACK] = new cAIAttack;
	m_vecPatterns[eAISTATE_ONHIT] = new cAIOnHit;
	m_vecPatterns[eAISTATE_THINK] = new cAIThink;
	m_vecPatterns[eAISTATE_RANDOMMOVE] = new cAIRandomMove;
	m_vecPatterns[eAISTATE_MOVETOTARGET] = new cAIMoveToTarget;
	
	m_pCurrentState = m_vecPatterns[eAISTATE_IDLE];
	m_pCurrentState->Start(this);
}

void cGameAIObject::Update(float fDelta){
	cGameActionSkinnedMeshObj::Update(fDelta);
	m_fPassedTime += fDelta;
	m_pCurrentState->Execute(this, fDelta);
}

void cGameAIObject::ChangeState(EAIOBJECTSTATE eState){
	if (m_pCurrentState != m_vecPatterns[eState]){
		m_pCurrentState->Exit(this);
		m_pPrevState = m_pCurrentState;
		m_pCurrentState = m_vecPatterns[eState];
	}
	m_pCurrentState->Start(this);
}

void cGameAIObject::ChangeState(int nState){
	this->ChangeState((EAIOBJECTSTATE)nState);
}

void cGameAIObject::OnActionFinish(cAction* pSender){
	cActionMove* pAction = new cActionMove;
	D3DXVECTOR3 curPos = GetPosition();
	pAction->SetFrom(curPos);
	pAction->SetDelegate(this);
	pAction->SetTo(curPos);
	// deligate가 콜되지 않게 충분히 큰숫자를 넣어준다.
	pAction->SetActionTime(100.0f);
	SetAction(pAction);
	SAFE_RELEASE(pAction);
	ChangeState(this->eAISTATE_IDLE);
}

void cGameAIObject::ChangeToPrevState(){
	if (m_pPrevState && m_pPrevState != m_pCurrentState){
		m_pCurrentState->Exit(this);
		m_pCurrentState = m_pPrevState;
		m_pPrevState = NULL;
	}
	//m_pCurrentState->Start(this);
}

void cGameAIObject::OnHitTarget(cGameObject* pTarget){
	if (pTarget != m_pTargetGameObject){
		m_pTargetGameObject = pTarget;
		m_pPrevState = NULL;
		this->ChangeState(eAISTATE_ONHIT);
	}
}