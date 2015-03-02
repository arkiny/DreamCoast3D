#include "stdafx.h"
#include "cLoadingScene.h"
#include "cSceneLoader.h"
#include <sstream>

static CRITICAL_SECTION				gCriticalSection;

cLoadingScene::cLoadingScene()
	:m_pFont(NULL)
{
	InitializeCriticalSection(&gCriticalSection);
}


cLoadingScene::~cLoadingScene()
{
	WaitForSingleObject(LoadNextScene, INFINITE);
	DeleteCriticalSection(&gCriticalSection);
	SAFE_RELEASE(m_pFont);
	if (m_pNextScene->GetRefCount() > 1){
		SAFE_RELEASE(m_pNextScene);
	}
}

void cLoadingScene::Setup(std::string sNextScene){
	m_sNextScenePath = sNextScene;

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

	if (m_bBoolWorkDone){
		// 로딩이 종료후 다음 씬으로 자동 전환
		m_pDelegate->ChangeSceneFromLoader(m_pNextScene);
		return;
	}
}

void cLoadingScene::Render(){
	std::stringstream s;
	s.precision(2);
	s << m_fLoadingTime << "sec" << std::endl;
	m_pFont->DrawText(NULL,				 //pSprite
		s.str().c_str(),	 //pString
		-1,					//Count
		&m_recFontRect,		//pRect
		DT_LEFT | DT_NOCLIP,//Format,
		0xFFFFFFFF);		//Color
}

void cLoadingScene::Exit(){

}

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