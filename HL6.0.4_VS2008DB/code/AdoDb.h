// AdoDb.h: interface for the CAdoDb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADODB_H__C30BD7A1_18DA_11D8_8AF2_000AE6A39D2F__INCLUDED_)
#define AFX_ADODB_H__C30BD7A1_18DA_11D8_8AF2_000AE6A39D2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "stdafx.h"
//Note:
// This class is designed for ado Database operating.
// Designed by BigDao 2003.12.07-2003.12.09

#pragma warning(disable:4146)
#include "wTypes.h"
#import "C:\Program Files\Common Files\System\ado\msado21.tlb" \
		no_namespace rename("EOF","adoEOF") \
		rename("BOF","adoBOF")
//��������
enum DriverTypeEmum{
	driverODBC = 0,
	driverSQLServer, 
	driverORACLE,
	driverACCESS97,
	driverACCESS2000,
	driverACCESSXP,
	driverEND
};

//����Ӱ�췶Χ
enum RowSelEnum{
	rowSelCur = 0,
	rowSelAll 
};

//���ݿ����Ӳ���
typedef struct DBPARA{
	BYTE  nDriverType;  //��������
	BYTE  nTimeOut;		//��ʱʱ��
	_bstr_t _Server;	//��������
	_bstr_t _User;		//�û���
	_bstr_t _Pwd;		//��¼����
	_bstr_t _Database;	//���ݿ���
	
	DBPARA::DBPARA(){
		::ZeroMemory(this,sizeof(DBPARA));
		nDriverType = driverSQLServer;
		nTimeOut    = 5;
	}

}DbPara;


typedef struct SQLPARA{

	_bstr_t _strSQL;
	unsigned int _nRecordNo;
	int _nMaxChar;	
	
	SQLPARA::SQLPARA(){
		::ZeroMemory(this,sizeof(SQLPARA));
	}

}SQLPARA;
/************************************************\
   ����ADO�����ݼ��࣬����ͨ�������ݼ�����
\************************************************/
class CAdoRecordset
{
public:
	CAdoRecordset(){
		::ZeroMemory((void*)this,sizeof(this));
		if (!CreateInstance()){
			//::OutputDebugString("CAdoRecordset::CAdoRecordset() CreateInitstance Failure!");
		};
	};
	virtual ~CAdoRecordset(){
		ReleaseInstance();
	};
	//////////////////////////////////////////
	void Update();
	BOOL CreateInstance();
	BOOL Delete(RowSelEnum rowSel=rowSelCur);
	void Attach(_RecordsetPtr _pRecordset);
	_RecordsetPtr Detach(){	return  m_pRecordset; };
	//////////////////////////////////////////
	void MoveFirst(){	m_pRecordset->MoveFirst();	};
	void MoveLast()	{	m_pRecordset->MoveLast();	};
	void MoveNext()	{	m_pRecordset->MoveNext();	};
	void MovePrev()	{	m_pRecordset->MovePrevious();};
	BOOL IsActive()	{	return m_pRecordset->State;	};
	void Close(){ if (IsActive()) m_pRecordset->Close(); };
	BOOL IsEOF(){ return m_pRecordset->adoEOF; };
	BOOL IsBOF(){ return m_pRecordset->adoBOF; };
	int  GetFieldsCount(){return m_pRecordset->Fields->GetCount();};
	int  GetRecordsCount(){return m_pRecordset->GetRecordCount();};
	BOOL GetFieldsName(CStringArray  &arryField,const int nId=-1);
	BOOL GetRecordValue(CStringArray &arryValue,const int nId=-1);
	BOOL ModifyRecord(const CStringArray &arryField,const CStringArray &arryValue);
	BOOL AddNewRecord(const CStringArray &arryField,const CStringArray &arryValue);
//	BOOL ModifyRecord(CIntArray 
	static CString VariantToCString(VARIANT var);
	static BOOL CStringToVariant(const CString strValue,int nType,VARIANT *var);
protected:
	int LocateFieldPos(CStringArray *parryAllField,CString strField);
	_RecordsetPtr m_pRecordset;
	BOOL ReleaseInstance();
};

/****************************************************\
   ����ADO�����ݿ⴦����,ÿ�������ֻ֧��һ�����ݿ�
\****************************************************/
class CAdoDB  
{
public:
	BOOL ExecuteSQLQuery(/*in*/_bstr_t _strSqlScript,CAdoRecordset &recordset,long* pAffectedRecord=NULL);
	CAdoDB();
	virtual ~CAdoDB();
	///////////////////////////////////
	BOOL BeginTrans();
	BOOL RollbackTrans();
	BOOL CommitTrans();
	void DisconnectDB();
	LPCTSTR  GetConnectionString(){return m_strSource;};
	BOOL IsDBConnected(){
		if (m_pConnection==NULL) return FALSE;
		return m_pConnection->GetState();
	};
	BOOL ExecuteSQLQuery(/*in*/_bstr_t _strSqlScript,long* pAffectedRecord=NULL);
	BOOL OpenSQLQuery(/*in*/_bstr_t _strSqlScript,/*out*/CAdoRecordset &Recordset);
	BOOL ConnectToDB(/*in*/const DBPARA &Para,/*in*/BOOL bCloseIfLive = FALSE);

protected:
	CString  m_strSource;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	BOOL CreateAdoInstance();
	void ReleaseAdoInstance();
};

#endif // !defined(AFX_ADODB_H__C30BD7A1_18DA_11D8_8AF2_000AE6A39D2F__INCLUDED_)
