


#pragma once


#include <math.h>

// CMat 命令目标
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
	
	//矩阵相加
	CMatrix Add(CMatrix other);
	
	//矩阵相减
	CMatrix Subtract(CMatrix other);
	
	//矩阵的数乘
	CMatrix Multiply(double value);
	
	//矩阵相乘
	CMatrix Multiply(CMatrix &other);

	//矩阵与列向量相乘
	CDot Multiply(CDot dot);
	
	//求逆矩阵
	BOOL CalInvertMat(CMatrix &mat);

	//打印矩阵信息
	void OutputMatView(CString strInfo = _T(""));
	
public:
	int m_nRow;
	int m_nCol;
	
private:
	CArray<double, double> m_arrayData;
};


