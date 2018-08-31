// DlgObjPrecisionSet.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgObjPrecisionSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgObjPrecisionSet dialog


CDlgObjPrecisionSet::CDlgObjPrecisionSet(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgObjPrecisionSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgObjPrecisionSet)
	m_dbPrecision = 1;
	m_nModelNo = 100;
	//}}AFX_DATA_INIT
}


void CDlgObjPrecisionSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjPrecisionSet)
	DDX_Text(pDX, IDC_EDIT_PRECISION, m_dbPrecision);
	DDV_MinMaxDouble(pDX, m_dbPrecision, 0.01, 9999999.);
	DDX_Text(pDX, IDC_EDIT_MODEL_NU, m_nModelNo);
	DDV_MinMaxInt(pDX, m_nModelNo, 1, 99999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjPrecisionSet, CDialog)
	//{{AFX_MSG_MAP(CDlgObjPrecisionSet)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjPrecisionSet message handlers
