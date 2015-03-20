#include "stdafx.h"
#include "cAnimationSet.h"

cAnimationSet::cAnimationSet(void)
	: m_pAnimationSet(NULL)
	, m_isLoop(false)
	, m_pPrevAnimation(NULL)
	, m_fPassedTime(0.0)
	, m_pDelegate(NULL)
	, m_nIndex(0)
{
}


cAnimationSet::~cAnimationSet(void)
{
}

void cAnimationSet::Start()
{
	m_fPassedTime = 0.0f;
}

void cAnimationSet::Update(float fDelta)
{
	//iPlayerState의 제어가 있는데 굳이 여기서 애니메이션을 제어할 필요가 있나? : 민우
	/*m_fPassedTime += fDelta;
	if (m_fPassedTime >= m_pAnimationSet->GetPeriod())
	{
		if (m_pDelegate)
		{
			m_pDelegate->OnFinishAnimation(this);
		}
	}*/
}
