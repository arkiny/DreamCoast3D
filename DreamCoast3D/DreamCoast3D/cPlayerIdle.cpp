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
	if (g_pControlManager->GetInputInfo('w') || 
		g_pControlManager->GetInputInfo('a') || 
		g_pControlManager->GetInputInfo('s') ||
		g_pControlManager->GetInputInfo('d')){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_MOVE);
	}
	// do nothing
}

void cPlayerIdle::Exit(cGamePlayableObject* pPlayer){
	// currently do nothing
}