#pragma once
#include "NodePosCali.h"

// CDlgObjPosCali 对话框

class CDlgObjPosCali : public CDialog
{
	DECLARE_DYNAMIC(CDlgObjPosCali)

public:
	CDlgObjPosCali(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgObjPosCali();

// 对话框数据
	enum { IDD = IDD_DIALOG_OBJ_POSCALI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_nCaliType;

	CNodePosCali *m_pNode;

	void Apply();
	void SetInfo(CNode* pNode);
};
