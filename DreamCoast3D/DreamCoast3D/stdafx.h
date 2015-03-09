// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// HACK : reference additional headers your program requires here
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <assert.h>
#include <sstream>

// D3DX Include
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// Macro
#define SAFE_RELEASE(p) if(p){p->Release(); p = NULL;}
#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){delete [] p; p = NULL;}
#define SAFE_RENDER(p) if(p){p->Render();}

#define SINGLETONE(class_name) private: class_name(void); ~class_name(void); \
public: static class_name* GetInstance() { static class_name instance; return &instance; }

#define SAFE_ADD_REF(p) if(p){p->AddRef();}

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual varType Get##funName(void) const { return varName; }\
public: virtual void Set##funName(const varType& var){ varName=var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	SAFE_ADD_REF(var);\
	SAFE_RELEASE(varName);\
	varName = var;\
			}\
}

// Extern
extern HWND g_hWnd;

// 모든 클래스가 알아야할 구조체들
struct ST_RHWC_VERTEX
{
	D3DXVECTOR4 p;
	D3DCOLOR	c;
	enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
};

struct ST_RHW_VERTEX
{
	D3DXVECTOR4 p;
	//D3DCOLOR	c;
	D3DXVECTOR2 t;

	enum { FVF = D3DFVF_XYZRHW | /*D3DFVF_DIFFUSE*/D3DFVF_TEX1 };
};

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ST_PT_VERTEX{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};

struct ST_PNT_VERTEX{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

struct ST_PNT2_VERTEX{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t1;
	D3DXVECTOR2 t2;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 };
};

struct ST_PN_VERTEX{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct ST_BOUNDING_SPHERE{
	D3DXVECTOR3		m_vCenter; // 스피어를 이루는 캐릭터의 중심 
	float			m_fRadius;
	ST_BOUNDING_SPHERE() : m_vCenter(0, 0, 0), m_fRadius(0.0f){};
	ST_BOUNDING_SPHERE(D3DXVECTOR3& vCenter, float fRadius) : m_vCenter(vCenter), m_fRadius(fRadius){};
};

struct ST_BOUNDING_BOX{
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
	ST_BOUNDING_BOX() : vMin(0, 0, 0), vMax(0,0,0){};
	ST_BOUNDING_BOX(D3DXVECTOR3& _vMin, D3DXVECTOR3& _vMax) : vMin(_vMin), vMax(_vMax){};
};

struct ST_POS_SAMPLE
{
	int				nKey;
	D3DXVECTOR3		v;
};

struct ST_ROT_SAMPLE
{
	int				nKey;
	D3DXQUATERNION	q;
};

struct ST_SIZE
{
	float fWidth;
	float fHeight;

	ST_SIZE() : fWidth(0), fHeight(0) {}
	ST_SIZE(float _fWidth, float _fHeight) : fWidth(_fWidth), fHeight(_fHeight) {}
};
class cMtlTex;



struct ST_TILE_GRIDPOS{
	int x;
	int z;
	ST_TILE_GRIDPOS() : x(0), z(0){}
	ST_TILE_GRIDPOS(int nX, int nZ) : x(nX), z(nZ){}
};

// TODO 
// 차후 인벤토리 등을 추가해서
// 아래 PlayerbleObject, 혹은 플레이어클래스를 새로 상속받아 만들어서
// 캐릭터 스탯 조절 실시
// 인벤토리는 중복 적용되지 않게 map으로 간단하게 구현 예정(D2D작업당시 비슷하게)
typedef struct ST_STAT_INFO{
	int nStrength = 1;				// 힘
	int nArmor = 1;					// 방어력
	int nAgility = 1;				// 민첩성
	int nAttackSpeed = 1;			// 공속
	int nMoveSpeed = 10;				// 이동속도

	// 실질적 전투수치
	float fStatAttackPower =1.0f;		//공격력
	float fStatDefensePower = 1.0f;	//방어력
	float fKnockingDownRate = 1.0f;	//다운유발확률
	float fKnockDownResist = 1.0f;				//다운저항치
	float fCriticalRate = 1.0f;				//크리티컬확률
	float fCriticalResist = 1.0f;				//크리티컬저항
	float fCriticalMultiplier = 1.0f;			//크리티컬배율

	// 기본 체력
	float fMaxHp = 100.0f;
	float fCurrentHp = 100.0f;
	float fMaxMana = 100.0f;
	float fCurrentMana = 100.0f;
	float fMaxExp = 100.0f;
	float fCurrentExp = 0.0f;
} *LPPLAYERINFO;

// 인터페이스들
#include "iSceneDelegate.h"
#include "iActionDelegate.h"
#include "iButtonDelegate.h"
#include "iMap.h"
#include "iGameObjectDelegate.h"
#include "iGameStateObject.h"
#include "iUIManagerDeligate.h"
#include "iEffectManagerDeligate.h"
#include "iGameEventDelegate.h"

// 모두가 알아야 하는 클래스들
#include "cObject.h"
#include "cGameObject.h"
#include "cGameActionObject.h"
#include "cUIObject.h"
#include "cUIWindow.h"

// Singletone Manager Classes
#include "cDeviceManager.h"
#include "cObjectManager.h"
#include "cControlManager.h"
#include "cFontManager.h"
#include "cTextureManager.h"
#include "cSkinnedMeshManager.h"
#include "cAseManager.h"

#include "cGameTimer.h"

