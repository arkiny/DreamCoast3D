#include "stdafx.h"
#include "cGameMapHeight.h"


cGameMapHeight::cGameMapHeight() 
	: cGameMapObject()
	, m_nTileN(256)
	, m_nBytePerColor(1)
	, m_pMesh(NULL)
	, m_pTexture(NULL)
	, m_pTexture2(NULL)
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
}


cGameMapHeight::~cGameMapHeight()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pTexture2);
	SAFE_RELEASE(m_pTextureNormal);
	//SAFE_RELEASE(m_pSpecularMapping);
	//SAFE_DELETE(m_pMousePicking);
}

void cGameMapHeight::LoadFromFiles(std::string sFilename, std::string sTextureFilename){
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	// 높이에따른 텍스처
	m_pTexture = g_pTextureManager->GetTexture(std::string("../Resources/Map/TerrainHeightGradient2.png"));
	
	// 재질 // 재질을 바르면 그림자 노말맵이 표현 안댐? 어떻게 해야함?
	m_pTexture2 = g_pTextureManager->GetTexture(std::string("../Resources/Map/terrain_detail_texture.jpg"));
	m_pTextureNormal = g_pTextureManager->GetTexture(std::string("../Resources/Map/Normal.tga"));

	std::vector<DWORD>			vecIndex;

	m_vecVertex.resize((m_nTileN + 1) * (m_nTileN + 1));
	vecIndex.reserve(m_nTileN * m_nTileN * 2 * 3);

	FILE* fp = NULL;
	fopen_s(&fp, sFilename.c_str(), "rb");

	for (int z = 0; z <= m_nTileN; ++z)
	{
		for (int x = 0; x <= m_nTileN; ++x)
		{
			unsigned char y = fgetc(fp);
			m_vecVertex[z * (m_nTileN + 1) + x].p = D3DXVECTOR3((float)x, y / 10.0f, (float)z);
			//m_vecVertex[z * (m_nTileN + 1) + x].t1 = D3DXVECTOR2((float)x / (float)m_nTileN, z / (float)m_nTileN);
			m_vecVertex[z * (m_nTileN + 1) + x].t1 = D3DXVECTOR2((y / 10.0f) / 25.6f, (y / 10.0f) / 25.6f);
			m_vecVertex[z * (m_nTileN + 1) + x].t2 = D3DXVECTOR2((float)x/5.0f, (float)z/5.0f);
			m_vecVertex[z * (m_nTileN + 1) + x].n = D3DXVECTOR3(0, 1, 0);
		}
	}
	
	// tiling
	//

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
	fclose(fp);
	// 1-3
	// |\|
	// 0-2
	for (int z = 0; z < m_nTileN; ++z)
	{
		for (int x = 0; x < m_nTileN; ++x)
		{
			DWORD _0 = (z + 0) * (m_nTileN + 1) + x + 0;
			DWORD _1 = (z + 1) * (m_nTileN + 1) + x + 0;
			DWORD _2 = (z + 0) * (m_nTileN + 1) + x + 1;
			DWORD _3 = (z + 1) * (m_nTileN + 1) + x + 1;
			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);
			vecIndex.push_back(_3);
			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
		}
	}


	HRESULT hr = D3DXCreateMeshFVF(vecIndex.size() / 3,
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
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	for (size_t i = 0; i < vecIndex.size() / 3; ++i)
	{
		pA[i] = 0;
	}
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdjBuffer(vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdjBuffer[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuffer[0], 0, 0, 0);


	m_sRawFile = sFilename;
	m_sTexturePath = sTextureFilename;
	g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx");
	g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx");
}

void cGameMapHeight::LoadFromFiles(char* szFilename, char* szTextureFilename){
	LoadFromFiles(std::string(szFilename), std::string(szTextureFilename));
}
	
void cGameMapHeight::Render(){
	//m_pMousePicking->Update();

	if (GetAsyncKeyState(VK_TAB)){
		//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		g_pD3DDevice->SetMaterial(&m_stMtl);
		g_pD3DDevice->SetTexture(0, m_pTexture);
		//g_pD3DDevice->SetFVF(m_pMesh->GetFVF());
		
		if (m_pTexture2){
			g_pD3DDevice->SetTexture(1, m_pTexture2);
			/*g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
*/
			g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);

		/*	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1,
								D3DTA_TEXTURE | D3DTA_ALPHAREPLICATE);*/
		}

		m_pMesh->DrawSubset(0);
		//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		g_pD3DDevice->SetTexture(1, NULL);
	}
	else{
		//	FAR_PLANE);

		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		D3DXMATRIXA16 matView;
		D3DXMATRIXA16 matProjection;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

		// 쉐이더 전역변수들을 설정
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetMatrix("gWorldMatrix", &matWorld);
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetMatrix("gViewMatrix", &matView);
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetMatrix("gProjectionMatrix", &matProjection);
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetVector("gLightColor", &gLightColor);
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetTexture("DiffuseMap_Tex", m_pTexture);
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetTexture("SpecularMap_Tex", m_pTextureNormal);
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetTexture("DetailMap_Tex", m_pTexture2);

		D3DLIGHT9 stLight;
		g_pD3DDevice->GetLight(0, &stLight);
		D3DXVECTOR3 pos = -1000 * stLight.Direction;
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetVector("gWorldLightPosition", &D3DXVECTOR4(pos, 0.0f));
		
		
		// 쉐이더를 시작한다.
		UINT numPasses = 0;
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->BeginPass(i);
				{
					m_pMesh->DrawSubset(0);
				}
				g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->EndPass();
			}
		}
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->End();


		//////////////////////////////////////////////////
		

		//D3DXMATRIXA16 matLightView;
		//{
		//	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		//	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		//	D3DXMatrixLookAtLH(&matLightView, &pos, &vLookatPt, &vUpVec);
		//}

		//D3DXMATRIXA16 matLightProjection; {
		//	D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);
		//}

		//g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		//g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
		//D3DXMATRIXA16 matViewProject; {
		//	D3DXMatrixMultiply(&matViewProject, &matView, &matProjection);
		//}

		//// 쉐이더 전역변수들을 설정
		//g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")
		//	->SetMatrix("gWorldMatrix", &matWorld);
		//
		////gLightViewMatrix
		////gLightProjectionMatrix
		////gWorldLightPosition
		////gViewProjectionMatrix
		////gObjectColor
		//g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")
		//	->SetMatrix("gLightViewMatrix", &matView);
		//g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")
		//	->SetMatrix("gLightProjectionMatrix", &matProjection);
		//g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")
		//	->SetVector("gWorldLightPosition", &D3DXVECTOR4(pos,1.0f));
		//g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")
		//	->SetMatrix("gViewProjectionMatrix", &matViewProject);
		//g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")
		//	->SetVector("gObjectColor", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		//g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")
		//	->SetTexture("ShadowMap_Tex", g_pShaderManager->GetShadowRenderTarget());
		//// 쉐이더를 시작한다.
		//UINT numPasses = 0;
		//g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")->Begin(&numPasses, NULL);
		//{
		//	for (UINT i = 0; i < numPasses; ++i)
		//	{
		//		g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")->BeginPass(i);
		//		{
		//			m_pMesh->DrawSubset(0);
		//		}
		//		g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")->EndPass();
		//	}
		//}
		//g_pShaderManager->GetShader("../Resources/Shader/ApplyShadow.fx")->End();
	}
}

