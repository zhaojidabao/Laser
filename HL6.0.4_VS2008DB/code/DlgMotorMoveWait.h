#pragma once


#define  WM_MOTORMOVEPROC_EXIT			( WM_USER+310 )
// CDlgMotorMoveWait �Ի���

class CDlgMotorMoveWait : public CDialog
{
	DECLARE_DYNAMIC(CDlgMotorMoveWait)

public:
	CDlgMotorMoveWait(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMotorMoveWait();

// �Ի�������
	enum { IDD = IDD_DIALOG_MOTORMOVE_WAIT };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMotorMoveExit(WPARAM wParam,LPARAM lParam);

	virtual BOOL OnInitDialog();

public:
	static void MotorMove();
	void SetData(int nMoveModel, int nAxisNo, double dbMoveValue, int nMoveType );
	
	static int m_nAxisNo;
	static double  m_dbMoveValue;
	static int m_nMoveType;
	static int m_nMoveModel;
	static BOOL m_bHome;
	static BOOL m_bProcess;
	afx_msg void OnClose();
};
