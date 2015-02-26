#include "stdafx.h"
#include "cPlayerAttack.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerAttack::cPlayerAttack()
{
}


cPlayerAttack::~cPlayerAttack()
{
}

void cPlayerAttack::Start(cGamePlayableObject* pPlayer){
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_ATTACK);
}

void cPlayerAttack::Execute(cGamePlayableObject* pPlayer, float fDelta){
	pPlayer->SetStatePassedTime(pPlayer->GetStatePassedTime() + fDelta);
	//if (g_pControlManager->GetInputInfo(VK_LBUTTON)){
		pPlayer->GetGameObjDeligate()->isGameAttackSphereCollided(pPlayer, *pPlayer->GetAttackSphere());
		//return;
		// do nothing
	//}
	if (pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pPlayer->GetStatePassedTime()){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}
}

void cPlayerAttack::Exit(cGamePlayableObject* pPlayer){

}