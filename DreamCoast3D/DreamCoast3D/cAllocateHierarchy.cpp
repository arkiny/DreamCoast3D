#include "stdafx.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"

cAllocateHierarchy::cAllocateHierarchy()
{
}


cAllocateHierarchy::~cAllocateHierarchy()
{
}

STDMETHODIMP cAllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	ST_BONE* pFrame = new ST_BONE;

	//pFrame->Name = NULL;
	if (Name){
		pFrame->Name = new char[strlen(Name) + 1];
		strcpy(pFrame->Name, Name);
	}
	else{
		pFrame->Name = NULL;
	}

	pFrame->pFrameFirstChild = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pMeshContainer = NULL;
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->matWorldTM);
	*ppNewFrame = pFrame;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::CreateMeshContainer(
	THIS_ LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	ST_BONE_MESH_SPHERE* pBoneMesh = new ST_BONE_MESH_SPHERE;
	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		cMtlTex* pMtlTex = new cMtlTex;
		std::string sFile(pMaterials[i].pTextureFilename);
		pMtlTex->pTex = g_pTextureManager->GetTexture((m_sFolder + sFile).c_str());
		pMtlTex->stMtl = pMaterials[i].MatD3D;
		pBoneMesh->vecMtlTex.push_back(pMtlTex);
	}

	if (pMeshData && D3DXMESHTYPE_MESH == pMeshData->Type)
	{
		pBoneMesh->MeshData.Type = pMeshData->Type;
		pBoneMesh->MeshData.pMesh = pMeshData->pMesh;
		pMeshData->pMesh->GetAttributeTable(0, &pBoneMesh->dwNumSubset);

		pMeshData->pMesh->AddRef();
	}

	if (pSkinInfo){
		// step 1. pSkinInfo ����
		pSkinInfo->Clone(&pBoneMesh->pSkinInfo);
		// step 2. ���� �޽� ����
		if (pMeshData && D3DXMESHTYPE_MESH == pMeshData->Type)		{
			pMeshData->pMesh->CloneMeshFVF(
				pMeshData->pMesh->GetOptions(),
				pMeshData->pMesh->GetFVF(),
				g_pD3DDevice,
				&pBoneMesh->pOrigMesh);
			// step 3. pSkinInfo->GetNumBones()�� ���� ������� ��ġ�� ��� ���� ���� ��Ʈ���� ���� ����
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			/*D3DXMATRIX**			ppBoneMatrixPtrs;
			D3DXMATRIX*				pBoneOffsetMatrices;
			D3DXMATRIX*				pCurrentBoneMatrices;*/
			// ppBoneMatrixPtrs, pBoneOffsetMatrices, pCurrentBoneMatrices�� �����Ҵ�
			pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];
			pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
			pBoneMesh->pCurrentBoneMatrices = new D3DXMATRIX[dwNumBones];

			//�浹�� �ٿ�� ���Ǿ ���� �����Ѵ�. : �ο�
			CreateCollisionBoundingSphere(pBoneMesh, pMeshData);

			// step 4. ���� �Ҵ�� pBoneOffsetMatrices ��Ʈ������ �� ����.
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				pBoneMesh->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
			}
		}
	}

	(*ppNewMeshContainer) = pBoneMesh;
	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	ST_BONE* pBone = (ST_BONE*)pFrameToFree;
	if (pBone->Name)
	{
		SAFE_DELETE_ARRAY(pBone->Name);
	}
	delete pFrameToFree;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	ST_BONE_MESH_SPHERE* pBoneMesh = (ST_BONE_MESH_SPHERE*)pMeshContainerToFree;
	SAFE_RELEASE(pBoneMesh->pSkinInfo);
	SAFE_RELEASE(pBoneMesh->MeshData.pMesh);
	SAFE_RELEASE(pBoneMesh->pOrigMesh);
	SAFE_DELETE_ARRAY(pBoneMesh->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(pBoneMesh->pBoneOffsetMatrices);
	SAFE_DELETE_ARRAY(pBoneMesh->pCurrentBoneMatrices);

	for each(auto p in pBoneMesh->vecMtlTex)
	{
		SAFE_RELEASE(p);
	}
	
	SAFE_RELEASE(pBoneMesh->pSphereMesh);
	delete pBoneMesh;
	return S_OK;
}

//�浹�� �ٿ�����Ǿ� ���� : �ο�
void cAllocateHierarchy::CreateCollisionBoundingSphere(ST_BONE_MESH* pBoneMesh, CONST D3DXMESHDATA *pMeshData)
{
	ST_BONE_MESH_SPHERE* p = (ST_BONE_MESH_SPHERE*)pBoneMesh;
	//�浹�� �ٿ�����Ǿ� ����
	D3DXVECTOR3* pVertices;
	pMeshData->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
	D3DXComputeBoundingSphere(pVertices,
		pMeshData->pMesh->GetNumVertices(),
		pMeshData->pMesh->GetNumBytesPerVertex(),
		&p->vCenter,
		&p->fRadius);
	pMeshData->pMesh->UnlockVertexBuffer();
	p->fOriginRadius = p->fRadius;
	D3DXCreateSphere(g_pD3DDevice, p->fRadius, 7, 5, &p->pSphereMesh, &p->pSphereAdj);
}