#include "stdafx.h"
#include "cLoadingScene.h"
#include "cSceneLoader.h"
#include <sstream>

#define PI           3.14159265f
#define FOV          ( D3DX_PI /4.0f)							// �þ߰�
#define ASPECT_RATIO (1280.0f/(float)720.0f)			// ȭ���� ��Ⱦ��
#define NEAR_PLANE   1									// ���� ���
#define FAR_PLANE    3000								// ���Ÿ� ���

static CRITICAL_SECTION				gCriticalSection;

//----------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------

// ��
LPD3DXMESH				gpTorus = NULL;
LPD3DXMESH				gpDisc = NULL;

// ���̴�
LPD3DXEFFECT			gpApplyShadowShader = NULL;
LPD3DXEFFECT			gpCreateShadowShader = NULL;

// ȸ����
float					gRotationY = 0.0f;

// ���� ��ġ
D3DXVECTOR4				gWorldLightPosition(500.0f, 500.0f, -500.0f, 1.0f);

// ī�޶� ��ġ
D3DXVECTOR4				gWorldCameraPosition(0.0f, 0.0f, -200.0f, 1.0f);

// ��ü�� ����
D3DXVECTOR4				gTorusColor(1, 1, 0, 1);
D3DXVECTOR4				gDiscColor(0.8, 0.8, 0.8, 1);

// �׸��ڸ� ����Ÿ��
LPDIRECT3DTEXTURE9		gpShadowRenderTarget = NULL;
LPDIRECT3DSURFACE9		gpShadowDepthStencil = NULL;


cLoadingScene::cLoadingScene()
	:m_pFont(NULL)
{
	InitializeCriticalSection(&gCriticalSection);
}


cLoadingScene::~cLoadingScene()
{
	WaitForSingleObject(LoadNextScene, INFINITE);
	LeaveCriticalSection(&gCriticalSection);
	DeleteCriticalSection(&gCriticalSection);
	SAFE_RELEASE(m_pFont);
	if (m_pNextScene->GetRefCount() > 1){
		SAFE_RELEASE(m_pNextScene);
	}

	// ���� release �Ѵ�.
	if (gpTorus)
	{
		gpTorus->Release();
		gpTorus = NULL;
	}

	if (gpDisc)
	{
		gpDisc->Release();
		gpDisc = NULL;
	}

	// ���̴��� release �Ѵ�.
	if (gpApplyShadowShader)
	{
		gpApplyShadowShader->Release();
		gpApplyShadowShader = NULL;
	}

	if (gpCreateShadowShader)
	{
		gpCreateShadowShader->Release();
		gpCreateShadowShader = NULL;
	}

	// �ؽ�ó�� release �Ѵ�.
	if (gpShadowRenderTarget)
	{
		gpShadowRenderTarget->Release();
		gpShadowRenderTarget = NULL;
	}

	if (gpShadowDepthStencil)
	{
		gpShadowDepthStencil->Release();
		gpShadowDepthStencil = NULL;
	}
	if (pHWBackBuffer){
		g_pD3DDevice->SetRenderTarget(0, pHWBackBuffer);
		g_pD3DDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);
		g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), /*D3DCOLOR_XRGB(80, 80, 80)*/0x00000000, 1.0f, 0);

		pHWBackBuffer->Release();
		pHWBackBuffer = NULL;
		pHWDepthStencilBuffer->Release();
		pHWDepthStencilBuffer = NULL;
	}

}

