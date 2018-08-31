#ifndef __LAYERMGN_H_
#define __LAYERMGN_H_

#include <afxtempl.h>

#define PIT_INT		0	//层参数值类型
#define PIT_DOUBLE	1

struct PARA_CFG
{
	PARA_CFG()
	{
		m_strAlias = _T("");
		m_propName = _T("");
		m_curValue = _T("");
		m_nPage = 0;
		m_cmbItems = _T("");
		m_nValueType = 0;
		m_dbMin = 0.0;
		m_dbMax = 0.0;
	}

	CString m_strAlias;		//英文缩写名称
	CString m_propName;		//属性全名
	CString m_curValue;		//当前值
	int		m_nPage;		//控件所在页号
	CString m_cmbItems;		//暂无用
	int		m_nValueType;	//值类型0:int 1:double 2:BOOL
	double	m_dbMin;		//极值
	double	m_dbMax;
};

struct PARA_PAGE
{
	CString m_strName;
	CArray<PARA_CFG, PARA_CFG&> m_arrCfg;

	void operator = (const PARA_PAGE& src)
	{
		m_strName = src.m_strName;
		m_arrCfg.Copy(src.m_arrCfg);
	}
};

typedef  CArray<PARA_PAGE, PARA_PAGE&> PARA_CFG_GROUP;

/* 参数->参数页->参数页组*/
BOOL GetParaGroup(PARA_CFG_GROUP& arrParaGroup, const CString& strHLPath);	//nLang 0x804 0x404 0x409
BOOL SetParaGroup(PARA_CFG_GROUP& arrParaGroup, const CString& strHLPath);


#endif