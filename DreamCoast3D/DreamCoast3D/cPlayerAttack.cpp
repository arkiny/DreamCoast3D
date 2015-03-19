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
	g_pSoundManager->executeAttack();
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_ATTACK);
}

void cPlayerAttack::Execute(cGamePlayableObject* pPlayer, float fDelta){

	if (g_pControlManager->GetInputInfo('C'))
	{
		if (pPlayer->GetSkill1CoolTime() >= pPlayer->GetSkill1DelayTime()
			&& pPlayer->GetStatInfo()->fCurrentMana > 10.0f){
			pPlayer->SetSkill1CoolTime(0.0f);
			pPlayer->GetStatInfo()->fCurrentMana -= 10.0f;
			pPlayer->GetGameObjDeligate()->RangeSkill(pPlayer);
		}
	}

	pPlayer->SetStatePassedTime(pPlayer->GetStatePassedTime() + fDelta);

	for (auto p : *pPlayer->GetAttackSpheres()){
		pPlayer->GetGameObjDeligate()->isGameAttackSphereCollided(pPlayer, p.second);
	}
	
	if (g_pControlManager->GetInputInfo('A')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle -= D3DX_PI / 2;
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);

		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 newPos(0.f, 0.f, 0.f);


		newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed() / 2.f;


		pPlayer->SetPosition(newPos);
	}

	if (g_pControlManager->GetInputInfo('D')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle += D3DX_PI / 2;
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);

		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 newPos(0.f, 0.f, 0.f);


		newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed() / 2.f;


		pPlayer->SetPosition(newPos);
	}

	D3DXVECTOR3 newPos;
	if (g_pControlManager->GetInputInfo('W')){
		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 addVec = (pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed());
		newPos = curPos + addVec;
		//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
		//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
		pPlayer->SetPosition(newPos);
		//}
	}

	if (g_pControlManager->GetInputInfo('S')){
		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 addVec = (pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed()/2.f);
		newPos = curPos - addVec;
		//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
		//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
		pPlayer->SetPosition(newPos);
		//}
	}

	if (pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pPlayer->GetStatePassedTime()){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}
}

void cPlayerAttack::Exit(cGamePlayableObject* pPlayer){

}