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

	MouseUpdate(pPlayer);

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

void cPlayerMove::MouseUpdate(cGamePlayableObject* pPlayer)
{
	if (g_pControlManager->GetInputInfo(VK_RBUTTON))
	{
		m_isMouseMove = true;
	}
	else
	{
		m_isMouseMove = false;
	}
	if (g_pControlManager->GetInputInfo(VK_SPACE))
	{
		D3DXMATRIXA16 matRotation;
		D3DXMatrixIdentity(&matRotation);

		POINT ptCurrMouse;
		GetCursorPos(&ptCurrMouse);
		ScreenToClient(g_hWnd, &ptCurrMouse);

		float fDeltaX = (ptCurrMouse.x - m_ptPrevMouse.x) / 100.f;

		m_fAngleX = pPlayer->GetPlayerAngle();
		m_fAngleX += fDeltaX;
		pPlayer->SetPlayerAngle(m_fAngleX);

		D3DXMatrixRotationY(&matRotation, m_fAngleX);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matRotation);
		pPlayer->SetFront(vDir);
		pPlayer->SetYangle(m_fAngleX);


		//RECT rectClient, rectMouseTrap;
		//GetClientRect(g_hWnd, &rectClient);
		//rectMouseTrap.right = (rectClient.right - rectClient.left) / 2 + 10;
		//rectMouseTrap.left = (rectClient.right - rectClient.left) / 2 - 10;
		//rectMouseTrap.top = (rectClient.bottom - rectClient.top) / 2 - 10;
		//rectMouseTrap.bottom = (rectClient.bottom - rectClient.top) / 2 + 10;


		//POINT p;
		//GetCursorPos(&p);
		//ScreenToClient(g_hWnd, &p);
		//POINT lefttop = { rectMouseTrap.left, rectMouseTrap.top };
		//POINT rightbottom = { rectMouseTrap.right, rectMouseTrap.bottom };
		//ClientToScreen(g_hWnd, &lefttop);
		//ClientToScreen(g_hWnd, &rightbottom);
		//if (p.x > rightbottom.x || p.x < lefttop.x){
		//	POINT target = { 640, 360 };
		//	ClientToScreen(g_hWnd, &target);
		//	SetCursorPos(target.x, target.y);
		//}
		//POINT target = { 640, 360 };
		//ClientToScreen(g_hWnd, &target);
		//SetCursorPos(target.x, target.y);

		m_ptPrevMouse = ptCurrMouse;
	}
}