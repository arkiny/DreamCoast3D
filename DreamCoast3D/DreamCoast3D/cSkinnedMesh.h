#pragma once
#include "cAnimationSet.h"

class cSkinnedMesh : public iAnimationSetDelegate, public cObject
{
protected:
	D3DXFRAME*					m_pRootFrame;
	LPD3DXANIMATIONCONTROLLER	m_pAnimControl;
	float						m_fPassedBlendTime;
	bool						m_isAnimationBlending;
	std::vector<cAnimationSet*>	m_vecAnimationSet;
	int							m_nCurrentAnimation;

	SYNTHESIZE(float, m_fAnimationBlendTime, AnimationBlendTime);

public:
	cSkinnedMesh(void);
	virtual ~cSkinnedMesh(void);

	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);
	virtual void Render(D3DXMATRIXA16* pParentWorldTM);
	virtual void SetAnimationIndex(DWORD dwIndex);
	virtual void SetAnimationLoop(DWORD dwIndex, bool isLoop);

protected:
	virtual void SetAnimationIndexBlend(DWORD dwIndex);
	virtual void UpdateWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent);
	virtual void Render(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM);
	virtual void UpdateSkinnedMesh(D3DXFRAME* pFrame);

public:
	virtual void OnFinishAnimation(cAnimationSet* pSender) override;
};

