#include "stdafx.h"
#include "cGameAIObject.h"
#include "cAIFSM.h"
#include "cActionMove.h"
#include "cTransform.h"
#include "cSkinnedMesh.h"
#include "cEffectFireBall.h"
#include <sstream>

cGameAIObject::cGameAIObject()
	:m_pCurrentState(NULL),
	m_pPrevState(NULL),
	m_fPassedTime(0),
	m_pTargetGameObject(NULL),
	m_eAITYPE(E_AI_AGGRESSIVE),
	m_fAttackRange(2.0f),
	m_vFront(0, 0, -1),
	m_fAIAngle(0),
	m_fAttackSpeed(2.0f),
	m_fAttackCoolTime(3.0f)
{
	m_fMoveSpeed = 10.0f;
	m_eGameObjectType = eGameObjectType::E_MOP;
	//SetYangle(2);

    m_fHP = 2.0f;
}


cGameAIObject::~cGameAIObject()
{
	for (auto p : m_vecPatterns){
		delete p;
	}
}

void cGameAIObject::Setup(std::string sFolder, std::string sFile){
	cGameActionSkinnedMeshObj::Setup(sFolder, sFile);
	
	m_vecPatterns.resize(EAIOBJECTSTATE::eAISTATE_MAX);
	m_vecPatterns[eAISTATE_IDLE] = new cAIIdle;
	m_vecPatterns[eAISTATE_MOVE] = new cAIMove;
	m_vecPatterns[eAISTATE_ATTACK] = new cAIAttack;
	m_vecPatterns[eAISTATE_ONHIT] = new cAIOnHit;
	m_vecPatterns[eAISTATE_THINK] = new cAIThink;
	m_vecPatterns[eAISTATE_RANDOMMOVE] = new cAIRandomMove;
	m_vecPatterns[eAISTATE_DEAD] = new cAIDead;
	m_vecPatterns[eAISTATE_MOVETOTARGET] = new cAIMoveToTarget;
}

void cGameAIObject::Start(){
	m_pTargetGameObject = NULL;
	m_fHP = 2;
	m_pCurrentState = m_vecPatterns[eAISTATE_IDLE];
	m_pCurrentState->Start(this);
}

void cGameAIObject::Update(float fDelta){
	cGameActionSkinnedMeshObj::Update(fDelta);
	m_fPassedTime += fDelta;

	m_fAttackCoolTime += fDelta;
	if (m_fAttackCoolTime > 3.0f){
		m_fAttackCoolTime = 3.0f;
	}

	m_fAiInvincibleCool += fDelta;

	if (m_fAiInvincibleCool > 2.0f) {
		// 혹시 모를 오버플로우 대비
		// TODO: 차후 맥스 경직값을 정해야함
		m_fAiInvincibleCool = 2.0f;
	}

	m_pCurrentState->Execute(this, fDelta);
	m_pGameObjDeligate->isGameObjectCollided(this);
}

void cGameAIObject::ChangeState(EAIOBJECTSTATE eState){
	if (m_pCurrentState != m_vecPatterns[eState]){
		m_pCurrentState->Exit(this);
		m_pPrevState = m_pCurrentState;
		m_pCurrentState = m_vecPatterns[eState];
	}
	m_pCurrentState->Start(this);
}

void cGameAIObject::ChangeState(int nState){
	this->ChangeState((EAIOBJECTSTATE)nState);
}

