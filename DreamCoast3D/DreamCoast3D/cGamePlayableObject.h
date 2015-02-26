#pragma once

#include "cGameSMeshBodyObject.h"
#include "iPlayerState.h"

// TODO
// 차후 playable 캐릭터 인터페이스를 이용해서 UI상호작용 실시
__interface iPlayableCharacter{
	void GetInventoryPointer();
};

// TODO 
// 차후 인벤토리 등을 추가해서
// 아래 PlayerbleObject, 혹은 플레이어클래스를 새로 상속받아 만들어서
// 캐릭터 스탯 조절 실시
// 인벤토리는 중복 적용되지 않게 map으로 간단하게 구현 예정(D2D작업당시 비슷하게)
typedef struct ST_PLAYER_INFO{
	int nStrength;				// 힘
	int nArmor;					// 방어력
	int nAgility;				// 민첩성
	int nAttackSpeed;			// 공속
	int nMoveSpeed;				// 이동속도

	// 실질적 전투수치
	float fStatAttackPower;		//공격력
	float fStatDefensePower;	//방어력
	float fKnockingDownRate;	//다운유발확률
	float fKnockDownResist;		//다운저항치
	float fCriticalRate;		//크리티컬확률
	float fCriticalResist;		//크리티컬저항
	float fCriticalMultiplier;	//크리티컬배율

	// 기본 체력
	float fMaxHp;
	float fCurrentHp;
	float fMaxMana;
	float fCurrentMana;
	float fMaxExp;
	float fCurrentExp;
} *LPPLAYERINFO;


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
	SYNTHESIZE(D3DXVECTOR3, m_vecFront, Front);
	SYNTHESIZE(float, m_fMoveSpeed, MoveSpeed);
	SYNTHESIZE(float, m_fPlayerAngleDegree, PlayerAngle);
	SYNTHESIZE(float, m_fStatePassedTime, StatePassedTime);
	
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




	//:민우
	//void cGamePlayableObject::CalcCollisionSphere(ST_BONE_MESH* pBoneMesh);
	//void cGamePlayableObject::RenderCollisionSphere(ST_BONE_MESH* pBoneMesh);
};