void cLoadingScene::Setup(std::string sNextScene){
	m_sNextScenePath = sNextScene;


	// ����Ÿ���� �����.
	const int shadowMapSize = 2048;
	if (FAILED(g_pD3DDevice->CreateTexture(shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &gpShadowRenderTarget, NULL)))
	{
	}

	// �׸��� �ʰ� ������ ũ���� ���̹��۵� �������� �Ѵ�.
	if (FAILED(g_pD3DDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&gpShadowDepthStencil, NULL)))
	{
	}

	gpApplyShadowShader = LoadShader("../Resources/Shader/ApplyShadow.fx");
	gpCreateShadowShader = LoadShader("../Resources/Shader/CreateShadow.fx");
	// �� �ε�
	gpTorus = LoadModel("../Resources/Char/Dwarf/Dwarf.x");
	gpDisc = LoadModel("../Resources/Shader/disc.x");

	D3DXCreateFont(g_pD3DDevice,		//D3D Device
		40,								//Font height
		0,								//Font width
		FW_NORMAL,						//Font Weight
		1,								//MipLevels
		false,							//Italic
		DEFAULT_CHARSET,				//CharSet
		OUT_DEFAULT_PRECIS,				//OutputPrecision
		ANTIALIASED_QUALITY,			//Quality
		DEFAULT_PITCH | FF_DONTCARE,	//PitchAndFamily
		"Arial",						//pFacename,
		&m_pFont);						//ppFont

	SetRect(&m_recFontRect, 0, 0, 400, 200);
}

void cLoadingScene::Start(){
	DWORD dwThID;
	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoadNextScene, this, NULL/*CREATE_SUSPENDED*/, &dwThID));
}

void cLoadingScene::Update(float fDelta){
	m_fLoadingTime += fDelta;


}

void cLoadingScene::Render(){
	// ����-�� ����� �����.
	D3DXMATRIXA16 matLightView;
	{
		D3DXVECTOR3 vEyePt(gWorldLightPosition.x, gWorldLightPosition.y, gWorldLightPosition.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &vLookatPt, &vUpVec);
	}

	// ����-���� ����� �����.
	D3DXMATRIXA16 matLightProjection;
	{
		D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1280.0f / 720.0f, 1, 3000);
	}

	// ��/��������� �����.
	D3DXMATRIXA16 matViewProjection;
	{
		// �� ����� �����.
		D3DXMATRIXA16 matView;
		D3DXVECTOR3 vEyePt(gWorldCameraPosition.x, gWorldCameraPosition.y, gWorldCameraPosition.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

		// ��������� �����.
		D3DXMATRIXA16			matProjection;
		D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

		D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	}

	// ��ȯü�� ��������� �����.
	D3DXMATRIXA16			matTorusWorld;
	{
		// �����Ӹ��� 0.4���� ȸ���� ��Ų��.
		gRotationY += 0.4f * PI / 180.0f;
		if (gRotationY > 2 * PI)
		{
			gRotationY -= 2 * PI;
		}
		D3DXMATRIXA16 matScale, matRot, matTrans;
		D3DXMatrixScaling(&matScale, 50, 50, 50);
		D3DXMatrixRotationY(&matRot, gRotationY);
		D3DXMatrixTranslation(&matTrans, 0, -70, 0);
		matTorusWorld = matScale*matRot*matTrans;
	}

	// ��ũ�� ��������� �����.
	D3DXMATRIXA16			matDiscWorld;
	{
		D3DXMATRIXA16 matScale;
		D3DXMatrixScaling(&matScale, 2, 2, 2);

		D3DXMATRIXA16 matTrans;
		D3DXMatrixTranslation(&matTrans, 0, -40, 0);

		D3DXMatrixMultiply(&matDiscWorld, &matScale, &matTrans);
	}

	// ���� �ϵ���� �����ۿ� ���̹���

	g_pD3DDevice->GetRenderTarget(0, &pHWBackBuffer);
	g_pD3DDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. �׸��� �����
	//////////////////////////////

	// �׸��� ���� ����Ÿ��� ���̹��۸� ����Ѵ�.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if (SUCCEEDED(gpShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		g_pD3DDevice->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	g_pD3DDevice->SetDepthStencilSurface(gpShadowDepthStencil);

	// ���� �����ӿ� �׷ȴ� �׸��� ������ ����
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

	// �׸��� ����� ���̴� ������������ ����
	gpCreateShadowShader->SetMatrix("gWorldMatrix", &matTorusWorld);
	gpCreateShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	gpCreateShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	// �׸��� ����� ���̴��� ����
	{
		UINT numPasses = 0;
		gpCreateShadowShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				gpCreateShadowShader->BeginPass(i);
				{
					// ��ȯü�� �׸���.
					gpTorus->DrawSubset(0);
				}
				gpCreateShadowShader->EndPass();
			}
		}
		gpCreateShadowShader->End();
	}


	//////////////////////////////
	// 2. �׸��� ������
	//////////////////////////////

	// �ϵ���� �����/���̹��۸� ����Ѵ�.
	g_pD3DDevice->SetRenderTarget(0, pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), /*D3DCOLOR_XRGB(80, 80, 80)*/0x00000000, 1.0f, 0);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = NULL;


	// �׸��� ������ ���̴� ������������ ����
	gpApplyShadowShader->SetMatrix("gWorldMatrix", &matTorusWorld);	//��ȯü
	gpApplyShadowShader->SetMatrix("gViewProjectionMatrix", &matViewProjection);
	gpApplyShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	gpApplyShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	gpApplyShadowShader->SetVector("gWorldLightPosition", &gWorldLightPosition);

	gpApplyShadowShader->SetVector("gObjectColor", &gTorusColor);

	gpApplyShadowShader->SetTexture("ShadowMap_Tex", gpShadowRenderTarget);


	// ���̴��� �����Ѵ�.
	UINT numPasses = 0;
	gpApplyShadowShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			gpApplyShadowShader->BeginPass(i);
			{
				// ��ȯü�� �׸���.
				gpTorus->DrawSubset(0);

				// ��ũ�� �׸���.
				gpApplyShadowShader->SetMatrix("gWorldMatrix", &matDiscWorld);
				gpApplyShadowShader->SetVector("gObjectColor", &gDiscColor);
				gpApplyShadowShader->CommitChanges();
				gpDisc->DrawSubset(0);
			}
			gpApplyShadowShader->EndPass();
		}
	}
	gpApplyShadowShader->End();

	std::stringstream s;
	s.precision(2);
	s << "Loading..."  << std::fixed << m_fLoadingTime << "sec" << std::endl;
	m_pFont->DrawText(NULL,				 //pSprite
		s.str().c_str(),	 //pString
		-1,					//Count
		&m_recFontRect,		//pRect
		DT_LEFT | DT_NOCLIP,//Format,
		0xFFFFFFFF);		//Color

	if (m_bBoolWorkDone){
		// �ε��� ������ ���� ������ �ڵ� ��ȯ
		m_pDelegate->ChangeSceneFromLoader(m_pNextScene);
		return;
	}
}

