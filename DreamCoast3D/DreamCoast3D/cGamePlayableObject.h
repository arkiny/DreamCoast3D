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
	enum EPLAYABLESTATE
	{
		EPLAYABLESTATE_IDLE,
		EPLAYABLESTATE_MOVE,
		EPLAYABLESTATE_ATTACK,
		EPLAYABLESTATE_ONHIT,
		EPLAYABLESTATE_MAX
	};

protected:
	ST_STAT_INFO* m_pPlayerStatInfo;
	
	SYNTHESIZE(D3DXVECTOR3, m_vecFront, Front);
	//SYNTHESIZE(float, m_fMoveSpeed, MoveSpeed);
	SYNTHESIZE(float, m_fPlayerAngleDegree, PlayerAngle);
	SYNTHESIZE(float, m_fStatePassedTime, StatePassedTime);

	//���� ����
	SYNTHESIZE(float, m_fPlayerInvincibleTime, PlayerInvincibleTime);
	SYNTHESIZE(float, m_fPlayerInvincibleCool, m_fPlayerInvincibleCoolTime); 


	float m_fCurHp = 100.0f;
	float m_fMaxHp = 100.0f;

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
};

