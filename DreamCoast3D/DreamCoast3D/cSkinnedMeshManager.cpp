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
	// 프레임 정보는 그대로 쓰지만,
	// 애니메이션 컨트롤러는 각각 다른 컴객체기 때문에 클론해준다.
	// 는 불가능 하기에 현 매니저는 프로토타입 패턴으로 이용한다.
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
	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.

	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
			// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
			// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
			// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
			// 찾아서 월드매트릭스를 걸어줘라.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(pFrameRoot, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] = &pBone->matWorldTM;
			}
		}
	}

	ST_BONE* pBone = (ST_BONE*)pFrame;
	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs(pBone->pFrameSibling, pFrameRoot);
	}

	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(pBone->pFrameFirstChild, pFrameRoot);
	}
}