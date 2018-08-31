


#pragma once


#include <math.h>

// CMat ����Ŀ��
class AFX_EXT_CLASS CMatrix
{
public:
	CMatrix();
	CMatrix(const CMatrix& src);
	virtual ~CMatrix();

	void operator = ( CMatrix & src );
	void Release();
	
	friend CMatrix operator +(CMatrix &m1, CMatrix &m2){ CMatrix mat; mat = m1.Add(m2); return mat; };
	friend CMatrix operator -(CMatrix &m1, CMatrix &m2){ CMatrix mat; mat = m1.Subtract(m2); return mat; };
	friend CMatrix operator *(CMatrix &m1, CMatrix &m2){ CMatrix mat; mat = m1.Multiply(m2); return mat; };	
	friend CMatrix operator *(CMatrix &m1, double &dbValue){ CMatrix mat; mat = m1.Multiply(dbValue); return mat; };
	friend CMatrix operator *(double &dbValue, CMatrix &m1){ CMatrix mat; mat = m1.Multiply(dbValue); return mat; };
	friend CDot operator *(CMatrix m1, CDot dot){ CDot ret; ret = m1.Multiply(dot); return ret; };
	
	int InitData(int nRow, int nCol);
	int InitData(int nRow, int nCol, double dataArray[]);
	double GetElement(int nRow, int nCol);
	void SetElement(int nRow, int nCol, double dbValue);
	
	//�������
	CMatrix Add(CMatrix other);
	
	//�������
	CMatrix Subtract(CMatrix other);
	
	//���������
	CMatrix Multiply(double value);
	
	//�������
	CMatrix Multiply(CMatrix &other);

	//���������������
	CDot Multiply(CDot dot);
	
	//�������
	BOOL CalInvertMat(CMatrix &mat);

	//��ӡ������Ϣ
	void OutputMatView(CString strInfo = _T(""));
	
public:
	int m_nRow;
	int m_nCol;
	
private:
	CArray<double, double> m_arrayData;
};


