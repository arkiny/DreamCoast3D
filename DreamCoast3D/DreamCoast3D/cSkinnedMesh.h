#pragma once
#include  "cAnimationSet.h"

struct ST_BONE;
enum EANIMBLENDTYPE
{
	EANIMBLENDTYPE_NORMAL,				//보통 블렌딩
	EANIMBLENDTYPE_CONTINUE_WEIGHT,		//밀려난 트랙의 가중치를 보존해서 계속 블렌딩
	EANIMBLENDTYPE_FREEZE_POSITION		//미구현
};
//애니메이션 블렌드 전환 시간 (숫자는 임시)
#define ANIM_BLEND_TIME 3.f
class cSkinnedMesh : public cObject, public iAnimationSetDelegate
{
	friend class cSkinnedMeshManager;

protected:
	// 저장용 정보
	SYNTHESIZE(int, m_nMeshRefNumber, MeshRefNumber);

	//하나만 생성
	SYNTHESIZE(ST_BONE*, m_pRootFrame, RootFrame);
	SYNTHESIZE(DWORD, m_dwWorkingPaletteSize, WorkingPaletteSize);
	SYNTHESIZE(D3DXMATRIX*, m_pmWorkingPalette, WorkingPalette);
	//SYNTHESIZE(LPD3DXEFFECT, m_pEffect, Effect);
	
	// 객체마다 생성
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

	EANIMBLENDTYPE				m_eAnimBlendType;		//애니메이션 블렌딩 방식. 애니메이션 바꿀 때 대입된 인수로 결정된다. : 민우
	float						m_fAnimContinueWeight;	//애니메이션 블랜딩때 이전 트랙에서 보존해온 가중치. : 민우
	double						m_dAnimFreezePosition;	//애니메이션 블렌딩때 이전 트랙에서 멈춰있을 애니메이션 진행 위치 : 민우
	
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
	//virtual void SetRandomTrackPosition(); // 테스트용
	
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

	//현재 이 함수를 호출해도 EX함수가 기본형으로 호출된다.
	virtual void SetAnimationIndexBlend(DWORD dwIndex);

	//애니메이션 블렌딩을 하는 방식을 제어한다. 기본인자가 있다. : 민우
	virtual void SetAnimationIndexBlendEX(DWORD dwIndex, EANIMBLENDTYPE eAnimBlendType = EANIMBLENDTYPE_NORMAL);

	//Setup에서 한 번만 동작한다
	void GetDebugOriginSphereBody(
		OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
		OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);

	//Update내부에서 갱신한다
	void GetDebugUpdateSphereBody(
		IN ST_BONE* pBone,
		OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
		OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);
	
	//Render내부에서 동작한다
	void RenderDebugUpdateSphereBody(IN ST_BONE* pBone, OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);

	

public:
	virtual void OnFinishAnimation(cAnimationSet* pSender) override;
};

