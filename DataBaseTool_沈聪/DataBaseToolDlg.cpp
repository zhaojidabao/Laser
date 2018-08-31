
// DataBaseToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataBaseTool.h"
#include "DataBaseToolDlg.h"
#include "DataSourceDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataBaseToolDlg �Ի���




CDataBaseToolDlg::CDataBaseToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataBaseToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	::InitializeCriticalSection(&csWriteLog);
}

void CDataBaseToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Date, m_listData);
	DDX_Control(pDX, IDC_COMBO_Name, m_ComName);
}

BEGIN_MESSAGE_MAP(CDataBaseToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CDataBaseToolDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_Inquiry, &CDataBaseToolDlg::OnBnClickedButtonInquiry)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CDataBaseToolDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON4, &CDataBaseToolDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDataBaseToolDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CDataBaseToolDlg ��Ϣ�������

BOOL CDataBaseToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_HIDE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	TCHAR szPath[MAX_PATH] = {0};                                                                 //.ini �ļ��Ĵ����Ͷ�ȡ
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	m_strPath = strPath + _T("\\SetCfg.ini");                                               //m_strPath ·���Ķ���        

	CString strResult;
	int nType;
	nType = GetPrivateProfileInt(_T("DATABASE"), _T("TYPE"), driverACCESSXP, m_strPath);
	m_dbPara.nDriverType = (BYTE)nType;
	GetPrivateProfileString(_T("DATABASE"), _T("NAME"), _T(""), strResult.GetBuffer(10240), 10240, m_strPath);
	strResult.ReleaseBuffer();
	strResult.TrimLeft();
	m_dbPara._Database = (_bstr_t)(LPCTSTR)strResult;

	GetPrivateProfileString(_T("DATABASE"), _T("PWD"), _T(""), strResult.GetBuffer(10240), 10240, m_strPath);
	strResult.ReleaseBuffer();
	strResult.TrimLeft();
	m_dbPara._Pwd = (_bstr_t)(LPCTSTR)strResult;

	GetPrivateProfileString(_T("DATABASE"), _T("USER"), _T(""), strResult.GetBuffer(10240), 10240, m_strPath);
	strResult.ReleaseBuffer();
	strResult.TrimLeft();
	m_dbPara._User = (_bstr_t)(LPCTSTR)strResult;

	GetPrivateProfileString(_T("DATABASE"), _T("SERVER"), _T(""), strResult.GetBuffer(10240), 10240, m_strPath);
	strResult.ReleaseBuffer();
	strResult.TrimLeft();
	m_dbPara._Server = (_bstr_t)(LPCTSTR)strResult;
//	m_CheckCode.InitAdb(m_dbPara);
	m_pCheckCode = new CCheckDBCode;
	m_pCheckCode->InitAdb(m_dbPara);
	GetExcelName();
	m_listData.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	return TRUE; 


	//���Ʊ��,�����ߣ� ���Ʊ��,�����ߣ�������չ���
	// ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDataBaseToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDataBaseToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDataBaseToolDlg::OnBnClickedButtonSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���������������
	CDataSourceDlg dlg;
	m_pCheckCode->DisConnect();
	dlg.m_strPath = m_strPath;
 	if (IDOK==dlg.DoModal())
 	{
		m_dbPara.nDriverType = (BYTE)dlg.m_nType;
		if (dlg.m_nType==driverODBC)
			m_dbPara._Database=(_bstr_t)(LPCTSTR)dlg.m_strDsn;
		else
			m_dbPara._Database=(_bstr_t)(LPCTSTR)dlg.m_strMdb;
		m_dbPara._Pwd  = (_bstr_t)(LPCTSTR)dlg.m_strPwd;
		m_dbPara._User = (_bstr_t)(LPCTSTR)dlg.m_strUserName;
		m_dbPara._Server = (_bstr_t)(LPCTSTR)dlg.m_strServer;	

// 		TCHAR szPath[MAX_PATH] = {0};
// 		GetModuleFileName(NULL, szPath, MAX_PATH);
// 		ShellExecute(this->m_hWnd, _T("open"), szPath, NULL, NULL, SW_SHOW);
// 		exit(0);
// 		return;
		delete m_pCheckCode;
		m_pCheckCode = new CCheckDBCode;
		m_pCheckCode->InitAdb(m_dbPara);
		GetExcelName();
 	}
}



