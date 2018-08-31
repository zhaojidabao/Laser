
// DataBaseToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataBaseTool.h"
#include "DataBaseToolDlg.h"
#include "DataSourceDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataBaseToolDlg 对话框




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


// CDataBaseToolDlg 消息处理程序

BOOL CDataBaseToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_HIDE);

	// TODO: 在此添加额外的初始化代码
	TCHAR szPath[MAX_PATH] = {0};                                                                 //.ini 文件的创建和读取
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	m_strPath = strPath + _T("\\SetCfg.ini");                                               //m_strPath 路径的定义        

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


	//绘制表格,网格线； 绘制表格,网格线（设置扩展风格）
	// 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDataBaseToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDataBaseToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDataBaseToolDlg::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	CString strName;
	m_ComName.GetWindowText(strName);
	if (strName.IsEmpty())
	{
		AfxMessageBox(_T("错误:表名为空"));
		return;
	}

	CString strSQL;
	strSQL.Format(_T("select  *  from  %s where  LaserTooling = 'p02a197b-201' "), strName);
	m_pCheckCode->DisConnect();
	if(m_pCheckCode->Connect() == DB_DISCONNECT)
	{
		AfxMessageBox(_T("数据库连接失败。"));
		return;
	}
	CStringArray array;
	CStringArray arrayName;
	CAdoRecordset Recordset;
	if(!m_pCheckCode->ExecuteSQL(strSQL,Recordset))
	{
		AfxMessageBox(_T("查询数据库为空"));
		return;
	}
	int nRow = Recordset.GetFieldsCount();
	int i = 0;
	if(Recordset.IsBOF())
	{
		AfxMessageBox(_T("没有匹配数据"));
		return;
	}
	Recordset.GetName(arrayName);
	SetShowDate(arrayName);
	Recordset.MoveFirst();
	while(!Recordset.IsEOF())
	{
		Recordset.GetRecordValue(array);
		//序号
		CString strIndex;
		strIndex.Format(_T("%d"),i+1);
		m_listData.InsertItem(i, strIndex);//插入行的首个元素
		int nItem = array.GetSize();
		if (m_nItem>nItem)
		{
			m_nItem = nItem;
		}
		for (int j=0; j<m_nItem; j++)
		{
			m_listData.SetItemText(i, j+1, array.GetAt(j));//插入行元素
		}
		i++;
		if (i>1000)
		{
			AfxMessageBox(_T("数据量大于10万"));
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
			m_listData.InsertColumn(i, _T("编号"), LVCFMT_CENTER, 60);
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
	// TODO: 在此添加控件通知处理程序代码
	CString strSql;
	GetDlgItemText(IDC_EDIT_DateSQL, strSql);
	if (strSql.IsEmpty())
	{
		AfxMessageBox(_T("错误:空语句"));
		return;
	}
	m_pCheckCode->DisConnect();
	if(m_pCheckCode->Connect() == DB_DISCONNECT)
	{
		AfxMessageBox(_T("数据库连接失败。"));
		return;
	}
	if(m_pCheckCode->ExecuteDateSQL(strSql))
	{
		AfxMessageBox(_T("执行语句失败:可能语法错误"));
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
		AfxMessageBox(_T("数据库连接失败。"));
		return;
	}
	CStringArray array;
	array.RemoveAll();
	if (!m_pCheckCode->GetName(array))
	{
		AfxMessageBox(_T("空数据表"));
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
	// TODO: 在此添加控件通知处理程序代码
	if (m_listData.GetItemCount() < 1)
	{
		AfxMessageBox(_T("数据项为空"));
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
		AfxMessageBox(_T("连接数据库失败"));
		return ;
	}

	CString strName;
	m_ComName.GetWindowText(strName);
	if (strName.IsEmpty())
	{
		AfxMessageBox(_T("表名为空"));
		return;
	}

	CString strSQL;
	strSQL.Format(_T("DROP  table  %s "), strName);
	AddCode.ExecuteDateSQL(strSQL);		// 删除旧表

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
	AddCode.ExecuteDateSQL(strSQL);	// 创建新表

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
	// TODO: 在此添加控件通知处理程序代码
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
	strSQL.Format(_T("线程：%d---OK"), nLin);
	AfxMessageBox(strSQL);
}
