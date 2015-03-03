#include "stdafx.h"
#include "cPlayerMove.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerMove::cPlayerMove()
{
}


cPlayerMove::~cPlayerMove()
{
}

void cPlayerMove::Start(cGamePlayableObject* pPlayer){
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_MOVE);
}

void cPlayerMove::Execute(cGamePlayableObject* pPlayer, float fDelta){

	
	D3DXVECTOR3 newPos;
	if (g_pControlManager->GetInputInfo('W') ||
		g_pControlManager->GetInputInfo('A') ||
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('D')){
		if (g_pControlManager->GetInputInfo('W')){
			D3DXVECTOR3 curPos = pPlayer->GetPosition();
			D3DXVECTOR3 addVec = (pPlayer->GetFront());

			D3DXVec3Normalize(&addVec, &addVec);
			D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
			vForce = pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer) + addVec;
			D3DXVec3Normalize(&vForce, &vForce);

			newPos = curPos + vForce*fDelta*pPlayer->GetMoveSpeed();
			//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
			//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
				pPlayer->SetPosition(newPos);
			//}
		}

		if (g_pControlManager->GetInputInfo('S')){
			D3DXVECTOR3 curPos = pPlayer->GetPosition();
			D3DXVECTOR3 addVec = -(pPlayer->GetFront());

			D3DXVec3Normalize(&addVec, &addVec);
			D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
			vForce = pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer) + addVec;
			D3DXVec3Normalize(&vForce, &vForce);

			newPos = curPos + vForce*fDelta*pPlayer->GetMoveSpeed();
			//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
			//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
				pPlayer->SetPosition(newPos);
			//}
		}

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
		if (g_pControlManager->GetInputInfo(VK_LBUTTON)){
			pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_ATTACK);
		}

		return;
	}

	pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
}

void cPlayerMove::Exit(cGamePlayableObject* pPlayer){

}