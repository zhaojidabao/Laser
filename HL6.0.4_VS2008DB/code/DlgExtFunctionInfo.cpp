// DlgExtFunctionInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgExtFunctionInfo.h"
#include "DlgChangeObjName.h"


// CDlgExtFunctionInfo 对话框

IMPLEMENT_DYNAMIC(CDlgExtFunctionInfo, CDialog)

CDlgExtFunctionInfo::CDlgExtFunctionInfo(CWnd* pParent  /*= NULL*/)
	: CDialog(CDlgExtFunctionInfo::IDD, pParent)
	, m_strCardID(_T(""))
	, m_strCardName(_T(""))
{
	m_bModify = FALSE;
}

CDlgExtFunctionInfo::~CDlgExtFunctionInfo()
{
}

void CDlgExtFunctionInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_listctrlInfo);
	DDX_Text(pDX, IDC_EDIT_CARDID, m_strCardID);
	DDX_Text(pDX, IDC_EDIT_CARDNAME, m_strCardName);
}


BEGIN_MESSAGE_MAP(CDlgExtFunctionInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgExtFunctionInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, &CDlgExtFunctionInfo::OnBnClickedButtonRename)
END_MESSAGE_MAP()



BOOL CDlgExtFunctionInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
			
	GetCardName();  //获取控制卡名称
	InitListInfo(); //初始化信息列表
	
	//
	return TRUE;
}




void CDlgExtFunctionInfo::InitListInfo()
{
	m_listctrlInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	
	CString str;
	str.LoadString(IDS_EXTFUNCTION_DESCRIBE_NO);
	m_listctrlInfo.InsertColumn(0,str,LVCFMT_LEFT,70);
	str.LoadString(IDS_EXTFUNCTION_DESCRIBE);
	m_listctrlInfo.InsertColumn(1,str,LVCFMT_LEFT,300);
	str.LoadString(IDS_EXTFUNCTION_DESCRIBE_ENABLE);
	m_listctrlInfo.InsertColumn(2,str,  LVCFMT_CENTER,70);
	
	//灰度位图打标
	m_listctrlInfo.InsertItem(0, _T("0"), 0);
	str.LoadString(IDS_EXTFUNCTION_BMPMARK);
	m_listctrlInfo.SetItemText(0, 1, str);
	
	if ( g_DAT.m_btExtendInfo.FunData.bGrayMark )
		str.LoadString(IDS_EXTFUNCTION_ABLE_ENABLE);
	else
		str.LoadString(IDS_EXTFUNCTION_ABLE_DISABLE);
	m_listctrlInfo.SetItemText(0, 2, str);
	
	//运动控制及旋转打标
	m_listctrlInfo.InsertItem(1, _T("1"), 0);
	str.LoadString(IDS_EXTFUNCTION_MOTION);
	m_listctrlInfo.SetItemText(1, 1, str);
	
	if (  g_DAT.m_btExtendInfo.FunData.bMotorMark )
		str.LoadString(IDS_EXTFUNCTION_ABLE_ENABLE);
	else
		str.LoadString(IDS_EXTFUNCTION_ABLE_DISABLE);
	m_listctrlInfo.SetItemText(1, 2, str);
	
	//高精度校正
	m_listctrlInfo.InsertItem(2, _T("2"), 0);
	str.LoadString(IDS_EXTFUNCTION_PRECISIONCALIBRATE);
	m_listctrlInfo.SetItemText(2, 1, str);
	
	if ( g_DAT.m_btExtendInfo.FunData.bAdvCali )
		str.LoadString(IDS_EXTFUNCTION_ABLE_ENABLE);
	else
		str.LoadString(IDS_EXTFUNCTION_ABLE_DISABLE);
	m_listctrlInfo.SetItemText(2, 2, str);
	
	//一机多卡切换
	m_listctrlInfo.InsertItem(3, _T("3"), 0);
	str.LoadString(IDS_EXTFUNCTION_MULTICARD);
	m_listctrlInfo.SetItemText(3, 1, str);
	
	if ( g_DAT.m_btExtendInfo.FunData.bMulCard )
		str.LoadString(IDS_EXTFUNCTION_ABLE_ENABLE);
	else
		str.LoadString(IDS_EXTFUNCTION_ABLE_DISABLE);
	m_listctrlInfo.SetItemText(3, 2, str);

	//3D打标
	m_listctrlInfo.InsertItem(4, _T("4"), 0);
	str.LoadString(IDS_EXTFUNCTION_3DMARK);
	m_listctrlInfo.SetItemText(4, 1, str);

	if ( g_DAT.m_btExtendInfo.FunData.bNormal3d )
		str.LoadString(IDS_EXTFUNCTION_ABLE_ENABLE);
	else
		str.LoadString(IDS_EXTFUNCTION_ABLE_DISABLE);
	m_listctrlInfo.SetItemText(4, 2, str);
}



// CDlgExtFunctionInfo 消息处理程序
void CDlgExtFunctionInfo::OnBnClickedOk()
{
	if ( TRUE == m_bModify )
	{
		UpdateData(TRUE);
		TCHAR szbuff[MAX_PATH] = { 0 };
		_stprintf_s(szbuff, MAX_PATH,_T("%s"), m_strCardName);
		::WritePrivateProfileString(_T("CardName"), g_DAT.m_strCardKey, szbuff, g_sys.m_strCardFile);
	}
	
	OnOK();
}




void CDlgExtFunctionInfo::GetCardName()
{
	m_strCardID = g_DAT.m_strCardKey;

	TCHAR szbuff[MAX_PATH] = { 0 };
	::GetPrivateProfileString(_T("CardName"), g_DAT.m_strCardKey, m_strCardID,szbuff, MAX_PATH, g_sys.m_strCardFile);
	m_strCardName = szbuff;

	UpdateData(FALSE);
}




void CDlgExtFunctionInfo::OnBnClickedButtonRename()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgChangeObjName dlg;
	dlg.m_strName = m_strCardName;
	if ( dlg.DoModal() == IDOK )
	{
		m_strCardName = dlg.m_strName;
		UpdateData(FALSE);
		m_bModify = TRUE;
	}
}
