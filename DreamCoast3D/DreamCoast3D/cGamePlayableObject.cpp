#include "stdafx.h"
#include "cGamePlayableObject.h"
#include "cPlayerIdle.h"
#include "cPlayerMove.h"
#include "cPlayerAttack.h"
#include "cPlayerOnHit.h"

cGamePlayableObject::cGamePlayableObject()
	:m_vecFront(0, 0, -1),
	//m_fMoveSpeed(10.0f),
	m_fPlayerAngleDegree(0.0f),
	m_fPlayerInvincibleTime(0.5f),
	m_fPlayerInvincibleCool(1.0f),
	m_pPlayerStatInfo(NULL)
{
	m_eGameObjectType = eGameObjectType::E_PLAYABLE;
	m_fMoveSpeed = 10.0f;
	m_pPlayerStatInfo = new ST_STAT_INFO;
}

cGamePlayableObject::~cGamePlayableObject()
{
	for (auto p : m_vecStates){
		delete p;
	}
	delete m_pPlayerStatInfo;
}

void cGamePlayableObject::Setup(
	std::string sFolder, std::string sFile,
	std::string sFolderHead, std::string sFileHead,
	std::string sFolderHair, std::string sFileHair
	){
	cGameSMeshBodyObject::Setup(sFolder, sFile, sFolderHead, sFileHead, sFolderHair, sFileHair);

	m_vecStates.resize(EPLAYABLESTATE::EPLAYABLESTATE_MAX);
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = new cPlayerIdle;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_MOVE] = new cPlayerMove;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_ATTACK] = new cPlayerAttack;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_ONHIT] = new cPlayerOnHit;

	m_pCurrentState = m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_IDLE];
	m_pCurrentState->Start(this);
}

void cGamePlayableObject::Update(float fDelta){
	cGameActionSkinnedMeshObj::Update(fDelta);
	m_pCurrentState->Execute(this, fDelta);
	
	m_pGameObjDeligate->isGameObjectCollided(this);
	m_fPlayerInvincibleCool += fDelta;

	if (m_fPlayerInvincibleCool > 2.0f) {
		// 혹시 모를 오버플로우 대비
		// TODO: 차후 맥스 경직값을 정해야함
		m_fPlayerInvincibleCool = 2.0f;
	}
}

void cGamePlayableObject::ChangeState(EPLAYABLESTATE eNewState){
	if (m_pCurrentState != m_vecStates[eNewState]){
		m_pCurrentState->Exit(this);
		m_pCurrentState = m_vecStates[eNewState];
		m_fStatePassedTime = 0.0f;
	}	
	m_pCurrentState->Start(this);
}

void cGamePlayableObject::ChangeState(int nState){
	this->ChangeState((EPLAYABLESTATE)nState);
}

int cGamePlayableObject::GetState() {
	return m_pCurrentState->GetCurrentStateType();
}

void cGamePlayableObject::OnHitTarget(cGameObject* pTarget){
	if (m_pCurrentState != m_vecStates[this->EPLAYABLESTATE_ONHIT]){
		if (m_fPlayerInvincibleCool > m_fPlayerInvincibleTime){
			this->ChangeState(this->EPLAYABLESTATE_ONHIT);
			m_fPlayerInvincibleCool = 0.0f;
		}
	}
}