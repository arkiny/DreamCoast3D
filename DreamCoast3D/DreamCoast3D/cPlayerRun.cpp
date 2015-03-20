#include "stdafx.h"
#include "cPlayerRun.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerRun::cPlayerRun()
{
	m_nCurrentStateType = 2; //2번 (장)Run
	//SetIsRestart(true);	//상태 재시작 여부
	m_IsRestart = true;
	m_IsDoing = false;
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_MOVE);

	/*m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1] = E_CANCEL_FORCE;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO1R] = E_CANCEL_FORCE;*/

	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_FORCE;		//이동중 키떼면 즉시 대기
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_CANTCANCEL;	//이동중 이동은 불가능
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_FORCE;

}


cPlayerRun::~cPlayerRun()
{
}
void cPlayerRun::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
	//pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_RUN);
}
void cPlayerRun::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	cPlayerCommon::Execute(pPlayer, fDelta);

	if (!(g_pControlManager->GetInputInfo('W') ||
		g_pControlManager->GetInputInfo('A') ||
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('D')))
	{
		//이동키가 떨어지면 대기상태로 전환
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}

	//애니메이션 코딩할땐 주석달고 시작
		if (g_pControlManager->GetInputInfo('Q')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle -= fDelta* 2.0f;
		pPlayer->SetPlayerAngle(angle);
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);
		pPlayer->SetFront(vDir);
		pPlayer->SetYangle(angle);
	}

	if (g_pControlManager->GetInputInfo('E')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle += fDelta* 2.0f;
		pPlayer->SetPlayerAngle(angle);
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);
		pPlayer->SetFront(vDir);
		pPlayer->SetYangle(angle);
	}

	if (g_pControlManager->GetInputInfo('W') ||
		g_pControlManager->GetInputInfo('A') ||
		g_pControlManager->GetInputInfo('S') ||
		g_pControlManager->GetInputInfo('D')){

		MoveFront(pPlayer, fDelta);
		MoveBack(pPlayer, fDelta);
		MoveRight(pPlayer, fDelta);
		MoveLeft(pPlayer, fDelta);
	}
}
void cPlayerRun::Exit(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Exit(pPlayer);
	// currently do nothing
}
void cPlayerRun::MoveFront(cGamePlayableObject* pPlayer, float fDelta)
{
	D3DXVECTOR3 newPos;
	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	D3DXVECTOR3 addVec = (pPlayer->GetFront());

	if (g_pControlManager->GetInputInfo('W')){
		D3DXVec3Normalize(&addVec, &addVec);

		D3DXVECTOR3 vForce(0.f, 0.f, 0.f);

		vForce = pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer) + addVec;
		D3DXVec3Normalize(&vForce, &vForce);

		if (g_pControlManager->GetInputInfo('A') || g_pControlManager->GetInputInfo('D'))
		{
			pPlayer->SetMoveSpeed(0.f);
		}
		else
		{
			pPlayer->SetMoveSpeed(10.f);
		}

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

}

void cPlayerRun::MoveBack(cGamePlayableObject* pPlayer, float fDelta)
{

	D3DXVECTOR3 newPos;
	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	D3DXVECTOR3 addVec = (pPlayer->GetFront());

	if (g_pControlManager->GetInputInfo('S')){

		pPlayer->SetMoveSpeed(5.f);

		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 addVec = -(pPlayer->GetFront());

		D3DXVec3Normalize(&addVec, &addVec);
		D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
		vForce = pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer) + addVec;
		D3DXVec3Normalize(&vForce, &vForce);
		newPos = curPos + vForce*fDelta*pPlayer->GetMoveSpeed() / 2.f;

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
	else
	{
		pPlayer->SetMoveSpeed(10.f);
	}
}

void cPlayerRun::MoveRight(cGamePlayableObject* pPlayer, float fDelta)
{
	D3DXVECTOR3 newPos;
	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	D3DXVECTOR3 addVec = (pPlayer->GetFront());

	if (g_pControlManager->GetInputInfo('D')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle += D3DX_PI / 2;
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);

		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 newPos(0.f, 0.f, 0.f);

		pPlayer->SetYangle(angle);

		if (g_pControlManager->GetInputInfo('S'))
		{
			pPlayer->SetYangle(angle - D3DX_PI / 4 - D3DX_PI / 2);
		}
		else
		{
			pPlayer->SetYangle(angle - D3DX_PI / 4);
		}

		D3DXVec3Normalize(&addVec, &addVec);
		D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
		vForce = (pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer))*1.5f + addVec;

		vDir = vDir + vForce;
		D3DXVec3Normalize(&vDir, &vDir);

		newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed();

		pPlayer->SetPosition(newPos);
	}

}

void cPlayerRun::MoveLeft(cGamePlayableObject* pPlayer, float fDelta)
{
	D3DXVECTOR3 newPos;
	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	D3DXVECTOR3 addVec = (pPlayer->GetFront());

	if (g_pControlManager->GetInputInfo('A')){
		D3DXMATRIXA16 matR;
		float angle = pPlayer->GetPlayerAngle();
		angle -= D3DX_PI / 2;
		D3DXMatrixRotationY(&matR, angle);
		D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&vDir, &vDir, &matR);

		D3DXVECTOR3 curPos = pPlayer->GetPosition();
		D3DXVECTOR3 newPos(0.f, 0.f, 0.f);

		if (g_pControlManager->GetInputInfo('S'))
		{
			pPlayer->SetYangle(angle + D3DX_PI / 4 + D3DX_PI / 2);
		}
		else
		{
			pPlayer->SetYangle(angle + D3DX_PI / 4);
		}
		D3DXVec3Normalize(&addVec, &addVec);
		D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
		vForce = (pPlayer->GetGameObjDeligate()->isCollidedStaticObject(pPlayer))*1.5f + addVec;

		vDir = vDir + vForce;
		D3DXVec3Normalize(&vDir, &vDir);

		newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed();

		pPlayer->SetPosition(newPos);
	}

}