#pragma once
#include "cGameActionSkinnedMeshObj.h"

class cGameAIObject : public cGameActionSkinnedMeshObj
{
protected:
	std::vector<cAction*> m_pPatterns;

public:
	cGameAIObject();
	virtual ~cGameAIObject();
	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);
	virtual void Render();		
};

