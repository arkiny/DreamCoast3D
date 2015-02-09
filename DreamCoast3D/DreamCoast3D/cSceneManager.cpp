#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"


cSceneManager::cSceneManager()
	:m_pCurrentScene(NULL)
{
}


cSceneManager::~cSceneManager()
{
	cDeviceManager* pDevice = cDeviceManager::GetInstance();
	pDevice->Destroy();
}

void cSceneManager::Setup(std::string sFilePath){
	// @TODO : 프레임웤 테스트용
	cScene* pScene = new cScene;
	pScene->Setup(std::string(""));
	pScene->SetDesc(std::string("Scene Test1"));

	// 씬 종료후 다음 씬 설정
	pScene->SetNextScene(NULL);

	m_vecScenes.push_back(pScene);
	m_pCurrentScene = pScene;

}

void cSceneManager::Update(float delta){
	// 업데이트는 여기서
	for (auto p : m_vecScenes){
		p->Update(delta);
	}
}

void cSceneManager::Render(){
	// 여기서 씬을 렌더한다. 각씬이 오브젝트들을 가지고 렌더한다.
	for (auto p : m_vecScenes){
		SAFE_RENDER(p);
	}
}

void cSceneManager::Destroy(){
	// 여기서 가지고 있는 모든 씬들을 릴리즈해준다
	for (auto p : m_vecScenes){
		SAFE_RELEASE(p);
	}
	this->Release();
}

//@TODO: 해당 씬이 종료되었을때 행동 양식
void cSceneManager::SceneFinished(cScene* pSender){
	m_pCurrentScene = pSender->GetNextScene();
}