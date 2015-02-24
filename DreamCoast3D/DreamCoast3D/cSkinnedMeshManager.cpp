#include "stdafx.h"
#include "cSkinnedMeshManager.h"
#include "cAllocateHierarchy.h"


cSkinnedMeshManager::cSkinnedMeshManager()
{
}


cSkinnedMeshManager::~cSkinnedMeshManager()
{
}

D3DXFRAME* cSkinnedMeshManager::GetSkinnedMesh(std::string sFolder, std::string sFile){
	if (m_mapAnimationControl.find(sFolder + sFile) == m_mapAnimationControl.end() ||
		m_mapSkinnedMeshes.find(sFolder + sFile) == m_mapSkinnedMeshes.end())
	{
		D3DXFRAME*					pFrame;
		LPD3DXANIMATIONCONTROLLER	pAnimCon;

		cAllocateHierarchy Alloc;
		Alloc.SetFolder(sFolder);

		HRESULT hr = D3DXLoadMeshHierarchyFromX(
			(sFolder + sFile).c_str(),
			D3DXMESH_MANAGED,
			g_pD3DDevice,
			&Alloc,
			NULL,
			&pFrame,
			&pAnimCon);
		_ASSERT(hr == S_OK);
		SetupBoneMatrixPtrs(pFrame, pFrame);
		m_mapSkinnedMeshes[sFolder + sFile] = pFrame;
		m_mapAnimationControl[sFolder + sFile] = pAnimCon;
	}

	return m_mapSkinnedMeshes[sFolder + sFile];
}

D3DXFRAME* cSkinnedMeshManager::GetSkinnedMesh(char* szFolder, char* szFile){
	return GetSkinnedMesh(std::string(szFolder), std::string(szFile));
}

D3DXFRAME* cSkinnedMeshManager::GetSkinnedMesh(std::string sFolder, std::string sFile, LPD3DXANIMATIONCONTROLLER& _pAnimCon){
	if (m_mapAnimationControl.find(sFolder + sFile) == m_mapAnimationControl.end() ||
		m_mapSkinnedMeshes.find(sFolder + sFile) == m_mapSkinnedMeshes.end())
	{
		D3DXFRAME*					pFrame;
		LPD3DXANIMATIONCONTROLLER	pAnimCon;

		cAllocateHierarchy Alloc;
		Alloc.SetFolder(sFolder);
		HRESULT hr = D3DXLoadMeshHierarchyFromX(
			(sFolder + sFile).c_str(),
			D3DXMESH_MANAGED,
			g_pD3DDevice,
			&Alloc,
			NULL,
			&pFrame,
			&pAnimCon);
		_ASSERT(hr == S_OK);
		
		SetupBoneMatrixPtrs(pFrame, pFrame);

		//return pFrame;
		m_mapSkinnedMeshes[sFolder + sFile] = pFrame;
		m_mapAnimationControl[sFolder + sFile] = pAnimCon;
	}
	// ������ ������ �״�� ������,
	// �ִϸ��̼� ��Ʈ�ѷ��� ���� �ٸ� �İ�ü�� ������ Ŭ�����ش�.
	// �� �Ұ��� �ϱ⿡ �� �Ŵ����� ������Ÿ�� �������� �̿��Ѵ�.
	LPD3DXANIMATIONCONTROLLER ret(NULL);
	HRESULT hr = m_mapAnimationControl[sFolder + sFile]->CloneAnimationController(
					m_mapAnimationControl[sFolder + sFile]->GetMaxNumAnimationOutputs(), 
					m_mapAnimationControl[sFolder + sFile]->GetMaxNumAnimationSets(), 
					m_mapAnimationControl[sFolder + sFile]->GetMaxNumTracks(), 
					m_mapAnimationControl[sFolder + sFile]->GetMaxNumEvents(), 
					&ret);

	_ASSERT(hr == S_OK);
	_pAnimCon = ret;
	return m_mapSkinnedMeshes[sFolder + sFile];
}

D3DXFRAME* cSkinnedMeshManager::GetSkinnedMesh(char* szFolder, char* szFile, LPD3DXANIMATIONCONTROLLER& pAnimCon){
	return GetSkinnedMesh(std::string(szFolder), std::string(szFile), pAnimCon);
}

void cSkinnedMeshManager::Destroy(){
	for each (auto p in m_mapSkinnedMeshes)
	{
		cAllocateHierarchy Alloc;
		D3DXFrameDestroy(p.second, &Alloc);
	}

	for each (auto p in m_mapAnimationControl)
	{		
		SAFE_RELEASE(p.second);
	}
}

void cSkinnedMeshManager::SetupBoneMatrixPtrs(D3DXFRAME* pFrame, D3DXFRAME* pFrameRoot)
{
	// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
	// �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.

	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
			// pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
			// pSkinInfo->GetBoneName(i) �� i�� �������� �̸��� ã��
			// D3DXFrameFind(��Ʈ ������, ������ �̸�) �� �������� ã��.
			// ã�Ƽ� �����Ʈ������ �ɾ����.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(pFrameRoot, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] = &pBone->matWorldTM;
			}
		}
	}

	ST_BONE* pBone = (ST_BONE*)pFrame;
	//��������� ��� �����ӿ� ���ؼ� ����.
	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs(pBone->pFrameSibling, pFrameRoot);
	}

	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(pBone->pFrameFirstChild, pFrameRoot);
	}
}