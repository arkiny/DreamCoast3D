#include "stdafx.h"
#include "cPlayerMove.h"
#include "cGamePlayableObject.h"

cPlayerMove::cPlayerMove()
{
}


cPlayerMove::~cPlayerMove()
{
}

void cPlayerMove::Start(cGamePlayableObject* pPlayer){

}

void cPlayerMove::Execute(cGamePlayableObject* pPlayer, float fDelta){
	if (g_pControlManager->GetInputInfo('W')){
		pPlayer->SetPosition(pPlayer->GetPosition() + (pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed()));
	}
	if (g_pControlManager->GetInputInfo('S')){
		pPlayer->SetPosition(pPlayer->GetPosition() + (pPlayer->GetFront()*-fDelta*pPlayer->GetMoveSpeed()));
	}
	if (g_pControlManager->GetInputInfo('A')){
		
	}
	if (g_pControlManager->GetInputInfo('D')){

	}
}

void cPlayerMove::Exit(cGamePlayableObject* pPlayer){

}