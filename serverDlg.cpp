
// serverDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "server.h"
#include "serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



void MyServer::AddWorkForAccept(TW_UserData* ap_user) //함수 재정의
{
	
	CString str;
	str.Format(L"%s에서 새로운사용자가 접속했습니다 !", ap_user->GetIP());
	mp_parent->AddEventString(str); //사용자를 IP 를 얻을수있으며 리스트박스에 추가를 할수있다 

}

void MyServer::AddWorkForCloseUser(TW_UserData* ap_user, int a_error_code)
{
	CString str;
	str.Format(L"%s에서 새로운사용자가 해제했습니다 !", ap_user->GetIP());
	mp_parent->AddEventString(str); //사용자를 IP 를 얻을수있으며 리스트박스에 추가를 할수있다
}

int  MyServer::ProcessRecvData(SOCKET ah_socket, unsigned char a_msg_id, char* ap_recv_data, BS a_body_size)
{
	TW_UserData* p_user = (TW_UserData*)FindUserData(ah_socket); //유저데이터 주소를 볼수있다 
	if (a_msg_id == NM_CHAT_DATA) {//클라이언트에서 채팅데이터 를 받기 위함
		CString str;
		str.Format(L"%s : %s ", p_user->GetIP(), (wchar_t*)ap_recv_data);
		mp_parent->AddEventString(str);

		for (int i = 0; i < m_max_user_count; i++) {
			// 현재 사용자가 접속 상태인지 확인한다.
			if (mp_user_list[i]->GetHandle() != -1) { // 0xFFFFFFFF
				SendFrameData(mp_user_list[i]->GetHandle(), NM_CHAT_DATA, (const char*)(const wchar_t*)str, (str.GetLength() + 1) * 2);
			}
		}


	}
	return 1; //소켓 동작을 위함 , 
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CserverDlg 대화 상자



CserverDlg::CserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent) , m_server(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_LIST, m_event_list);
}

BEGIN_MESSAGE_MAP(CserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(25001, &CserverDlg::OnAcceptUser)
	ON_MESSAGE(25002, &CserverDlg::OnReadAndClose)
END_MESSAGE_MAP()


// CserverDlg 메시지 처리기

BOOL CserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_server.StartServer(L"192.168.219.104",50000, m_hWnd);//실행 서버아이피 와 포트 설정 
	AddEventString(L"채팅서버 서비스를 시작합니다 "); //실행하면 리스트박스에 메세지 발생
	
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//자기가 출력하고싶은 정보가있으면 대화상자에 AddEventString 호출을 하고문자를 사용하면 대화상자에 리스트박스에 저장이된다
void CserverDlg::AddEventString(const wchar_t* ap_string) //멤버함수
{
	int index = m_event_list.InsertString(-1, ap_string);
	m_event_list.SetCurSel(index);
}

afx_msg LRESULT CserverDlg::OnAcceptUser(WPARAM wParam, LPARAM lParam)
{
	m_server.ProcessToAccept(wParam, lParam);
	return 0;
}



afx_msg LRESULT CserverDlg::OnReadAndClose(WPARAM wParam, LPARAM lParam)
{
	m_server.ProcessClientEvent(wParam,lParam);
	return 0;
}
