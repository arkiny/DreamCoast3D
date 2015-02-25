#pragma once
#include  "cAnimationSet.h"

struct ST_BONE;

class cSkinnedMesh : public cObject, public iAnimationSetDelegate
{
	friend class cSkinnedMeshManager;

protected:
	//�ϳ��� ����
	SYNTHESIZE(ST_BONE*, m_pRootFrame, RootFrame);
	SYNTHESIZE(DWORD, m_dwWorkingPaletteSize, WorkingPaletteSize);
	SYNTHESIZE(D3DXMATRIX*, m_pmWorkingPalette, WorkingPalette);
	SYNTHESIZE(LPD3DXEFFECT, m_pEffect, Effect);

	// ��ü���� ����
	SYNTHESIZE(LPD3DXANIMATIONCONTROLLER, m_pAnimController, AnimController);

	float						m_fPassedBlendTime;
	bool						m_isAnimationBlending;
	std::vector<cAnimationSet*>	m_vecAnimationSet;
	int							m_nCurrentAnimation;

	SYNTHESIZE(ST_BOUNDING_SPHERE, m_stBoundingSphere, BoundingSphere);
	SYNTHESIZE(ST_BOUNDING_SPHERE, m_stAttacSphere, AttackSphere);

	SYNTHESIZE(ST_BOUNDING_SPHERE, m_stUpdateBoundingSphere, UpdateBoundingSphere);
	SYNTHESIZE(ST_BOUNDING_SPHERE, m_stUpdateAttacSphere, UpdateAttackSphere);

	SYNTHESIZE(float, m_fAnimationBlendTime, AnimationBlendTime);
	LPD3DXMESH					m_pDebugSphereBody;

public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	cSkinnedMesh(std::string sFolder, std::string sFile);
	virtual ~cSkinnedMesh(void);

	virtual void UpdateAndRender(D3DXMATRIXA16* pParentWorldTM = NULL);
	
	// todo
	//virtual void SetAnimationIndex(int nIndex);
	virtual void SetAnimationIndex(DWORD dwIndex);
	virtual void SetAnimationLoop(DWORD dwIndex, bool isLoop);

	virtual ST_BOUNDING_SPHERE&	GetCollisionSphere(){
		return m_stBoundingSphere;
	}

	virtual void SetRandomTrackPosition(); // �׽�Ʈ��
	virtual void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);

protected:
	cSkinnedMesh();

	virtual void Load(char* szFolder, char* szFilename);
	virtual void Load(std::string sFolder, std::string sFile);

	virtual LPD3DXEFFECT LoadEffect(char* szFilename);
	virtual void Render(ST_BONE* pBone = NULL);
	virtual void SetupBoneMatrixPtrs(ST_BONE* pBone);
	virtual void Destroy();

	virtual void SetAnimationIndexBlend(DWORD dwIndex);

public:
	virtual void OnFinishAnimation(cAnimationSet* pSender) override;
};

