#include "stdafx.h"
#include "cHeightMapTerrainEdit.h"
#include "cMousePicking.h"

cHeightMapTerrainEdit::cHeightMapTerrainEdit() :
m_pVertexBuffer(NULL),
m_pIndexBuffer(NULL)
{
	// MS
	m_isRclick = false;
	eClick = E_ClickEmpty;
	ZeroMemory(&m_vClickFrom, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vClickTo, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vTileFrom, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vTileTo, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_stBoxMtl, sizeof(D3DMATERIAL9));
	
	m_fHeight = 1.f;
	m_fWidth = 1.f;
	m_isRButton = false;

}


cHeightMapTerrainEdit::~cHeightMapTerrainEdit()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pBoxMesh);

}

void cHeightMapTerrainEdit::Setup(){

	D3DXCreateBox(g_pD3DDevice, 1.0f, 1.0f, 1.0f, &m_pBoxMesh, 0);
	
	m_stMtl.Ambient = D3DXCOLOR(0.2f, 0.4f, 0.2f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.2f, 0.4f, 0.2f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.2f, 0.4f, 0.2f, 1.0f);

	m_stBoxMtl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.7f, 1.0f);
	m_stBoxMtl.Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.7f, 1.0f);
	m_stBoxMtl.Specular = D3DXCOLOR(0.2f, 0.2f, 0.7f, 1.0f);

	m_vecVertex.resize((m_nTileN + 1) * (m_nTileN + 1));

	for (int z = 0; z <= m_nTileN; ++z)
	{
		for (int x = 0; x <= m_nTileN; ++x)
		{
			m_vecVertex[z * (m_nTileN + 1) + x].p = D3DXVECTOR3((float)x, 0.0f, (float)z);
			m_vecVertex[z * (m_nTileN + 1) + x].t1 = D3DXVECTOR2((float)x / (float)m_nTileN, z / (float)m_nTileN);
			m_vecVertex[z * (m_nTileN + 1) + x].t2 = D3DXVECTOR2((float)x / (float)m_nTileN, z / (float)m_nTileN);
			m_vecVertex[z * (m_nTileN + 1) + x].n = D3DXVECTOR3(0, 1, 0);
		}
	}

	for (int z = 1; z < m_nTileN; ++z)
	{
		for (int x = 1; x < m_nTileN; ++x)
		{
			DWORD l = z * (m_nTileN + 1) + x - 1;
			DWORD r = z * (m_nTileN + 1) + x + 1;
			DWORD d = (z - 1) * (m_nTileN + 1) + x;
			DWORD u = (z + 1) * (m_nTileN + 1) + x;
			D3DXVECTOR3 du = m_vecVertex[u].p - m_vecVertex[d].p;
			D3DXVECTOR3 lr = m_vecVertex[r].p - m_vecVertex[l].p;
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);
			m_vecVertex[z * (m_nTileN + 1) + x].n = n;
		}
	}

	m_vecIndex.reserve(m_vecVertex.size() / 4 * 6);

	for (int z = 0; z < m_nTileN; ++z)
	{
		for (int x = 0; x < m_nTileN; ++x)
		{
			DWORD _0 = (z + 0) * (m_nTileN + 1) + x + 0;
			DWORD _1 = (z + 1) * (m_nTileN + 1) + x + 0;
			DWORD _2 = (z + 0) * (m_nTileN + 1) + x + 1;
			DWORD _3 = (z + 1) * (m_nTileN + 1) + x + 1;
			m_vecIndex.push_back(_0);
			m_vecIndex.push_back(_1);
			m_vecIndex.push_back(_2);
			m_vecIndex.push_back(_3);
			m_vecIndex.push_back(_2);
			m_vecIndex.push_back(_1);
		}
	}

	g_pD3DDevice->CreateVertexBuffer(
		m_vecVertex.size() * sizeof(ST_PNT2_VERTEX),
		D3DUSAGE_WRITEONLY,
		ST_PNT2_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVertexBuffer,
		0
		);

	g_pD3DDevice->CreateIndexBuffer(
		m_vecIndex.size() * sizeof(DWORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_pIndexBuffer,
		0);

	ST_PNT2_VERTEX* v;
	m_pVertexBuffer->Lock(0, 0, (void**)&v, 0);
	memcpy(v, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT2_VERTEX));
	m_pVertexBuffer->Unlock();
	
	DWORD* indices = 0;
	m_pIndexBuffer->Lock(0, 0, (void**)&indices, 0);
	memcpy(indices, &m_vecIndex[0], m_vecIndex.size() * sizeof(DWORD));
	m_pIndexBuffer->Unlock();

	HRESULT hr = D3DXCreateMeshFVF(m_vecIndex.size() / 3,
		m_vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT2_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT2_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT2_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &m_vecIndex[0], m_vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	for (size_t i = 0; i < m_vecIndex.size() / 3; ++i)
	{
		pA[i] = 0;
	}
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdjBuffer(m_vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdjBuffer[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuffer[0], 0, 0, 0);
}

void cHeightMapTerrainEdit::Update(float fDelta){


	D3DXVECTOR2 vFrom(0.f, 0.f);
	D3DXVECTOR2 vTo(0.f, 0.f);

	vFrom.x = (int)m_vTileFrom.x;
	vFrom.y = (int)m_vTileFrom.z;

	vTo.x = (int)m_vTileTo.x;
	vTo.y = (int)m_vTileTo.z;

	m_vBoxPos.x = (m_vTileFrom.x + ((m_vTileTo.x - m_vTileFrom.x) / 2.0f)) - 0.5f;
	m_vBoxPos.z = (m_vTileTo.z - ((m_vTileTo.z - m_vTileFrom.z) / 2.0f)) - 0.5f;
	m_vBoxPos.y = 0.0f;

	m_vBoxScale.x = m_vTileTo.x - m_vTileFrom.x;
	m_vBoxScale.z = m_vTileTo.z - m_vTileFrom.z;
	m_vBoxScale.y = 1.0f;

	if (GetAsyncKeyState(VK_UP)){
		ChangeMapYVertexCoord(vFrom, vTo, 0.1f);
	}
	if (GetAsyncKeyState(VK_DOWN)){
		ChangeMapYVertexCoord(vFrom, vTo, -.1f);
	}
	if (GetAsyncKeyState(VK_F5)){
		SaveToRawFile();
	}

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		ChangeMapYVertexCoord(vFrom, vTo, 0.0f);
		NormalizeTile();
	}


}

