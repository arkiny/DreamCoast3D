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