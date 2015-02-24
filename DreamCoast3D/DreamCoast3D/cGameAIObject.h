#pragma once
#include "cGameActionSkinnedMeshObj.h"
#include "iAIState.h"

class cGameAIObject : public cGameActionSkinnedMeshObj
{
public:
	enum EAIOBJECTSTATE{
		eAISTATE_IDLE,
		eAISTATE_MOVE,
		eAISTATE_ATTACK,
		eAISTATE_ONHIT,
		eAISTATE_THINK,
		eAISTATE_MAX
	};

protected:
	std::vector<iAIState*>	m_vecPatterns;
	iAIState*				m_pCurrentState;

public:
	cGameAIObject();
	virtual ~cGameAIObject();

	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);

	virtual void ChangeState(EAIOBJECTSTATE eState);
};