void cHeightMapTerrainEdit::Render(){
	//if (GetAsyncKeyState(VK_TAB)){
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtl);
		g_pD3DDevice->SetTexture(0, NULL);
	
		g_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(ST_PNT2_VERTEX));
		g_pD3DDevice->SetIndices(m_pIndexBuffer);

		g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0, 0, m_vecVertex.size(), 0, m_vecIndex.size());

		if (m_pMesh){
			m_pMesh->DrawSubset(0);
		}

		if (m_pBoxMesh){
			D3DXMATRIXA16 mat, matS;
			
			D3DXMatrixScaling(&matS, m_vBoxScale.x, m_vBoxScale.y, m_vBoxScale.z);
			D3DXMatrixTranslation(&mat, m_vBoxPos.x, m_vBoxPos.y, m_vBoxPos.z);
			
			mat = matS * mat;
			
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
			g_pD3DDevice->SetMaterial(&m_stBoxMtl);
			g_pD3DDevice->SetTexture(0, NULL);
			m_pBoxMesh->DrawSubset(0);
		}

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	
	/*}
	else{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtl);
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(ST_PNT2_VERTEX));
		g_pD3DDevice->SetIndices(m_pIndexBuffer);

		g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0, 0, m_vecVertex.size(), 0, m_vecIndex.size());
	}*/
}

