#include "stdafx.h"
#include "cPlayerDead.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"


cPlayerDead::cPlayerDead()
	: m_fDeadAccumTime(0.f)
{
	m_nCurrentStateType = 6;	//3¹ø (Àå)Combo1
	m_IsRestart = false;
	SetIsDoing(false);
}


cPlayerDead::~cPlayerDead()
{
}

void cPlayerDead::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_DEAD);
}

void cPlayerDead::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	cPlayerCommon::Execute(pPlayer, fDelta);
	m_fDeadAccumTime += fDelta;
	if (m_fDeadAccumTime >= 2.0f)
	{
		m_fDeadAccumTime = 0.f;
		pPlayer->GetEventDelegate()->DeadScene();
	}
}

void cPlayerDead::Exit(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Exit(pPlayer);
}
