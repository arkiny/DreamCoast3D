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

#include "cMapExample.h"

#include "cUIExample.h"
#include "cZealot.h"

#include "cGameObjLoader.h"
#include "cMapLoader.h"

#include "cGameASEObject.h"



cSceneExample::cSceneExample()
	:cScene() // 베이스클래스 초기화
{
}

cSceneExample::~cSceneExample()
{
}

void cSceneExample::Setup(std::string sFilePath){
	cScene::Setup(sFilePath);

	// TODO : 차후 각 씬은 sFilePath에서 로딩할 오브젝트 정보를 받아와서 로딩하도록 한다.
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

	// 3. 맵 로딩

	//cMapExample* pGameMapExample = new cMapExample;
	//pGameMapExample->Setup(30, 1);
	//cScene::AddMap(pGameMapExample);

	cUIExample* pUIExample = new cUIExample;
	pUIExample->Setup();
	//pUIExample->SetPosition(D3DXVECTOR3(0, 0, 0));
	cScene::AddUIObj(pUIExample);
	SAFE_RELEASE(pUIExample);


	//cGameASEObject* pAseObject = new cGameASEObject;
	//pAseObject->Setup(std::string("../Resources/Building/"), std::string("building.ASE"));
	//m_pGameObjManager->AddGameObj(pAseObject);
	//pAseObject->SetPosition(D3DXVECTOR3(100, 0, 100));
	//pAseObject->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));

	//pAseObject->GetPosition();

	//SAFE_RELEASE(pAseObject);


	//pAseObject = new cGameASEObject;
	//pAseObject->Setup(std::string("../Resources/Building/"), std::string("building.ASE"));
	//m_pGameObjManager->AddGameObj(pAseObject);
	//pAseObject->SetPosition(D3DXVECTOR3(150, 0, 150));
	//pAseObject->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	//SAFE_RELEASE(pAseObject);


	//// 메쉬 오브젝트
	//cZealot* pZealotExample = new cZealot;
	//pZealotExample->Setup(std::string("../Resources/Char/Zealot/"), std::string("zealot.X"));
	//pZealotExample->SetAnimationLoop(0, false);
	//pZealotExample->SetAnimationLoop(1, false);
	//pZealotExample->SetAnimationLoop(2, false);
	//pZealotExample->SetAnimationLoop(3, true);
	//pZealotExample->SetAnimationLoop(4, true);
	//pZealotExample->SetAnimation(4);
	//m_pGameObjManager->AddGameObj(pZealotExample);
	//SAFE_RELEASE(pZealotExample);

	// 메쉬 오브젝트
	/*cZealot* pZealotExample = new cZealot;
	pZealotExample = new cZealot;
	pZealotExample->Setup(std::string("../Resources/Char/Zealot/"), std::string("zealot.X"));
	pZealotExample->SetAnimationLoop(0, false);
	pZealotExample->SetAnimationLoop(1, false);
	pZealotExample->SetAnimationLoop(2, false);
	pZealotExample->SetAnimationLoop(3, true);
	pZealotExample->SetAnimationLoop(4, true);
	pZealotExample->SetAnimation(4);
	pZealotExample->SetPosition(D3DXVECTOR3(-4.0f, 0.0f, 2.0f));
	m_pGameObjManager->AddGameObj(pZealotExample);
	SAFE_RELEASE(pZealotExample);*/

	cGameObjLoader cGOL;
	cGOL.LoadGameObjectsFromFile(m_pGameObjManager, std::string("../Resources/"), std::string("SCENE1_GAMEOBJDATA.txt"));

	cMapLoader cML;
	cML.LoadGameMapFromFile(this, std::string("../Resources/"), std::string("SCENE1_MAPDATA.txt"));
	SetCurrentMap(m_vecGameMaps.size()-1);
}

void cSceneExample::Update(float delta){
	cScene::Update(delta);
}

void cSceneExample::Render(){
	cScene::Render();
}