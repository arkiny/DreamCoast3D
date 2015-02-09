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
	// @TODO : �����ӟp �׽�Ʈ��
	cScene* pScene = new cScene;
	pScene->Setup(std::string(""));
	pScene->SetDesc(std::string("Scene Test1"));

	// �� ������ ���� �� ����
	pScene->SetNextScene(NULL);

	m_vecScenes.push_back(pScene);
	m_pCurrentScene = pScene;

}

void cSceneManager::Update(float delta){
	// ������Ʈ�� ���⼭
	for (auto p : m_vecScenes){
		p->Update(delta);
	}
}

void cSceneManager::Render(){
	// ���⼭ ���� �����Ѵ�. ������ ������Ʈ���� ������ �����Ѵ�.
	for (auto p : m_vecScenes){
		SAFE_RENDER(p);
	}
}

void cSceneManager::Destroy(){
	// ���⼭ ������ �ִ� ��� ������ ���������ش�
	for (auto p : m_vecScenes){
		SAFE_RELEASE(p);
	}
	this->Release();
}

//@TODO: �ش� ���� ����Ǿ����� �ൿ ���
void cSceneManager::SceneFinished(cScene* pSender){
	m_pCurrentScene = pSender->GetNextScene();
}