// QueryDBDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "QueryDBDlg.h"
#include "AdoDb.h"
#include "GlobalIni.h"


// CQueryDBDlg �Ի���

IMPLEMENT_DYNAMIC(CQueryDBDlg, CDialog)

CQueryDBDlg::CQueryDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryDBDlg::IDD, pParent)
	, m_strRes(_T(""))
	, m_nCount(0)
{

}

CQueryDBDlg::~CQueryDBDlg()
{
}

void CQueryDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_strRes);
	DDX_Text(pDX, IDC_EDIT1, m_nCount);
}


BEGIN_MESSAGE_MAP(CQueryDBDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, &CQueryDBDlg::OnBnClickedButtonReplace)
END_MESSAGE_MAP()


// CQueryDBDlg ��Ϣ�������

BOOL CQueryDBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDBPara( );
	//CGlobalIni::GetInstance()->SetDBPara(m_IniPara);

	CString strMsg;
	if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	{ 
		if (!m_adoDB.ConnectToDB(m_IniPara, TRUE))
		{
 			//strMsg.LoadString(_T("δ���ӵ����ݿ⣡"));
 			AfxMessageBox(_T("δ���ӵ����ݿ⣡"));
			return FALSE;
		}
	}	

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CQueryDBDlg::OnBnClickedButtonReplace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strSQL;
	CStringArray array;


	strSQL.Format(_T("SELECT * FROM [%s]"), m_strTable);
	CString strMsg;
	if (!m_adoDB.OpenSQLQuery((_bstr_t)strSQL, Recordset))
	{
		//strMsg.LoadString(_T("δ��ѯ�����ݣ�"));
		AfxMessageBox(_T("δ��ѯ�����ݣ�"));
		return;
	}
	
 	int i = 0;
	int nRaw = Recordset.GetRecordsCount();
	
	if(Recordset.IsBOF())
	{
		return;
	}

	Recordset.MoveFirst();

	while(!Recordset.IsEOF())
	{
		Recordset.GetRecordValue(array);
		CString strRes = array.GetAt(0);
		//�滻
 		CString strModelName;
 		strModelName.Format(_T("%d"), i);
 		if (!m_pChain->TextReplace(strRes, strModelName))
 		{
 			CString strErr; 
 			strErr.Format(_T("�滻����[%s]ʧ��"),strModelName);
 			::AfxMessageBox(strErr);
 			return;
 		}
		i++;
		if (m_nCount == i) break;
		m_strRes+= strRes + _T("\r\n");

		Recordset.MoveNext();
		
		
	}
	//strSQL.Format(_T("INSERT INTO [%s]([MarkCode], [MarkTime]) values(9��\'abc\', 5)"), 
	//	m_strTable);
	//if(!m_adoDB.ExecuteSQLQuery((_bstr_t)strSQL))
	//	AfxMessageBox(_T("д�����ݿ�ʧ��"));
	m_pDoc->UpdateAllViews(NULL);
	UpdateData(FALSE);
}


void CQueryDBDlg::GetDBPara()
{
	int nType = 0;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("TYPE"),nType,driverACCESSXP);
	m_IniPara.nDriverType = (BYTE)nType;

	CString strResult;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("NAME"),strResult,_T(""));
	m_IniPara._Database = (_bstr_t)(LPCTSTR)strResult;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("PWD"),strResult,_T(""));
	m_IniPara._Pwd = (_bstr_t)(LPCTSTR)strResult;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("USER"),strResult,_T(""));
	m_IniPara._User = (_bstr_t)(LPCTSTR)strResult;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("SERVER"),strResult,_T(""));
	m_IniPara._Server = (_bstr_t)(LPCTSTR)strResult;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("TABLENAME"),m_strTable,_T("Test"));
}