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
	//iPlayerState�� ��� �ִµ� ���� ���⼭ �ִϸ��̼��� ������ �ʿ䰡 �ֳ�? : �ο�
	/*m_fPassedTime += fDelta;
	if (m_fPassedTime >= m_pAnimationSet->GetPeriod())
	{
		if (m_pDelegate)
		{
			m_pDelegate->OnFinishAnimation(this);
		}
	}*/
}
