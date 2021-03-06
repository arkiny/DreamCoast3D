Filter 디렉토리별 클래스 설명
_STDAFX
- 사전 컴파일 될 헤더

Core
- DreamCoast3D : WinApi Entry
- cGameCore : cMainGame과 같은 역할을 하는 게임 매니저
- cGameTimer : 전체 게임 타이머 // 싱글톤 처리를 할수 있으나, 
				Update의 인자로 받아서 내려가는 걸 프레임워크 작성자가 선호 XXX : 현재 싱글턴화되어있음
- Resource : WinApi 리소스
- Targetver : ???

Singletons
- cDeviceManager 
	Direct3D 디바이스 매니저, 디스트로이 콜시 COM객체가 남아있나 확인한다, 
	남아있을 시 Assert Fail 호출
- cObjectManager 
	cObject 인스탄스들을 관리하는 매니저, 디스트로이 콜시 cObject 인스탄스들이 남아있나 확인한다. 
	남아있을 시 Assert Fail 호출
- cControlManager 
	입출력 장치, 인풋 컨트롤 정보들을 관리하는 싱글턴 클래스. 
	모든 입출력 정보는 여기에서 저장되고 여기서 확인한다.
- cFontManager 
	폰트 매니저, 폰트를 콜시 해당 폰트가 없으면 생성 리턴, 있으면 찾아서 리턴해준다.
- cSkinnedMeshManager
	스킨드 메쉬 매니저, 스킨드 메쉬의 D3DXFRAME을 생성하여 인스탄싱한다. 
	애니메이션 컨트롤은 본래 컨트롤을 클로닝(Deep Copy)해서 넘겨준다.
	- cAllocateHierarchy 
		ID3DXAllocateHierarchy를 상속해서 확장한 클래스. 
		XXX : D3DXMESH를 확장한 ST_BONE 및 D3DXMESHCONTAINER를 확장한 ST_BONE_MESH를 저장한다.
- cAseManager
	cAseInstance 클래스를 인스탄싱 작업해주는 클래스

Object
- cObject
	오브젝트 베이스 클래스

	- cGameObjManager 
		cGameObject들을 가지고, 관리하는 매니저, Frustom Culling을 여기서 처리, 
		차후 GameObj 델리게이트(충돌처리)도 여기서 처리할까 생각중

		- cGameObject
			위치와 렌더를 가지는 모든 오브젝트의 베이스 클래스
		
			- cTransForm 
				위치를 가지는 오브젝트를 위한 트랜스폼 클래스
			- cFrustum 
				프러스텀 컬링을 위한 클래스, 각 GameObjManager는 해당 클래스를 하나씩 가지고 컬링한다.

		- cGameActionObject 
				움직이거나 입력된 방식에 따라 행동이 실시되는 게임 오브젝트(캐릭터, 몬스터 등등)

			- Action 
				- cAction 
					차후 모든 오브젝트 행동에 대한 베이스 클래스
				- cActionMove 
					사전에 입력된 시작점과 끝점에 대해서 이동
				- cActionSeq 
					액션들을 가지고 있으며, 한 행동이 끝나면 다른 행동을 실시할수 있는 액션 시퀀스
				- cActionRepeat
					위의 액션 시퀀스나 한개의 액션을 반복

			- cGameActionSkinnedMeshObj
				cGameActionObject를 상속받고, D3DXFRAME을 통한 SkinnedMesh 애니메이션을 가지는 게임오브젝트
				 - cSkinnedMesh
					- D3DXFRAME과 Animation Control을 가지고 실제 캐릭터 애니메이션을 실시하는 오브젝트
					- TODO : 차후 스피어를 노드에 붙일때 여기서 붙여서 cGameActionSkinnedMeshObj로 리턴하는 방안을 고민
				 - cAnimationSet
					- 애니메이션 컨트롤을 통해 애니메이션을 실시할 세트

		- cGameBillBoardingObject
			항상 화면을 정면으로 바라보는 오브젝트			

		- cGameMapObject 
			iMap 인터페이스를 상속받은 게임맵오브젝트, 근접맵 정보를 설정해주면, 
			근접 맵주소를 가질수 있다.
				-iMap : 맵의 높이를 리턴해주는 퓨어함수를 가진 인터페이스

		- cGameAseObject
			cASEInstance를 이용해 렌더하는 오브젝트
				-cASEInstance
					ASE파일해서 로딩해온 정보를 가지고 있는 클래스
					cAseManager에 의해 최초 한번만 로딩하고 이후에는 가져다가 쓴다.

	- cUIObjManager
		UI오브젝트들을 관리하고 업데이트하거나 델리게이트를 실행해주는 매니저
		제일 마지막에 렌더함으로서 가장 상위 레이어를 가진다
		차후 메인메뉴 역시 이 것으로 관리할 것으로 보인다.
		- cUIObject 
			UI관련 Object들, 모두 cUIObject를 상속 받으며 조합하여 새로운 UI를 만든다.
				- cUIImageButton
					이미지버튼, 델리게이트 포인트를 가지고 있으며, 클릭시 해당 델리게이트를 통해 처리
				- cUIImageView
					그냥 텍스쳐 스프라이트 이미지를 출력하는 UI오브젝트
				- cTextView
					정해진 텍스트를 출력하는 UI오브젝트
				- iButtonDelegate
					버튼을 누르면 행하는 행동양식에 관련된 인터페이스

