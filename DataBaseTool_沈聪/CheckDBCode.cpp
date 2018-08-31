// CheckDBCode.cpp: implementation of the CCheckDBCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckDBCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckDBCode::CCheckDBCode()
{
	m_bCheckCode = TRUE;
}

CCheckDBCode::~CCheckDBCode()
{
	m_adoDB.DisconnectDB();
}

//��ʼ�����ݿ����
int CCheckDBCode::InitAdb()
{
	if (!m_bCheckCode)
		return DB_OK;

	m_adoDB.DisconnectDB();
	CString strPath = GetAppPath();
	strPath = strPath + _T("\\MarkData_V1.mdb");
	//DBPARA  paradb;
	m_paradb._Database		= _bstr_t(strPath);
	m_paradb._Server		= _bstr_t("127.0.0.1");
	m_paradb._User			= _bstr_t("");
	m_paradb._Pwd			= _bstr_t("");
	m_paradb.nDriverType	=  4;
	
	if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	{
		if (!m_adoDB.ConnectToDB(m_paradb,TRUE))
		{
			CString strMsg;
			strMsg = _T("���ݿ�����ʧ��.");
			::OutputDebugString(strMsg);
			return DB_DISCONNECT;
		}
	}
	
	//ֻ����60����Ϣ
	return DelByDate();
	////////////////
}



int CCheckDBCode::InitAdbDBPARA(DBPARA paradb)
{
	if (!m_bCheckCode)
		return DB_OK;

	m_adoDB.DisconnectDB();
	m_paradb = paradb;

	//ֻ����60����Ϣ
	return DelByDate();
	////////////////
}

//��ʼ�����ݿ����
int CCheckDBCode::InitAdb(DBPARA paradb)
{
	if (!m_bCheckCode)
		return DB_OK;

	m_adoDB.DisconnectDB();
	m_paradb = paradb;

	if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	{
		if (!m_adoDB.ConnectToDB(m_paradb,TRUE))
		{
			CString strMsg;
			strMsg = _T("���ݿ�����ʧ��.");
			::OutputDebugString(strMsg);
			return DB_DISCONNECT;
		}
	}

	//ֻ����60����Ϣ
	return DelByDate();
	////////////////
}

///////////д�����ݿ�
BOOL CCheckDBCode::WriteToDb(CString strCode, CString strReadCode, CString strPatchID)
{
	if (!m_bCheckCode)
		return DB_OK;

	if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	{
		if (!m_adoDB.ConnectToDB(m_paradb, TRUE))
		{
			CString strMsg;
			strMsg = _T("���ݿ�����ʧ�ܡ�");
			::OutputDebugString(strMsg);
			return DB_DISCONNECT;
		}
	}

	//ȡ����
	if (strCode.IsEmpty())
	{
		return DB_OTHER_ERROR;
	}
	
	CAdoRecordset Recordset;
	CString strSQL;

	CTime time = CTime::GetCurrentTime();
	CString strMarkTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));

	strSQL.Format(_T("INSERT INTO [markdata]([MarkCode], [MarkType], [MarkTime], [PatchID]) values(\'%s\', \'%s\', \'%s\', \'%s\' )"), 
		strCode, strReadCode, strMarkTime, strPatchID);

	if(!m_adoDB.ExecuteSQLQuery((_bstr_t)strSQL))
	{
		CString strMsg;
		strMsg = _T("д�����ݿ�����ʧ�ܡ�");
		::OutputDebugString(strMsg);
		return DB_WRITE_FAIL;
	}

	return DB_OK;
}
//.....................


