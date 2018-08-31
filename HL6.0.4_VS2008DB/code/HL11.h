// HL.h : main header file for the HL application
//

#if !defined(AFX_HL_H__B8BC1847_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_HL_H__B8BC1847_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Stroke.h"
#include "RedMarkThread.h"
#include "SelMark.h"
#include "FastMark.h"
#include "DogCheck.h"
#include "DataMng.h"

/////////////////////////////////////////////////////////////////////////////
// CHLApp:
// See HL.cpp for the implementation of this class

class CHLApp : public CWinApp
{
public:
	CHLApp();
	
	void   StartCalibrate();
	void   RegistryFileType();
	BOOL InitCheckDog();
	UINT  LoadImageProcLib();
	BOOL	IsProcessRunAsAdmin();
	BOOL UserLoginCheck(); 
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHLApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CHLApp)
	afx_msg void OnFileOpen();
	afx_msg void OnAppAbout();
	afx_msg void ShowNewFlag();
	afx_msg void OnSetHardware();
	afx_msg void OnOption();
	afx_msg void OnFileMuldoc();
	afx_msg void OnMarkStat();
	afx_msg void OnCalibrate();
	afx_msg void OnMarkMul();
	afx_msg void OnGrayCurrent();
	afx_msg void OnGrayCompensate();
	afx_msg void OnSetTestio();
	afx_msg void OnDownMul();
	afx_msg void OnUpdateDownMul(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraycurrent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraycompensate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetHardware(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CSelMark		m_SelMark;
	CFastMark		m_FastMark;
	CRedMarkThread	m_RedPreView;
	HINSTANCE		m_hLangDLL;

private:
	HMODULE		m_hLib;

public:
	afx_msg void OnChangeCard();
	afx_msg void OnExtfunctionInfo();
	afx_msg void OnUpdateSelCard(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOption(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetTestio(CCmdUI *pCmdUI);
	afx_msg void OnEyeDisplay();
	afx_msg void OnHelpChs();
	afx_msg void OnHelpEnglish();
	afx_msg void OnUpdateMarkMul(CCmdUI *pCmdUI);
};

extern CDataMng  g_dataMng;
extern CDogCheck g_DogCheck;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HL_H__B8BC1847_DE24_11D3_845D_444553540001__INCLUDED_)
