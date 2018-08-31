#pragma once
#include "afxwin.h"
#include "HL.h"
#include "HLDoc.h"
#include <mmsystem.h>
#include "MainFrm.h"
// CDlg3DAlign �Ի���

class CDlg3DAlign : public CDialog
{
	DECLARE_DYNAMIC(CDlg3DAlign)

public:
	CDlg3DAlign(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg3DAlign();

// �Ի�������
	enum { IDD = IDD_DIALOG_3DALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAlign();
	virtual BOOL OnInitDialog();
	CHLDoc* m_pBaseDoc;
	double m_dbBaseX;
	double m_dbBaseY;
	double m_dbBaseZ;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonClose();
};
