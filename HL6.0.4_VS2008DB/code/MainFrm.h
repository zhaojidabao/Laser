// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__B8BC184B_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_MAINFRM_H__B8BC184B_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "xcBar.h"
#include "ChangDlg.h"
#include "ProperDlg.h"
#include "MacroDefine.h"
#include "ObjectListDlg.h"
#include "StatusMsgBar.h"
#include "FillEditDlg.h"
#include "KeyBind.h"
#include "XStatusBar.h"
#include "AniButton.h"
#include "OXTabClientWnd.h"

#define _SCB_REPLACE_MINIFRAME

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

	class CExtStatusBarProgressCtrl : public CProgressCtrl
	{
		virtual LRESULT WindowProc(    
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam
			)
		{
			if ( uMsg == WM_TIMER )
			{
				StepIt();
			}
			if ( uMsg == WM_DESTROY )
			{
				KillTimer(0);
			}
			LRESULT lResult = CProgressCtrl::WindowProc(uMsg, wParam, lParam);
			return lResult;
		}
		
		virtual void PostNcDestroy()
		{
			delete this;
		}
	};

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual CDocument* GetActiveDocument();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateProperty(BOOL bNoDoc = FALSE);
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CChangDlg       m_PSDlg;
	CProperDlg      m_PropDlg;
	CStatusMsgBar   m_StatusBar;
	CAniButton     *m_pWndButton;

#ifdef _SPECIALPROP_EDIT
	CObjectListDlg  m_ObjectListDlg;
	CFillEditDlg    m_FillEditDlg;
#endif

public:

    void RefreshCmdSet();
	void AddStatusButton();

	void SetPos(double dbX,double dbY,double dbZ);
	void SetSize(double dbSizeX,double dbSizeY);
	void SetUserInfo();
	void SetLengthInfo(double length);
	void SetMousePosInfo(double x, double y);
	void OpenDoor();

	void StopAvi();

	int  SetObjecSel(CGroupObj *pGroupObj);
	void SetAllObjecUnSel();

	CToolBar	m_wndDrawBar;//画图工具栏
	CToolBar	m_wndZoomBar;//缩放工具栏

protected:
	CToolBar    m_wndToolBar;//视图工具栏
	CToolBar	m_wndChangeBar;
	CToolBar	m_wndArrangeBar;//对齐排列工具条
	CToolBar    m_wndMotion;    //运动控制工具条
	XControlBar m_XBarPos;
	XControlBar m_XBarProperty;
	CXStatusBar m_wndStatusBar;

	CKeyBind    m_keyBind;

#ifdef _SPECIALPROP_EDIT
	XControlBar m_XObjectlist;
	XControlBar m_XDBSpecialty;
#endif
	
	COXTabClientWnd m_WndTab;
	CView   *m_pView;
	BOOL     m_bActivate;
	CToolBar m_wndDisplay;
	CToolBar m_wndSort;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnDisplay();
	afx_msg void OnUpdateDisplay(CCmdUI* pCmdUI);
	afx_msg void OnHide();
	afx_msg void OnUpdateHide(CCmdUI* pCmdUI);
	afx_msg void OnKeybindSet();
	afx_msg void OnDestroy();
	afx_msg void OnSetCalibrate();
	afx_msg void OnSetLaserPara();
	afx_msg void OnAdjustM();
	afx_msg void OnUpdateSetCalibrate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetLaserpara(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAdjustM(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKeybindSet(CCmdUI* pCmdUI);
	afx_msg void OnChangeuser();
	afx_msg void OnRightmng();
	afx_msg void OnLogoutUser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnDisplayInfor(WPARAM wpara = 0,LPARAM lpara = 0);
	afx_msg LRESULT OnDispCardStatus(WPARAM wpara = 0,LPARAM lpara = 0);

	// 主界面启动刷新
	afx_msg LRESULT OnStartMachine(WPARAM wpara = 0,LPARAM lpara = 0);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:	
	afx_msg void OnLangEnglish();
	afx_msg void OnUpdateLangEnglish(CCmdUI *pCmdUI);
	afx_msg void OnLangChineses();
	afx_msg void OnUpdateLangChineses(CCmdUI *pCmdUI);
	afx_msg void OnLangChineset();
	afx_msg void OnUpdateLangChineset(CCmdUI *pCmdUI);
	afx_msg void OnRunHlex();
	afx_msg BOOL OnQueryEndSession();

private:
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);

public:
	
	BOOL m_bStartCalibrate;
	CString m_strProcessName;
	HICON m_hicGreen;
	HICON m_hicRed;
	
	BOOL StartWorkConsoleDlg();
	void StartProcess(CString strProcessName);
	void SetCalibrate();

	//
	void SetMotorPos(int nAxis, LONGLONG nPos);
	void SetMotorLeftFlag(BOOL bFlag);
	void SetMotorRightFlag(BOOL bFlag);
	void SetMarkTime( double dbMarkTime );
	void CheckMotorStatus();
	void LanguageChange();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B8BC184B_DE24_11D3_845D_444553540001__INCLUDED_)