float cGameMapHeight::GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition){
	if (pvPosition->x < 0 ||
		pvPosition->z < 0 ||
		pvPosition->x >= m_nTileN ||
		pvPosition->z >= m_nTileN)
	{
		isLand = false;
		return 0.0f;
	}

	// 1-3
	// |\|
	// 0-2
	_ASSERT(pvPosition);

	isLand = true;

	int x = (int)pvPosition->x;
	int z = (int)pvPosition->z;

	DWORD _0 = (z + 0) * (m_nTileN + 1) + x + 0;
	DWORD _1 = (z + 1) * (m_nTileN + 1) + x + 0;
	DWORD _2 = (z + 0) * (m_nTileN + 1) + x + 1;
	DWORD _3 = (z + 1) * (m_nTileN + 1) + x + 1;

	float fDeltaX = pvPosition->x - x;
	float fDeltaZ = pvPosition->z - z;

	if (fDeltaX + fDeltaZ < 1.0f)
	{
		D3DXVECTOR3 v01 = m_vecVertex[_1].p - m_vecVertex[_0].p;
		D3DXVECTOR3 v02 = m_vecVertex[_2].p - m_vecVertex[_0].p;
		D3DXVECTOR3 vDelta = v01 * fDeltaZ + v02 * fDeltaX;
		return m_vecVertex[_0].p.y + vDelta.y;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;
		D3DXVECTOR3 v31 = m_vecVertex[_1].p - m_vecVertex[_3].p;
		D3DXVECTOR3 v32 = m_vecVertex[_2].p - m_vecVertex[_3].p;
		D3DXVECTOR3 vDelta = v31 * fDeltaX + v32 * fDeltaZ;
		return m_vecVertex[_3].p.y + vDelta.y;
	}

	return 0.0f;
}

std::string cGameMapHeight::SaveAsStringInfo(){
	std::stringstream ss;
	ss << std::endl;
	ss << "*GAME_MAP_INFO {" << std::endl;
	ss << "*GAME_MAP_COUNT " << 1 << std::endl;
	ss << "*HEIGHT_MAP " << 0 << " {" << std::endl;
	ss << "*GAME_MAP_HEIGHT_RAW_FILE " << "\"" << m_sRawFile << "\"" << std::endl;
	ss << "*GAME_MAP_HEIGHT_TEXTURE_PATH " << "\"" << m_sTexturePath << "\"" << std::endl;
	ss << "}" << std::endl;
	ss << "}" << std::endl;
	return ss.str();
}