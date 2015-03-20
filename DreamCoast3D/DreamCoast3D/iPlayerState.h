#pragma once

enum ECANCELTYPE
{
	E_CANCEL_CANTCANCEL,
	E_CANCEL_REAR,
	E_CANCEL_FORCE,
	E_CANCEL_MAX
};
enum ESTATEGROUP
{
	E_STATEGROUP_IDLE,			//������ ����
	E_STATEGROUP_MOVE,			//�̵����� ����
	E_STATEGROUP_ATTACK,		//��Ÿ ����
	E_STATEGROUP_SKILL,			//���ݽ�ų ����
	E_STATEGROUP_DODGE,			//ȸ�ǻ��� ����
	E_STATEGROUP_GUARD,			//������ ����
	E_STATEGROUP_PASSIVE,		//�ǰ�, ���, ���
	E_STATEGROUP_INVINCIBLE,	//��������
	E_STATEGROUP_END,			//����� ���� ���� ����
	E_STATEGROUP_MAX
};

class cGamePlayableObject;

__interface iPlayerState{
	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	int GetCurrentStateType();
	virtual std::map < int, ECANCELTYPE > GetCancelInfo();
	bool GetIsDoing();
	ESTATEGROUP GetCurrentStateGroup();
};
//�Ƚ� ��� : �ο�
//#pragma once
//
//class cGamePlayableObject;
//
//__interface iPlayerState{
//	void Start(cGamePlayableObject* pPlayer);
//	void Execute(cGamePlayableObject* pPlayer, float fDelta);
//	void Exit(cGamePlayableObject* pPlayer);
//	int GetCurrentStateType();
//};