void cLoadingScene::Exit(){

}

// XXX
// �ε��߿� â�� ����� ����� �޸𸮰� Ǯ���� �ʴ´�.
// ���� �Լ��� �߰� �߶� ���� ������ �ִ� ���� �ٶ��� �ҵ� �ϴ�.
void cLoadingScene::LoadNextScene(LPVOID pParam){
	cLoadingScene* pLoadingScene = (cLoadingScene*)pParam;
	EnterCriticalSection(&gCriticalSection);
	
	cSceneLoader SceneLoader;
	cScene* ret = SceneLoader.ParseScene(pLoadingScene->GetNextScenePath());
	ret->AddRef();

	if (ret){
		ret->SetDelegate(pLoadingScene->GetDelegate());
		ret->Start();
	}
	pLoadingScene->SetNextScene(ret);
	pLoadingScene->SetWorkDone(true);

	LeaveCriticalSection(&gCriticalSection);
}

// ���̴� �ε�
LPD3DXEFFECT LoadShader(const char * filename)
{
	LPD3DXEFFECT ret = NULL;

	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(g_pD3DDevice, filename,
		NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

	// ���̴� �ε��� ������ ��� outputâ�� ���̴�
	// ������ ������ ����Ѵ�.
	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	return ret;
}

// �� �ε�
LPD3DXMESH LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, g_pD3DDevice, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("�� �ε� ����: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	};

	return ret;
}

// �ؽ�ó �ε�
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice, filename, &ret)))
	{
		OutputDebugString("�ؽ�ó �ε� ����: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	}

	return ret;
}