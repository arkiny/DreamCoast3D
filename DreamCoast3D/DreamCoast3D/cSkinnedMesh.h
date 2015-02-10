#pragma once
#include "cAnimationSet.h"

class cSkinnedMesh : public iAnimationSetDelegate, public cObject
{
private:
	D3DXFRAME*					m_pRootFrame;
	LPD3DXANIMATIONCONTROLLER	m_pAnimControl;
	float						m_fPassedBlendTime;
	bool						m_isAnimationBlending;
	std::vector<cAnimationSet*>	m_vecAnimationSet;
	int							m_nCurrentAnimation;

	SYNTHESIZE(float, m_fAnimationBlendTime, AnimationBlendTime);

public:
	cSkinnedMesh(void);
	~cSkinnedMesh(void);

	void Setup(std::string sFolder, std::string sFile);
	void Update(float fDelta);
	
	void Render(D3DXMATRIXA16* pParentWorldTM);
	
	void SetAnimationIndex(DWORD dwIndex);
	void SetAnimationLoop(DWORD dwIndex, bool isLoop);

private:
	void SetAnimationIndexBlend(DWORD dwIndex);
	void UpdateWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent);
	void Render(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM);
	void UpdateSkinnedMesh(D3DXFRAME* pFrame);

public:
	virtual void OnFinishAnimation(cAnimationSet* pSender) override;
};

