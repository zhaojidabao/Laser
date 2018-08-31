// CheckDBCode.h: interface for the CCheckDBCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKDBCODE_H__357459B7_5F0B_4C41_B20B_9B7765991084__INCLUDED_)
#define AFX_CHECKDBCODE_H__357459B7_5F0B_4C41_B20B_9B7765991084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//������
enum DB_ERRORCODE
{
	DB_OK    = 0,//ִ����ȷ
	DB_DISCONNECT = 1,//DB�Ͽ�
	DB_SELECT_FAIL = 2,//DB��ѯʧ��
	DB_WRITE_FAIL = 3,//DBд��ʧ��
	DB_DELETE_FAIL = 4,//DBɾ��ʧ��
	DB_UPDATE_FAIL = 5,//DB����ʧ��
	DB_REPEATCODE = 6,//����
	DB_OTHER_ERROR = 7,//DB��������
	DB_END
};

#define CHECK_DB_RETURN_VALUE(nRetCode)\
if( 0 != nRetCode )\
{\
	switch(nRetCode){\
		case 1:\
		AfxMessageBox(IDS_STRING220);\
		break;\
		case 2:\
		AfxMessageBox(IDS_STRING221);\
		break;\
		case 3:\
		AfxMessageBox(IDS_STRING222);\
		break;\
		case 4:\
		AfxMessageBox(IDS_STRING223);\
		break;\
		case 5:\
		AfxMessageBox(IDS_STRING224);\
		break;\
		case 6:\
		AfxMessageBox(IDS_STRING225);\
		break;\
		case 7:\
		AfxMessageBox(IDS_STRING226);\
		break;\
		default:\
		AfxMessageBox(IDS_STRING227);\
		break;\
	}\
}\


#include "adodb.h"

class CCheckDBCode 
{
public:
	CCheckDBCode();
	virtual ~CCheckDBCode();

public:

	int InitAdbDBPARA(DBPARA paradb);
	//��ʼ�����ݿ�
	int InitAdb();
	//��ʼ�����ݿ����
	int InitAdb(DBPARA paradb);
	//д�����ݿ�
	int WriteToDb(CString strCode, CString strReadCode, CString strPatchID);
	///////////ɾ�����ݿ�
	int DelByDate(int nDay = 90);
	//ɾ����������
	int DelAllData();
	//ɾ���ض�����
	int DeleteSerial(CString strSerial);
	//�������
	int CheckCodeOK(CString strCode);
	//ȡ�û����ü����־λ
	BOOL GetCheckFlag() const { return m_bCheckCode; }
	void SetCheckFlag(BOOL bNewFalg) { m_bCheckCode = bNewFalg; }



	BOOL IsConnect();
	int Connect();
	int DisConnect();



	int OrcaleTest();

	int OrcaleSelect(CString strsbar);
	int OrcaleSelectLot(CString strLot, CString strPart);  // check �����Ϻ� 
	int OrcaleInsert(CString strloc,CString strSbar,CString strlotnum,CString strpartnum,CString strdouid);
private:
	//�Ƿ��������
	BOOL m_bCheckCode;
	//���ݿ����
	CAdoDB m_adoDB;
	//���ݿ����
	DBPARA  m_paradb;
	////��ȡִ��Ŀ¼
	void GetExepath(TCHAR *pPath);
	//ȡ��·��
	CString GetAppPath();
	
public:
	// ִ��sql���
	BOOL ExecuteSQL(CString strSQL,CAdoRecordset& rs);	// ��ѯ
	BOOL	ExecuteMarkData(CString SQL, CString& strBourd);
	BOOL ExecuteDateSQL(CString strSQL );		// ��ɾ��
	BOOL		GetName(CStringArray& arrayName );
};

#endif // !defined(AFX_CHECKDBCODE_H__357459B7_5F0B_4C41_B20B_9B7765991084__INCLUDED_)
