#pragma once
#include "cGameActionSkinnedMeshObj.h"
#include "iAIState.h"

class cGameAIObject : public cGameActionSkinnedMeshObj, public iActionDelegate
{
public:
	enum EAIOBJECTSTATE{
		eAISTATE_IDLE,
		eAISTATE_MOVE,
		eAISTATE_RANDOMMOVE,
		eAISTATE_MOVETOTARGET,
		eAISTATE_ATTACK,
		eAISTATE_ONHIT,
		eAISTATE_THINK,
		eAISTATE_DEAD,
		eAISTATE_BOSSPAGE,
		eAISTATE_BOSSPAGEFIRST,
		eAISTATE_BOSSPAGESECOND,
		eAISTATE_MAX
	};

	enum E_AI_TYPE{
		E_AI_PASSIVE,
		E_AI_AGGRESSIVE,
		E_AI_BOSS,
		E_AI_MAX
	};

protected:
	SYNTHESIZE(float, m_fPassedTime, PassedTime);
	SYNTHESIZE(E_AI_TYPE, m_eAITYPE, AItype);

	// TODO ���� ���ݻ����Ÿ� ���ȿ� ���Ե� ����
	SYNTHESIZE(float, m_fAttackRange, AttackRange);

	// Hack �������� �̵��� ���������� ���� Action�� ���
	SYNTHESIZE(D3DXVECTOR3, m_vFront, Front);
	SYNTHESIZE(float, m_fAIAngle, AIAngle);

	// Check ������ ���ݼӵ� �ʴ� Ƚ��
	SYNTHESIZE(float, m_fAttackSpeed, AttackSpeed);
	SYNTHESIZE(float, m_fAttackCoolTime, AttackCoolTime);

	std::vector<iAIState*>	m_vecPatterns;
	iAIState*				m_pCurrentState;
	iAIState*				m_pPrevState;
	cGameObject*			m_pTargetGameObject;
	float					m_fAiInvincibleCool = 0.0f;
	
	std::map<cGameObject*, float> m_mapAggromap;

	//SYNTHESIZE(cGameObject*, m_pTargetGameObject, TargetObject);

    // MS
    SYNTHESIZE(float, m_fHP, HP);

public:
	cGameAIObject();
	virtual ~cGameAIObject();

	virtual void Setup(std::string sFolder, std::string sFile);
	
	virtual void Start() override;
	virtual void Update(float fDelta);

	virtual void ChangeState(EAIOBJECTSTATE eState);
	virtual void ChangeState(int nState) override;

	virtual void ChangeToPrevState();
	virtual void SetTargetObject(cGameObject* pTarget){ m_pTargetGameObject = pTarget; }
	
	virtual void OnHitTarget(cGameObject* pTarget, float fDamage, D3DXVECTOR3 vHitPosition) override;
	virtual cGameObject* GetTargetObject() { return m_pTargetGameObject; }
	virtual iAIState*	GetCurrentState() { return m_pCurrentState; }
	virtual iAIState*	GetPrevState(){ return m_pPrevState; }

	virtual void AddGameObjToAggroMap(cGameObject* pGameObj);
	virtual void CheckAggroMapAndSetTarget();



	// ���� �̻��
	virtual void OnActionFinish(cAction* pSender);

	virtual void Clone(cGameObject** pTarget) override;
	std::string SaveAsStringInfo() override;
};

