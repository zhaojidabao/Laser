// DataSourceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataSourceDlg.h"

// CDataSourceDlg �Ի���

IMPLEMENT_DYNAMIC(CDataSourceDlg, CDialog)

CDataSourceDlg::CDataSourceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataSourceDlg::IDD, pParent)
	, m_nType(0)
	, m_strDsn(_T(""))
	, m_strMdb(_T(""))
	, m_strServer(_T(""))
	, m_strUserName(_T(""))
	, m_strPwd(_T(""))
{

}

CDataSourceDlg::~CDataSourceDlg()
{
}

void CDataSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DSN, m_strDsn);
	DDX_Text(pDX, IDC_EDIT_MDB, m_strMdb);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServer);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDX_CBIndex(pDX, IDC_COMBO_DATA, m_nType);
}


BEGIN_MESSAGE_MAP(CDataSourceDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA, &CDataSourceDlg::OnCbnSelchangeComboData)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDataSourceDlg::OnBnClickedButtonTest)
	ON_CBN_EDITCHANGE(IDC_COMBO_DATA, &CDataSourceDlg::OnCbnEditchangeComboData)
	ON_BN_CLICKED(IDC_BUTTON_MDB, &CDataSourceDlg::OnBnClickedButtonMdb)
	ON_BN_CLICKED(IDOK, &CDataSourceDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDataSourceDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDataSourceDlg ��Ϣ�������

BOOL CDataSourceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	CString strResult;
	m_nType = GetPrivateProfileInt(_T("DATABASE"), _T("TYPE"), driverACCESSXP, m_strPath);
	GetPrivateProfileString(_T("DATABASE"), _T("NAME"), _T(""), strResult.GetBuffer(10240), 10240, m_strPath);
	strResult.ReleaseBuffer();
	strResult.TrimLeft();
	if (m_nType==driverODBC)
	{
		m_strDsn = strResult;
	}
	else
	{
		m_strMdb = strResult;
	}

	GetPrivateProfileString(_T("DATABASE"), _T("PWD"), _T(""), strResult.GetBuffer(10240), 10240, m_strPath);
	strResult.ReleaseBuffer();
	strResult.TrimLeft();
	m_strPwd = strResult;

	GetPrivateProfileString(_T("DATABASE"), _T("USER"), _T(""), strResult.GetBuffer(10240), 10240, m_strPath);
	strResult.ReleaseBuffer();
	strResult.TrimLeft();
	m_strUserName = strResult;

	GetPrivateProfileString(_T("DATABASE"), _T("SERVER"), _T(""), strResult.GetBuffer(10240), 10240, m_strPath);
	strResult.ReleaseBuffer();
	strResult.TrimLeft();
	m_strServer = strResult;

	UpdateFrame();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDataSourceDlg::UpdateFrame(void)      //�Զ���
{
	GetDlgItem(IDC_EDIT_DSN)->EnableWindow(m_nType==driverODBC);
	GetDlgItem(IDC_EDIT_SERVER)->EnableWindow((m_nType==driverSQLServer)||(m_nType==driverORACLE));
	GetDlgItem(IDC_EDIT_MDB)->EnableWindow(m_nType!=driverODBC);
	GetDlgItem(IDC_BUTTON_MDB)->EnableWindow(m_nType>=driverACCESS97);
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(m_nType>=0);
	UpdateData(FALSE);								 //�Ի����ʼ������ֵ��
}

void CDataSourceDlg::OnCbnSelchangeComboData()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	UpdateFrame();
}

void CDataSourceDlg::OnBnClickedButtonTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);												//ȡֵ

	DBPARA Para;
	Para.nDriverType = m_nType;
	if (m_nType==driverODBC)
		Para._Database=_bstr_t(m_strDsn);
	else
		Para._Database=_bstr_t(m_strMdb);
		Para._Pwd  = _bstr_t(m_strPwd);
		Para._User = _bstr_t(m_strUserName);
		Para._Server = _bstr_t(m_strServer);

	BeginWaitCursor();
	CAdoDB adoDb;
	if (adoDb.ConnectToDB(Para))
		AfxMessageBox(_T("�������ӳɹ���"), MB_OK|MB_ICONQUESTION);
	else
		AfxMessageBox(_T("��������ʧ�ܣ�"), MB_OK|MB_ICONQUESTION);

	EndWaitCursor();
}

void CDataSourceDlg::OnCbnEditchangeComboData()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	UpdateFrame();	
}

void CDataSourceDlg::OnBnClickedButtonMdb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE, _T("MDB"), _T("*.mdb"), NULL, _T("Access Files(*.mdb)"), NULL);
	if (fileDlg.DoModal ()==IDOK)
	{
		m_strMdb = fileDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDataSourceDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	CString strTmp;
	if (m_nType==driverODBC)
	{
		strTmp =  (LPCTSTR)_bstr_t(m_strDsn);
	}
	else
	{
		strTmp =  (LPCTSTR)_bstr_t(m_strMdb);
	}

	WritePrivateProfileString(_T("DATABASE"), _T("NAME"), strTmp, m_strPath);
	strTmp.Format(_T("%d"), m_nType);
	WritePrivateProfileString(_T("DATABASE"), _T("TYPE"), strTmp, m_strPath);
	WritePrivateProfileString(_T("DATABASE"), _T("PWD"),   (LPCTSTR)_bstr_t(m_strPwd), m_strPath);
	WritePrivateProfileString(_T("DATABASE"), _T("USER"),   (LPCTSTR)_bstr_t(m_strUserName), m_strPath);
	WritePrivateProfileString(_T("DATABASE"), _T("SERVER"),   (LPCTSTR)_bstr_t(m_strServer), m_strPath);

	CDialog::OnOK();
}

void CDataSourceDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CDataSourceDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
