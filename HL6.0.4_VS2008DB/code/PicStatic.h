#pragma once
////////////////////////////////////////////////////////////////////
// ע��:CStatic����������Ӧonpaint��,�������ڸ������е�OnCtrlColor�͵�ǰ���CtrlColor��Ӧ
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


