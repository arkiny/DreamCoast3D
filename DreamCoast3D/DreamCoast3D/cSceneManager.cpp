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
	
}

void cSceneManager::Setup(std::string sFolder, std::string sFile){	
	//  TODO : 씬매니저 예시, 차후 삭제
	// cScene을 상속받은 cSceneExample을 이용해 화면을 새로 만들었다.
	// cSceneExample* pScene = new cSceneExample;
	// pScene->Setup(std::string(""));
	// pScene->SetDesc(std::string("Scene Example1"));
	// 씬 종료후 다음 씬 설정
	// pScene->SetNextScene(NULL);
	// m_setScenes.insert(pScene);
	// m_pCurrentScene = pScene;
	m_pCurrentScene = *m_setScenes.begin();
}

void cSceneManager::Update(float delta){
	// 업데이트는 여기서
	//for (auto p : m_setScenes){
	if (m_pCurrentScene){
		m_pCurrentScene->Update(delta);
	}
	//}
}

void cSceneManager::Start(){
	m_pCurrentScene->Start();
}

void cSceneManager::AddScene(cScene* pScene){
	if (pScene){
		if (m_setScenes.find(pScene) == m_setScenes.end()){
			SAFE_ADD_REF(pScene);
			m_setScenes.insert(pScene);
		}
	}
}

void cSceneManager::Render(){
	// 여기서 현재씬을 렌더한다. 각씬이 오브젝트들을 가지고 렌더한다.
	SAFE_RENDER(m_pCurrentScene);	
}

void cSceneManager::Destroy(){
	// 여기서 가지고 있는 모든 씬들을 릴리즈해준다
	for (auto p : m_setScenes){
		p->Destroy();
	}
	this->Release();
}

// TODO : 해당 씬이 종료되었을때 행동 양식
void cSceneManager::SceneFinished(cScene* pSender){
	if (pSender->GetNextScene()){
		m_pCurrentScene = pSender->GetNextScene();
	}
}