void CDataBaseToolDlg::OnBnClickedButtonInquiry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strName;
	m_ComName.GetWindowText(strName);
	if (strName.IsEmpty())
	{
		AfxMessageBox(_T("����:����Ϊ��"));
		return;
	}

	CString strSQL;
	strSQL.Format(_T("select  *  from  %s where  LaserTooling = 'p02a197b-201' "), strName);
	m_pCheckCode->DisConnect();
	if(m_pCheckCode->Connect() == DB_DISCONNECT)
	{
		AfxMessageBox(_T("���ݿ�����ʧ�ܡ�"));
		return;
	}
	CStringArray array;
	CStringArray arrayName;
	CAdoRecordset Recordset;
	if(!m_pCheckCode->ExecuteSQL(strSQL,Recordset))
	{
		AfxMessageBox(_T("��ѯ���ݿ�Ϊ��"));
		return;
	}
	int nRow = Recordset.GetFieldsCount();
	int i = 0;
	if(Recordset.IsBOF())
	{
		AfxMessageBox(_T("û��ƥ������"));
		return;
	}
	Recordset.GetName(arrayName);
	SetShowDate(arrayName);
	Recordset.MoveFirst();
	while(!Recordset.IsEOF())
	{
		Recordset.GetRecordValue(array);
		//���
		CString strIndex;
		strIndex.Format(_T("%d"),i+1);
		m_listData.InsertItem(i, strIndex);//�����е��׸�Ԫ��
		int nItem = array.GetSize();
		if (m_nItem>nItem)
		{
			m_nItem = nItem;
		}
		for (int j=0; j<m_nItem; j++)
		{
			m_listData.SetItemText(i, j+1, array.GetAt(j));//������Ԫ��
		}
		i++;
		if (i>1000)
		{
			AfxMessageBox(_T("����������10��"));
			break;
		}
		Recordset.MoveNext();
	}
	m_pCheckCode->DisConnect();
	arrayName.RemoveAll();
	array.RemoveAll();
}
void	CDataBaseToolDlg::SetShowDate(CStringArray& arrayName)
{
	m_listData.DeleteAllItems();
	m_ListHandeName.RemoveAll();
	int   nColumnCount   =   m_listData.GetHeaderCtrl()->GetItemCount();   
	for   (int   i=0;i   <   nColumnCount;i++)   
	{   
		m_listData.DeleteColumn(0);   
	}
	
	m_nItem = arrayName.GetSize();
	for (int i=0; i<=m_nItem; i++)
	{
		if (i == 0)
		{
			m_listData.InsertColumn(i, _T("���"), LVCFMT_CENTER, 60);
		}
		else
		{
			m_ListHandeName.Add(arrayName.GetAt(i-1));
			m_listData.InsertColumn(i, arrayName.GetAt(i-1), LVCFMT_CENTER, 60);
		}
	}
}

void CDataBaseToolDlg::OnBnClickedButtonRun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strSql;
	GetDlgItemText(IDC_EDIT_DateSQL, strSql);
	if (strSql.IsEmpty())
	{
		AfxMessageBox(_T("����:�����"));
		return;
	}
	m_pCheckCode->DisConnect();
	if(m_pCheckCode->Connect() == DB_DISCONNECT)
	{
		AfxMessageBox(_T("���ݿ�����ʧ�ܡ�"));
		return;
	}
	if(m_pCheckCode->ExecuteDateSQL(strSql))
	{
		AfxMessageBox(_T("ִ�����ʧ��:�����﷨����"));
		return;
	}
	AfxMessageBox(_T("OK"));
}

void	CDataBaseToolDlg::GetExcelName()
{
	m_ComName.ResetContent();

	m_pCheckCode->DisConnect();
	if(m_pCheckCode->Connect() == DB_DISCONNECT)
	{
		AfxMessageBox(_T("���ݿ�����ʧ�ܡ�"));
		return;
	}
	CStringArray array;
	array.RemoveAll();
	if (!m_pCheckCode->GetName(array))
	{
		AfxMessageBox(_T("�����ݱ�"));
	}

	for (int i=0; i<array.GetSize(); i++)
	{
		m_ComName.AddString(array.GetAt(i));
	}
	m_ComName.SetCurSel(0);
	m_pCheckCode->DisConnect();
}

