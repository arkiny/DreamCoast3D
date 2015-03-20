#pragma once

#include "cGameSMeshBodyObject.h"
#include "iPlayerState.h"

// TODO
// ���� playable ĳ���� �������̽��� �̿��ؼ� UI��ȣ�ۿ� �ǽ�
__interface iPlayableCharacter{
	void GetInventoryPointer();
};

// ���� �׸��� ���� ���� �ӽ� �׽��ú��� �÷��̾� ĳ���͸鼭 ���� 
// �����Ͽ� ���� �÷��̾�� ĳ���Ͱ� �� Ŭ����
class cGamePlayableObject : public cGameSMeshBodyObject
{
public:
	//enum EPLAYABLESTATE
	//{
	//	EPLAYABLESTATE_IDLE,
	//	EPLAYABLESTATE_MOVE,
	//	EPLAYABLESTATE_ATTACK,
	//	EPLAYABLESTATE_ONHIT,
	//	EPLAYABLESTATE_DEAD,
	//	EPLAYABLESTATE_MAX
	//};
	public:
	enum EPLAYABLESTATE
	{
		EPLAYABLESTATE_UNARMEDWAIT = 0,		//(����)������ ���ֱ�
		EPLAYABLESTATE_IDLE = 1,			//(����)���
		EPLAYABLESTATE_RUN = 2,				//(����)���⸦ ��� �޸���
		EPLAYABLESTATE_COMBO1 = 3,			//(����)��Ÿ1
		EPLAYABLESTATE_COMBO1R = 4,			//(����)��Ÿ1����
		EPLAYABLESTATE_ONHIT = 5,			//(����)�ǰ�
		EPLAYABLESTATE_DEAD = 6,			//(����)���
		//EPLAYABLESTATE_ATTACK,		//(����)��Ÿ
		//EPLAYABLESTATE_WALK,			//(����)õõ�� �ȱ�
		EPLAYABLESTATE_MAX
	};


protected:
	SYNTHESIZE(EPLAYABLESTATE, m_ePlayableState, PlayableState);//�÷��̾� ���� Enum�� ������ �־����
	ST_STAT_INFO* m_pPlayerStatInfo;

	SYNTHESIZE(D3DXVECTOR3, m_vecFront, Front);
	//SYNTHESIZE(float, m_fMoveSpeed, MoveSpeed);
	SYNTHESIZE(float, m_fPlayerAngleDegree, PlayerAngle);
	SYNTHESIZE(float, m_fStatePassedTime, StatePassedTime);

	//���� ����
	SYNTHESIZE(float, m_fPlayerInvincibleTime, PlayerInvincibleTime);
	SYNTHESIZE(float, m_fPlayerInvincibleCool, m_fPlayerInvincibleCoolTime);

	std::vector<iPlayerState*>	m_vecStates;
	iPlayerState*				m_pCurrentState;

	const float					m_fSkill1Delay = 3.0f;
	float						m_fSkill1CoolTime = 3.0f;

public:
	cGamePlayableObject();
	virtual ~cGamePlayableObject();

	virtual void Setup(
		std::string sFolder, std::string sFile,
		std::string sFolderHead, std::string sFileHead,
		std::string sFolderHair, std::string sFileHair
		);

	virtual void Update(float fDelta);

	virtual void ChangeState(int nState) override;
	virtual void ChangeState(EPLAYABLESTATE eNewState);
	virtual int GetState() override;

	void OnHitTarget(cGameObject* pTarget, float fDamage, D3DXVECTOR3 vHitPosition) override;

	ST_STAT_INFO* GetStatInfo() override { return m_pPlayerStatInfo; }
	//:�ο�
	//void cGamePlayableObject::CalcCollisionSphere(ST_BONE_MESH* pBoneMesh);
	//void cGamePlayableObject::RenderCollisionSphere(ST_BONE_MESH* pBoneMesh);

	virtual void Clone(cGameObject** pTarget) override;
	std::string SaveAsStringInfo() override;

	float GetSkill1CoolTime() { return m_fSkill1CoolTime; }
	void SetSkill1CoolTime(float fCool) { m_fSkill1CoolTime = fCool; }
	float GetSkill1DelayTime() { return m_fSkill1Delay; }

	virtual bool JudgeChange(ESTATEGROUP eNewStateGroup);//���°� ���� �� �ִ��� �Ǵ��� ��ȯ

};

