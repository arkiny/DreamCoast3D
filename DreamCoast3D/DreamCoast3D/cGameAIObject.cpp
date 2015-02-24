#include "stdafx.h"
#include "cGameAIObject.h"
#include "cAIFSM.h"

cGameAIObject::cGameAIObject()
	:m_pCurrentState(NULL)
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

	m_pCurrentState = m_vecPatterns[eAISTATE_IDLE];
	m_pCurrentState->Start(this);
}

void cGameAIObject::Update(float fDelta){
	cGameActionSkinnedMeshObj::Update(fDelta);
	m_pCurrentState->Execute(this, fDelta);
}

void cGameAIObject::ChangeState(EAIOBJECTSTATE eState){
	if (m_pCurrentState != m_vecPatterns[eState]){
		m_pCurrentState->Exit(this);
		m_pCurrentState = m_vecPatterns[eState];
	}
	m_pCurrentState->Start(this);
}