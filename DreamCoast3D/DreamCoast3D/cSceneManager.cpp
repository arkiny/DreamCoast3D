#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"
#include "cSceneExample.h"
#include "cSceneLoader.h"
#include "cLoadingScene.h"

cSceneManager::cSceneManager()
	:m_pCurrentScene(NULL)
{
}

cSceneManager::~cSceneManager()
{
	SAFE_RELEASE(m_pCurrentScene);
}

void cSceneManager::Setup(std::string sFolder, std::string sFile){	

}

void cSceneManager::Update(float delta){
	// 업데이트는 여기서
	//for (auto p : m_setScenes){
	if (m_pCurrentScene){
		m_pCurrentScene->Update(delta);
	}

	if (m_bIsLoading == false){
		if (GetAsyncKeyState('1') /*&& m_pCurrentScene != m_vecScenes[0]*/){
			ChangeScene(0);
			return;
		}
		if (GetAsyncKeyState('2') /*&& m_pCurrentScene != m_vecScenes[1]*/){
			ChangeScene(1);
			return;
		}
	}
	//}
}

void cSceneManager::Start(){
	// 일단 첫씬 0번은 미리 로딩
	cSceneLoader SceneLoader;
	if (!m_vecSceneInfoFilePath.empty()){
		ChangeScene(0);
	}
}


void cSceneManager::AddSceneFilePath(std::string sFilePath){
	m_vecSceneInfoFilePath.push_back(sFilePath);
}

void cSceneManager::Render(){
	// 여기서 현재씬을 렌더한다. 각씬이 오브젝트들을 가지고 렌더한다.
	SAFE_RENDER(m_pCurrentScene);	
}

void cSceneManager::Destroy(){
	this->Release();
}

// TODO : 해당 씬이 종료되었을때 행동 양식
void cSceneManager::SceneFinished(cScene* pSender){
	if (pSender->GetNextScene()){
		m_pCurrentScene = pSender->GetNextScene();
	}
}

void cSceneManager::ChangeScene(int nNextSceneIndex){
	assert(nNextSceneIndex < m_vecSceneInfoFilePath.size());
	m_bIsLoading = true;
	if (m_pCurrentScene){
		m_pCurrentScene->Exit();
		SAFE_RELEASE(m_pCurrentScene);
		g_pSkinnedMeshManager->Destroy();
		g_pAseManager->Destroy();
		g_pTextureManager->Destroy();
	}
	m_pCurrentScene = new cLoadingScene;
	m_pCurrentScene->Setup(m_vecSceneInfoFilePath[nNextSceneIndex]);

	// 리소스 로딩
	m_pCurrentScene->SetDelegate(this);
	m_pCurrentScene->Start();
}

void cSceneManager::ChangeSceneFromLoader(cScene* pNextScene){
	m_pCurrentScene->Exit();
	SAFE_RELEASE(m_pCurrentScene);

	m_pCurrentScene = pNextScene;
	m_pCurrentScene->SetDelegate(this);
	m_bIsLoading = false;
}