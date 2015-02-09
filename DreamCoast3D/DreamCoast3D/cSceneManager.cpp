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
	// @TODO : ���Ŵ��� ����, ���� ����
	// cScene�� ��ӹ��� cSceneExample�� �̿��� ȭ���� ���� �������.
	cSceneExample* pScene = new cSceneExample;
	pScene->Setup(std::string(""));
	pScene->SetDesc(std::string("Scene Example1"));

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
	// ���⼭ ������� �����Ѵ�. ������ ������Ʈ���� ������ �����Ѵ�.
	SAFE_RENDER(m_pCurrentScene);	
}

void cSceneManager::Destroy(){
	// ���⼭ ������ �ִ� ��� ������ ���������ش�
	for (auto p : m_vecScenes){
		p->Destroy();
	}
	this->Release();
}

//@TODO: �ش� ���� ����Ǿ����� �ൿ ���
void cSceneManager::SceneFinished(cScene* pSender){
	if (pSender->GetNextScene()){
		m_pCurrentScene = pSender->GetNextScene();
	}
}