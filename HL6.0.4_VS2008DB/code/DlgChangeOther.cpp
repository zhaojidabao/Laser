// DlgChangeOther.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgChangeOther.h"
#include "ExternViewDlg.h"
#include "baseMacro.h"

// CDlgChangeOther �Ի���

IMPLEMENT_DYNAMIC(CDlgChangeOther, CDialog)

CDlgChangeOther::CDlgChangeOther(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgChangeOther::IDD, pParent)
	, m_dbShearX(0)
	, m_dbShearY(0)
{
	m_bShellEdit = FALSE;
}

CDlgChangeOther::~CDlgChangeOther()
{
}

void CDlgChangeOther::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SHEARX, m_dbShearX);
	DDX_Text(pDX, IDC_SHEARY, m_dbShearY);
}


BEGIN_MESSAGE_MAP(CDlgChangeOther, CDialog)
	ON_BN_CLICKED(IDC_CENTER, &CDlgChangeOther::OnBnClickedCenter)
	ON_BN_CLICKED(IDC_BUTTON_EXTENDOBJ, &CDlgChangeOther::OnBnClickedButtonExtendobj)
	ON_EN_CHANGE(IDC_SHEARX, &CDlgChangeOther::OnEnChangeShearx)
	ON_EN_CHANGE(IDC_SHEARY, &CDlgChangeOther::OnEnChangeSheary)
END_MESSAGE_MAP()


// CDlgChangeOther ��Ϣ�������

void CDlgChangeOther::OnBnClickedCenter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;

	CHLDoc *pDoc = GetCurActiveDocument();
	if ( !pDoc )  return;

	if ( pDoc->HaveSelectObject() )
	{
		CQuad quad = pDoc->GetLimit();
		CDot dot;
		dot.x = (quad.left+quad.right)/2;
		dot.y = (quad.top+quad.bottom)/2;
		pDoc->Drag(dot,1,1,-dot.x,-dot.y);
	}
}

void CDlgChangeOther::OnBnClickedButtonExtendobj()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( !( g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		return;

	CHLView   *pView = (CHLView*)GetCurActiveView();
	if ( pView == NULL )
		return;

	CHLDoc    *pDoc  = pView->GetDocument();
	if ( pDoc == NULL )
		return;

	CExternViewDlg dlg;
	dlg.SetList(pDoc);
	dlg.DoModal();
	
	pView->SetFocus();
}

void CDlgChangeOther::OnEnChangeShearx()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_bShellEdit = TRUE;
}

void CDlgChangeOther::OnEnChangeSheary()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_bShellEdit = TRUE;
}
