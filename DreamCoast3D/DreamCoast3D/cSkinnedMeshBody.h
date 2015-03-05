#pragma once
#include "cSkinnedMesh.h"


class cSkinnedMeshBody : public cSkinnedMesh
{
protected:
	cSkinnedMesh*	m_pHead;
	cSkinnedMesh*	m_pHair;
	LPD3DXMESH		m_pMesh;

	//충돌용 바운딩 스피어 값 채우기용. ST_BONE_MESH 에서 그대로 받아올거다. : 민우
	float					fRadius;				//애니메이션 현재 프레임의 충돌용 바운딩 스피어 메쉬의 반지름.
	D3DXVECTOR3				vCenter;				//충돌용 바운딩 스피어의 중심점

public:
	cSkinnedMeshBody();
	cSkinnedMeshBody(std::string sFolder, std::string sFile,
		std::string sFolderHead, std::string sFileHead,
		std::string sFolderHair, std::string sFileHair);
	
	virtual ~cSkinnedMeshBody();

	//virtual void Setup(
	//	std::string sFolder, std::string sFile,
	//	std::string sFolderHead, std::string sFileHead,
	//	std::string sFolderHair, std::string sFileHair
	//	);

	virtual void SetAnimationIndex(DWORD dwIndex) override;
	virtual void SetAnimationLoop(DWORD dwIndex, bool isLoop) override;	

	virtual void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent) override;
	virtual void Render(ST_BONE* pBone = NULL) override;

	//void RenderBoundingSphere(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM);
	//void RenderDetailBoundingSphere(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM);

	//void GetCollisionBoundingSphere(OUT D3DXVECTOR3& vCenter, OUT float& fRadius);
	
	//void GetDetailCollisionBoundingSpheres(OUT std::vector<ST_BOUNDING_SPHERE>& vecSphere);
	//void RecursivePushBoundingSphereAllBones(D3DXFRAME* pFrame, std::vector<ST_BOUNDING_SPHERE>& vecSphere);
	virtual int GetHeadRefNum();
	virtual int GetHairRefNum();

private:
	////Setup에서 한 번만 동작한다
	//void GetDebugOriginSphereBody(
	//	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
	//	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);
	////Update내부에서 갱신한다
	//void GetDebugUpdateSphereBody(
	//	IN ST_BONE* pBone,
	//	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
	//	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);
	////Render내부에서 동작한다
	//void RenderDebugUpdateSphereBody(IN ST_BONE* pBone, OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);
	
};

