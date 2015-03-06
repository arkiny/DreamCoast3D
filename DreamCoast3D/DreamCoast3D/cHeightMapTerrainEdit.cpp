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
	ZeroMemory(m_vClickFrom, sizeof(D3DXVECTOR3));
	ZeroMemory(m_vClickTo, sizeof(D3DXVECTOR3));
	ZeroMemory(m_vTileFrom, sizeof(D3DXVECTOR3));
	ZeroMemory(m_vTileTo, sizeof(D3DXVECTOR3));
	m_fHeight = 1.f;
	m_fWidth = 1.f;

	m_pMousPicking = nullptr;
}


cHeightMapTerrainEdit::~cHeightMapTerrainEdit()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	SAFE_DELETE(m_pMousPicking);
}

void cHeightMapTerrainEdit::Setup(){
	
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	m_vecVertex.resize((m_nTileN + 1) * (m_nTileN + 1));

	for (int z = 0; z <= m_nTileN; ++z)
	{
		for (int x = 0; x <= m_nTileN; ++x)
		{
			m_vecVertex[z * (m_nTileN + 1) + x].p = D3DXVECTOR3((float)x, 0.0f, (float)z);
			m_vecVertex[z * (m_nTileN + 1) + x].t = D3DXVECTOR2((float)x / (float)m_nTileN, z / (float)m_nTileN);
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
		m_vecVertex.size() * sizeof(ST_PNT_VERTEX),
		D3DUSAGE_WRITEONLY,
		ST_PNT_VERTEX::FVF,
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

	ST_PNT_VERTEX* v;
	m_pVertexBuffer->Lock(0, 0, (void**)&v, 0);
	memcpy(v, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pVertexBuffer->Unlock();
	
	DWORD* indices = 0;
	m_pIndexBuffer->Lock(0, 0, (void**)&indices, 0);
	memcpy(indices, &m_vecIndex[0], m_vecIndex.size() * sizeof(DWORD));
	m_pIndexBuffer->Unlock();

	m_pMousPicking = new cMousePicking;
	m_pMousPicking->SetVertex(m_vecVertex);
}

void cHeightMapTerrainEdit::Update(float fDelta){

	m_pMousPicking->Update();

	MouseUpdate();
	MouseRangeUpdate();
	TileRangeUpdate();

	D3DXVECTOR2 vFrom(0.f, 0.f);
	D3DXVECTOR2 vTo(0.f, 0.f);

	vFrom.x = (int)m_vTileFrom.x;
	vFrom.y = (int)m_vTileFrom.z;

	vTo.x = (int)m_vTileTo.x;
	vTo.y = (int)m_vTileTo.z;

	if (GetAsyncKeyState(VK_UP)){
		ChangeMapYVertexCoord(vFrom, vTo, 1.0f);

	}
	if (GetAsyncKeyState(VK_DOWN)){
		ChangeMapYVertexCoord(vFrom, vTo, -1.0f);
	}
	if (GetAsyncKeyState(VK_F5)){
		SaveToRawFile();
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
	
		g_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(ST_PNT_VERTEX));
		g_pD3DDevice->SetIndices(m_pIndexBuffer);

		g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0, 0, m_vecVertex.size(), 0, m_vecIndex.size());
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	/*}
	else{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtl);
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(ST_PNT_VERTEX));
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
	// MS ¹üÀ§
	D3DRECT rt;
	rt.x1 = vMin.x;
	rt.y1 = vMin.y;
	rt.x2 = vMax.x;
	rt.y2 = vMax.y;

	float fWidth = (rt.x2 - rt.x1) / 2;
	float wHeight = (rt.y2 - rt.y1) / 2;

	D3DRECT rtLeft;
	D3DRECT rtTop;
	D3DRECT rtRight;
	D3DRECT rtBottom;

	rtLeft.x1 = rt.x1 - fWidth;
	rtLeft.y1 = rt.y1;
	rtLeft.x2 = rt.x1;
	rtLeft.y2 = rt.y2;

	rtTop.x1 = rt.x1;
	rtTop.y1 = rt.y1 - m_fHeight;
	rtTop.x2 = rt.x2;
	rtTop.y2 = rt.y1;

	rtRight.x1 = rt.x2;
	rtRight.y1 = rt.y1;
	rtRight.x2 = rt.x2 + m_fWidth;
	rtRight.y2 = rt.y2;

	rtBottom.x1 = rt.x1;
	rtBottom.y1 = rt.y2;
	rtBottom.x2 = rt.x2;
	rtBottom.y2 = rt.y2 + m_fHeight;

	for (int z = rtLeft.y1; z < rtLeft.y2; z++)
	{
		D3DXVECTOR3 vFrom(0.f, 0.f, 0.f);
		D3DXVECTOR3 vTo(0.f, 0.f, 0.f);
		vTo.x = rtLeft.x2;
		vTo.z = z;
		vTo.y = m_vecVertex[vTo.x + vTo.z * 257].p.y;
		for (int x = rtLeft.x1; x < rtLeft.x2; x++)
		{
			vFrom.x = x;
			vFrom.z = z;
			vFrom.y = m_vecVertex[vFrom.x + vFrom.z * 257].p.y;
			float nX = (x - rtLeft.x1);
			float nY = (rtLeft.x2 - rtLeft.x1);
			float fDelta = nX / nY;
			m_vecVertex[x + z * 257].p.y -= Linear(vFrom, vTo, fDelta).y;
		}
	}

	ST_PNT_VERTEX* v;
	m_pVertexBuffer->Lock(0, 0, (void**)&v, 0);
	memcpy(v, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pVertexBuffer->Unlock();
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

void cHeightMapTerrainEdit::MouseUpdate()
{
	if (g_pControlManager->GetInputInfo(VK_RBUTTON))
	{
		if (eClick == E_ClickOn)
		{
			eClick = E_Clicking;
		}
		if (eClick == E_ClickEmpty)
		{
			eClick = E_ClickOn;
		}
	}

	if (!g_pControlManager->GetInputInfo(VK_RBUTTON))
	{
		if (eClick == E_ClickOff)
		{
			eClick = E_ClickEmpty;
		}

		if (eClick == E_Clicking)
		{
			eClick = E_ClickOff;
		}
	}
}

void cHeightMapTerrainEdit::MouseRangeUpdate()
{
	if (eClick == E_ClickOn)
	{
		m_vClickFrom = m_pMousPicking->GetPickingPoint();
	}

	if (eClick == E_ClickOff)
	{
		m_vClickTo = m_pMousPicking->GetPickingPoint();
		m_vTileFrom = m_vClickFrom;
		m_vTileTo = m_vTileFrom;
	}
}

void cHeightMapTerrainEdit::TileRangeUpdate()
{
	if (g_pControlManager->GetInputInfo('P'))
	{
		m_vTileTo.x += m_fWidth;
	}

	if (g_pControlManager->GetInputInfo('O'))
	{
		m_vTileTo.z += m_fHeight;
	}
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