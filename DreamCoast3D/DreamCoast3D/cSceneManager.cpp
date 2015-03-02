#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"
#include "cSceneExample.h"
#include "cSceneLoader.h"

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
	// ������Ʈ�� ���⼭
	//for (auto p : m_setScenes){
	if (m_pCurrentScene){
		m_pCurrentScene->Update(delta);
	}

	if (GetAsyncKeyState('1') /*&& m_pCurrentScene != m_vecScenes[0]*/){
		ChangeScene(0);
		return;
	}
	if (GetAsyncKeyState('2') /*&& m_pCurrentScene != m_vecScenes[1]*/){
		ChangeScene(1);
		return;
	}
	//}
}

void cSceneManager::Start(){
	cSceneLoader SceneLoader;

	if (!m_vecSceneInfoFilePath.empty()){
		m_pCurrentScene = SceneLoader.ParseScene(m_vecSceneInfoFilePath[0]);
		m_pCurrentScene->Start();
	}
}


void cSceneManager::AddSceneFilePath(std::string sFilePath){
	m_vecSceneInfoFilePath.push_back(sFilePath);
}

void cSceneManager::Render(){
	// ���⼭ ������� �����Ѵ�. ������ ������Ʈ���� ������ �����Ѵ�.
	SAFE_RENDER(m_pCurrentScene);	
}

void cSceneManager::Destroy(){
	this->Release();
}

// TODO : �ش� ���� ����Ǿ����� �ൿ ���
void cSceneManager::SceneFinished(cScene* pSender){
	if (pSender->GetNextScene()){
		m_pCurrentScene = pSender->GetNextScene();
	}
}

void cSceneManager::ChangeScene(int nNextSceneIndex){
	assert(nNextSceneIndex < m_vecSceneInfoFilePath.size());
	m_pCurrentScene->Exit();
	SAFE_RELEASE(m_pCurrentScene);
	g_pSkinnedMeshManager->Destroy();
	g_pAseManager->Destroy();
	g_pTextureManager->Destroy();

	cSceneLoader SceneLoader;
	m_pCurrentScene = SceneLoader.ParseScene(m_vecSceneInfoFilePath[nNextSceneIndex]);

	// ���ҽ� �ε�
	m_pCurrentScene->Start();
}