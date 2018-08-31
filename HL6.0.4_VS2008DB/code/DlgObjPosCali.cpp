// DlgObjPosCali.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgObjPosCali.h"


// CDlgObjPosCali �Ի���

IMPLEMENT_DYNAMIC(CDlgObjPosCali, CDialog)

CDlgObjPosCali::CDlgObjPosCali(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgObjPosCali::IDD, pParent)
	, m_nCaliType(0)
{
	m_pNode = NULL;
}

CDlgObjPosCali::~CDlgObjPosCali()
{
}

void CDlgObjPosCali::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CALITYPE, m_nCaliType);
}


BEGIN_MESSAGE_MAP(CDlgObjPosCali, CDialog)
END_MESSAGE_MAP()


// CDlgObjPosCali ��Ϣ�������
void CDlgObjPosCali::Apply()
{
	if ( m_pNode == NULL || !m_pNode->IsSelect())
		return;

	UpdateData(TRUE);

	m_pNode->m_nCaliType = m_nCaliType;
}


void CDlgObjPosCali::SetInfo(CNode* pNode)
{
	if ( !pNode )
		return;
	
	if ( pNode->GetType()  !=  NODE_POSCALI )
		return;
	
	m_pNode = (CNodePosCali *)pNode;
	
	m_nCaliType = m_pNode->m_nCaliType;
	UpdateData(FALSE);
}