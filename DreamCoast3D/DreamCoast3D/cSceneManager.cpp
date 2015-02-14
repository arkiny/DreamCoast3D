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
	//  TODO : ���Ŵ��� ����, ���� ����
	// cScene�� ��ӹ��� cSceneExample�� �̿��� ȭ���� ���� �������.
	// cSceneExample* pScene = new cSceneExample;
	// pScene->Setup(std::string(""));
	// pScene->SetDesc(std::string("Scene Example1"));
	// �� ������ ���� �� ����
	// pScene->SetNextScene(NULL);
	// m_setScenes.insert(pScene);
	// m_pCurrentScene = pScene;
	m_pCurrentScene = *m_setScenes.begin();
}

void cSceneManager::Update(float delta){
	// ������Ʈ�� ���⼭
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
	// ���⼭ ������� �����Ѵ�. ������ ������Ʈ���� ������ �����Ѵ�.
	SAFE_RENDER(m_pCurrentScene);	
}

void cSceneManager::Destroy(){
	// ���⼭ ������ �ִ� ��� ������ ���������ش�
	for (auto p : m_setScenes){
		p->Destroy();
	}
	this->Release();
}

// TODO : �ش� ���� ����Ǿ����� �ൿ ���
void cSceneManager::SceneFinished(cScene* pSender){
	if (pSender->GetNextScene()){
		m_pCurrentScene = pSender->GetNextScene();
	}
}