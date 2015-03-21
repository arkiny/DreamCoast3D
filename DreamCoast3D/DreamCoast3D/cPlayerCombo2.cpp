#include "stdafx.h"
#include "cPlayerCombo2.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

//RootFrame�� �̵��ϴ� �Ÿ��� ���� ĳ������ ���� ��ǥ�� ������ �޾ƾ� �ǰ� ī�޶� ������ �޾ƾ� �Ѵ�.
cPlayerCombo2::cPlayerCombo2()
{
	//cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO2;
	m_nCurrentStateType = cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_COMBO2;	//5�� (��)Combo2
	SetIsRestart(false);
	SetIsDoing(false);
	SetStateGroup(ESTATEGROUP::E_STATEGROUP_ATTACK);
	
	/*m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_RUN] = E_CANCEL_REAR;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo2] = E_CANCEL_CANTCANCEL;
	m_mapCancelInfo[cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_Combo2R] = E_CANCEL_CANTCANCEL;*/

	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_IDLE] = E_CANCEL_CANTCANCEL;	//������ ���� ��� ��ȯ �Ұ�
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_MOVE] = E_CANCEL_REAR;		//������ �̵��� �ĵ�ĵ��
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_ATTACK] = E_CANCEL_REAR;		//���ݿ� ������ �ĵ�ĵ��
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_SKILL] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_DODGE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_GUARD] = E_CANCEL_REAR;		//������ ���� �ĵ�ĵ��
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_PASSIVE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_INVINCIBLE] = E_CANCEL_FORCE;
	m_mapCancelInfo[ESTATEGROUP::E_STATEGROUP_END] = E_CANCEL_FORCE;

}

cPlayerCombo2::~cPlayerCombo2()
{
}
void cPlayerCombo2::Start(cGamePlayableObject* pPlayer)
{
	cPlayerCommon::Start(pPlayer);
	pPlayer->SetComboCount(pPlayer->GetComboCount()+1);	//��Ÿ ��ȣ ����
	g_pSoundManager->executeAttack();

	//pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_Combo2);
}
void cPlayerCombo2::Execute(cGamePlayableObject* pPlayer, float fDelta)
{
	//pPlayer->SetStatePassedTime(pPlayer->GetStatePassedTime() + fDelta);

	//std::map<std::string, ST_BOUNDING_SPHERE>* pSp = pPlayer->GetAttackSpheres();
	// ��ƼŬ�� �̿��� Ʈ���� ����Ʈ�� ���
	/*for (auto p : *pSp){
	pPlayer->GetEffectDelegate()->AddEffect(6, p.second.m_vCenter);
	}*/

	for (auto p : *pPlayer->GetAttackSpheres()){
		pPlayer->GetGameObjDeligate()->isGameAttackSphereCollided(pPlayer, p.second);
	}

	//if (g_pControlManager->GetInputInfo('A')){
	//	D3DXMATRIXA16 matR;
	//	float angle = pPlayer->GetPlayerAngle();
	//	angle -= D3DX_PI / 2;
	//	D3DXMatrixRotationY(&matR, angle);
	//	D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
	//	D3DXVec3TransformNormal(&vDir, &vDir, &matR);

	//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	//	D3DXVECTOR3 newPos(0.f, 0.f, 0.f);


	//	newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed() / 2.f;


	//	pPlayer->SetPosition(newPos);
	//}

	//if (g_pControlManager->GetInputInfo('D')){
	//	D3DXMATRIXA16 matR;
	//	float angle = pPlayer->GetPlayerAngle();
	//	angle += D3DX_PI / 2;
	//	D3DXMatrixRotationY(&matR, angle);
	//	D3DXVECTOR3 vDir = D3DXVECTOR3(0, 0, -1.f);
	//	D3DXVec3TransformNormal(&vDir, &vDir, &matR);

	//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	//	D3DXVECTOR3 newPos(0.f, 0.f, 0.f);


	//	newPos = curPos + vDir*fDelta*pPlayer->GetMoveSpeed() / 2.f;


	//	pPlayer->SetPosition(newPos);
	//}

	//D3DXVECTOR3 newPos;
	//if (g_pControlManager->GetInputInfo('W')){
	//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	//	D3DXVECTOR3 addVec = (pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed());
	//	newPos = curPos + addVec;
	//	//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
	//	//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
	//	pPlayer->SetPosition(newPos);
	//	//}
	//}

	//if (g_pControlManager->GetInputInfo('S')){
	//	D3DXVECTOR3 curPos = pPlayer->GetPosition();
	//	D3DXVECTOR3 addVec = (pPlayer->GetFront()*fDelta*pPlayer->GetMoveSpeed() / 2.f);
	//	newPos = curPos - addVec;
	//	//if (/*pPlayer->GetGridTileSystem()->GetObjectOnGrid((int)curPos.x, (int)curPos.z).size() == 1
	//	//	&&*/ pPlayer->GetGameObjDeligate()->isGameObjectCollided(pPlayer) == false){
	//	pPlayer->SetPosition(newPos);
	//	//}
	//}

	/*if (pPlayer->GetSkinnedMesh()->GetCurrentAnimationPeriodTime() < pPlayer->GetStatePassedTime()){
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_IDLE);
	}*/
	cPlayerCommon::Execute(pPlayer, fDelta);
}
void cPlayerCombo2::Exit(cGamePlayableObject* pPlayer)
{
	//cPlayerCommon::Exit(pPlayer);

	//����� ��ġ�� �̵��ϴ� ��� ��ǥ ����ȭ
	SyncAfterAnimPos(pPlayer);

	SetIsDoing(false);
	//����۵Ǵ� �����̸� ������ϰ�(�ݺ�)
	if (m_IsRestart)
	{
		Start(pPlayer);
	}
	else
	{
		pPlayer->ChangeState(pPlayer->EPLAYABLESTATE_COMBO2R);
	}
}