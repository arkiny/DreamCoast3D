#include "stdafx.h"
#include "cUIWindow.h"


cUIWindow::cUIWindow()
	: m_pUIRoot(NULL)
{
}

cUIWindow::~cUIWindow()
{
	SAFE_RELEASE(m_pSprite);
}

void cUIWindow::Setup(){

}
void cUIWindow::Update(float fDelta){

}
void cUIWindow::Render(){

}

void cUIWindow::SetUIManagerDeligate(iUIManagerDeligate* pUImanagerDeligate){
	for (auto p : m_vecChild){
		p->SetUIManagerDeligate(pUImanagerDeligate);
	}
	m_pUIManagerDeligate = pUImanagerDeligate;
	if (m_pUIRoot){
		m_pUIRoot->SetUIManagerDeligate(pUImanagerDeligate);
	}
}