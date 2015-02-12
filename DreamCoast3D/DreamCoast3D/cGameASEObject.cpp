#include "stdafx.h"
#include "cGameASEObject.h"
#include "cASEInstance.h"


cGameASEObject::cGameASEObject() 
	: m_pASEInstance(NULL)
{
}


cGameASEObject::~cGameASEObject()
{
}

void cGameASEObject::Setup(std::string sFolder, std::string sFile){
	m_pASEInstance = g_pAseManager->GetAseObject(sFolder, sFile);
}

void cGameASEObject::Update(float fDelta){

}

void cGameASEObject::Render(){
	m_pASEInstance->Render(GetTransformMatrix());
}