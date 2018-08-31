// CheckDBCode.h: interface for the CCheckDBCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKDBCODE_H__357459B7_5F0B_4C41_B20B_9B7765991084__INCLUDED_)
#define AFX_CHECKDBCODE_H__357459B7_5F0B_4C41_B20B_9B7765991084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//错误码
enum DB_ERRORCODE
{
	DB_OK    = 0,//执行正确
	DB_DISCONNECT = 1,//DB断开
	DB_SELECT_FAIL = 2,//DB查询失败
	DB_WRITE_FAIL = 3,//DB写入失败
	DB_DELETE_FAIL = 4,//DB删除失败
	DB_UPDATE_FAIL = 5,//DB更新失败
	DB_REPEATCODE = 6,//重码
	DB_OTHER_ERROR = 7,//DB其他错误
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
	//初始化数据库
	int InitAdb();
	//初始化数据库参数
	int InitAdb(DBPARA paradb);
	//写入数据库
	int WriteToDb(CString strCode, CString strReadCode, CString strPatchID);
	///////////删除数据库
	int DelByDate(int nDay = 90);
	//删除所有数据
	int DelAllData();
	//删除特定内容
	int DeleteSerial(CString strSerial);
	//检测重码
	int CheckCodeOK(CString strCode);
	//取得或设置检码标志位
	BOOL GetCheckFlag() const { return m_bCheckCode; }
	void SetCheckFlag(BOOL bNewFalg) { m_bCheckCode = bNewFalg; }



	BOOL IsConnect();
	int Connect();
	int DisConnect();



	int OrcaleTest();

	int OrcaleSelect(CString strsbar);
	int OrcaleSelectLot(CString strLot, CString strPart);  // check 批号料号 
	int OrcaleInsert(CString strloc,CString strSbar,CString strlotnum,CString strpartnum,CString strdouid);
private:
	//是否检验重码
	BOOL m_bCheckCode;
	//数据库对象
	CAdoDB m_adoDB;
	//数据库参数
	DBPARA  m_paradb;
	////获取执行目录
	void GetExepath(TCHAR *pPath);
	//取得路径
	CString GetAppPath();
	
public:
	// 执行sql语句
	BOOL ExecuteSQL(CString strSQL,CAdoRecordset& rs);	// 查询
	BOOL	ExecuteMarkData(CString SQL, CString& strBourd);
	BOOL ExecuteDateSQL(CString strSQL );		// 增删改
	BOOL		GetName(CStringArray& arrayName );
};

#endif // !defined(AFX_CHECKDBCODE_H__357459B7_5F0B_4C41_B20B_9B7765991084__INCLUDED_)
