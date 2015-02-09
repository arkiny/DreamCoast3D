#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"
#include "cSceneExample.h"

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
	// @TODO : 씬매니저 예시, 차후 삭제
	// cScene을 상속받은 cSceneExample을 이용해 화면을 새로 만들었다.
	cSceneExample* pScene = new cSceneExample;
	pScene->Setup(std::string(""));
	pScene->SetDesc(std::string("Scene Example1"));

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
	// 여기서 현재씬을 렌더한다. 각씬이 오브젝트들을 가지고 렌더한다.
	SAFE_RENDER(m_pCurrentScene);	
}

void cSceneManager::Destroy(){
	// 여기서 가지고 있는 모든 씬들을 릴리즈해준다
	for (auto p : m_vecScenes){
		p->Destroy();
	}
	this->Release();
}

//@TODO: 해당 씬이 종료되었을때 행동 양식
void cSceneManager::SceneFinished(cScene* pSender){
	if (pSender->GetNextScene()){
		m_pCurrentScene = pSender->GetNextScene();
	}
}