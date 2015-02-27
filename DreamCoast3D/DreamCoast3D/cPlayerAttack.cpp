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

	if (g_pControlManager->GetInputInfo('A')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle -= fDelta* 4.0f;
		pPlayer->SetPlayerAngle(angle);
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);
		pPlayer->SetFront(vDir);
		pPlayer->SetYangle(angle);
	}

	if (g_pControlManager->GetInputInfo('D')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle += fDelta* 4.0f;
		pPlayer->SetPlayerAngle(angle);
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);
		pPlayer->SetFront(vDir);
		pPlayer->SetYangle(angle);
	}
	if (pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pPlayer->GetStatePassedTime()){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}
}

void cPlayerAttack::Exit(cGamePlayableObject* pPlayer){

}