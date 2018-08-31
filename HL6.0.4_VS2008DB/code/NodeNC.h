// NodeNC.h: interface for the CNodeNC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODENC_H__3521A44_167_19D4_A425_00FD5A75CEQ__INCLUDED_)
#define AFX_NODENC_H__3521A44_167_19D4_A425_00FD5A75CEQ__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeV.h"
#include "Property.h"	// Added by ClassView

class CNodeNC : public CNodeV  
{
public:
	CNodeNC();
	virtual ~CNodeNC();

public:
	virtual BOOL SetLayerSelect(int nItem, BOOL bFlag);
	virtual void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual BOOL CanUnGroup();
	virtual void LoadWheelList(int nType, StrokeList *pListStroke);
	virtual  CTree * UnGroup();
	BOOL    DeCode(CString file, BOOL bCenter, BOOL bBetter);
	CNodeLine* UnGroupplt(); 

	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodeNC();
		return pNode;
	};

private:
	void CreateCircle(CDot dotCenter, double dbRadius);
	void Process(CString strData);					//��������
	void CreatePen(int pen);

	CStdioFileEx  m_file;				//ָ���ļ���ָ��
	BOOL			m_bFirst;			//�����ױ�
	CDot			m_dotCur;
	CStroke*	m_pStroke;			//��ǰ�ʻ�

	BOOL        m_bPenUp;			//��̧����������
	BOOL        m_bCoorA;			//���������ʶ
	BOOL        m_bCircle;			//��Բ��ʶ
	BOOL        m_bMMUint;          //���Ƴߴ� mm

};

#endif // !defined(AFX_NODENC_H__3521A44_167_19D4_A425_00FD5A75CEQ__INCLUDED_)
