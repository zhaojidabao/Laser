#pragma once

// CDataBase ����Ŀ��
class AFX_EXT_CLASS CDataBase : public CObject
{
public:
	CDataBase();
	virtual ~CDataBase();

	virtual void SetParent( CDataBase *pParent );
	virtual CDataBase* GetParent();

private:
	CDataBase *m_pParent;
};


