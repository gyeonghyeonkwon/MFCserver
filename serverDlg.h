
// serverDlg.h: 헤더 파일
//

#pragma once
#define MAX_USER_COUNT 50 //50명까지 접속 가능 
#define NM_CHAT_DATA   1 //채팅 데이터 생성 

class CserverDlg; //컴파일 에러를 없애 준다 

class MyServer : public TW_ServerSocket
{
private :
	CserverDlg* mp_parent; //포인터 를 기억할수있음 , 나중에 리스트 박스에 출력

public:
	MyServer(CserverDlg* ap_parent) : TW_ServerSocket(0x27, MAX_USER_COUNT, new TW_UserData)
	{
		mp_parent = ap_parent;// 프로토콜 헤더의 시작 바이트를 0x 27값으로 하고 최댛 사용자의 수를 50명으로 정한다 
                              // 사용자 관리용 클래스트 TW_Userdata 를 사용 한다.
	}
	virtual void AddWorkForAccept(TW_UserData* ap_user);
	virtual void AddWorkForCloseUser(TW_UserData* ap_user, int a_error_code);
	virtual int  ProcessRecvData(SOCKET ah_socket, unsigned char a_msg_id, char* ap_recv_data, BS a_body_size);
}; //클라이언트 데이터가 수신 되명 해당 함수가 호출이 된다.



// CserverDlg 대화 상자
class CserverDlg : public CDialogEx
{
private: 
	MyServer m_server; 
	
	//대화상자를 알기위한 정의 
// 생성입니다.
public:
	CserverDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	void AddEventString(const wchar_t* ap_string);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_event_list;
protected:
	afx_msg LRESULT OnAcceptUser(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnReadAndclose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReadAndClose(WPARAM wParam, LPARAM lParam);
};
