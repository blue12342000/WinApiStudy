// main.cpp

/*
	Win32 API
	: Windows Application Programming Interface
	운영체제가 응용프로그램 개발을 위해 제공하는 함수의 집합

	핸들(Handle) : 각 윈도우 객체를 구분하는 인덱스 번호(정수)
	메세지(Message) : 윈도우에서 발생하는 이벤트를 운영체제가 윈도우에 전달하는 데이터 모음
*/

#include <Windows.h>
#include <string>

using namespace std;

// 전역변수
// Insrance : 메모리상에 존재하는 객체
HINSTANCE g_Instance;								// HANDLE + INSTANCE = 핸들의 인스턴스	ex) 크롬.exe 의 프로그램ID
HWND g_hWnd;										// HANDLE + WINDOW = 각각의 윈도우 핸들 ex) 크롬프로그램 안에서의 각각의 탭 하나 하나의 ID
LPSTR g_lpszClass = (LPSTR)TEXT("윈메인의 시작");	// TEXT 메크로 : 여러형태의 문자열(2바이트,1바이트)를 내부적으로 알아서 처리해주는 메크로

// 함수 두 개
// CALLBACK : 
/*
	HWND hWnd			: 이 프로그램으로 보낸 메세지의 주인
	UINT iMessage		: 윈도우 메세지 
	WPARAM wParam		: WORD + PARAM = 실제 데이터값을 받아올때 사용
	LPARAM lParam		: LONG + PARAM = 포인터를 받아올때 사용
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

/*
	HINSTANCE _hInstance	 : 이 프로그램의 ID를 응용프로그램이 실행될때 매개변수로 받아옴
	HINSTANCE _hPrevInstance : 호환성을 위한것으로 현재는 사용안함
	LPSTR _lpszCmdParam		 : 이 프로그램으로 들어오는 cmd 파라미터, 문자열 하나로 들어오기때문에 사용자가 가공하여 사용하여야한다. ex) Cmd.exe "파라미터"
	int nCmdShow			 : 이 프로그램이 어떤식으로 초기에 보여질것인가(최대화, 최소화 같은것) -> 바로가기 아이콘을 생성한뒤 속성으로가서 실행에서 확인이 가능하다.
*/
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpszCmdParam, int nCmdShow);

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpszCmdParam, int nCmdShow)
{
	g_Instance = _hInstance;

	// 윈도우를 생성하기 위한 기본셋팅
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;										// 클레스의 여분 메모리 설정
	wndClass.cbWndExtra = 0;										// 윈도우의 여분 메모리 설정
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// HANDLE + BRUSH + BACKGRAOUND = 백그라운드 브러시의 핸들 설정
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// 마우스 포인터의 모양을 설정
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// 프로그램의 아이콘 이미지을 설정
	wndClass.hInstance = g_Instance;								// 이 프로그램의 핸들
	wndClass.lpfnWndProc = WndProc;									// 프로그램의 콜백함수 등록
	wndClass.lpszClassName = g_lpszClass;							// 이 프로그램이 실행될때 상태창에 뜨는 문자열 설정
	wndClass.lpszMenuName = NULL;									// 프로그램의 메뉴
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						// CLASS + STYLE = CS_ 로 시작하는 것을 비트연산을 통해 전달

	// 윈도우 클래스 등록
	RegisterClass(&wndClass);

	// 윈도우 생성
	/*
		lpClassName			: 이 윈도우의 클래스명
		lpWindowName		: 이 윈도우의 이름
		dwStyle				: WINDOW + STYLE = WS_로 시작하는 윈도우의 스타일
		x					: 윈도우의 시작위치
		y					: 윈도우의 시작위치
		width				: 윈도우의 가로길이
		height				: 윈도우의 세로길이
		hWndParent			: 이 윈도우의 부모의 핸들
		hMenu				: 이 윈도우의 메뉴핸들
		hInstance			: 이 윈도우의 번호
		lpParam				: 이 윈도우로 넘길 파라미터
	*/
	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, 400, 400, NULL, NULL, g_Instance, NULL);

	// 윈도우 출력
	ShowWindow(g_hWnd, nCmdShow);

	// 메세지 큐에 있는 메세지 처리
	/*
		::GetMessage
		LPMSG				: 메세지 주소
		HWND				: 
		wMessageFilterMin	: 
		wMessageFilterMax	: 
	*/
	MSG message;
	while (GetMessage(&message, 0, 0, 0))
	{
		// Message의 정보를 어떤키가 눌린것인지 변환
		TranslateMessage(&message);

		// 변환된 Message정보를 WinProc로 보내는 역할
		DispatchMessage(&message);
	}

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	switch (iMessage)
	{
	case WM_QUIT:
		MessageBox(g_hWnd, (LPCSTR)TEXT("WM_QUIT"), TEXT("핸들의 정보"), MB_OK);
		break;
	case WM_SYSCOMMAND:
		MessageBox(g_hWnd, (LPCSTR)TEXT("WM_SYSCOMMAND"), TEXT("핸들의 정보"), MB_OK);
		break;
	case WM_CLOSE:
		MessageBox(g_hWnd, (LPCSTR)TEXT("WM_CLOSE"), TEXT("핸들의 정보"), MB_OK);
		break;
	case WM_DESTROY:
		MessageBox(g_hWnd, (LPCSTR)TEXT("WM_DESTROY"), TEXT("핸들의 정보"), MB_OK);
		PostQuitMessage(0);
		break;
	case WM_NCDESTROY:
		MessageBox(g_hWnd, (LPCSTR)TEXT("WM_NCDESTROY"), TEXT("핸들의 정보"), MB_OK);
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}