void cHeightMapTerrainEdit::ChangeMapYVertexCoord(D3DXVECTOR2 vMin, D3DXVECTOR2 vMax, float fDelta){
	for (int z = (int)vMin.y; z < (int)vMax.y; z++){
		for (int x = (int)vMin.x; x < (int)vMax.x; x++){
			
			float check = m_vecVertex[z*(m_nTileN + 1) + x].p.y + fDelta;
			if (check > 25.6f){
				check = 25.5f;
			}
			else if (check < 0){
				check = 0;
			}
			m_vecVertex[z*(m_nTileN + 1) + x].p.y = check;
		}
	}

	CalBazier(vMin, vMax);


	ST_PNT2_VERTEX* v;
	m_pVertexBuffer->Lock(0, 0, (void**)&v, 0);
	memcpy(v, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT2_VERTEX));
	m_pVertexBuffer->Unlock();

	// 라데온 글카에선 프리미티브인덱스가 잘 안보임 ㅡㅡ
	ST_PNT2_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT2_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &m_vecIndex[0], m_vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	//DWORD* pA = NULL;
	//m_pMesh->LockAttributeBuffer(0, &pA);
	//for (size_t i = 0; i < m_vecIndex.size() / 3; ++i)
	//{
	//	pA[i] = 0;
	//}
	//m_pMesh->UnlockAttributeBuffer();

	//std::vector<DWORD> vecAdjBuffer(m_vecIndex.size());
	//m_pMesh->GenerateAdjacency(0.0f, &vecAdjBuffer[0]);

	//m_pMesh->OptimizeInplace(
	//	D3DXMESHOPT_ATTRSORT |
	//	D3DXMESHOPT_COMPACT |
	//	D3DXMESHOPT_VERTEXCACHE,
	//	&vecAdjBuffer[0], 0, 0, 0);
}

void cHeightMapTerrainEdit::SaveToRawFile(){
	FILE* fp = NULL;
	fopen_s(&fp, "../Resources/Map/test.raw", "w+b");

	//unsigned char y = fgetc(fp);
	//m_vecVertex[z * (m_nTileN + 1) + x].p = D3DXVECTOR3((float)x, y / 10.0f, (float)z);

	unsigned int c;
	for (auto p : m_vecVertex){
		unsigned char uc = (unsigned char)(p.p.y * 10.0f);
		fprintf(fp, "%c", uc);
	}

	//fputc()

	fclose(fp);
}

void cHeightMapTerrainEdit::SetClickRange(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo)
{
	D3DXVECTOR3 vFromFilterd(0.f, 0.f, 0.f);
	D3DXVECTOR3 vToFilterd(0.f, 0.f, 0.f);

	vFromFilterd = vFrom;
	vToFilterd = vTo;

	vFromFilterd.y = 0.f;
	vToFilterd.y = 0.f;
}

D3DXVECTOR3 cHeightMapTerrainEdit::Linear(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo, float fDelta)
{
	D3DXVECTOR3 vLinear(0.f, 0.f, 0.f);
	vLinear = fDelta*(vFrom - vTo) + vFrom;
	return vLinear;
}

D3DXVECTOR3 cHeightMapTerrainEdit::Bazier(D3DXVECTOR3 vFirst, D3DXVECTOR3 vSecond, D3DXVECTOR3 vThird, float fDelta)
{
	D3DXVECTOR3 vFrom = Linear(vFirst, vSecond, fDelta);
	D3DXVECTOR3 vTo = Linear(vSecond, vThird, fDelta);
	return Linear(vFrom, vTo, fDelta);
}