//#define _DBDAOINT_H_
//#include <afxdao.h>
#include "MyCreateMde.h"
void CDataBaseToolDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_listData.GetItemCount() < 1)
	{
		AfxMessageBox(_T("������Ϊ��"));
		return;
	}

	CMyCreateMde dlg;
	CString strTemp = dlg.CreateMdbFile();

	DBPARA Para;
	Para.nDriverType = 3;
	Para._Database=_bstr_t(strTemp);
	Para._Pwd  = _bstr_t(_T(""));
	Para._User = _bstr_t(_T(""));
	Para._Server = _bstr_t(_T(""));

	CCheckDBCode AddCode;
	if(AddCode.InitAdb(Para) == 1)
	{
		AfxMessageBox(_T("�������ݿ�ʧ��"));
		return ;
	}

	CString strName;
	m_ComName.GetWindowText(strName);
	if (strName.IsEmpty())
	{
		AfxMessageBox(_T("����Ϊ��"));
		return;
	}

	CString strSQL;
	strSQL.Format(_T("DROP  table  %s "), strName);
	AddCode.ExecuteDateSQL(strSQL);		// ɾ���ɱ�

	strSQL.Format(_T("Create  table  %s( "), strName);
	int nLen = m_ListHandeName.GetCount();

	for (int i=0; i<nLen; i++)
	{
		CString strCount = m_ListHandeName.GetAt(i);
		strSQL += strCount;
		if (i == nLen-1)
		{
			strSQL += _T(" varchar(50) )");
		}
		else
		{
			strSQL += _T(" varchar(50), ");
		}	
	}
	AddCode.ExecuteDateSQL(strSQL);	// �����±�

	int nLen2 = m_listData.GetItemCount();

	strSQL.Format(_T("INSERT INTO %s ("), strName );
	for (int j=0; j<nLen; j++)
	{
		CString strCount = m_ListHandeName.GetAt(j);
		strSQL += strCount;
		if (j == nLen - 1)
		{
			strSQL += _T(" )");
		}
		else
		{
			strSQL += _T(", ");
		}	
	}
	strSQL += _T(" VALUES ");
	for (int i=0; i<nLen2; i++)
	{
		CString strSQL2 = _T("('");
		for (int j=0; j<nLen; j++)
		{
			CString strCount = m_listData.GetItemText(i, j+1);
			strSQL2 += strCount;
			if (nLen-1 == j)
			{
				strSQL2 += _T("') ");
			}
			else
			{
				strSQL2 += _T("', '");
			}	
		}
		strSQL2 = strSQL + strSQL2;
		AddCode.ExecuteDateSQL(strSQL2);
	}

	AfxMessageBox(_T("OK"));
}

void CDataBaseToolDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(int i=0; i<10; i++)
	{
		AfxBeginThread(FunThread, this);
	}
	
}

UINT FunThread(LPVOID plg)
{
	CDataBaseToolDlg* dlg = (CDataBaseToolDlg*)plg;
	dlg->StartFun();
	return 0;
}

void CDataBaseToolDlg::StartFun()
{
	CString strSQL;
	int nLin = GetCurrentThreadId();
	for (int i =0; i<10000000; i++)
	{
		::EnterCriticalSection(&csWriteLog);
		m_pCheckCode->DisConnect();
		if(m_pCheckCode->Connect() == DB_DISCONNECT)
		{
			::LeaveCriticalSection(&csWriteLog);
			Sleep(1000);
			continue;
		}

		CTime nTime = CTime::GetCurrentTime();
		CString  strTime = nTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		strSQL.Format(_T("INSERT INTO SerialZSHY(EndCustCode,PartNumber,LaserTooling,markedSerial,UniqueSegment,\
						 MachineID,Create_time,Operator,LotNo) VALUES('BSH','P04B947P-213-23','p02a197b-201',\
						 '%dAAAAAAAA%d','%dBBBBBBBBBB%d','A','%s','','')"), nLin, i, nLin, i, strTime);
		if(m_pCheckCode->ExecuteDateSQL(strSQL))
		{
			::LeaveCriticalSection(&csWriteLog);
			Sleep(1000);
			continue;
		}
		strSQL.Format(_T("INSERT INTO SerialZSHY(markedSerial, Create_time) VALUES(	'%dCCCCCCCCC%d','%s')"), nLin, -i, strTime);
		if(m_pCheckCode->ExecuteDateSQL(strSQL))
		{
			::LeaveCriticalSection(&csWriteLog);
			Sleep(1000);
			continue;
		}
		::LeaveCriticalSection(&csWriteLog);
		Sleep(10);
	}
	strSQL.Format(_T("�̣߳�%d---OK"), nLin);
	AfxMessageBox(strSQL);
}
