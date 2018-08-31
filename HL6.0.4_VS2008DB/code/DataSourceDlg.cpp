// DataSourceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DataSourceDlg.h"
#include "GlobalIni.h"

// CDataSourceDlg 对话框

IMPLEMENT_DYNAMIC(CDataSourceDlg, CDialog)

CDataSourceDlg::CDataSourceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataSourceDlg::IDD, pParent)
	, m_strMdb(_T(""))
	, m_strTable(_T(""))
	, m_strServer(_T(""))
	, m_strUserName(_T(""))
	, m_strPwd(_T(""))
	, m_nType(0)
{

}

CDataSourceDlg::~CDataSourceDlg()
{
}

void CDataSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MDB, m_strMdb);
	DDX_Text(pDX, IDC_EDIT_TABLE, m_strTable);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServer);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDX_CBIndex(pDX, IDC_COMBO_DATA, m_nType);
}


BEGIN_MESSAGE_MAP(CDataSourceDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDataSourceDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_MDB, &CDataSourceDlg::OnBnClickedButtonMdb)
END_MESSAGE_MAP()


// CDataSourceDlg 消息处理程序

BOOL CDataSourceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDBPara( );
	// TODO:  在此添加额外的初始化
	m_nType = m_Para.nDriverType;

// 	if (m_nType==driverODBC)
// 		m_strDsn = (LPCTSTR)m_Para._Database;
// 	else
		m_strMdb = (LPCTSTR)m_Para._Database;

	m_strServer = (LPCTSTR)m_Para._Server;
	m_strUserName = (LPCTSTR)m_Para._User;
	m_strPwd = (LPCTSTR)m_Para._Pwd;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDataSourceDlg::OnBnClickedButtonTest()
{
	UpdateData(TRUE);

	m_Para.nDriverType = m_nType;

	//if (m_nType==driverODBC)
	//	m_Para._Database = _bstr_t(m_strDsn);
	//else
		m_Para._Database = _bstr_t(m_strMdb);

	m_Para._Pwd  = _bstr_t(m_strPwd);
	m_Para._User = _bstr_t(m_strUserName);
	m_Para._Server = _bstr_t(m_strServer);

	BeginWaitCursor();
	CAdoDB adoDb;
	if (adoDb.ConnectToDB(m_Para))
		AfxMessageBox(_T("测试连接成功"), MB_OK|MB_ICONQUESTION);
	else
		AfxMessageBox(_T("测试连接失败"), MB_OK|MB_ICONQUESTION);
	
	EndWaitCursor();
}

void CDataSourceDlg::OnBnClickedButtonMdb()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE, _T("MDB"), _T("*.mdb"), NULL, _T("Access Files(*.mdb)"), NULL);
	if (fileDlg.DoModal()==IDOK)
	{
		m_strMdb = fileDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDataSourceDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);

	m_Para.nDriverType = m_nType;

	//if (m_nType==driverODBC)
	//	m_Para._Database = _bstr_t(m_strDsn);
	//else
		m_Para._Database = _bstr_t(m_strMdb);

	m_Para._Pwd  = _bstr_t(m_strPwd);
	m_Para._User = _bstr_t(m_strUserName);
	m_Para._Server = _bstr_t(m_strServer);	

	SetDBPara( );
	CDialog::OnOK();
}

void CDataSourceDlg::GetDBPara()
{
	int nType = 0;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("TYPE"),nType,driverACCESSXP);
	m_Para.nDriverType = (BYTE)nType;

	CString strResult;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("NAME"),strResult,_T(""));
	m_Para._Database = (_bstr_t)(LPCTSTR)strResult;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("PWD"),strResult,_T(""));
	m_Para._Pwd = (_bstr_t)(LPCTSTR)strResult;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("USER"),strResult,_T(""));
	m_Para._User = (_bstr_t)(LPCTSTR)strResult;
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("SERVER"),strResult,_T(""));
	m_Para._Server = (_bstr_t)(LPCTSTR)strResult;
	//CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("USE"),m_bUseData,0);
	CGlobalIni::GetInstance()->ReadValue(_T("DATABASE"), _T("TABLENAME"),m_strTable,_T("markdata"));
}

void CDataSourceDlg::SetDBPara()
{
	CGlobalIni::GetInstance()->SaveValue(_T("DATABASE"), _T("TYPE"),(INT)m_Para.nDriverType);
	CString strTmp = (LPCTSTR)m_Para._Database;
	CGlobalIni::GetInstance()->SaveValue(_T("DATABASE"), _T("NAME"), strTmp);
	strTmp = (LPCTSTR)m_Para._Pwd;
	CGlobalIni::GetInstance()->SaveValue(_T("DATABASE"), _T("PWD"), strTmp);
	strTmp = (LPCTSTR)m_Para._User;
	CGlobalIni::GetInstance()->SaveValue(_T("DATABASE"), _T("USER"), strTmp);
	strTmp = (LPCTSTR)m_Para._Server;
	CGlobalIni::GetInstance()->SaveValue(_T("DATABASE"), _T("SERVER"), strTmp);
	//CGlobalIni::GetInstance()->SaveValue(_T("DATABASE"), _T("USE"),m_bUseData);
	CGlobalIni::GetInstance()->SaveValue(_T("DATABASE"), _T("TABLENAME"),m_strTable);
}