#include "stdafx.h"
#include "cSceneExample.h"
#include "cLightSource.h"
#include "cGameObjManager.h"
#include "cTransform.h"

#include "cAction.h"
#include "cActionMove.h"
#include "cActionRepeat.h"
#include "cActionSeq.h"

#include "cSphere.h"
#include "cSphereAction.h"
#include "cBillingExample.h"


cSceneExample::cSceneExample()
	:cScene() // 부모클래스 초기화
{
}

cSceneExample::~cSceneExample()
{
}

void cSceneExample::Setup(std::string sFilePath){
	cScene::Setup(sFilePath);

	//1. 각 씬은 광원이 다를 수 있으므로 상속 받은 곳에서 설정한다.
	//@HACK: LightSource 매니저를 따로 뽑을 건지 고민
	D3DLIGHT9 stLight;
	D3DXVECTOR3 vDir = D3DXVECTOR3(1.5, -1, 1);

	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	stLight.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	cLightSource* pLightSource = new cLightSource;
	pLightSource->Setup(stLight, vDir, m_vecLightSources.size());
	pLightSource->SetDesc("Light Source Example1");
	m_vecLightSources.push_back(pLightSource);

	// @TODO: 차후 각 씬은 sFilePath에서 로딩할 오브젝트 정보를 받아와서 로딩하도록 한다.
	// 로딩화면 처리는 음.... 나중에 생각해보자

	// 게임 오브젝트 추가 예시
	// 2. 각 씬은 각자 다른 오브젝트를 가지고 있을수 있으므로, 상속받은 클래스에서 로딩한다.
	cSphere* pGameObj = new cSphere;
	pGameObj->Setup();
	pGameObj->SetDesc(std::string("Sphere: GameObject Example1"));
	m_pGameObjManager->AddGameObj(pGameObj);
	// AddGameObj 함수에서 리퍼런스를 추가해주므로 바로 릴리즈
	// AddGameObj 함수에서 AddRef가 호출되므로 인스턴스는 사라지지 않는다.
	SAFE_RELEASE(pGameObj);

	// 게임 액션 오브젝트 추가 예시 // 차후 파일이나 스크립트로 AI 확정시 사용
	cSphereAction* pActionSphere = new cSphereAction;
	pActionSphere->Setup();
	pActionSphere->SetDesc("SphereAction: GameActionObject Example1");
	
	// 시퀀스가 반복될때 사용
	cActionRepeat* pActionRepeat = new cActionRepeat;
	// 모든 액션들은 시퀀스에 따라서 움직인다.
	cActionSeq* pActionSeq = new cActionSeq;
	// 이동 액션
	cActionMove* pActionMove = new cActionMove;
	pActionMove->SetActionTime(1.0f);
	pActionMove->SetDelegate(pActionSeq); // 델리게이트는 시퀀스가 관리하므로 시퀀스에서 처리
	D3DXVECTOR3 from = pActionSphere->GetTransform()->GetPosition(); 
	pActionMove->SetFrom(from);					
	pActionMove->SetTo(D3DXVECTOR3(from.x, from.y, from.z + 2.0f));
	pActionMove->SetOwner(pActionSphere);	// 오너는 액션 스피어
	pActionSeq->AddAction(pActionMove);		// 첫번째 이동액션을 시퀀스에 저장
	SAFE_RELEASE(pActionMove);

	pActionMove = new cActionMove;
	pActionMove->SetActionTime(1.0f);
	pActionMove->SetDelegate(pActionSeq);
	pActionMove->SetFrom(D3DXVECTOR3(from.x, from.y, from.z + 2.0f));
	pActionMove->SetTo(from);
	pActionMove->SetOwner(pActionSphere);
	pActionSeq->AddAction(pActionMove);		// 두번째 이동액션을 시퀀스에 저장
	SAFE_RELEASE(pActionMove);

	pActionRepeat->SetAction(pActionSeq);	// 이 시퀀스는 반복되므로 반복액션에 저장
	SAFE_RELEASE(pActionSeq);
	
	pActionSphere->SetAction(pActionRepeat);
	pActionRepeat->Start();					// 액션 시작
	SAFE_RELEASE(pActionRepeat);

	m_pGameObjManager->AddGameObj(pActionSphere);
	SAFE_RELEASE(pActionSphere);

	cBillingExample* pBillingExample = new cBillingExample;
	pBillingExample->Setup();
	pBillingExample->SetDesc("BillBoarding Object Example");
	m_pGameObjManager->AddGameObj(pBillingExample);
	SAFE_RELEASE(pBillingExample);
}

void cSceneExample::Update(float delta){
	cScene::Update(delta);
}

void cSceneExample::Render(){
	cScene::Render();
}