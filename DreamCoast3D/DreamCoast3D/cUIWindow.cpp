#include "stdafx.h"
#include "cUIWindow.h"


cUIWindow::cUIWindow()
	: m_pSprite(NULL),
	  m_pUIRoot(NULL)
{
}


cUIWindow::~cUIWindow()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUIWindow::Setup(){

}
void cUIWindow::Update(float fDelta){

}
void cUIWindow::Render(){

}