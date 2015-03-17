#pragma once
#include  "cAnimationSet.h"

struct ST_BONE;
enum EANIMBLENDTYPE
{
	EANIMBLENDTYPE_NORMAL,				//���� ����
	EANIMBLENDTYPE_CONTINUE_WEIGHT,		//�з��� Ʈ���� ����ġ�� �����ؼ� ��� ����
	EANIMBLENDTYPE_FREEZE_POSITION		//�̱���
};
//�ִϸ��̼� ���� ��ȯ �ð� (���ڴ� �ӽ�)
#define ANIM_BLEND_TIME 3.f
class cSkinnedMesh : public cObject, public iAnimationSetDelegate
{
	friend class cSkinnedMeshManager;

protected:
	// ����� ����
	SYNTHESIZE(int, m_nMeshRefNumber, MeshRefNumber);

	//�ϳ��� ����
	SYNTHESIZE(ST_BONE*, m_pRootFrame, RootFrame);
	SYNTHESIZE(DWORD, m_dwWorkingPaletteSize, WorkingPaletteSize);
	SYNTHESIZE(D3DXMATRIX*, m_pmWorkingPalette, WorkingPalette);
	//SYNTHESIZE(LPD3DXEFFECT, m_pEffect, Effect);
	
	// ��ü���� ����
	SYNTHESIZE(LPD3DXANIMATIONCONTROLLER, m_pAnimController, AnimController);
	SYNTHESIZE(float, m_fAnimationBlendTime, AnimationBlendTime);

	SYNTHESIZE(ST_BOUNDING_SPHERE, m_stBoundingSphere, BoundingSphere);

	//SYNTHESIZE(ST_BOUNDING_SPHERE, m_stAttacSphere, AttackSphere);
	//SYNTHESIZE(ST_BOUNDING_SPHERE, m_stUpdateAttacSphere, UpdateAttackSphere);

	ST_BOUNDING_SPHERE			m_stUpdateBoundingSphere;

	float						m_fPassedBlendTime;
	bool						m_isAnimationBlending;
	std::vector<cAnimationSet*>	m_vecAnimationSet;
	int							m_nCurrentAnimation;

	EANIMBLENDTYPE				m_eAnimBlendType;		//�ִϸ��̼� ���� ���. �ִϸ��̼� �ٲ� �� ���Ե� �μ��� �����ȴ�. : �ο�
	float						m_fAnimContinueWeight;	//�ִϸ��̼� ������ ���� Ʈ������ �����ؿ� ����ġ. : �ο�
	double						m_dAnimFreezePosition;	//�ִϸ��̼� ������ ���� Ʈ������ �������� �ִϸ��̼� ���� ��ġ : �ο�
	
	LPD3DXMESH					m_pDebugSphereBody;
	LPD3DXMESH					m_pDebugDetailSphereBody;
	LPD3DXMESH					m_pATMesh;
	
	std::map<std::string, ST_BOUNDING_SPHERE> m_mapDebugOriginSphereBody;
	std::map<std::string, ST_BOUNDING_SPHERE> m_mapDebugUpdateSphereBody;
	
	std::map<std::string, ST_BOUNDING_SPHERE> m_mapAttackSphere;

	///
	std::string					m_sMainCollisionSphere;
	float						m_fMianColisionSphereRadius;

	std::vector<std::string>	m_vecCollisionSpheres;
	std::vector<float>			m_vecCollisionSpheresRadius;

	std::vector<std::string>	m_vecAttackSpheres;
	std::vector<float>			m_vecAttackSpheresRadius;
	///
	
	SYNTHESIZE_PASS_BY_REF(std::string, m_sSkinnedFolder, SkinnedFolderPath);
	SYNTHESIZE_PASS_BY_REF(std::string, m_sSkinnedFile, SkinnedFilePath);

public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	cSkinnedMesh(std::string sFolder, std::string sFile);
	virtual ~cSkinnedMesh(void);

	virtual void UpdateAndRenderShadow(D3DXMATRIXA16* pParentWorldTM = NULL);
	virtual void Render(D3DXMATRIXA16* pParentWorldTM = NULL);

	// todo
	//virtual void SetAnimationIndex(int nIndex);
	virtual void SetAnimationIndex(DWORD dwIndex);
	virtual void SetAnimationLoop(DWORD dwIndex, bool isLoop);
	//virtual void SetRandomTrackPosition(); // �׽�Ʈ��
	
	virtual double GetCurrentAnimationPeriodTime();

	virtual ST_BOUNDING_SPHERE&	GetCollisionSphere(){
		return m_stBoundingSphere;
	}
	
	virtual ST_BOUNDING_SPHERE GetUpdateBoundingSphere(){
		return m_stUpdateBoundingSphere;
	}

	std::map<std::string, ST_BOUNDING_SPHERE>& GetUpdatedDetailedSphere(){
		return m_mapDebugUpdateSphereBody; 
	};

	std::map<std::string, ST_BOUNDING_SPHERE>* GetAttackSpheres() { return &m_mapAttackSphere; }

	// Interface
	virtual int GetHeadRefNum(){ return -1; }
	virtual int GetHairRefNum(){ return -1; }

protected:
	cSkinnedMesh();

	virtual void Load(char* szFolder, char* szFilename);
	virtual void Load(std::string sFolder, std::string sFile);

	virtual LPD3DXEFFECT LoadEffect(char* szFilename);
	virtual void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	
	virtual void Render(ST_BONE* pBone = NULL);
	virtual void RenderShadow(ST_BONE* pBone = NULL);

	virtual void SetupBoneMatrixPtrs(ST_BONE* pBone);
	virtual void Destroy();

	//���� �� �Լ��� ȣ���ص� EX�Լ��� �⺻������ ȣ��ȴ�.
	virtual void SetAnimationIndexBlend(DWORD dwIndex);

	//�ִϸ��̼� ������ �ϴ� ����� �����Ѵ�. �⺻���ڰ� �ִ�. : �ο�
	virtual void SetAnimationIndexBlendEX(DWORD dwIndex, EANIMBLENDTYPE eAnimBlendType = EANIMBLENDTYPE_NORMAL);

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

