#include "stdafx.h"
//#include "cPlayerDead.h"
//#include "cGamePlayableObject.h"
//#include "cSkinnedMesh.h"
//
//
//cPlayerDead::cPlayerDead()
//	: m_fDeadAccumTime(0.f)
//{
//}
//
//
//cPlayerDead::~cPlayerDead()
//{
//}
//
//void cPlayerDead::Start(cGamePlayableObject* pPlayer)
//{
//	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_DEAD);
//}
//
//void cPlayerDead::Execute(cGamePlayableObject* pPlayer, float fDelta)
//{
//	m_fDeadAccumTime += fDelta;
//	if (m_fDeadAccumTime >= 2.0f)
//	{
//		m_fDeadAccumTime = 0.f;
//		pPlayer->GetEventDelegate()->DeadScene();
//	}
//}
//
//void cPlayerDead::Exit(cGamePlayableObject* pPlayer)
//{
//
//}
