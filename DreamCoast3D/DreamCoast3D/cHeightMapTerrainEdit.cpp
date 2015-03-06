#include "stdafx.h"
#include "cHeightMapTerrainEdit.h"


cHeightMapTerrainEdit::cHeightMapTerrainEdit() :
m_pVertexBuffer(NULL),
m_pIndexBuffer(NULL)
{
}


cHeightMapTerrainEdit::~cHeightMapTerrainEdit()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
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

}

void cHeightMapTerrainEdit::Update(float fDelta){
	if ( GetAsyncKeyState(VK_UP)){
		ChangeMapYVertexCoord(D3DXVECTOR2(30, 30), D3DXVECTOR2(50, 50), 10.0f);

	}
	if (GetAsyncKeyState(VK_DOWN)){
		ChangeMapYVertexCoord(D3DXVECTOR2(30, 30), D3DXVECTOR2(50, 50), -10.0f);
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

void cHeightMapTerrainEdit::SaveToRawFile(){
	FILE* fp = NULL;
	fopen_s(&fp, "test.raw", "w+b");

	for (auto p : m_vecVertex){
		fputc(static_cast<char>(p.p.y * 10), fp);
	}

	//fputc()

	fclose(fp);
}