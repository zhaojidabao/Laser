// AniButton.h: interface for the CAniButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIBUTTON_H__486B54E6_48B1_4F54_ACF2_773052C25AFE__INCLUDED_)
#define AFX_ANIBUTTON_H__486B54E6_48B1_4F54_ACF2_773052C25AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CAniButton : public CButton  
{
public:
	CAniButton();
	virtual ~CAniButton();

public:
    UINT m_nAniID;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAniButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitAviItem();
	void LoadAVI(UINT nAniID);
	void stop();
    void play();

	// Generated message map functions
protected:
    CAnimateCtrl m_AnimateCtrl;
    BOOL         m_bPlaying;
	//{{AFX_MSG(CAniButton)
	//}}AFX_MSG
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawButton(CDC* pDC, UINT nState, CRect rect);
    DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_ANIBUTTON_H__486B54E6_48B1_4F54_ACF2_773052C25AFE__INCLUDED_)
