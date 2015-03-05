#pragma once

#include "cGameSMeshBodyObject.h"
#include "iPlayerState.h"

// TODO
// 차후 playable 캐릭터 인터페이스를 이용해서 UI상호작용 실시
__interface iPlayableCharacter{
	void GetInventoryPointer();
};

// 만수 그리드 맵을 위한 임시 테스팅베드 플레이어 캐릭터면서 차후 
// 개선하여 실제 플레이어블 캐릭터가 될 클래스
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

	//전투 공방
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
	//:민우
	//void cGamePlayableObject::CalcCollisionSphere(ST_BONE_MESH* pBoneMesh);
	//void cGamePlayableObject::RenderCollisionSphere(ST_BONE_MESH* pBoneMesh);

	virtual void Clone(cGameObject** pTarget) override;
	std::string SaveAsStringInfo() override;
};

