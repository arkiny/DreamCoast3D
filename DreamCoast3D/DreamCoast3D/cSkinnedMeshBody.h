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
};

