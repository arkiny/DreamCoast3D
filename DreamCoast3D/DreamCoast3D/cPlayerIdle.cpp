#include "stdafx.h"
#include "cPlayerIdle.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerIdle::cPlayerIdle()
{
}


cPlayerIdle::~cPlayerIdle()
{
}

void cPlayerIdle::Start(cGamePlayableObject* pPlayer){
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_IDLE);
}

void cPlayerIdle::Execute(cGamePlayableObject* pPlayer, float fDelta){
	if (g_pControlManager->GetInputInfo('W') || 
		g_pControlManager->GetInputInfo('A') || 
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('D')){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_MOVE);
		return;
	}

	// do nothing
}

void cPlayerIdle::Exit(cGamePlayableObject* pPlayer){
	// currently do nothing
}