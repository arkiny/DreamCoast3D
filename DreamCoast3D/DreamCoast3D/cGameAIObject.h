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
		eAISTATE_ATTACK,
		eAISTATE_ONHIT,
		eAISTATE_THINK,
		eAISTATE_MAX
	};

protected:
	std::vector<iAIState*>	m_vecPatterns;
	iAIState*				m_pCurrentState;
	SYNTHESIZE(float, m_fPassedTime, PassedTime);

public:
	cGameAIObject();
	virtual ~cGameAIObject();

	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);

	virtual void ChangeState(EAIOBJECTSTATE eState);
	virtual void OnActionFinish(cAction* pSender);
};

