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
	:cScene() // �θ�Ŭ���� �ʱ�ȭ
{
}

cSceneExample::~cSceneExample()
{
}

void cSceneExample::Setup(std::string sFilePath){
	cScene::Setup(sFilePath);

	//1. �� ���� ������ �ٸ� �� �����Ƿ� ��� ���� ������ �����Ѵ�.
	//@HACK: LightSource �Ŵ����� ���� ���� ���� ���
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

	// @TODO: ���� �� ���� sFilePath���� �ε��� ������Ʈ ������ �޾ƿͼ� �ε��ϵ��� �Ѵ�.
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
}

void cSceneExample::Update(float delta){
	cScene::Update(delta);
}

void cSceneExample::Render(){
	cScene::Render();
}