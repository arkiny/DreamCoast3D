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
	

	//��Ų��޽� Body���� �Ѿ �浹�� �ٿ�� ���Ǿ� �� : �ο�
	float					m_fRadius;				//�ִϸ��̼� ���� �������� �浹�� �ٿ�� ���Ǿ� �޽��� ������.
	D3DXVECTOR3				m_vCenter;				//�浹�� �ٿ�� ���Ǿ��� �߽���
	
	SYNTHESIZE(ST_BOUNDING_SPHERE, m_stBoundingSphere, BoundingSphere);
	SYNTHESIZE(float, m_fAnimationBlendTime, AnimationBlendTime);

	LPD3DXMESH					m_pDebugSphereBody;

public:
	cSkinnedMesh(void);
	virtual ~cSkinnedMesh(void);

	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);
	virtual void Render(D3DXMATRIXA16* pParentWorldTM);
	virtual void SetAnimationIndex(DWORD dwIndex);
	virtual void SetAnimationLoop(DWORD dwIndex, bool isLoop);
	
	virtual void CalcCollisionSphere(ST_BONE_MESH* pBoneMesh);
	virtual void RenderCollisionSphere(ST_BONE_MESH* pBoneMesh);

protected:
	virtual void SetAnimationIndexBlend(DWORD dwIndex);
	virtual void UpdateWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent);
	virtual void Render(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM);
	virtual void UpdateSkinnedMesh(D3DXFRAME* pFrame);

public:
	virtual void OnFinishAnimation(cAnimationSet* pSender) override;
};

