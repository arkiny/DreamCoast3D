#include "stdafx.h"
#include "cPlayerOnHit.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerOnHit::cPlayerOnHit()
{
}


cPlayerOnHit::~cPlayerOnHit()
{
}

void cPlayerOnHit::Start(cGamePlayableObject* pPlayer){
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_ONHIT);
}

void cPlayerOnHit::Execute(cGamePlayableObject* pPlayer, float fDelta){
	pPlayer->SetStatePassedTime(fDelta + pPlayer->GetStatePassedTime());
	if (pPlayer->GetStatePassedTime()){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
		return;
	}
}

void cPlayerOnHit::Exit(cGamePlayableObject* pPlayer){

}
