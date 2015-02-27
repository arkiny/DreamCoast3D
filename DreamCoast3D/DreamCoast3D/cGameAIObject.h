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

	std::vector<iAIState*>	m_vecPatterns;
	iAIState*				m_pCurrentState;
	iAIState*				m_pPrevState;
	cGameObject*			m_pTargetGameObject;

	std::map<cGameObject*, int> m_mapAggromap;

	//SYNTHESIZE(cGameObject*, m_pTargetGameObject, TargetObject);

public:
	cGameAIObject();
	virtual ~cGameAIObject();

	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);

	virtual void ChangeState(EAIOBJECTSTATE eState);
	virtual void ChangeState(int nState) override;

	virtual void ChangeToPrevState();

	virtual void OnActionFinish(cAction* pSender);

	virtual void SetTargetObject(cGameObject* pTarget){ m_pTargetGameObject = pTarget; }
	
	virtual void OnHitTarget(cGameObject* pTarget) override;
	virtual cGameObject* GetTargetObject() { return m_pTargetGameObject; }
	virtual iAIState*	GetCurrentState() { return m_pCurrentState; }
	virtual iAIState*	GetPrevState(){ return m_pPrevState; }

	virtual void AddGameObjToAggroMap(cGameObject* pGameObj);
	virtual void CheckAggroMapAndSetTarget();
};

