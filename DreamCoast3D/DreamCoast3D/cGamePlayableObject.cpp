#include "stdafx.h"
#include "cGamePlayableObject.h"
#include "cPlayerIdle.h"
#include "cPlayerMove.h"
#include "cPlayerAttack.h"
#include "cPlayerOnHit.h"
#include "cPlayerDead.h"
#include "cEffectParticle.h"
#include "cSkinnedMesh.h"

#include "cPlayerRun.h"
#include "cPlayerUnarmedWait.h"
#include "cPlayerCombo1.h"
#include "cPlayerCombo1R.h"

//���� ����
//���� ���� 7������ �ٲ㼭 �����ְ� cSceneManager.cpp/Start() 
//(������)�÷��̾�� ĳ���͸� EX�� �����Ҵ� ���״�. cGameObjLoader.cpp/ParsePlayerbleObj()

//cGameObjLoader 383 ���� �ּ�ó��. SetIsLoop�����ε� �����Ҷ� �ڼ��� ó��
//�ִϸ��̼ǰ� ��ȯ ������ �� �޼��Ǿ� �ִ�.
//REAR_CANCEL �κ� ä���־�ߵȴ�
//�������� ���� �ε�
//�������� ĳ���� �̵��� ���� ī�޶�� ���������� ��ǥ�̵�
//�׸��� ���� �����Ű�� �Ϸ�.
//�������� ���ҽ�+Ŭ�������� �۾�

cGamePlayableObject::cGamePlayableObject()
	:m_vecFront(0, 0, -1),
	//m_fMoveSpeed(10.0f),
	m_fPlayerAngleDegree(0.0f),
	m_fPlayerInvincibleTime(0.5f),
	m_fPlayerInvincibleCool(1.0f),
	m_pPlayerStatInfo(NULL)
{
	m_eGameObjectType = eGameObjectType::E_PLAYABLE;
	m_fMoveSpeed = 10.0f;
	m_pPlayerStatInfo = new ST_STAT_INFO;
}

cGamePlayableObject::~cGamePlayableObject()
{
	for (auto p : m_vecStates){
		SAFE_DELETE(p);
	}
	delete m_pPlayerStatInfo;
}

void cGamePlayableObject::Setup(
	std::string sFolder, std::string sFile,
	std::string sFolderHead, std::string sFileHead,
	std::string sFolderHair, std::string sFileHair
	){
	cGameSMeshBodyObject::Setup(sFolder, sFile, sFolderHead, sFileHead, sFolderHair, sFileHair);

	/*m_vecStates.resize(EPLAYABLESTATE::EPLAYABLESTATE_MAX);
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = new cPlayerIdle;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_MOVE] = new cPlayerMove;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_ATTACK] = new cPlayerAttack;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_ONHIT] = new cPlayerOnHit;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_DEAD] = new cPlayerDead;*/

	m_vecStates.resize(EPLAYABLESTATE::EPLAYABLESTATE_MAX);
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_UNARMEDWAIT] = new cPlayerUnarmedWait;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_IDLE] = new cPlayerIdle;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_RUN] = new cPlayerRun;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_COMBO1] = new cPlayerCombo1;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_COMBO1R] = new cPlayerCombo1R;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_ONHIT] = new cPlayerOnHit;
	m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_DEAD] = new cPlayerDead;


	m_pCurrentState = m_vecStates[EPLAYABLESTATE::EPLAYABLESTATE_IDLE];
	m_pCurrentState->Start(this);
}

void cGamePlayableObject::Update(float fDelta){
	cGameActionSkinnedMeshObj::Update(fDelta);
	m_pCurrentState->Execute(this, fDelta);
	
	m_pGameObjDeligate->isGameObjectCollided(this);
	m_fPlayerInvincibleCool += fDelta;
	m_fSkill1CoolTime += fDelta;

	if (m_fSkill1CoolTime > m_fSkill1Delay){
		m_fSkill1CoolTime = m_fSkill1Delay;
	}

	if (m_fPlayerInvincibleCool > 2.0f) {
		// Ȥ�� �� �����÷ο� ���
		// TODO: ���� �ƽ� �������� ���ؾ���
		m_fPlayerInvincibleCool = 2.0f;
	}

}