void cGameAIObject::OnActionFinish(cAction* pSender){
	//cActionMove* pAction = new cActionMove;
	//D3DXVECTOR3 curPos = GetPosition();
	//pAction->SetFrom(curPos);
	//pAction->SetDelegate(this);
	//pAction->SetisNoMove(true);
	//
	//if (m_pTargetGameObject){
	//	D3DXVECTOR3 targetPos = m_pTargetGameObject->GetPosition();
	//	pAction->SetTo(targetPos);
	//}
	//else {
	//	D3DXMATRIXA16 matR;
	//	float yangle = m_pTransform->GetYAxisAngle();
	//	D3DXVECTOR3 vStd(0, 0, -1);
	//	D3DXVec3TransformNormal(&vStd, &vStd, &matR);
	//	D3DXVECTOR3 to = curPos + vStd;
	//	pAction->SetTo(to);
	//}

	//// deligate가 콜되지 않게 충분히 큰숫자를 넣어준다.
	//pAction->SetActionTime(100.0f);
	//SetAction(pAction);
	//SAFE_RELEASE(pAction);
	//ChangeState(this->eAISTATE_THINK);
}

void cGameAIObject::ChangeToPrevState(){
	if (m_pPrevState && m_pPrevState != m_pCurrentState){
		m_pCurrentState->Exit(this);
		m_pCurrentState = m_pPrevState;
		m_pPrevState = NULL;
	}
	//m_pCurrentState->Start(this);
}

void cGameAIObject::OnHitTarget(cGameObject* pTarget, float fDamage, D3DXVECTOR3 vHitPosition){
	if (m_fAiInvincibleCool > 1.0f){
		m_fAiInvincibleCool = 0.0f;
		if (pTarget != m_pTargetGameObject){
			m_pTargetGameObject = pTarget;
		}
		m_pPrevState = NULL;

		//cEffectFireBall* p = new cEffectFireBall;
		//p->Setup();
		////D3DXVECTOR3 playerPos = this->GetPosition();
		////playerPos.y = playerPos.y + 1.0f;
		//p->SetPosition(vHitPosition);
		//this->GetEffectDelegate()->AddEffect(p);
		//p->Release();

		this->GetEffectDelegate()->AddEffect(cEffect::E_EFFECT_ONHIT, vHitPosition);


		// TODO 데미지에 따라 체력 저하
		m_fHP--;
		if (m_fHP > 0){
			this->ChangeState(eAISTATE_ONHIT);
		}
		else {
			this->ChangeState(eAISTATE_DEAD);
		}
	}
}

void cGameAIObject::AddGameObjToAggroMap(cGameObject* pGameObj){
	if (pGameObj->GetGameObjectType() == pGameObj->E_PLAYABLE && pGameObj != this){
		m_mapAggromap[pGameObj] = 110.0f;
	}
}

void cGameAIObject::CheckAggroMapAndSetTarget(){
	for (auto p : m_mapAggromap){
		if (p.second > 100.0f){
			m_pTargetGameObject = p.first;
		}
	}
}

void cGameAIObject::Clone(cGameObject** pTarget){
	//assert(pTarget == NULL); // pTarget should be empty pointer
	cGameAIObject* p = new cGameAIObject;
	p->Setup(m_sFolder, m_sFile);

	D3DXVECTOR3 pCopyPos = this->GetPosition();
	p->SetPosition(pCopyPos);
	D3DXVECTOR3 vCopyScale = this->GetScale();
	p->SetScale(vCopyScale);
	p->SetAItype(m_eAITYPE);
	p->Start();
	*pTarget = p;
}

std::string cGameAIObject::SaveAsStringInfo(){
	std::stringstream ss;
	ss.precision(2);

	ss << std::endl;
	ss << "*GAMEAIOBJ {" << std::endl;
	ss << "*SKINNEDMESH_REF " << m_pSkinnedMesh->GetMeshRefNumber() << std::endl;
	D3DXVECTOR3 pos = GetPosition();
	ss << "*POISTION " << std::fixed << pos.x << "\t" << pos.y << "\t" << pos.z << std::endl;
	D3DXVECTOR3 scale = GetScale();
	ss << "*SCALE " << std::fixed << scale.x << "\t" << scale.y << "\t" << scale.z << std::endl;
	ss << "*GAMEAIOBJ_AI_PATTERN " << m_eAITYPE << std::endl;
	ss << "*SETINITANIMATION " << eAISTATE_IDLE << std::endl;
	ss << "}" << std::endl;

	return ss.str();
}