BOOL CCheckDBCode::IsConnect()
{
	if (m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CCheckDBCode::Connect()
{
	if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	{
		if (!m_adoDB.ConnectToDB(m_paradb, TRUE))
		{
			CString strMsg;
			strMsg = _T("���ݿ�����ʧ�ܡ�");
			::OutputDebugString(strMsg);
			return DB_DISCONNECT;
		}
	}
	return DB_OK;
}

int CCheckDBCode::DisConnect()
{
	m_adoDB.DisconnectDB();
	return DB_OK;
}


//ɾ����������
int CCheckDBCode::DelAllData()
{

	int nConRet = Connect();
	if (nConRet)
	{
		return nConRet;
	}

	//if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	//{
	//	if (!m_adoDB.ConnectToDB(m_paradb, TRUE))
	//	{
	//		CString strMsg;
	//		strMsg = _T("���ݿ�����ʧ�ܡ�");
	//		::OutputDebugString(strMsg);
	//		return DB_DISCONNECT;
	//	}
	//}
	
	CAdoRecordset Recordset;
	CString strSQL = _T("delete from [markdata]");
	
	if(!m_adoDB.OpenSQLQuery((_bstr_t)strSQL,Recordset))
	{
		CString strMsg;
		strMsg = _T("ɾ�����ݿ�ʧ�ܡ�");
		::OutputDebugString(strMsg);
		return DB_DELETE_FAIL;
	}

	DisConnect();
	return DB_OK;
}

///////////ɾ�����ݿ�
int CCheckDBCode::DelByDate(int nDay)
{
	//if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	//{
	//	if (!m_adoDB.ConnectToDB(m_paradb, TRUE))
	//	{
	//		CString strMsg;
	//		strMsg = _T("���ݿ�����ʧ�ܡ�");
	//		return DB_DISCONNECT;
	//	}
	//}

	int nConRet = Connect();
	if (nConRet)
	{
		return nConRet;
	}
	
	CAdoRecordset Recordset;
	CString strSQL;
	
	CTime time = CTime::GetCurrentTime();
	CTimeSpan span(nDay, 0, 0, 0);
	time -= span;
	CString strMarkTime = time.Format("%Y-%m-%d %H:%M:%S");
	strSQL.Format(_T("delete from [markdata] where [MarkTime] < \'%s\'"), strMarkTime);
	
	if(!m_adoDB.OpenSQLQuery((_bstr_t)strSQL,Recordset))
	{
		CString strMsg;
		strMsg = _T("ɾ�����ݿ�����ʧ�ܡ�");
		return DB_DELETE_FAIL;
	}
	DisConnect();

	return DB_OK;
}

//ɾ���ض�����
int CCheckDBCode::DeleteSerial(CString strSerial)
{
	if (!m_bCheckCode)
		return DB_OK;

	//if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	//{
	//	if (!m_adoDB.ConnectToDB(m_paradb, TRUE))
	//	{
	//		CString strMsg;
	//		strMsg = _T("���ݿ�����ʧ�ܡ�");
	//		::OutputDebugString(strMsg);
	//		return DB_DISCONNECT;
	//	}
	//}

	int nConRet = Connect();
	if (nConRet)
	{
		return nConRet;
	}

	CString strSQL;
	CAdoRecordset rs;
	strSQL.Format(_T("DELETE FROM [markdata] WHERE MarkCode=\'%s\'"),strSerial);
	::OutputDebugString(strSQL);
	if (!m_adoDB.ExecuteSQLQuery((_bstr_t)strSQL))
	{
		CString strMsg;
		strMsg = _T("ɾ�����ݿ�ʧ�ܡ�");
		::OutputDebugString(strMsg);
		return DB_DELETE_FAIL;
	}

	DisConnect();

	return DB_OK;
}

int CCheckDBCode::CheckCodeOK(CString strCode)
{
	if (!m_bCheckCode)
		return DB_OK;

	if(strCode.IsEmpty())
		return DB_OTHER_ERROR;

	//if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	//{
	//	if (!m_adoDB.ConnectToDB(m_paradb, TRUE))
	//	{
	//		CString strMsg;
	//		strMsg = _T("���ݿ�����ʧ�ܡ�");
	//		::OutputDebugString(strMsg);
	//		return DB_DISCONNECT;
	//	}
	//}

	int nConRet = Connect();
	if (nConRet)
	{
		return nConRet;
	}

	CString strSQL;
	CAdoRecordset rs;
	strSQL.Format(_T("SELECT * FROM [markdata] WHERE [MarkCode] = \'%s\' "), strCode);
	::OutputDebugString(strSQL);
	if (m_adoDB.OpenSQLQuery((_bstr_t)strSQL, rs))
	{
		if (!rs.IsEOF() && !rs.IsBOF())//����ADOͷβ���������ݣ���������������ݿ�
		{
			CString strMsg;
			strMsg.Format(_T("��⵽�ظ�����Ϊ: %s"),strCode);
			::OutputDebugString(strMsg);
			return DB_REPEATCODE;
		}
	}
	else
	{
		CString strMsg;
		strMsg = _T("Execute QueryDB is defeat!");
		::OutputDebugString(strMsg);
		return DB_SELECT_FAIL;
	}

	DisConnect();
	return DB_OK;
}

////��ȡִ��Ŀ¼
void CCheckDBCode::GetExepath(TCHAR *pPath)  
{
	TCHAR pExePath[256] = {0};
	memset(pExePath, 0, 256);
	GetModuleFileName(NULL, pExePath, 256);
	TCHAR * pEnd = _tcsrchr(pExePath,_T('\\'));
	*(pEnd+1) = NULL;

	if(pPath)
		_tcscpy(pPath,pExePath);

	return;
}
//ȡ��·��
CString CCheckDBCode::GetAppPath()
{
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
	return strPath;
}

int CCheckDBCode::OrcaleTest()
{
	if (!m_bCheckCode)
		return DB_OK;

	//if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	//{
	//	if (!m_adoDB.ConnectToDB(m_paradb, TRUE))
	//	{
	//		CString strMsg;
	//		strMsg = _T("���ݿ�����ʧ�ܡ�");
	//		::OutputDebugString(strMsg);
	//		return DB_DISCONNECT;
	//	}
	//}

	int nConRet = Connect();
	if (nConRet)
	{
		return nConRet;
	}

	CString strSQL;
	CAdoRecordset rs;
	strSQL.Format(_T("select * from [sys.student]"));
	::OutputDebugString(strSQL);
	if (m_adoDB.OpenSQLQuery((_bstr_t)strSQL, rs))
	{
		int nRow = rs.GetFieldsCount();
		int i = 0;
		if(rs.IsBOF())
		{
			AfxMessageBox(_T("��ѯ����Ϊ�գ�"));
			return DB_OTHER_ERROR;
		}

		rs.MoveFirst();
		while(!rs.IsEOF())
		{
			CString str1,str2;
			CStringArray array;
			rs.GetRecordValue(array);
			str1 = array.GetAt(0);//�����е��׸�Ԫ��
			for(int j=1;j<nRow;j++)
			{
				str2 = array.GetAt(j);//������Ԫ��
			}
			i++;
			rs.MoveNext();
		}
	}
	else
	{
		CString strMsg;
		strMsg = _T("Execute QueryDB is defeat!");
		::OutputDebugString(strMsg);
		return DB_SELECT_FAIL;
	}

	DisConnect();

	return DB_OK;
}


int CCheckDBCode::OrcaleSelect(CString strsbar)
{
	if (!m_bCheckCode)
		return DB_OK;

	CString strSQL;
	CAdoRecordset rs;
	strSQL.Format(_T("select sbar from sfcdata.vf_stripbar where sbar = \'%s\' "), strsbar);
	::OutputDebugString(strSQL);
	if (m_adoDB.OpenSQLQuery((_bstr_t)strSQL, rs))
	{
		if(rs.IsBOF())
		{
			return TRUE;
		}
		else
			return DB_OK;
	}
	else
	{
		CString strMsg;
		strMsg = _T("Execute QueryDB is defeat!");
		::OutputDebugString(strMsg);
		return DB_SELECT_FAIL;
	}

	//m_adoDB.DisconnectDB();
	return DB_OK;
}


int CCheckDBCode::OrcaleInsert(CString strloc,CString strSbar,CString strlotnum,CString strpartnum,CString strdouid)
{
	if (!m_bCheckCode)
		return DB_OK;

	CTime tNow = CTime::GetCurrentTime();
	CString strdodate= tNow.Format(_T("%Y%m%d%H%M%S"));


	CString strSQL;
	CAdoRecordset rs;

	CString strTempSqL1,strTempSqL2;
	strTempSqL1.Format(_T("insert into sfcdata.vf_stripbar(loc,sbar,lotnum,partnum,douid)"));
	strTempSqL2.Format(_T("values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')"),strloc,strSbar,strlotnum,strpartnum,strdouid);

	strSQL = strTempSqL1 + strTempSqL2;
	::OutputDebugString(strSQL);


	if (m_adoDB.ExecuteSQLQuery((_bstr_t)strSQL, rs))
	{

	}
	else
	{
		CString strMsg;
		strMsg = _T("Execute QueryDB is defeat!");
		::OutputDebugString(strMsg);
		return DB_SELECT_FAIL;
	}

	//DisConnect();
	return DB_OK;
}



// ִ��sql���
BOOL CCheckDBCode::ExecuteSQL(CString strSQL,CAdoRecordset& rs)
{
	if (!m_adoDB.ExecuteSQLQuery((_bstr_t)strSQL, rs))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CCheckDBCode::ExecuteDateSQL(CString strSQL )
{
	if (!m_bCheckCode)
		return DB_OK;

	CAdoRecordset rs;
	::OutputDebugString(strSQL);


	if (m_adoDB.ExecuteSQLQuery((_bstr_t)strSQL, rs))
	{
			
	}
	else
	{
		CString strMsg;
		strMsg = _T("Execute QueryDB is defeat!");
		::OutputDebugString(strMsg);
		return DB_SELECT_FAIL;
	}

	//DisConnect();
	return DB_OK;
}

int CCheckDBCode::OrcaleSelectLot(CString strLot, CString strPart)
{
	if (!m_bCheckCode)
		return DB_OK;

	CString strSQL;
	CAdoRecordset rs;
 	strSQL.Format(_T("select * from sfcdata.pmmstknwip where stknwip002 = \'373\' and stknwip006 >= 1 and lotnum = \'%s\' and stknwip003 = \'%s\' "),
 		strLot, strPart);
	::OutputDebugString(strSQL);
	if (m_adoDB.OpenSQLQuery((_bstr_t)strSQL, rs))
	{
		if(rs.IsBOF())
		{
			return TRUE;	// NULL  û���ҵ�
		}
		else
		{
			return FALSE;	// check ok �ҵ�
		}
	}
	else
	{
		CString strMsg;
		strMsg = _T("Execute QueryDB is defeat!");
		::OutputDebugString(strMsg);
		return DB_SELECT_FAIL;
	}

	//m_adoDB.DisconnectDB();
	return FALSE;
}

BOOL	CCheckDBCode::GetName(CStringArray& arrayName )
{
	if (m_adoDB.GetExcelName(arrayName))
	{
		return true;
	}
	return false;
}

BOOL	CCheckDBCode::ExecuteMarkData(CString SQL, CString& strBourd)
{
	if (!m_bCheckCode)
		return DB_OK;

	if(SQL.IsEmpty())
		return DB_OTHER_ERROR;

	if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	{
		if (!m_adoDB.ConnectToDB(m_paradb, TRUE))
		{
			CString strMsg;
			strMsg = _T("���ݿ�����ʧ�ܡ�");
			::OutputDebugString(strMsg);
			return DB_DISCONNECT;
		}
	}

	CString strSQL;
	CAdoRecordset rs;
	strSQL = SQL;
	::OutputDebugString(strSQL);
	if (m_adoDB.OpenSQLQuery((_bstr_t)strSQL, rs))
	{
		if (rs.IsEOF())
		{
			return DB_SELECT_FAIL;
		}
		rs.MoveFirst();
		while(!rs.IsEOF())
		{
	//		_variant_t Dimension = rs.GetCollect("BOARD_SN");
	//		strBourd = (TCHAR *)_bstr_t(Dimension);//ת����ʽ
			return DB_OK;
			//			rs.MoveNext();
		}
	}
	else
	{
		CString strMsg;
		strMsg = _T("Execute QueryDB is defeat!");
		::OutputDebugString(strMsg);
		return DB_SELECT_FAIL;
	}

	return DB_OK;
}
