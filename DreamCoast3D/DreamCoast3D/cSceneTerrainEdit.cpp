#include "stdafx.h"
#include "cSceneTerrainEdit.h"
#include "cHeightMapTerrainEdit.h"
#include "cMousePicking.h"
#include "cEffectManager.h"
#include "cLightSource.h"
#include "cGameObjManager.h"
#include "cUIObjManager.h"
#include "cTransform.h"
#include "cUILoader.h"
#include "cEffectManager.h"
#include "cMousePicking.h"
#include "cCamera.h"
#include <sstream>

cSceneTerrainEdit::cSceneTerrainEdit()
	:m_pMousPicking(NULL)
{
}


cSceneTerrainEdit::~cSceneTerrainEdit()
{
	//SAFE_RELEASE(m_pCurrentMap);
	SAFE_DELETE(m_pMousPicking);

}


void cSceneTerrainEdit::Setup(std::string sFilePath){
	cScene::Setup(sFilePath);
	m_pMousPicking = new cMousePicking;
	m_pMousPicking->SetEffectDelegate(m_pEffectManager);
}

// Start에서 리소스를 로딩한다.
void cSceneTerrainEdit::Start(){
	cUILoader cUL;
	cUL.LoadGameUIFromFile(this->GetUIObjMng(), m_sGameUIPath);
	
	m_pCurrentMap = new cHeightMapTerrainEdit;
	m_pCurrentMap->Setup();
}

void cSceneTerrainEdit::Update(float delta){
	if (m_pCamera){
		m_pCamera->Update(delta);
	}
	if (m_pCurrentMap){
		m_pCurrentMap->Update(delta);
	}	
	if (m_pMousPicking && m_pCurrentMap){
		m_pMousPicking->SetVertex(m_pCurrentMap->GetVertexVectorByRef());
		m_pMousPicking->Update();
	}
	if (m_pEffectManager){
		m_pEffectManager->Update(delta);
	}
}

void cSceneTerrainEdit::Render(){
	if (m_pCurrentMap){
		m_pCurrentMap->Render();
	}
	if (m_pEffectManager){
		m_pEffectManager->Render();
	}
}

void cSceneTerrainEdit::Exit(){

}