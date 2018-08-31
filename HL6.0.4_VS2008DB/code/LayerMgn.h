#ifndef __LAYERMGN_H_
#define __LAYERMGN_H_

#include <afxtempl.h>

#define PIT_INT		0	//�����ֵ����
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

	CString m_strAlias;		//Ӣ����д����
	CString m_propName;		//����ȫ��
	CString m_curValue;		//��ǰֵ
	int		m_nPage;		//�ؼ�����ҳ��
	CString m_cmbItems;		//������
	int		m_nValueType;	//ֵ����0:int 1:double 2:BOOL
	double	m_dbMin;		//��ֵ
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

/* ����->����ҳ->����ҳ��*/
BOOL GetParaGroup(PARA_CFG_GROUP& arrParaGroup, const CString& strHLPath);	//nLang 0x804 0x404 0x409
BOOL SetParaGroup(PARA_CFG_GROUP& arrParaGroup, const CString& strHLPath);


#endif