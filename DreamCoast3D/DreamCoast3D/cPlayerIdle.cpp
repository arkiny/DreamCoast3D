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
	//static int a = 0;
	//++a;
}

void cPlayerIdle::Execute(cGamePlayableObject* pPlayer, float fDelta){

	pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer);

	if (g_pControlManager->GetInputInfo('W') || 
		g_pControlManager->GetInputInfo('A') || 
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('D')){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_MOVE);
		return;
	}

	if (g_pControlManager->GetInputInfo(VK_LBUTTON)){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_ATTACK);
		return;
	}

	// do nothing
}

void cPlayerIdle::Exit(cGamePlayableObject* pPlayer){
	// currently do nothing
}