#pragma once
#include "cSkinnedMesh.h"


class cSkinnedMeshBody : public cSkinnedMesh
{
protected:
	cSkinnedMesh*	m_pHead;
	cSkinnedMesh*	m_pHair;
	LPD3DXMESH		m_pMesh;

	//�浹�� �ٿ�� ���Ǿ� �� ä����. ST_BONE_MESH ���� �״�� �޾ƿðŴ�. : �ο�
	float					fRadius;				//�ִϸ��̼� ���� �������� �浹�� �ٿ�� ���Ǿ� �޽��� ������.
	D3DXVECTOR3				vCenter;				//�浹�� �ٿ�� ���Ǿ��� �߽���

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
	////Setup���� �� ���� �����Ѵ�
	//void GetDebugOriginSphereBody(
	//	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
	//	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);
	////Update���ο��� �����Ѵ�
	//void GetDebugUpdateSphereBody(
	//	IN ST_BONE* pBone,
	//	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
	//	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);
	////Render���ο��� �����Ѵ�
	//void RenderDebugUpdateSphereBody(IN ST_BONE* pBone, OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody);
	
};

