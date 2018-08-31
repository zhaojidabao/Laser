// TestFocuslDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "LocaleFocuslDlg.h"
#include "MainFrm.h"

// CLocaleFocusDlg 对话框

IMPLEMENT_DYNAMIC(CLocaleFocusDlg, CDialog)

CLocaleFocusDlg::CLocaleFocusDlg(CWnd* pParent  /*= NULL*/)
	: CDialog(CLocaleFocusDlg::IDD, pParent)
	, m_dbZDerStep(0.05)
	, m_nSelect(10)
{
	m_pDoc = NULL;
	m_pMarkMgr = NULL;
}

CLocaleFocusDlg::~CLocaleFocusDlg()
{
	g_DAT.m_bEnableZOffset = TRUE;
}

void CLocaleFocusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TESTDIST, m_dbZDerStep);
	DDX_Text(pDX, IDC_EDIT_SETFOCUS, m_nSelect);
	DDV_MinMaxInt(pDX, m_nSelect, 1, 20);
}


BEGIN_MESSAGE_MAP(CLocaleFocusDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MARK, &CLocaleFocusDlg::OnBnClickedButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CLocaleFocusDlg::OnBnClickedButtonStop)

	ON_MESSAGE(WM_MARK_END,OnMarkEnd)
	ON_MESSAGE(WM_MARK_STOP,OnMarkStop)
	ON_MESSAGE(WM_OUTPUT_MARK_MSG,OnOutPutMsg)
	ON_BN_CLICKED(IDOK, &CLocaleFocusDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLocaleFocusDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLocaleFocusDlg 消息处理程序

BOOL CLocaleFocusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	m_pMarkMgr = new CMarkMgr();
	m_pMarkMgr->RunThread((LPVOID)GetSafeHwnd());
	m_pMarkMgr->SetChain(&(m_pDoc->m_chain));
	m_pMarkMgr->SetMarkType(0);

	g_DAT.OutStatus(O_ENT_MARK_ENV,O_ENT_MARK_ENV);
	g_DAT.m_hMarkWnd = GetSafeHwnd();
	g_DAT.m_bEnableZOffset = FALSE;

	m_bMarking = FALSE;
	UpdateView();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CLocaleFocusDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_bMarking)
	{	
		if ( WM_KEYDOWN  == pMsg->message )
		{
			if ( (GetKeyState(VK_RETURN) & 0x8000) || 
				(GetKeyState(VK_ESCAPE) & 0x8000) )
			{
				return TRUE;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CLocaleFocusDlg::UpdateView()
{
	GetDlgItem(IDC_EDIT_TESTDIST)->EnableWindow(!m_bMarking);
	GetDlgItem(IDC_EDIT_SETFOCUS)->EnableWindow(!m_bMarking);
	GetDlgItem(IDC_BUTTON_MARK)->EnableWindow(!m_bMarking);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bMarking);
	GetDlgItem(IDOK)->EnableWindow(!m_bMarking);
	GetDlgItem(IDCANCEL)->EnableWindow(!m_bMarking);
}




void CLocaleFocusDlg::OnBnClickedButtonMark()
{
	UpdateData();
	
	StrokeList strokeList;
	CGroupObj *pGroupObj = NULL;
	CStrokeB *pStroke = NULL;
	POSITION posStk = NULL;
	int i(0), n(0), iGroup(0);
	
	POSITION pos = m_pDoc->m_chain.m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_pDoc->m_chain.m_listGroup.GetNext(pos);
		pGroupObj->LoadStrokeList(&strokeList, FALSE, -1);
		
		posStk = strokeList.GetHeadPosition();
		while( posStk )
		{
			pStroke = strokeList.GetNext(posStk);
			n = pStroke->GetSize();
			
			for( i = 0; i<n; ++i )
				pStroke->m_dotArray[i].z = (iGroup-10)*m_dbZDerStep;
		}
		
		iGroup++;
		strokeList.RemoveAll();
	}
	
	//
	m_pMarkMgr->StartMark();
	m_bMarking = TRUE;
	UpdateView();
}




void CLocaleFocusDlg::OnBnClickedButtonStop()
{
	if ( WaitForSingleObject(hMarkSts, 0)  !=  WAIT_OBJECT_0 )
		return;

	m_pMarkMgr->StopMark();
	for( int i = 0; i<100; i++ )
	{
		if ( WAIT_TIMEOUT == WaitForSingleObject(hMarkSts, 20)  )
			break;
	}

	OnMarkEnd(0,0);
}

void CLocaleFocusDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	////////////////////////
	::g_SysCfg.m_dbZOffset = (m_nSelect-11)*m_dbZDerStep;
	::g_sys.WriteProfileDouble( _T("SysCfg"), _T("MarkZOFFSET"), ::g_SysCfg.m_dbZOffset );
	
	m_pMarkMgr->SetChain(NULL);
	m_pMarkMgr->StopThread();

	OnOK();
}

LRESULT CLocaleFocusDlg::OnMarkEnd(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
		case MARK_TEND:
			m_bMarking = FALSE;
			UpdateView();
			CMainFrame *pMFrame = (CMainFrame*)AfxGetMainWnd();
			if ( pMFrame  !=  NULL )
				pMFrame->SetMarkTime(g_DAT.GetMarkTime());
			break;
	}
	
	return 0;
}

LRESULT CLocaleFocusDlg::OnMarkStop(WPARAM wParam,LPARAM lParam)
{
	OnBnClickedButtonStop();
	return 0;
}

LRESULT CLocaleFocusDlg::OnOutPutMsg(WPARAM wParam,LPARAM lParam)
{
	UINT nType    = (UINT)((lParam & 0xFFFF0000) >> 16);
	UINT nValue   = (UINT)(lParam  & 0xFFFF);
	switch( nType )
	{
	case 1:
		{
			switch(nValue)
			{
			case 0:
				break;
			case 1:
				OnBnClickedButtonStop();
				break;
			case 2:
				m_pMarkMgr->PauseMark();
				break;
			case 3:
				m_pMarkMgr->ReMark();
				break;
			default:
				break;
			}
			break;
		}
	default:
		break;
	}

	return 0;
}

void CLocaleFocusDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pMarkMgr->SetChain(NULL);
	m_pMarkMgr->StopThread();
	
	OnCancel();
}
