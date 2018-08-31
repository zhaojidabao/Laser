#pragma once
////////////////////////////////////////////////////////////////////
// 注意:CStatic派生的类响应onpaint后,不能再在父窗口中的OnCtrlColor和当前类的CtrlColor响应
//
/////////////////////////////////////////////////////////////////////

// CPicStatic

class CPicStatic : public CStatic
{
	DECLARE_DYNAMIC(CPicStatic)

public:
	CPicStatic();
	virtual ~CPicStatic();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();

	double m_dbRadius;
	double m_dbDistance;
};


