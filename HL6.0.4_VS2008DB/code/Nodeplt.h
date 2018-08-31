// NodePlt.h: interface for the CNodePlt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEPLT_H__82862A44_4819_11D4_A483_00105A615525__INCLUDED_)
#define AFX_NODEPLT_H__82862A44_4819_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeV.h"
#include "Nodeline.h"
#include "Property.h"	// Added by ClassView

class CNodePlt : public CNodeV  
{
public:
	CNodePlt();
	virtual ~CNodePlt();

public:
	virtual	BOOL SetLayerSelect(int nItem, BOOL bFlag);
	virtual	void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	virtual	BOOL CanUnGroup();
	virtual	void LoadWheelList(int nType, StrokeList *pListStroke);
	virtual	CTree * UnGroup();
	CNodeLine* UnGroupplt(); 

	virtual CNode*	CreateNewNode(){
		CNode *pNode = new CNodePlt();
		return pNode;
	};   

private:
	void	CreateCircle(CDot dotCenter, double dbRadius);
	void	CreatePen(int pen);
	void	AddStroke();
	
	CFile   		m_file;				//ָ���ļ���ָ��
	BOOL			m_bPen;				//���ʱ�ʶ
	CDot			m_dotCur;
	CStroke*	m_pStroke;			//��ǰ�ʻ�

	BOOL        m_bPenUp;			//��ʱ�ʶ
	BOOL        m_bCoorA;			//���������ʶ
	BOOL        m_bCircle;			//��Բ��ʶ
	BOOL		m_bFloat;			//��������

	//----------------------------------------------------
public:
	BOOL  DeCode(CString file,BOOL bCenter,BOOL bBetter);
	void   DecodeBuf( BYTE *pBuf, UINT nSize );
	void ProCommand( CString &strData );
	void ProCoord( CString &strData );
	int ReadValue( CString &strData, double &dbValue );
	
	BOOL HandleBuffSeg( BYTE *pBuff, UINT &nIndex, UINT nSize, CString &strPack );

	char  *m_pBufSave;
	UINT  m_nSaveIndex;
	UINT m_nBufSaveSize;
	int	  m_nPenNo;
	double m_dbRadius;
};

#endif // !defined(AFX_NODEPLT_H__82862A44_4819_11D4_A483_00105A615525__INCLUDED_)
