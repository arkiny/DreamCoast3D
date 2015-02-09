Filter 디렉토리별 클래스 설명
_STDAFX
- 사전 컴파일 될 헤더

Core
- DreamCoast3D : WinApi Entry
- cGameCore : cMainGame과 같은 역할을 하는 게임 매니저
- cGameTimer : 전체 게임 타이머 // 싱글톤 처리를 할수 있으나, 
				Update의 인자로 받아서 내려가는 걸 프레임워크 작성자가 선호
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

Object
- cObject
	오브젝트 인스탄싱 베이스 클래스
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

		- cGameBillBoardingObject
			항상 화면을 정면으로 바라보는 오브젝트

		- cGameMapObject 
			iMap 인터페이스를 상속받은 게임맵오브젝트, 근접맵 정보를 설정해주면, 
			근접 맵주소를 가질수 있다.
				-iMap : 맵의 높이를 리턴해주는 퓨어함수를 가진 인터페이스

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

Examples - 프레임웤 예제들
- cSphere 
	cGameObject를 상속받은 클래스의 사용예제

- cSphereAction	
	cGameActionObject를 상속받은 클래스의 사용예제, 
	응용에 따라 컨트롤이 가능한 게임오브젝트도 생성가능( HACK : 차후 예제 추가)

- cBillingExample 
	cGameBillBoardingObject를 상속받은 클래스의 사용예제

- cMapExample 
	cGameMapObject를 상속받은 클래스의 사용예제겸 그리드맵

- cUIExample
	간단한 퀘스트 박스 제작으로 만들어본 cUIObject 사용 예제
	좀더 개선히 필요해 보인다.

- cSceneExample 
	cScene을 상속받은 클래스의 사용예제, cActionMove, cActionRepeat, cActionSeq의 사용예제