void cHeightMapTerrainEdit::CalBazier(D3DXVECTOR2 vMin, D3DXVECTOR2 vMax)
{
	// MS 범위
	D3DRECT rt;
	rt.x1 = vMin.x;
	rt.y1 = vMin.y;
	rt.x2 = vMax.x;
	rt.y2 = vMax.y;

	float fWidth = ceil((rt.x2 - rt.x1) / 2);
    float fHeight = ceil((rt.y2 - rt.y1) / 2);

	D3DRECT rtLeft;
	D3DRECT rtTop;
	D3DRECT rtRight;
	D3DRECT rtBottom;



	rtLeft.x1 = rt.x1 - fWidth;
	rtLeft.y1 = rt.y1;
	rtLeft.x2 = rt.x1;
	rtLeft.y2 = rt.y2;

	rtTop.x1 = rt.x1;
	rtTop.y1 = rt.y1 - fHeight;
	rtTop.x2 = rt.x2;
	rtTop.y2 = rt.y1;

	rtRight.x1 = rt.x2 - 1.f;
	rtRight.y1 = rt.y1;
	rtRight.x2 = rt.x2 + fWidth - 1;
	rtRight.y2 = rt.y2;

	rtBottom.x1 = rt.x1;
	rtBottom.y1 = rt.y2 - 1.f;
	rtBottom.x2 = rt.x2;
	rtBottom.y2 = rt.y2 + fHeight - 1.f;



	D3DXVECTOR3 vFirst(0.f, 0.f, 0.f);
	D3DXVECTOR3 vSecond(0.f, 0.f, 0.f);
	D3DXVECTOR3 vThird(0.f, 0.f, 0.f);


    std::vector<ST_PNT2_VERTEX> vecVertex = m_vecVertex;

	// LEFT
	for (int z = rtLeft.y1; z < rtLeft.y2; z++)
	{
		vFirst.x = rtLeft.x1;
		vFirst.z = z;
		vFirst.y = m_vecVertex[vFirst.x + vFirst.z * 257].p.y;

		vThird.x = rtLeft.x2;
		vThird.z = z;
		vThird.y = m_vecVertex[vThird.x + vThird.z * 257].p.y;

		vSecond.x = rtLeft.x2;
		vSecond.z = z;
		vSecond.y = m_vecVertex[vSecond.x + vSecond.z * 257].p.y;

		for (int x = rtLeft.x1; x < rtLeft.x2; x++)
		{
			float fX = (rtLeft.x2 - x);
			float fY = (rtLeft.x2 - rtLeft.x1);
			float fD = fX / fY;
			D3DXVECTOR3 v = Bazier(vThird, vSecond, vFirst, fD);
			m_vecVertex[x + z * 257].p.y = v.y;
		}
	}

	// TOP

	for (int x = rtTop.x1; x < rtTop.x2; x++)
	{
		vFirst.x = x;
		vFirst.z = rtTop.y1;
		vFirst.y = m_vecVertex[vFirst.x + vFirst.z * 257].p.y;

		vThird.x = x;
		vThird.z = rtTop.y2;
		vThird.y = m_vecVertex[vThird.x + vThird.z * 257].p.y;

		vSecond.x = x;
		vSecond.z = rtTop.y2;
		vSecond.y = m_vecVertex[vSecond.x + vSecond.z * 257].p.y;
		for (int z = rtTop.y1; z < rtTop.y2; z++)
		{
			float fX = (rtTop.y2 - z);
			float fY = (rtTop.y2 - rtTop.y1);
			float fD = fX / fY;
			D3DXVECTOR3 v = Bazier(vThird, vSecond, vFirst, fD);
			m_vecVertex[x + z * 257].p.y = v.y;
		}
	}

	// RIGHT
	for (int z = rtRight.y1; z < rtRight.y2; z++)
	{
		vFirst.x = rtRight.x1;
		vFirst.z = z;
		vFirst.y = m_vecVertex[vFirst.x + vFirst.z * 257].p.y;

		vThird.x = rtRight.x2;
		vThird.z = z;
		vThird.y = m_vecVertex[vThird.x + vThird.z * 257].p.y;

		vSecond.x = rtRight.x1;
		vSecond.z = z;
		vSecond.y = m_vecVertex[vSecond.x + vSecond.z * 257].p.y;

		for (int x = rtRight.x1; x < rtRight.x2; x++)
		{
			float fX = (rtRight.x2 - x);
			float fY = (rtRight.x2 - rtRight.x1);
			float fD = fX / fY;
			D3DXVECTOR3 v = Bazier(vFirst, vSecond, vThird, 1 - fD);
			m_vecVertex[x + z * 257].p.y = v.y;
		}
	}

	// BOTTOM
	for (int x = rtBottom.x1; x < rtBottom.x2; x++)
	{
		vFirst.x = x;
		vFirst.z = rtBottom.y1;
		vFirst.y = m_vecVertex[vFirst.x + vFirst.z * 257].p.y;

		vThird.x = x;
		vThird.z = rtBottom.y2;
		vThird.y = m_vecVertex[vThird.x + vThird.z * 257].p.y;

		vSecond.x = x;
		vSecond.z = rtBottom.y1;
		vSecond.y = m_vecVertex[vSecond.x + vSecond.z * 257].p.y;
		for (int z = rtBottom.y1; z < rtBottom.y2; z++)
		{
			//float fX = (z - rtBottom.y2);
			float fX = (rtBottom.y2 - z);
			float fY = (rtBottom.y2 - rtBottom.y1);
			float fD = fX / fY;
			D3DXVECTOR3 v = Bazier(vFirst, vSecond, vThird, 1 - fD);
			m_vecVertex[x + z * 257].p.y = v.y;


		}
	}

	D3DRECT rtLeftTop;
	D3DRECT rtRightTop;
	D3DRECT rtLeftBottom;
	D3DRECT rtRightBottom;

	// LeftBottom
	std::vector<ST_Bazier> vecBazierLeftBottom;

	rtLeftBottom.x1 = rtLeft.x1;
	rtLeftBottom.y1 = rtLeft.y2;
	rtLeftBottom.x2 = rtBottom.x1;
	rtLeftBottom.y2 = rtBottom.y2;

	float fLBWidth = (rtLeftBottom.x2 - rtLeftBottom.x1);
	float fLBHeight = (rtLeftBottom.y2 - rtLeftBottom.y1);
	for (int z = 0; z < fLBHeight; z++)
	{
		for (int x = 0; x < fLBWidth; x++)
		{
			ST_Bazier stBazier;
			stBazier.vPos.x = x + rtLeftBottom.x1;
			stBazier.vPos.y = z + rtLeftBottom.y1;
			float fX = (stBazier.vPos.x - rt.x1)*(stBazier.vPos.x - rt.x1);
			float fY = (stBazier.vPos.y - rt.y2)*(stBazier.vPos.y - rt.y2);
			stBazier.fDis = int(sqrt(fX + fY));
			vecBazierLeftBottom.push_back(stBazier);
		}
	}

	for (int i = 0; i < vecBazierLeftBottom.size(); i++)
	{
		D3DXVECTOR3 vThird(0.f, 0.f, 0.f);
		vThird.x = rtLeftBottom.x2;
		vThird.z = rtLeftBottom.y1;
		vThird.y = m_vecVertex[vThird.x + vThird.z * 257].p.y;

		D3DXVECTOR3 vSecond(0.f, 0.f, 0.f);
		vSecond.x = vThird.x;
		vSecond.z = vThird.z;

		float fDistFromCore = vecBazierLeftBottom[i].fDis;

		if (fDistFromCore < fLBHeight)
		{
			D3DXVECTOR3 vFirst(-1.f, -1.f, -1.f);
			vFirst.x = vecBazierLeftBottom[i].vPos.x;
			vFirst.z = vecBazierLeftBottom[i].vPos.y;
			vFirst.y = m_vecVertex[vFirst.x + vFirst.z * 257].p.y;

			float fD = vecBazierLeftBottom[i].fDis / fLBHeight;

			D3DXVECTOR3 v(-1.f, -1.f, -1.f);
			v = Bazier(vThird, vSecond, vFirst, 1 - fD);
			m_vecVertex[vFirst.x + vFirst.z * 257].p.y = v.y / 4.5f;
		}
	}

	// LeftTop
	std::vector<ST_Bazier> vecBazierLeftTop;

	rtLeftTop.x1 = rtLeft.x1;
	rtLeftTop.y1 = rtTop.y1;
	rtLeftTop.x2 = rt.x1;
	rtLeftTop.y2 = rt.y1;

	float fLTWidth = (rtLeftTop.x2 - rtLeftTop.x1);
	float fLTHeigth = (rtLeftTop.y2 - rtLeftTop.y1);
	for (int z = 0; z < fLTHeigth; z++)
	{
		for (int x = 0; x < fLTWidth; x++)
		{
			ST_Bazier stBazier;
			stBazier.vPos.x = x + rtLeftTop.x1;
			stBazier.vPos.y = z + rtLeftTop.y1;
			float fX = (stBazier.vPos.x - rt.x1)*(stBazier.vPos.x - rt.x1);
			float fY = (stBazier.vPos.y - rt.y1)*(stBazier.vPos.y - rt.y1);
			stBazier.fDis = int(sqrt(fX + fY));
			vecBazierLeftTop.push_back(stBazier);
		}
	}

	for (int i = 0; i < vecBazierLeftTop.size(); i++)
	{
		D3DXVECTOR3 vThird(0.f, 0.f, 0.f);
		vThird.x = rtLeftTop.x2;
		vThird.z = rtLeftTop.y2;
		vThird.y = m_vecVertex[vThird.x + vThird.z * 257].p.y;

		D3DXVECTOR3 vSecond(0.f, 0.f, 0.f);
		vSecond.x = vThird.x;
		vSecond.z = vThird.z;

		float fDistFromCore = vecBazierLeftTop[i].fDis;

		if (fDistFromCore < fLTWidth)
		{
			D3DXVECTOR3 vFirst(-1.f, -1.f, -1.f);
			vFirst.x = vecBazierLeftTop[i].vPos.x;
			vFirst.z = vecBazierLeftTop[i].vPos.y;
			vFirst.y = m_vecVertex[vFirst.x + vFirst.z * 257].p.y;

			float fD = vecBazierLeftTop[i].fDis / fLTWidth;

			D3DXVECTOR3 v(-1.f, -1.f, -1.f);
			v = Bazier(vThird, vSecond, vFirst, 1 - fD);
			m_vecVertex[vFirst.x + vFirst.z * 257].p.y = v.y / 4.5f;
		}
	}

	// RightTop
	std::vector<ST_Bazier> vecBazierRightTop;

	rtRightTop.x1 = rtTop.x2;
	rtRightTop.y1 = rtTop.y1;
	rtRightTop.x2 = rtRight.x2;
	rtRightTop.y2 = rtRight.y1;

	float fRTWidth = (rtRightTop.x2 - rtRightTop.x1);
	float fRTHeigth = (rtRightTop.y2 - rtRightTop.y1);
	for (int z = 0; z < fRTHeigth; z++)
	{
		for (int x = 0; x < fRTWidth; x++)
		{
			ST_Bazier stBazier;
			stBazier.vPos.x = x + rtRightTop.x1;
			stBazier.vPos.y = z + rtRightTop.y1;
			float fX = (stBazier.vPos.x - rt.x2)*(stBazier.vPos.x - rt.x2);
			float fY = (stBazier.vPos.y - rt.y1)*(stBazier.vPos.y - rt.y1);
			stBazier.fDis = int(sqrt(fX + fY));
			vecBazierRightTop.push_back(stBazier);
		}
	}

	for (int i = 0; i < vecBazierRightTop.size(); i++)
	{
		D3DXVECTOR3 vThird(0.f, 0.f, 0.f);
		vThird.x = rtRightTop.x1;
		vThird.z = rtRightTop.y2;
		vThird.y = m_vecVertex[vThird.x + vThird.z * 257].p.y;

		D3DXVECTOR3 vSecond(0.f, 0.f, 0.f);
		vSecond.x = vThird.x;
		vSecond.z = vThird.z;

		float fDistFromCore = vecBazierRightTop[i].fDis;

		if (fDistFromCore < fRTWidth)
		{
			D3DXVECTOR3 vFirst(-1.f, -1.f, -1.f);
			vFirst.x = vecBazierRightTop[i].vPos.x;
			vFirst.z = vecBazierRightTop[i].vPos.y;
			vFirst.y = m_vecVertex[vFirst.x + vFirst.z * 257].p.y;

			float fD = vecBazierRightTop[i].fDis / fRTWidth;

			D3DXVECTOR3 v(-1.f, -1.f, -1.f);
			v = Bazier(vThird, vSecond, vFirst, 1 - fD);
			m_vecVertex[vFirst.x + vFirst.z * 257].p.y = v.y / 4.5f;
		}
	}

	// RightBottom
	std::vector<ST_Bazier> vecBazierRightBottom;

	rtRightBottom.x1 = rt.x2 - 1.f;
	rtRightBottom.y1 = rt.y2;
	rtRightBottom.x2 = rtRight.x2 -1.f;
	rtRightBottom.y2 = rtBottom.y2;

	float fRBWidth = (rtRightBottom.x2 - rtRightBottom.x1);
	float fRBHeight = (rtRightBottom.y2 - rtRightBottom.y1);

	for (int z = 0; z < fRBHeight; z++)
	{
		for (int x = 0; x < fRBWidth; x++)
		{
			ST_Bazier stBazier;
			stBazier.vPos.x = x + rtRightBottom.x1;
			stBazier.vPos.y = z + rtRightBottom.y1;
			float fX = (stBazier.vPos.x - rt.x2)*(stBazier.vPos.x - rt.x2);
			float fY = (stBazier.vPos.y - rt.y2)*(stBazier.vPos.y - rt.y2);
			stBazier.fDis = int(sqrt(fX + fY));
			vecBazierRightBottom.push_back(stBazier);
		}
	}

	for (int i = 0; i < vecBazierRightBottom.size(); i++)
	{
		D3DXVECTOR3 vThird(0.f, 0.f, 0.f);
		vThird.x = rtRightBottom.x1;
		vThird.z = rtRightBottom.y1;
		vThird.y = m_vecVertex[vThird.x + vThird.z * 257].p.y;

		D3DXVECTOR3 vSecond(0.f, 0.f, 0.f);
		vSecond.x = vThird.x;
		vSecond.z = vThird.z;

		float fDistFromCore = vecBazierRightBottom[i].fDis;

		if (fDistFromCore < fRBHeight)
		{
			D3DXVECTOR3 vFirst(-1.f, -1.f, -1.f);
			vFirst.x = vecBazierRightBottom[i].vPos.x;
			vFirst.z = vecBazierRightBottom[i].vPos.y;
			vFirst.y = m_vecVertex[vFirst.x + vFirst.z * 257].p.y;

			float fD = vecBazierRightBottom[i].fDis / fRBHeight;

			D3DXVECTOR3 v(-1.f, -1.f, -1.f);
			v = Bazier(vThird, vSecond, vFirst, 1 - fD);
			m_vecVertex[vFirst.x + vFirst.z * 257].p.y = v.y / 4.5f;
		}
	}
}

