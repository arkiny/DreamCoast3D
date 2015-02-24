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
	D3DXVECTOR3 newPos;
	if (g_pControlManager->GetInputInfo('W') ||
		g_pControlManager->GetInputInfo('A') ||
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('D')){
		if (g_pControlManager->GetInputInfo('W')){
			D3DXVECTOR3 curPos = pPlayer->GetPosition();
			D3DXVECTOR3 addVec = (pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed());
			newPos = curPos + addVec;
			if (pPlayer->GetGridTileSystem()->GetAdjObject(newPos.x, newPos.z).size() == 0){
				pPlayer->SetPosition(newPos);
			}
		}

		if (g_pControlManager->GetInputInfo('S')){
			D3DXVECTOR3 curPos = pPlayer->GetPosition();
			D3DXVECTOR3 addVec = (pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed());
			newPos = curPos - addVec;
			if (pPlayer->GetGridTileSystem()->GetAdjObject(newPos.x, newPos.z).size() == 0){
				pPlayer->SetPosition(newPos);
			}
		}

		if (g_pControlManager->GetInputInfo('A')){
			D3DXMATRIXA16 matR;
			float angle = pPlayer->GetPlayerAngle();
			angle -= fDelta* 4.0f;
			pPlayer->SetPlayerAngle(angle);
			D3DXMatrixRotationY(&matR, angle);
			D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, 1.f);
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
			D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, 1.f);
			D3DXVec3TransformNormal(&vDir, &vDir, &matR);
			pPlayer->SetFront(vDir);
			pPlayer->SetYangle(angle);
		}
		return;
	}

	pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
}

void cPlayerMove::Exit(cGamePlayableObject* pPlayer){

}