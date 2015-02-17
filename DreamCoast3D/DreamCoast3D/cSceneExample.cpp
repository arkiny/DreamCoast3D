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
	:cScene() // ���̽�Ŭ���� �ʱ�ȭ
{
}

cSceneExample::~cSceneExample()
{
}

void cSceneExample::Setup(std::string sFilePath){
	cScene::Setup(sFilePath);

	// TODO : ���� �� ���� sFilePath���� �ε��� ������Ʈ ������ �޾ƿͼ� �ε��ϵ��� �Ѵ�.
	// �ε�ȭ�� ó���� ��.... ���߿� �����غ���

	// ���� ������Ʈ �߰� ����
	// 2. �� ���� ���� �ٸ� ������Ʈ�� ������ ������ �����Ƿ�, ��ӹ��� Ŭ�������� �ε��Ѵ�.
	cSphere* pGameObj = new cSphere;
	pGameObj->Setup();
	pGameObj->SetDesc(std::string("Sphere: GameObject Example1"));
	m_pGameObjManager->AddGameObj(pGameObj);
	// AddGameObj �Լ����� ���۷����� �߰����ֹǷ� �ٷ� ������
	// AddGameObj �Լ����� AddRef�� ȣ��ǹǷ� �ν��Ͻ��� ������� �ʴ´�.
	SAFE_RELEASE(pGameObj);

	// ���� �׼� ������Ʈ �߰� ���� // ���� �����̳� ��ũ��Ʈ�� AI Ȯ���� ���
	cSphereAction* pActionSphere = new cSphereAction;
	pActionSphere->Setup();
	pActionSphere->SetDesc("SphereAction: GameActionObject Example1");
	
	// �������� �ݺ��ɶ� ���
	cActionRepeat* pActionRepeat = new cActionRepeat;
	// ��� �׼ǵ��� �������� ���� �����δ�.
	cActionSeq* pActionSeq = new cActionSeq;
	// �̵� �׼�
	cActionMove* pActionMove = new cActionMove;
	pActionMove->SetActionTime(1.0f);
	pActionMove->SetDelegate(pActionSeq); // ��������Ʈ�� �������� �����ϹǷ� ���������� ó��
	D3DXVECTOR3 from = pActionSphere->GetTransform()->GetPosition(); 
	pActionMove->SetFrom(from);					
	pActionMove->SetTo(D3DXVECTOR3(from.x, from.y, from.z + 2.0f));
	pActionMove->SetOwner(pActionSphere);	// ���ʴ� �׼� ���Ǿ�
	pActionSeq->AddAction(pActionMove);		// ù��° �̵��׼��� �������� ����
	SAFE_RELEASE(pActionMove);

	pActionMove = new cActionMove;
	pActionMove->SetActionTime(1.0f);
	pActionMove->SetDelegate(pActionSeq);
	pActionMove->SetFrom(D3DXVECTOR3(from.x, from.y, from.z + 2.0f));
	pActionMove->SetTo(from);
	pActionMove->SetOwner(pActionSphere);
	pActionSeq->AddAction(pActionMove);		// �ι�° �̵��׼��� �������� ����
	SAFE_RELEASE(pActionMove);

	pActionRepeat->SetAction(pActionSeq);	// �� �������� �ݺ��ǹǷ� �ݺ��׼ǿ� ����
	SAFE_RELEASE(pActionSeq);
	
	pActionSphere->SetAction(pActionRepeat);
	pActionRepeat->Start();					// �׼� ����
	SAFE_RELEASE(pActionRepeat);

	m_pGameObjManager->AddGameObj(pActionSphere);
	SAFE_RELEASE(pActionSphere);

	cBillingExample* pBillingExample = new cBillingExample;
	pBillingExample->Setup();
	pBillingExample->SetDesc("BillBoarding Object Example");
	m_pGameObjManager->AddGameObj(pBillingExample);
	SAFE_RELEASE(pBillingExample);

	// 3. �� �ε�

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

	//SAFE_RELEASE(pAseObject);


	//pAseObject = new cGameASEObject;
	//pAseObject->Setup(std::string("../Resources/Building/"), std::string("building.ASE"));
	//m_pGameObjManager->AddGameObj(pAseObject);
	//pAseObject->SetPosition(D3DXVECTOR3(150, 0, 150));
	//pAseObject->SetScale(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	//SAFE_RELEASE(pAseObject);


	//// �޽� ������Ʈ
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

	// �޽� ������Ʈ
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