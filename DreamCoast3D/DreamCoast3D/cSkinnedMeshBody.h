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
	virtual void Render(ST_BONE* pBone = NULL) override;	void cSkinnedMeshBody::RenderBoundingSphere(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM);
	void cSkinnedMeshBody::RenderDetailBoundingSphere(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM);
	void cSkinnedMeshBody::GetCollisionBoundingSphere(OUT D3DXVECTOR3& vCenter, OUT float& fRadius);
	void cSkinnedMeshBody::GetDetailCollisionBoundingSpheres(OUT std::vector<ST_BOUNDING_SPHERE>& vecSphere);
	void cSkinnedMeshBody::RecursivePushBoundingSphereAllBones(D3DXFRAME* pFrame, std::vector<ST_BOUNDING_SPHERE>& vecSphere);
};

