#include "stdafx.h"
#include "cGamePlayableObject.h"
#include "cPlayerIdle.h"
#include "cPlayerMove.h"
#include "cPlayerAttack.h"
#include "cPlayerOnHit.h"

cGamePlayableObject::cGamePlayableObject()
	:m_vecFront(0, 0, -1),
	//m_fMoveSpeed(10.0f),
	m_fPlayerAngleDegree(0.0f)
{
	m_eGameObjectType = eGameObjectType::E_PLAYABLE;
	m_fMoveSpeed = 10.0f;
}

cGamePlayableObject::~cGamePlayableObject()
{
	for (auto p : m_vecStates){
		delete p;
	}
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
}

void cGamePlayableObject::ChangeState(EPLAYABLESTATE eNewState){
	m_pCurrentState->Exit(this);
	m_pCurrentState = m_vecStates[eNewState];
	m_fStatePassedTime = 0.0f;
	m_pCurrentState->Start(this);
}

void cGamePlayableObject::ChangeState(int nState){
	this->ChangeState((EPLAYABLESTATE)nState);
}

int cGamePlayableObject::GetState() {
	return m_pCurrentState->GetCurrentStateType();
}