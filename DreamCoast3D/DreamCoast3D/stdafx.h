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
#include <string>

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

_declspec(align(16)) struct ST_BONE : public D3DXFRAME
{
	D3DXMATRIXA16 matWorldTM;
};

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
	DWORD					dwNumSubset;
	std::vector<cMtlTex*>	vecMtlTex;

	LPD3DXMESH				pOrigMesh;				// 원본 메쉬
	D3DXMATRIX**			ppBoneMatrixPtrs;		// 이 메쉬에 영향을 주는 프레임'들'의 월드매트릭스 포인터 배열
	D3DXMATRIX*				pBoneOffsetMatrices;	// 원본 메쉬를 로컬스페이스로 보내는 매트릭스들.
	D3DXMATRIX*				pCurrentBoneMatrices;	// 각 본의 계산된 월드매트릭스
};

struct ST_BONE_MESH_SPHERE : public ST_BONE_MESH{
	//충돌용 바운딩 스피어 값 채우기용 : 민우
	float					fOriginRadius;			//애니메이션 첫 프레임의 충돌용 바운딩 스피어 메쉬의 반지름. 변환의 기준이 된다.
	float					fRadius;				//애니메이션 현재 프레임의 충돌용 바운딩 스피어 메쉬의 반지름.
	D3DXVECTOR3				vCenter;				//충돌용 바운딩 스피어의 중심점
	LPD3DXMESH				pSphereMesh;			//충돌용 바운딩 스피어의 메쉬
	LPD3DXBUFFER			pSphereAdj;				//충돌용 바운딩 스피어의 Adj버퍼
	D3DXMATRIX				matSphereW;				//충돌용 바운딩 스피어의 월드변환행렬
};

// 인터페이스들
#include "iSceneDelegate.h"
#include "iActionDelegate.h"
#include "iButtonDelegate.h"
#include "iMap.h"

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