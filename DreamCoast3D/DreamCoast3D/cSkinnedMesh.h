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
	SYNTHESIZE(float, m_fAnimationBlendTime, AnimationBlendTime);

	SYNTHESIZE(ST_BOUNDING_SPHERE, m_stBoundingSphere, BoundingSphere);
	SYNTHESIZE(ST_BOUNDING_SPHERE, m_stAttacSphere, AttackSphere);
	SYNTHESIZE(ST_BOUNDING_SPHERE, m_stUpdateAttacSphere, UpdateAttackSphere);
	ST_BOUNDING_SPHERE			m_stUpdateBoundingSphere;

	float						m_fPassedBlendTime;
	bool						m_isAnimationBlending;
	std::vector<cAnimationSet*>	m_vecAnimationSet;
	int							m_nCurrentAnimation;
	
	LPD3DXMESH					m_pDebugSphereBody;
	LPD3DXMESH					m_pDebugDetailSphereBody;
	
	std::map<std::string, ST_BOUNDING_SPHERE> m_mapDebugOriginSphereBody;
	std::map<std::string, ST_BOUNDING_SPHERE> m_mapDebugUpdateSphereBody;

	//SYNTHESIZE(ST_BOUNDING_SPHERE, m_stUpdateBoundingSphere, UpdateBoundingSphere);
	//std::vector<ST_BOUNDING_SPHERE> m_vecDetailBoundingSphere; //FIX: �����ͳ� ���۷���������
public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	cSkinnedMesh(std::string sFolder, std::string sFile);
	virtual ~cSkinnedMesh(void);

	virtual void UpdateAndRender(D3DXMATRIXA16* pParentWorldTM = NULL);
	
	// todo
	//virtual void SetAnimationIndex(int nIndex);
	virtual void SetAnimationIndex(DWORD dwIndex);
	virtual void SetAnimationLoop(DWORD dwIndex, bool isLoop);
	virtual void SetRandomTrackPosition(); // �׽�Ʈ��
	
	virtual float GetCurrentAnimationPeriodTime();

	virtual ST_BOUNDING_SPHERE&	GetCollisionSphere(){
		return m_stBoundingSphere;
	}
	virtual ST_BOUNDING_SPHERE GetUpdateBoundingSphere(){
		return m_stUpdateBoundingSphere;
	}
	std::map<std::string, ST_BOUNDING_SPHERE>& GetUpdatedDetailedSphere(){ return m_mapDebugUpdateSphereBody; };
protected:
	cSkinnedMesh();

	virtual void Load(char* szFolder, char* szFilename);
	virtual void Load(std::string sFolder, std::string sFile);

	virtual LPD3DXEFFECT LoadEffect(char* szFilename);
	virtual void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	virtual void Render(ST_BONE* pBone = NULL);
	virtual void SetupBoneMatrixPtrs(ST_BONE* pBone);
	virtual void Destroy();

	virtual void SetAnimationIndexBlend(DWORD dwIndex);


	//Setup���� �� ���� �����Ѵ�
	void GetDebugOriginSphereBody(
		OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
		OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);
	//Update���ο��� �����Ѵ�
	void GetDebugUpdateSphereBody(
		IN ST_BONE* pBone,
		OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
		OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);
	//Render���ο��� �����Ѵ�
	void RenderDebugUpdateSphereBody(IN ST_BONE* pBone, OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);

	

public:
	virtual void OnFinishAnimation(cAnimationSet* pSender) override;
};

