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
	for (int z = vMin.y; z < vMax.y; z++){
		for (int x = vMin.x; x < vMax.x; x++){
			m_vecVertex[z*(m_nTileN + 1) + x].p.y += fDelta;
		}
	}

	ST_PNT_VERTEX* v;
	m_pVertexBuffer->Lock(0, 0, (void**)&v, 0);
	memcpy(v, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pVertexBuffer->Unlock();
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