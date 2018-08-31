// DlgMotorMoveWait.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgMotorMoveWait.h"

int			CDlgMotorMoveWait::m_nAxisNo = 0;
double	CDlgMotorMoveWait::m_dbMoveValue = 0.0;
int			CDlgMotorMoveWait::m_nMoveModel = 0;
int			CDlgMotorMoveWait::m_nMoveType = 0;
BOOL		CDlgMotorMoveWait::m_bHome = FALSE;
BOOL		CDlgMotorMoveWait::m_bProcess = FALSE;

UINT MotorMove_Process(LPVOID lpPara)
{	
	CDlgMotorMoveWait *pDlg = (CDlgMotorMoveWait *)lpPara;
	if ( ! pDlg  )	
		return 0;

	HWND hWnd = pDlg->GetSafeHwnd();
	CDlgMotorMoveWait::m_bProcess = TRUE;
	pDlg->MotorMove();
	CDlgMotorMoveWait::m_bProcess = FALSE;

	PostMessage(hWnd,WM_MOTORMOVEPROC_EXIT,0,0);

	return 0;
}



// CDlgMotorMoveWait 对话框
IMPLEMENT_DYNAMIC(CDlgMotorMoveWait, CDialog)

CDlgMotorMoveWait::CDlgMotorMoveWait(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgMotorMoveWait::IDD, pParent)
{

}

CDlgMotorMoveWait::~CDlgMotorMoveWait()
{
}

void CDlgMotorMoveWait::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMotorMoveWait, CDialog)
	ON_MESSAGE(WM_MOTORMOVEPROC_EXIT,OnMotorMoveExit)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgMotorMoveWait 消息处理程序
BOOL CDlgMotorMoveWait::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	AfxBeginThread(MotorMove_Process,(LPVOID)this,THREAD_PRIORITY_NORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMotorMoveWait::SetData(int nMoveModel, int nAxisNo, double dbMoveValue, int nMoveType )
{
	m_nMoveModel = nMoveModel;
	m_nAxisNo = nAxisNo;
	m_dbMoveValue = dbMoveValue;
	m_nMoveType = nMoveType;
}

void CDlgMotorMoveWait::MotorMove()
{
	switch ( m_nMoveModel )
	{
	case 0://绝对
		g_Motion.AMove(m_nAxisNo,m_dbMoveValue,m_nMoveType);
		break;
		
	case 1://相对
		g_Motion.RMove(m_nAxisNo,m_dbMoveValue,m_nMoveType);
		break;

	case 2://回零
		if ( g_Motion.Home(m_nAxisNo,FALSE) )
		{
			m_bHome = TRUE;
		}
		break;

	case 3://回软件原点
		{
			g_Motion.GoSoftHome();
		}
		break;
	}
}


LRESULT CDlgMotorMoveWait::OnMotorMoveExit(WPARAM wParam,LPARAM lParam)
{
	::Sleep(50);

	CDialog::OnCancel();
	return 0;
}



void CDlgMotorMoveWait::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_bProcess )
		return;
	
	CDialog::OnClose();
}


