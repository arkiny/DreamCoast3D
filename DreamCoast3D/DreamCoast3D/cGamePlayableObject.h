#pragma once

#include "cGameSMeshBodyObject.h"
#include "iPlayerState.h"

// TODO ���� playable ĳ���� �������̽��� �̿��ؼ� UI��ȣ�ۿ� �ǽ�
__interface iPlayableCharacter{
	void GetInventoryPointer();
};

// TODO 
// ���� �κ��丮 ���� �߰��ؼ�
// �Ʒ� PlayerbleObject, Ȥ�� �÷��̾�Ŭ������ ���� ��ӹ޾� ����
// ĳ���� ���� ���� �ǽ�
// �κ��丮�� �ߺ� ������� �ʰ� map���� �����ϰ� ���� ����(D2D�۾���� ����ϰ�)
typedef struct ST_PLAYER_INFO{
	int nStrength;				// ��
	int nArmor;					// ����
	int nAgility;				// ��ø��
	int nAttackSpeed;			// ����
	int nMoveSpeed;				// �̵��ӵ�

	// ������ ������ġ
	float fStatAttackPower;		//���ݷ�
	float fStatDefensePower;	//����
	float fKnockingDownRate;	//�ٿ�����Ȯ��
	float fKnockDownResist;		//�ٿ�����ġ
	float fCriticalRate;		//ũ��Ƽ��Ȯ��
	float fCriticalResist;		//ũ��Ƽ������
	float fCriticalMultiplier;	//ũ��Ƽ�ù���

	// �⺻ ü��
	float fMaxHp;
	float fCurrentHp;
	float fMaxMana;
	float fCurrentMana;
	float fMaxExp;
	float fCurrentExp;
} *LPPLAYERINFO;


// ���� �׸��� ���� ���� �ӽ� �׽��ú��� �÷��̾� ĳ���͸鼭 ���� 
// �����Ͽ� ���� �÷��̾�� ĳ���Ͱ� �� Ŭ����
class cGamePlayableObject : public cGameSMeshBodyObject
{	
public:
	enum EPLAYABLESTATE
	{
		EPLAYABLESTATE_IDLE,
		EPLAYABLESTATE_MOVE,
		EPLAYABLESTATE_MAX
	};
protected:
	SYNTHESIZE(D3DXVECTOR3, m_vecFront, Front);
	SYNTHESIZE(float, m_fMoveSpeed, MoveSpeed);
	SYNTHESIZE(float, m_fPlayerAngleDegree, PlayerAngle);
	
	std::vector<iPlayerState*>	m_vecStates;
	iPlayerState*				m_pCurrentState;

public:
	cGamePlayableObject();
	virtual ~cGamePlayableObject();

	virtual void Setup(
		std::string sFolder, std::string sFile,
		std::string sFolderHead, std::string sFileHead,
		std::string sFolderHair, std::string sFileHair
		);

	virtual void Update(float fDelta);

	virtual void ChangeState(EPLAYABLESTATE eNewState);
};