void cHeightMapTerrainEdit::NormalizeTile()
{
	float fNormalizeTile = 0.f;

	if (m_isRButton)
	{
		int nSelectSize = 2;

		std::vector<D3DXVECTOR3> vecTile;

		int nX = (int)m_vSelectTile.x;
		int nZ = (int)m_vSelectTile.z;

		for (int z = nZ - 1; z < nZ + nSelectSize; z++)
		{
			for (int x = nX - 1; x < nX + nSelectSize; x++)
			{
				D3DXVECTOR3 vTile(0.f, 0.f, 0.f);
				vTile.x = x;
				vTile.z = z;

				vecTile.push_back(vTile);
			}
		}

		for (int i = 0; i < vecTile.size(); i++)
		{
			int nTileX = 0;
			int nTileZ = 0;

			nTileX = vecTile[i].x;
			nTileZ = vecTile[i].z;
			float fHeight = m_vecVertex[nTileX + nTileZ * 257].p.y;

			fNormalizeTile += fHeight;
		}
		fNormalizeTile = fNormalizeTile / vecTile.size();

		if (GetAsyncKeyState(VK_SPACE))
		{
			for (int i = 0; i < vecTile.size(); i++)
			{
				int nTileX = 0;
				int nTileZ = 0;

				nTileX = vecTile[i].x;
				nTileZ = vecTile[i].z;

				//if (m_vecVertex[nTileX + nTileZ * 257].p.y > fNormalizeTile)
				//{
				//	m_vecVertex[nTileX + nTileZ * 257].p.y -= fNormalizeTile / 10;
				//}
				//else if (m_vecVertex[nTileX + nTileZ * 257].p.y < fNormalizeTile)
				//{
				//	m_vecVertex[nTileX + nTileZ * 257].p.y += fNormalizeTile / 10;
				//}
				//else
				{
					m_vecVertex[nTileX + nTileZ * 257].p.y = fNormalizeTile;
				}

			}
		}
		else
		{
			if (m_vecVertex[nX + nZ * 257].p.y > fNormalizeTile)
			{
				m_vecVertex[nX + nZ * 257].p.y -= fNormalizeTile / 10;
			}
			else if (m_vecVertex[nX + nZ * 257].p.y < fNormalizeTile)
			{
				m_vecVertex[nX + nZ * 257].p.y += fNormalizeTile / 10;
			}
			else
			{
				m_vecVertex[nX + nZ * 257].p.y = fNormalizeTile;
			}
		}
	}

}