void cGamePlayableObject::ChangeState(EPLAYABLESTATE eNewState){
		//if (m_pCurrentState != m_vecStates[eNewState])
	//{
	//	//m_pCurrentState->Exit(this);
	//	SetPlayableState(eNewState);
	//	m_pCurrentState = m_vecStates[eNewState];
	//	//m_fStatePassedTime = 0.0f; //Start���� ��
	//}
	if (JudgeChange(eNewState))
	{
		SetPlayableState(eNewState);
		m_pCurrentState = m_vecStates[eNewState];
		m_pCurrentState->Start(this);
	};
}

void cGamePlayableObject::ChangeState(int nState){
	this->ChangeState((EPLAYABLESTATE)nState);
}

bool cGamePlayableObject::JudgeChange(EPLAYABLESTATE eNewState)
{
	//�ִϸ��̼��� ����ǰ� ���¸� ��ȯ�ϴ°� �׳� True
	if (!m_pCurrentState->GetIsDoing())
	{
		return true;
	}
	//int a = m_pCurrentState->GetCancelInfo()[eNewState];
	
	//CANCEL_CANT
	if (m_pCurrentState->GetCancelInfo()[eNewState] == 0)
	{

	}
	//CANCEL_REAR
	else if (m_pCurrentState->GetCancelInfo()[eNewState] == 1)
	{
		//return true;
	}
	//CANCEL_FORCE
	else if (m_pCurrentState->GetCancelInfo()[eNewState] == 2)
	{
		return true;
	}
	return false;
}

int cGamePlayableObject::GetState() {
	return m_pCurrentState->GetCurrentStateType();
}

void cGamePlayableObject::OnHitTarget(cGameObject* pTarget, float fDamage, D3DXVECTOR3 vHitPosition){
	if (m_pCurrentState != m_vecStates[this->EPLAYABLESTATE_ONHIT]){
		if (m_fPlayerInvincibleCool > m_fPlayerInvincibleTime){

			m_fPlayerInvincibleCool = 0.0f;


			D3DXVECTOR3 playerPos = this->GetPosition();
			playerPos.y = playerPos.y + 1.0f;


			// todo : �ǰ� ���� ��üȭ
			//p->SetPosition(vHitPosition);

			this->GetEffectDelegate()->AddEffect(cEffect::E_EFFECT_ONHIT, vHitPosition);
			g_pSoundManager->executeOnHit(1);

			if (this->GetStatInfo()->fCurrentHp <= 0)
			{
				return;
			}
			this->GetStatInfo()->fCurrentHp -= 10.0f;

			this->ChangeState(this->EPLAYABLESTATE_ONHIT);
		}
	}
	if (this->GetStatInfo()->fCurrentHp <= 0)
	{
		this->ChangeState(this->EPLAYABLESTATE_DEAD);
	}
}

void cGamePlayableObject::Clone(cGameObject** pTarget){
	//assert(pTarget == NULL);
	cGamePlayableObject* p = new cGamePlayableObject;
	p->Setup(m_sFolder, m_sFile, m_sHeadFolder, m_sHeadFile, m_sHairFolder, m_sHairFile);
	D3DXVECTOR3 pCopyPos = this->GetPosition();
	p->SetPosition(pCopyPos);
	D3DXVECTOR3 vCopyScale = this->GetScale();
	p->SetScale(vCopyScale);
	p->Start();
	*pTarget = p;
}

std::string cGamePlayableObject::SaveAsStringInfo(){
	std::stringstream ss;
	ss.precision(2);

	ss << std::endl;
	ss << "*PLAYABLEOBJ {" << std::endl;
	ss << "*SKINNEDMESH_REF " << m_pSkinnedMesh->GetMeshRefNumber() << std::endl;
	ss << "*SKINNEDMESHHEAD_REF " << m_pSkinnedMesh->GetHeadRefNum() << std::endl;
	ss << "*SKINNEDMESHAIR_REF " << m_pSkinnedMesh->GetHairRefNum() << std::endl;
	D3DXVECTOR3 pos = GetPosition();
	ss << "*POISTION " << std::fixed << pos.x << "\t" << pos.y << "\t" << pos.z << std::endl;
	D3DXVECTOR3 scale = GetScale();
	ss << "*SCALE " << std::fixed << scale.x << "\t" << scale.y << "\t" << scale.z << std::endl;


	//ss << "*GAMEAIOBJ_AI_PATTERN " << m_eAITYPE << std::endl;
	ss << "*SETINITANIMATION " << EPLAYABLESTATE_IDLE << std::endl;
	ss << "}" << std::endl;
	//ss << "}" << std::endl;

	return ss.str();
}