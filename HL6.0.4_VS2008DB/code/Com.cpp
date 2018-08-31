// Com.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "Com.h"


// CCom 对话框

IMPLEMENT_DYNAMIC(CCom, CDialog)

CCom::CCom(CWnd* pParent /*=NULL*/)
	: CDialog(CCom::IDD, pParent)
	, m_bUsed(FALSE)
{

}

CCom::~CCom()
{
}

void CCom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bUsed);
	DDX_Control(pDX, IDC_COMBO1, m_ccom);
	DDX_Control(pDX, IDC_COMBO2, m_cBandRate);
	DDX_Control(pDX, IDC_COMBO3, m_cDataBit);
	DDX_Control(pDX, IDC_COMBO4, m_cCheckBit);
	DDX_Control(pDX, IDC_COMBO5, m_cStopBit);
}


BEGIN_MESSAGE_MAP(CCom, CDialog)
END_MESSAGE_MAP()


// CCom 消息处理程序
