#pragma once
#include "NodePosCali.h"

// CDlgObjPosCali �Ի���

class CDlgObjPosCali : public CDialog
{
	DECLARE_DYNAMIC(CDlgObjPosCali)

public:
	CDlgObjPosCali(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgObjPosCali();

// �Ի�������
	enum { IDD = IDD_DIALOG_OBJ_POSCALI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	int m_nCaliType;

	CNodePosCali *m_pNode;

	void Apply();
	void SetInfo(CNode* pNode);
};
