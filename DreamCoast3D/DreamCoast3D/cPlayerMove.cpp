#include "stdafx.h"
#include "cPlayerMove.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerMove::cPlayerMove()
	: m_isMouseMove(false)
	, m_fAngleX(0.f)
	, m_fAngleY(0.f)
{
	m_ptPrevMouse = { 0.f, 0.f };
}


cPlayerMove::~cPlayerMove()
{
}

void cPlayerMove::Start(cGamePlayableObject* pPlayer){
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_MOVE);
}

void cPlayerMove::Execute(cGamePlayableObject* pPlayer, float fDelta){

	D3DXVECTOR3 newPos;
	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	D3DXVECTOR3 addVec = (pPlayer->GetFront());
	if (g_pControlManager->GetInputInfo('W') ||
		g_pControlManager->GetInputInfo('A') ||
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('D')){
		if (g_pControlManager->GetInputInfo('W')){
			D3DXVec3Normalize(&addVec, &addVec);

			D3DXVECTOR3 vForce(0.f, 0.f, 0.f);

			vForce = pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer) + addVec;
			D3DXVec3Normalize(&vForce, &vForce);

			newPos = curPos + vForce*fDelta*pPlayer->GetMoveSpeed();

			pPlayer->GetGameObjDeligate()->SetNextPosition(newPos);
			pPlayer->GetGameObjDeligate()->SetCurrentPosition(curPos);
			
			if (pPlayer->GetGameObjDeligate()->CalGradient(pPlayer))
			{
				newPos = curPos;
			}
			
			D3DXVECTOR3 vGravity(0.f, 0.f, 0.f);
			vGravity = pPlayer->GetGameObjDeligate()->GravityForce() + addVec;

			newPos += vGravity*fDelta;

			pPlayer->SetPosition(newPos);
			pPlayer->GetEventDelegate()->CheckEventFromRange(pPlayer, 1);
		}

		if (g_pControlManager->GetInputInfo('S')){
			D3DXVECTOR3 curPos = pPlayer->GetPosition();
			D3DXVECTOR3 addVec = -(pPlayer->GetFront());

			D3DXVec3Normalize(&addVec, &addVec);
			D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
			vForce = pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer) + addVec;
			D3DXVec3Normalize(&vForce, &vForce);
			newPos = curPos + vForce*fDelta*pPlayer->GetMoveSpeed();

			pPlayer->GetGameObjDeligate()->SetNextPosition(newPos);
			pPlayer->GetGameObjDeligate()->SetCurrentPosition(curPos);

			if (pPlayer->GetGameObjDeligate()->CalGradient(pPlayer))
			{
				newPos = curPos;
			}

			D3DXVECTOR3 vGravity(0.f, 0.f, 0.f);
			vGravity = pPlayer->GetGameObjDeligate()->GravityForce() + addVec;

			newPos += vGravity*fDelta;

			pPlayer->SetPosition(newPos);

		}

		if (g_pControlManager->GetInputInfo('A')){
			D3DXMATRIXA16 matR;
			float angle = pPlayer->GetPlayerAngle();
			angle -= D3DX_PI / 2;
			D3DXMatrixRotationY(&matR, angle);
			D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
			D3DXVec3TransformNormal(&vDir, &vDir, &matR);

			D3DXVECTOR3 curPos = pPlayer->GetPosition();
			D3DXVECTOR3 newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed();
			pPlayer->SetYangle(angle + D3DX_PI / 4);
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
			D3DXVECTOR3 newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed();
			pPlayer->SetPosition(newPos);
			pPlayer->SetYangle(angle - D3DX_PI / 4);
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