Scene
- iSceneDelegate 
	씬들간의 이동에 쓰일 델리게이트 인터페이스

- cScene 
	씬, cGameObjManager와 cCamera, cLightSource, cGameMapObject(vector)를 가지고 처리한다.
	각 씬은 여러개의맵을 가질수 있다.

- cSceneManager 
	씬들을 관리하는 매니저, 씬델리게이트는 여기서 처리

- cCamera 
	씬을 비추는 카메라, 각 씬은 두개 이상의 카메라를 가지고 있을 수 있으나, 우선 1개만 처리

- cLightSource 
	씬의 광원, 광원은 D3DX9 광원 제한량에 따라 8개까지 쓸수 있으므로 따로 클래스로 관리

ParseAndLaoder
- cLoader
	ASE파서의 로더를 약간 트윜한 클래스, 차후 모든 로더 클래스의 베이스 클래스가 된다.
- cGameLoader
	로더들이 다 구현되면 최종적으로 모든 리소스를 하나의 정보파일로부터 로딩해올 클래스
- cGameObjLoader
	게임 오브젝트 로더, 현재는 cGamActionSkinnedMeshObject로딩만 구현되어 있다.
- cMapLoader
	맵 파일을 불러와 맵오브젝트 및 맵에 관련된 정보를 로딩하는 클래스
- cAseLoader
	ASE파일을 불러와서 ASE파일을 cASEInstance로 가공해주는 클래스

Examples - 프레임웤 예제들
- cSphere 
	cGameObject를 상속받은 클래스의 사용예제

- cSphereAction	
	cGameActionObject를 상속받은 클래스의 사용예제, 
	응용에 따라 컨트롤이 가능한 게임오브젝트도 생성가능( HACK : 차후 예제 추가 )

- cBillingExample 
	cGameBillBoardingObject를 상속받은 클래스의 사용예제

- cMapExample 
	cGameMapObject를 상속받은 클래스의 사용예제 겸 그리드맵

- cUIExample
	간단한 퀘스트 박스 제작으로 만들어본 cUIObject 사용 예제
	좀더 개선이 필요해 보인다.

- cZealot
	cGameActionSkinnedMeshObj를 상속받은 클래스의 사용예제

- cCameraEditing
	에디팅용 카메라, 현재는 Chasing Camera의 원리를 사용하고 있다.

- cSceneExample 
	cScene을 상속받은 클래스의 사용예제, cActionMove, cActionRepeat, cActionSeq의 사용예제
	cZealot이 같은 프레임을 인스탄싱 받아서 활용하는 부분이 여기 나와있다.
	또한 cGameObjLoader를 이용해 리소스를 로딩하는 예시 역시 여기 포함되었다.
	맵로더와 오브젝트로 로더의 이용예시 역시 여기 포함되어 있으며,
	해당 양식은 각각 Resources\SCENE1_MAPDATA.txt와, Resources\SCENE1_GAMEOBJDATA.txt에 저장되어있다.