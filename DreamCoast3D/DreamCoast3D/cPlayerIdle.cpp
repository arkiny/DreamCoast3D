#include "stdafx.h"
#include "cPlayerIdle.h"
#include "cGamePlayableObject.h"

cPlayerIdle::cPlayerIdle()
{
}


cPlayerIdle::~cPlayerIdle()
{
}

void cPlayerIdle::Start(cGamePlayableObject* pPlayer){

}

void cPlayerIdle::Execute(cGamePlayableObject* pPlayer, float fDelta){
	if (g_pControlManager->GetInputInfo('W') || 
		g_pControlManager->GetInputInfo('A') || 
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('D')){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_MOVE);
	}
	// do nothing
}

void cPlayerIdle::Exit(cGamePlayableObject* pPlayer){
	// currently do nothing
}