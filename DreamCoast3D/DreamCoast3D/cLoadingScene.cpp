#include "stdafx.h"
#include "cLoadingScene.h"
#include "cSceneLoader.h"
#include <sstream>

#define PI           3.14159265f
#define FOV          ( D3DX_PI /4.0f)							// 시야각
#define ASPECT_RATIO (1280.0f/(float)720.0f)			// 화면의 종횡비
#define NEAR_PLANE   1									// 근접 평면
#define FAR_PLANE    3000								// 원거리 평면

static CRITICAL_SECTION				gCriticalSection;

//----------------------------------------------------------------------
// 전역변수
//----------------------------------------------------------------------

// 모델
LPD3DXMESH				gpTorus = NULL;
LPD3DXMESH				gpDisc = NULL;

// 쉐이더
LPD3DXEFFECT			gpApplyShadowShader = NULL;
LPD3DXEFFECT			gpCreateShadowShader = NULL;

// 회전값
float					gRotationY = 0.0f;

// 빛의 위치
D3DXVECTOR4				gWorldLightPosition(500.0f, 500.0f, -500.0f, 1.0f);

// 카메라 위치
D3DXVECTOR4				gWorldCameraPosition(0.0f, 0.0f, -200.0f, 1.0f);

// 물체의 색상
D3DXVECTOR4				gTorusColor(1, 1, 0, 1);
D3DXVECTOR4				gDiscColor(0.8, 0.8, 0.8, 1);

// 그림자맵 렌더타깃
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

	// 모델을 release 한다.
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

	// 쉐이더를 release 한다.
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

	// 텍스처를 release 한다.
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


	// 렌더타깃을 만든다.
	const int shadowMapSize = 2048;
	if (FAILED(g_pD3DDevice->CreateTexture(shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &gpShadowRenderTarget, NULL)))
	{
	}

	// 그림자 맵과 동일한 크기의 깊이버퍼도 만들어줘야 한다.
	if (FAILED(g_pD3DDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&gpShadowDepthStencil, NULL)))
	{
	}

	gpApplyShadowShader = LoadShader("../Resources/Shader/ApplyShadow.fx");
	gpCreateShadowShader = LoadShader("../Resources/Shader/CreateShadow.fx");
	// 모델 로딩
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
	// 광원-뷰 행렬을 만든다.
	D3DXMATRIXA16 matLightView;
	{
		D3DXVECTOR3 vEyePt(gWorldLightPosition.x, gWorldLightPosition.y, gWorldLightPosition.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &vLookatPt, &vUpVec);
	}

	// 광원-투영 행렬을 만든다.
	D3DXMATRIXA16 matLightProjection;
	{
		D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1280.0f / 720.0f, 1, 3000);
	}

	// 뷰/투영행렬을 만든다.
	D3DXMATRIXA16 matViewProjection;
	{
		// 뷰 행렬을 만든다.
		D3DXMATRIXA16 matView;
		D3DXVECTOR3 vEyePt(gWorldCameraPosition.x, gWorldCameraPosition.y, gWorldCameraPosition.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

		// 투영행렬을 만든다.
		D3DXMATRIXA16			matProjection;
		D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

		D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	}

	// 원환체의 월드행렬을 만든다.
	D3DXMATRIXA16			matTorusWorld;
	{
		// 프레임마다 0.4도씩 회전을 시킨다.
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

	// 디스크의 월드행렬을 만든다.
	D3DXMATRIXA16			matDiscWorld;
	{
		D3DXMATRIXA16 matScale;
		D3DXMatrixScaling(&matScale, 2, 2, 2);

		D3DXMATRIXA16 matTrans;
		D3DXMatrixTranslation(&matTrans, 0, -40, 0);

		D3DXMatrixMultiply(&matDiscWorld, &matScale, &matTrans);
	}

	// 현재 하드웨어 벡버퍼와 깊이버퍼

	g_pD3DDevice->GetRenderTarget(0, &pHWBackBuffer);
	g_pD3DDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. 그림자 만들기
	//////////////////////////////

	// 그림자 맵의 렌더타깃과 깊이버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if (SUCCEEDED(gpShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		g_pD3DDevice->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	g_pD3DDevice->SetDepthStencilSurface(gpShadowDepthStencil);

	// 저번 프레임에 그렸던 그림자 정보를 지움
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

	// 그림자 만들기 쉐이더 전역변수들을 설정
	gpCreateShadowShader->SetMatrix("gWorldMatrix", &matTorusWorld);
	gpCreateShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	gpCreateShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	// 그림자 만들기 쉐이더를 시작
	{
		UINT numPasses = 0;
		gpCreateShadowShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				gpCreateShadowShader->BeginPass(i);
				{
					// 원환체를 그린다.
					gpTorus->DrawSubset(0);
				}
				gpCreateShadowShader->EndPass();
			}
		}
		gpCreateShadowShader->End();
	}


	//////////////////////////////
	// 2. 그림자 입히기
	//////////////////////////////

	// 하드웨어 백버퍼/깊이버퍼를 사용한다.
	g_pD3DDevice->SetRenderTarget(0, pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), /*D3DCOLOR_XRGB(80, 80, 80)*/0x00000000, 1.0f, 0);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = NULL;


	// 그림자 입히기 쉐이더 전역변수들을 설정
	gpApplyShadowShader->SetMatrix("gWorldMatrix", &matTorusWorld);	//원환체
	gpApplyShadowShader->SetMatrix("gViewProjectionMatrix", &matViewProjection);
	gpApplyShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	gpApplyShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	gpApplyShadowShader->SetVector("gWorldLightPosition", &gWorldLightPosition);

	gpApplyShadowShader->SetVector("gObjectColor", &gTorusColor);

	gpApplyShadowShader->SetTexture("ShadowMap_Tex", gpShadowRenderTarget);


	// 쉐이더를 시작한다.
	UINT numPasses = 0;
	gpApplyShadowShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			gpApplyShadowShader->BeginPass(i);
			{
				// 원환체를 그린다.
				gpTorus->DrawSubset(0);

				// 디스크를 그린다.
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
		// 로딩이 종료후 다음 씬으로 자동 전환
		m_pDelegate->ChangeSceneFromLoader(m_pNextScene);
		return;
	}
}

void cLoadingScene::Exit(){

}

// XXX
// 로딩중에 창을 종료시 제대로 메모리가 풀리지 않는다.
// 좀더 함수를 잘게 잘라서 종료 조건을 넣는 것이 바람직 할듯 하다.
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

// 쉐이더 로딩
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

	// 쉐이더 로딩에 실패한 경우 output창에 쉐이더
	// 컴파일 에러를 출력한다.
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

// 모델 로딩
LPD3DXMESH LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, g_pD3DDevice, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("모델 로딩 실패: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	};

	return ret;
}

// 텍스처 로딩
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice, filename, &ret)))
	{
		OutputDebugString("텍스처 로딩 실패: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	}

	return ret;
}