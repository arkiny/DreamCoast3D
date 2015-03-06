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
	// ������Ʈ�� ���⼭
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
		if (GetAsyncKeyState('3') /*UI����� �� ��:�ο�*/){
			ChangeScene(2);
			return;
		}
		if (GetAsyncKeyState('4')){ // Edit����� ��
			ChangeScene(3);
			return;
		}
		if (GetAsyncKeyState('5')){ // �������� ��
			ChangeScene(4);
			return;
		}
		if (GetAsyncKeyState('9')){ // �������� ��
			ChangeScene(5);
			return;
		}
	}
	//}
}

void cSceneManager::Start(){
	// �ϴ� ù�� 0���� �̸� �ε�
	cSceneLoader SceneLoader;
	if (!m_vecSceneInfoFilePath.empty()){
		ChangeScene(0);
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

	// ���ҽ� �ε�
	m_pCurrentScene->SetDelegate(this);
	m_pCurrentScene->Start();
}

void cSceneManager::ChangeSceneFromLoader(cScene* pNextScene){
	m_pCurrentScene->Exit();
	SAFE_RELEASE(m_pCurrentScene);

	m_pCurrentScene = pNextScene;
	m_bIsLoading = false;
}