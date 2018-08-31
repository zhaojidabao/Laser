// HLDoc.cpp : implementation of the CHLDoc class
//
#include "stdafx.h"
#include "HL.h"
#include "resource.h"
#include <afxole.h>

#include "HLDoc.h"
#include "MainFrm.h"

#include "Node.h"
#include "NodeDib.h"
#include "NodeCodeBar.h"
#include "NodeText.h"
#include "DrawFillDlg.h"
#include "MarkNormalDlg.h"
#include "MarkPreviewDlg.h"
#include "MarkWheelDlg.h"
#include "MarkWheelDlg2.h"
#include "ArrayDlg.h"
#include "DibDlg.h"
#include "KBChar.h"

#include "CmdStoreDoc.h"
#include "CmdMove.h"
#include "CmdAddNode.h"
#include "CmdScale.h"
#include "CmdDelete.h"
#include "CmdRotate.h"
#include "CmdShear.h"
#include "CmdCopy.h"
#include "CmdGroup.h"
#include "CmdUngroup.h"
#include "CmdUnion.h"
#include "CmdUnunion.h"
#include "CmdPaste.h"
#include "CmdFill.h"
#include "CmdSetLay.h"
#include "CmdDrawSet.h"
#include "CmdArray.h"
#include "Cmdchanglay.h"
#include "CmdNewDrag.h"
#include "CmdEditWeld.h"
#include "SetLayer.h"
#include "HLView.h"
#include "CmdDotEdit.h"
#include "CmdDelDot.h"
#include "CmdHitOtherObj.h"
#include "CmdConvert.h"
#include "SplitPointCmd.h"
#include "JoinPointCmd.h"
#include "MoveAccCmd.h"
#include "CmdPathOptimize.h"
#include "DlgExportPlt.h"
#include "CmdAddPoint.h"
#include "CmdFilterDocPoint.h"
#include "CmdExtendObj.h"
#include "CmdKillAngle.h"
#include "CmdConverStroke.h"
#include "CmdSelGroupByNum.h"
#include "CmdDelObj.h"
#include "CmdMAlign.h"
#include "CmdAutoSegment.h"
#include "DcpMsg.h"
#include "MarkSpecialDlg.h"
#include "SelSort.h"
#include "DlgPathOptimizeSet.h"
#include "CmdDragFrame.h"
#include "ModifyZCmd.h"

#include "DlgMotionInfoSet.h"
#include "DlgRotateWheelSet.h"
#include "DlgIntensity.h"
#include "DlgRulerEdit.h"
#include "baseMacro.h"
#include "Dlg3DLoad.h"
#include "Dlg3DTransform.h"
#include "DlgModifyZ.h"
#include "LocaleFocuslDlg.h"
#include "SetLangRegionDlg.h"
#include "WeldEditPro.h"
#include "DotInterFilter.h"
#include "DlgInterspaceSet.h"
#include "CmdDotInterFilter.h"
#include "Dlg3DAlign.h"
#include "CmdBroadenStroke.h"
#include "DlgBroadenSet.h"
#include "BroadenStroke.h"
#include "DlgMoveMarkParam.h"
#include "DlgMotorMoveWait.h"
#include "ComMode.h"
#include "DataSourceDlg.h"
#include "QueryDBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString  g_OpenFileName;
CString  g_OldFileName;
BOOL     g_bClipBoardCopyNode = FALSE;

// 辅助对象
CAssistantObj  g_ObjDrawAssist;
BOOL   g_bEnableAssist = FALSE;
extern  CHLApp	theApp;
extern  BOOL  g_bAdminPrivilege;
extern  CStringArray g_arryDisk;
/////////////////////////////////////////////////////////////////////////////
// CHLDoc
IMPLEMENT_DYNCREATE(CHLDoc, CDocument)

BEGIN_MESSAGE_MAP(CHLDoc, CDocument)
	//{{AFX_MSG_MAP(CHLDoc)
	ON_COMMAND(ID_GROUP, OnGroup)
	ON_COMMAND(ID_UN_GROUP, OnUnGroup)
	ON_UPDATE_COMMAND_UI(ID_GROUP, OnUpdateGroup)
	ON_UPDATE_COMMAND_UI(ID_UN_GROUP, OnUpdateUnGroup)
	ON_COMMAND(ID_MARK_NORMAL, OnMarkNormal)
	ON_COMMAND(ID_RED_PREVIEW, OnRedPreview)
	ON_UPDATE_COMMAND_UI(ID_RED_PREVIEW, OnUpdateRedPreview)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_DRAW_FILL, OnDrawFill)
	ON_COMMAND(ID_CHANGE_CENTER, OnChangeCenter)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_CENTER, OnUpdateChangeCenter)
	ON_COMMAND(ID_CHANGE_CCW, OnChangeCcw)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_CCW, OnUpdateChangeCcw)
	ON_COMMAND(ID_CHANGE_CW, OnChangeCw)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_CW, OnUpdateChangeCw)
	ON_COMMAND(ID_CHANGE_MIRROR_X, OnChangeMirrorX)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_MIRROR_X, OnUpdateChangeMirrorX)
	ON_COMMAND(ID_CHANGE_MIRROR_Y, OnChangeMirrorY)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_MIRROR_Y, OnUpdateChangeMirrorY)
	ON_COMMAND(ID_MARK_PREVIEW, OnMarkPreview)
	ON_COMMAND(ID_DRAW_SET, OnDrawSet)
	ON_COMMAND(ID_ARRAY, OnArray)
	ON_UPDATE_COMMAND_UI(ID_ARRAY, OnUpdateArray)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_COMMAND(ID_DRAW_BMP, OnDrawBmp)
	ON_COMMAND(ID_SET_LAYER, OnSetLayer)
	ON_COMMAND(ID_DRAW_BARTEXT, OnDrawBartext)
	ON_COMMAND(IDM_EXPORT_PLT, OnExportPlt)
	ON_COMMAND(IDC_STATUS_BAR_BUTTON, OnStatusButton)
	ON_COMMAND(ID_MARK_SPC, OnMarkSpc)
	ON_UPDATE_COMMAND_UI(ID_MARK_SPC, OnUpdateMarkSpc)
    ON_COMMAND(ID_FAST_MARK, OnMarkFast)
	ON_UPDATE_COMMAND_UI(ID_FAST_MARK, OnUpdateMarkFast)
	ON_COMMAND(ID_DWONLOAD_MARKDATA, OnDownLoad)
	ON_UPDATE_COMMAND_UI(ID_DWONLOAD_MARKDATA, OnUpdateDownLoad)
	ON_COMMAND(ID_CLEAR_DOWNLOAD, OnClearDownLoad)
	ON_COMMAND(ID_EDIT_DB_COPY, OnEditDBCopy)
	ON_COMMAND(ID_EDIT_DB_PASTE, OnEditBDPaste)
	ON_UPDATE_COMMAND_UI(IDD_ADDONETREE, OnUpdateConvertOneTree)
	ON_COMMAND(IDD_ADDONETREE, OnConvertOneTree)
	ON_COMMAND(ID_PATH_OPTIMIZE, OnPathOptimize)
	ON_COMMAND(ID_PATH_NORMAL, OnNormalPath)
	ON_UPDATE_COMMAND_UI(ID_JOINTSTROKE_CURDOC, OnUpdateJointstrokeCurdoc)
	ON_UPDATE_COMMAND_UI(ID_PATH_OPTIMIZE, OnUpdatePathOptimize)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT, OnUpdateFileImport)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_VECTOR, OnUpdateFileImportVector)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_BMP, OnUpdateFileImportBmp)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_NC, OnUpdateFileImportNc)
	ON_UPDATE_COMMAND_UI(ID_MODULE_SWITCH, OnUpdateModuleSwitch)
	ON_COMMAND(ID_MODULE_SWITCH, OnModuleSwitch)
	ON_UPDATE_COMMAND_UI(ID_3DVIEW_XYZ, OnUpdate3dViewXYZ)
	ON_COMMAND(ID_3DVIEW_XYZ, On3dViewXYZ)
	ON_UPDATE_COMMAND_UI(ID_3DVIEW_XY, OnUpdate3dViewXY)
	ON_COMMAND(ID_3DVIEW_XY, On3dViewXY)
	ON_UPDATE_COMMAND_UI(ID_3DVIEW_YZ, OnUpdate3dViewYZ)
	ON_COMMAND(ID_3DVIEW_YZ, On3dViewYZ)
	ON_UPDATE_COMMAND_UI(ID_3DVIEW_XZ, OnUpdate3dViewXZ)
	ON_COMMAND(ID_3DVIEW_XZ, On3dViewXZ)
	ON_UPDATE_COMMAND_UI(ID_MODULE_LOAD, OnUpdateModuleLoad3D)
	ON_COMMAND(ID_MODULE_LOAD, OnModuleLoad3D)
	// build model
	ON_UPDATE_COMMAND_UI(ID_MODULE_TRANSFORM, OnUpdateModuleTransform3D)
	ON_COMMAND(ID_MODULE_TRANSFORM, OnModuleTransform3D)
	ON_UPDATE_COMMAND_UI(ID_MARK_NORMAL, OnUpdateMarkNormal)
	ON_UPDATE_COMMAND_UI(ID_DOWN_MUL, OnUpdateDownMul)
	ON_UPDATE_COMMAND_UI(ID_CLEAR_DOWNLOAD, OnUpdateClearDownload)
	ON_UPDATE_COMMAND_UI(IDM_EXPORT_PLT, OnUpdateExportPlt)
	ON_COMMAND(ID_UNION, OnUnion)
	ON_UPDATE_COMMAND_UI(ID_UNION, OnUpdateUnion)
	ON_COMMAND(ID_UNUNION, OnUnunion)
	ON_UPDATE_COMMAND_UI(ID_UNUNION, OnUpdateUnunion)
	ON_COMMAND(ID_EDIT_WHEEL, OnEditWheel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_WHEEL, OnUpdateEditWheel)
	
	ON_COMMAND(ID_CHECK_FOCUS, OnCheckFocus)
	ON_UPDATE_COMMAND_UI(ID_CHECK_FOCUS, OnUpdateCheckFocus)

	ON_COMMAND(ID_MOTIONINFO_SET, OnMotioninfoSet)
	ON_UPDATE_COMMAND_UI(ID_MOTIONINFO_SET, OnUpdateMotioninfoSet)
	ON_COMMAND(ID_ROTATEWHEEL_SET, OnRotatewheelSet)
	ON_COMMAND(ID_MOTION_MARK_PARAM, OnMotionMarkSet)
	ON_UPDATE_COMMAND_UI(ID_MOTION_MARK_PARAM, OnUpdateMotionMarkSet)
	ON_UPDATE_COMMAND_UI(ID_ROTATEWHEEL_SET, OnUpdateRotatewheelSet)

	ON_COMMAND(ID_MARK_WHEEL, OnMarkWheel)
	ON_UPDATE_COMMAND_UI(ID_MARK_WHEEL, OnUpdateMarkWheel)
	ON_COMMAND(ID_MARK_WHEEL2, OnMoveMark)
	ON_UPDATE_COMMAND_UI(ID_MARK_WHEEL2, OnUpdateMoveMark)
	ON_COMMAND(ID_BMP_Intensity, OnBMPIntensity)
	ON_UPDATE_COMMAND_UI(ID_BMP_Intensity, OnUpdateBMPIntensity)
	ON_COMMAND(ID_EDIT_RULER, OnEditRuler)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RULER, OnUpdateEditRuler)
	ON_COMMAND(ID_MODIFYZ, OnModifyZ)
	//}}AFX_MSG_MAP
	
	ON_UPDATE_COMMAND_UI(57604, &CHLDoc::OnUpdateSaveAS)
	ON_COMMAND(57604, &CHLDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, &CHLDoc::OnFileSave)
	ON_COMMAND(IDD_EDIT_WELD, &CHLDoc::OnEditWeld)
	ON_UPDATE_COMMAND_UI(IDD_EDIT_WELD, &CHLDoc::OnUpdateEditWeld)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_CARD, &CHLDoc::OnUpdateChgCard)
	ON_COMMAND(ID_ASSISTANT_POINT, &CHLDoc::OnAssistantPoint)
	ON_COMMAND(IDD_EDIT_InterFilter, &CHLDoc::OnEditInterfilter)
	ON_UPDATE_COMMAND_UI(ID_ASSISTANT_POINT, &CHLDoc::OnUpdateAssistantPoint)
	ON_UPDATE_COMMAND_UI(IDD_EDIT_InterFilter, &CHLDoc::OnUpdateEditInterfilter)
	ON_UPDATE_COMMAND_UI(ID_MODULE_ALIGN, OnUpdateModuleAlign)
	ON_COMMAND(ID_MODULE_ALIGN, OnModuleAlign)
	ON_COMMAND(IDD_EDIT_BROADEN, &CHLDoc::OnEditBroaden)
	ON_UPDATE_COMMAND_UI(IDD_EDIT_BROADEN, &CHLDoc::OnUpdateEditBroaden)
	ON_COMMAND(ID_COMUNICATION, &CHLDoc::OnComunication)
	ON_COMMAND(ID_SETDATABASE, &CHLDoc::OnSetdatabase)
	ON_COMMAND(ID_DATABASE, &CHLDoc::OnDatabase)
	END_MESSAGE_MAP()
	
/////////////////////////////////////////////////////////////////////////////
// CHLDoc construction/destruction

CHLDoc::CHLDoc()
{
	int nUndoMax = g_SysCfg.m_nUndoCount;
	m_cmdProc.SetMaxCommands(nUndoMax);
	
	m_nStatus = CChain::selectNull;
	m_quadTracker.SetEmpty();
	
	m_bEditCut = FALSE;
	m_nPosType = 4;
	
	m_pTracker = NULL;
	m_bExternOpt = TRUE;
	
	m_Sort.RemoveAll();
	m_ASort.RemoveAll();
	m_GSort.RemoveAll();
	m_QuadList.RemoveAll();
	
	m_bInit3D = FALSE;
	InitChain3D();

	memset(&m_DBCopyPara, 0, sizeof(DOUBLEOUTPARA));
	m_QuadList.SetSize(100000, 1000);
	
	m_bPathOptimize = FALSE;
	m_nPathOptimizeType = 0;
	m_bPathModify = FALSE;
	m_motor.nMoveModel = 1;
	
	m_b3D = FALSE;
	m_b3dTransForce = FALSE;
}

void CHLDoc::InitChain3D()
{
	if ( m_bInit3D )
		return;
	//
	double dbX(0), dbY(0), dbMarkRange(0);
	if ( !g_DAT.GetMarkRect(dbX, dbY) )
		return;

	dbMarkRange = (dbX > dbY ? dbX : dbY);

	int nGrdCount = (int)(dbMarkRange/g_SysCfg.m_db3dModelPrecision);
	m_chain3d.Init(dbMarkRange, nGrdCount>1000 ? 1000 : nGrdCount);

	m_bInit3D = TRUE;
}


CHLDoc::~CHLDoc()
{
	CMainFrame *pFrame = (CMainFrame *)theApp.m_pMainWnd;
	if ( NULL == pFrame )
		return;
	
	pFrame->m_FillEditDlg.m_BarEditDlg.ResetData();
	
	//
	m_QuadList.RemoveAll();
}


BOOL CHLDoc::OnNewDocument()
{
	TCHAR szPath[256] = {0};
	GetExepath(szPath,256);
	TCHAR szFileName[256] = {0};
	_stprintf_s(szFileName, 255,_T("%s%s%s"), szPath, _T("default."), g_DAT.m_strEMCCLaser);
	
	BOOL bUnicode = CStdioFileEx::IsFileUnicode(szFileName);
	FILE *fp = _tfopen( szFileName, bUnicode ? _T("rb") : _T("rt") );
	if ( fp  !=  NULL )
	{
		m_lay.Serialize(fp, FALSE, EMCC,bUnicode);
		fclose(fp);
	}
	
	//打标控制参数设置缺省值
	for( int i = m_lay.GetSize(); i < g_SysCfg.m_nLayerCnt; i++ )
	{
		CLayer ly;	ly.m_nLayer = i;
		switch(i)
		{
			case 0:	ly.m_color = BLACK;break;
			case 1:	ly.m_color = RED;break;
			case 2:	ly.m_color = YELLOW;break;
			case 3:	ly.m_color = GREEN;break;
			case 4:	ly.m_color = YANGREEN;break;
			case 5:	ly.m_color = BLUE;break;
			case 6:	ly.m_color = MAGENTA;break;
			case 7:	ly.m_color = GRAY;break;
			default: ly.m_color = BLACK;	break;
		}
		m_lay.Add(ly);
	}
	m_lay.SetCur(0);
	
	CMainFrame *pFWnd = (CMainFrame*)theApp.m_pMainWnd;
	if ( NULL  !=  pFWnd )
	{
		pFWnd->m_PropDlg.OnNewDocument(m_lay);
		pFWnd->m_PSDlg.OnNewDocument();
		
#ifdef _SPECIALPROP_EDIT
		pFWnd->m_FillEditDlg.m_TextEditDlg.OnNewDocment();
#endif
	}
	
	g_dwVersion = CUR_DOC_VERSION;
	CArray<SELQUAD,SELQUAD> quadList;
	quadList.RemoveAll();
	theApp.m_RedPreView.SetQuadArray(quadList);
	
	return CDocument::OnNewDocument();
}

void CHLDoc::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	CMySerialize sa(fp,bUnicode);
	CString str;
	
	if ( bFlag )
	{
		//保存文件
		sa  <<  _T("HAN'S SOFTWARE 6.0.001");
		//
		sa << 1001;
		sa << m_bPathOptimize;
		sa << 1002;
		sa << m_nPathOptimizeType;
		
		sa << 1003;
		CString strValue;
		for ( int i = 0; i<10; i++ )
		{
			strValue = m_docExtent.szExtInfo[i];
			sa  <<  strValue;
			sa  <<  m_docExtent.nReserved[i];
			sa  <<  m_docExtent.dwReserved[i];
			sa  <<  m_docExtent.dbReserved[i];
		}
		
		//数据序列化操作
		m_chain.Serialize(fp,bFlag,bUnicode);
		
		//层序列化操作
		m_lay.Serialize(fp,bFlag,g_nCardType,bUnicode);		
		sa << _T("END FILE");
	}
	else
	{
		//打开文件
		sa >> str;
		if ( str.Left(14) == _T("HAN'S SOFTWARE") )
		{
			if ( 0 == str.CompareNoCase(_T("HAN'S SOFTWARE 6.0.001")) )
			{
				g_dwVersion = 60001;
			}
			else if ( 0 == str.CompareNoCase(_T("HAN'S SOFTWARE 6.0.000")) )
			{
				g_dwVersion = 60000;
			}
			else if ( 0 == str.CompareNoCase(_T("HAN'S SOFTWARE 5.3.0005")) )
			{
				g_dwVersion = 53005;
			}
			else if ( 0 == str.CompareNoCase(_T("HAN'S SOFTWARE 5.3.0004")) )
			{
				g_dwVersion = 53004;
			}
			else if ( 0 == str.CompareNoCase(_T("HAN'S SOFTWARE 5.3.0001")) )
			{
				g_dwVersion = 53001;
			}
			else
			{
				g_dwVersion = 52001;
			}
			
			while( (sa >> str ) )
			{
				if ( str == _T("END FILE") )
					break;
				
				/////////////
				if ( str == _T("1001") )
				{
					sa >> m_bPathOptimize;
				}
				else if ( str == _T("1002") )
				{
					sa >> m_nPathOptimizeType;					
				}
				else if ( str == _T("1003") )//扩展
				{
					CString strValue;
					for( int i = 0; i<10; i++ )
					{
						sa >> strValue;
						_stprintf_s(m_docExtent.szExtInfo[i],100, _T("%s"), strValue);
						sa >> m_docExtent.nReserved[i];
						sa >> m_docExtent.dwReserved[i];
						sa >> m_docExtent.dbReserved[i];
					}
				}
				else if ( str == _T("D_B") )//数据序列化操作
				{
					if ( g_dwVersion  >=  53005 )
						m_chain.Serialize(fp, bFlag,bUnicode);
					else
						m_chain.SerializeOld(fp, bFlag,bUnicode);
				}
				else if ( str == _T("L_B") )//层序列化操作
				{
					m_lay.Serialize(fp,bFlag,g_nCardType,bUnicode);
				}
			}
		}
		////////////
		if ( m_bPathOptimize )
		{
			NormalPath();
			PathOptimizeCmd();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
// CHLDoc diagnostics
#ifdef _DEBUG
void CHLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG 

/////////////////////////////////////////////////////////////////////////////
// CHLDoc commands

void CHLDoc::AddNodeCmd(CNode *pNode, int nObjType)
{
	CTree *pTree = new CTree(pNode);
	if ( NULL == pTree )
		return;
	
	CGroupObj *pGroupObj = new CGroupObj();
	if ( pGroupObj == NULL )
		return;
	
	pGroupObj->Add(pTree);
	m_chain.Add(pGroupObj);
	
	SetModifiedFlag();
	m_chain.SetSelect(FALSE);//其它为没选择
	pGroupObj->SetSelect(TRUE);  //新的为选择
	CQuad quad = pGroupObj->GetLimit();
	
	SetTrackerQuad(quad);
	SelectDraw(pGroupObj);
}

//
void CHLDoc::AddNodeCmd(CTree *pTree)
{
	if ( pTree == NULL )
		return;
	
	CGroupObj *pGroupObj = new CGroupObj();
	if ( pGroupObj == NULL )
		return;
	
   	pGroupObj->Add(pTree);
	m_chain.Add(pGroupObj);
	
	m_chain.SetSelect(FALSE);
	pGroupObj->SetSelect(TRUE);
	SetModifiedFlag();
	
	CQuad quad = pGroupObj->GetLimit();
	SetTrackerQuad(quad);
	SelectDraw(pGroupObj);
}


//
void CHLDoc::AddNode(CNode *pNode, int nObjType)
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	if ( m_cmdProc.GetMaxCommands() == 0 )
		AddNodeCmd(pNode,nObjType);
	else
		m_cmdProc.Submit(new CCmdAddNode(this,pNode));
	
	SetObjProp();
}


void CHLDoc::Draw(XDC *pDC)
{
	m_chain.Draw(pDC);
}


BOOL CHLDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_OPEN) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return FALSE;
	}
	
	BOOL b3DFile(FALSE);
	CString  strExt = CString(lpszPathName).Right(3).MakeLower();
	if ( strExt.CompareNoCase(_T(".hd")) == 0  )
	{
		b3DFile = TRUE;
	#ifndef _SOFTMODE_EDITOR_
		if ( !g_DAT.m_btExtendInfo.FunData.bNormal3d )
		{
			AfxMessageBox(IDS_RIGHT_NO);
			return FALSE;
		}
	#endif
	}
	
	//采用自制的序列化流程
	FILE *fp = NULL;
	BOOL bUnicode = FALSE;
	g_OpenFileName = lpszPathName;
	
	if ( !b3DFile )//2D格式文件
	{
		bUnicode = CStdioFileEx::IsFileUnicode(lpszPathName);
		fp = _tfopen(lpszPathName, bUnicode ? _T("rb") : _T("rt") );
		if ( fp == NULL )	return FALSE;
	}
	else //3D格式文件
	{
		fp = _tfopen(lpszPathName,_T("rb"));
		if  ( fp == NULL )  return FALSE;
		/////////////////////
		HDFILEHEAD head;
		fread(&head, sizeof(HDFILEHEAD), 1, fp);
		if ( strlen(head.szFlag) > 0  && strncmp(head.szFlag,"^3D^",4)  !=  0 )
		{
			AfxMessageBox(IDS_ERROR_FILEFORMAT);
			return FALSE;
		}
		if ( strcmp(head.szVersion,"")  !=  0 )
			bUnicode = TRUE;

		if ( !bUnicode  ) //2D部分为ANSI,需要转换
		{
			fclose(fp);
			fp = _tfopen(lpszPathName,_T("rt"));
			fseek(fp,sizeof(HDFILEHEAD),0);
		}
	}
	
#ifdef _UNICODE
	char* old_locale = NULL;
	if ( !bUnicode )
	{
		//unicode下读取Ansi文件，需要设定区域，否则多字节字符将无法准确读出
		CSetLangRegionDlg dlg;
		if ( IDOK == dlg.DoModal() )
		{
			old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
			setlocale( LC_CTYPE,dlg.m_szLang); 
		}
		else
		{
			fclose(fp);
			return  FALSE;
		}
	}
#endif

	BeginWaitCursor();
	///////////////
	Serialize(fp,FALSE,bUnicode); //2D数据序列化
	//////////////////////
#ifdef _UNICODE
	if ( !bUnicode )//ANSI文件
	{
		setlocale( LC_CTYPE, old_locale ); //还原语言区域 
		free( old_locale );
	}
#endif

	if ( b3DFile )
	{
		if ( !bUnicode ) //2D数据为ANSI,需要重新打开
		{
			int curPos = ftell(fp);
			fclose(fp);
			////////////
			fp = _tfopen(lpszPathName,_T("rb"));
			fseek(fp,curPos,0);
		}
		m_chain3d.Serialize( fp, FALSE );
		OnModuleSwitch();
	}
	fclose( fp );
	EndWaitCursor();

	//////////////
	int nLayerCount = m_lay.GetSize();
	//打标控制参数设置缺省值
	for( int i = nLayerCount; i < g_SysCfg.m_nLayerCnt; i++ )
	{
		CLayer ly;
		ly.m_nLayer = i;
		ly.m_para.emcc = g_DAT.m_stEMCCParaDft;
		switch(i)
		{
		case 0:
			ly.m_color = BLACK;
			break;
		case 1:
			ly.m_color = RED;
			break;
		case 2:
			ly.m_color = YELLOW;
			break;
		case 3:
			ly.m_color = GREEN;
			break;
		case 4:
			ly.m_color = YANGREEN;
			break;
		case 5:
			ly.m_color = BLUE;
			break;
		case 6:
			ly.m_color = MAGENTA;
			break;
		case 7:
			ly.m_color = GRAY;
			break;
         default:
			ly.m_color = BLACK;
			break;
		}
		m_lay.Add(ly);
	}
	
	m_lay.SetCur(0);
	CMainFrame *pFWnd = (CMainFrame*)theApp.m_pMainWnd;
	if ( NULL  !=  pFWnd )
	{
		pFWnd->m_PropDlg.OnNewDocument(m_lay);
		pFWnd->m_PSDlg.OnNewDocument();
	}
	
	m_quadTracker = m_chain.GetLimit();
	return TRUE;
}



//
void CHLDoc::DeleteCmd()
{
	CQuad quad;
	m_chain.CalLimit(CDot(0,0),TRUE);
	m_quadTracker = m_chain.GetLimit();
	
	m_chain.Delete();
	SetModifiedFlag();
	
	SetTrackerQuad(m_quadTracker);
	quad.SetEmpty();
	SetTrackerQuad(quad);
}


//
void CHLDoc::Delete()
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		DeleteCmd();
	else
		m_cmdProc.Submit(new CCmdDelete(this));

	CHLView   *pView = GetCurActiveView();
	if ( NULL == pView )
		return;
	
	if ( NULL == m_pTracker )
	{
		m_pTracker = &(pView->m_track);
	}
	
	m_pTracker->ClearAll();
	ReLoadStrokeList();
	
	pView->DispSelObjProp();
}

//
void CHLDoc::GroupCmd() 
{
	m_chain.Group();
	m_pTracker->ClearAll();
	ReLoadStrokeList();
	SetModifiedFlag();
	
	RefreshSelectQuadList();
}



void CHLDoc::OnGroup() 
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		GroupCmd();
	else
		m_cmdProc.Submit(new CCmdGroup(this));
}


void CHLDoc::UngroupCmd() 
{
	m_chain.UnGroup();

	SetModifiedFlag();
	RefreshSelectQuadList();
}


void CHLDoc::OnUnGroup()
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		UngroupCmd();
	else
		m_cmdProc.Submit(new CCmdUngroup(this));
}


void CHLDoc::OnUpdateGroup(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if ( m_nStatus&CChain::selectMul  && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CHLDoc::OnUpdateUnGroup(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if ( m_nStatus&CChain::selectGroup && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

BOOL CHLDoc::OutOfLimit(BOOL bCheckX,BOOL bCheckY)
{
	if (m_b3D)
	{
		return m_chain3d.OutOfLimit();
	}
	
	//
	CQuad qd = m_chain.GetMarkRange();
	
	double dbBoxX = 0;
	double dbBoxY = 0;
	g_DAT.GetMarkRect(dbBoxX,dbBoxY);
	
	double dbAjust = 0.001;
	double x1 = (CENTER-dbBoxX/2) - dbAjust;
	double x2 = (CENTER+dbBoxX/2) + dbAjust;
	double y1 = (CENTER-dbBoxY/2) - dbAjust;
	double y2 = (CENTER+dbBoxY/2) + dbAjust;
	
	if ( bCheckX && (qd.left < x1 || qd.right > x2) )
		return TRUE;
	
	if ( bCheckY && (qd.bottom < y1 || qd.top > y2) )
		return TRUE;
	
	return FALSE;
}


//
void CHLDoc::OnRedPreview()
{
	if (!(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	if ( !g_DAT.IsCheckStatusOK() )
		return;
	
	RedPreview();
}



//
void CHLDoc::RedPreview(BOOL bIsRed, BOOL bIsArray) 
{
	theApp.m_SelMark.StopSelMark();
	if ( !g_bIsRedPreview )
	{
		g_bIsRedPreview = TRUE;
		g_DAT.SetLay(m_lay);
		g_DAT.OutStatus(O_RED, O_RED);
		theApp.m_RedPreView.StartPreview(bIsRed, bIsArray);
	}
	else
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
		g_DAT.OutStatus(O_RED, 0);
	}
}


//
void CHLDoc::OnUpdateRedPreview(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK;
#ifdef _SOFTMODE_EDITOR_
	bEnable = FALSE;
#endif

	pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck( g_bIsRedPreview );
}

//
void CHLDoc::OnMarkNormal()
{
	if ( !g_DAT.IsCheckStatusOK() )
		return;
	
	if ( OutOfLimit() )
	{
		AfxMessageBox(IDS_ERROT_OUTAREA);
		return;
	}
	
	//打标参数更新
	g_DAT.SetLay(m_lay);
	
	//没有打标内容
	if ( m_chain.GetCount() == 0 )
		return;
	
	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview(); 
		return;
	}
	if ( g_bIsSelMark )
	{
		theApp.m_SelMark.StopSelMark();
		return;
	}
	if ( g_bIsFastMark )
	{
		theApp.m_FastMark.StopFastMark();
		return;
	}
	
	CMarkNormalDlg dlg;
	dlg.m_pDoc = this;
	dlg.m_pChain = (CChain *)&m_chain;
	dlg.m_pChain3d =  m_b3D?&m_chain3d:NULL;
	if ( g_DAT.EnterMarkEnv() )
	{
		//硬件部分准备就绪时才能进入此操作
		dlg.DoModal();
		g_DAT.LeaveMarkEnv();
	}
	g_DAT.m_hMarkWnd = NULL;
	
	UpdateAllViews(NULL,NULL);
}

//
BOOL CHLDoc::IsSelectedObj()
{
	return (!(m_nStatus == CChain::selectNull));
}

//
void CHLDoc::SetTrackerQuad(CQuad &quad,double angle)
{
	m_quadTracker = quad;
	m_quadTracker.Normal();
	
	double dbX(0),dbY(0);
	g_DAT.GetMarkRect(dbX,dbY);
	dbX = max(100.0,dbX);
	double dbExt = 5.0*dbX/100.0;
	
	CQuad Quad  = quad;
	Quad.top     +=  dbExt;
	Quad.bottom  -=  dbExt;
	Quad.left    -=  dbExt;
	Quad.right   +=  dbExt;
	
	//更新所有的视
	if ( !m_b3D )
	{
		UpdateAllViews( NULL,UPDATE_LOGIC,(CObject *)(&Quad) );
	}
	//得到当前的选择状态
	m_nStatus = m_chain.GetStatus();
}

//
CQuad CHLDoc::GetTrackerQuad()
{
	m_quadTracker.Normal();
	return m_quadTracker;
}


//
void CHLDoc::SetObjSelInQuadList()
{
	if ( !CheckSetTrack() )
		return;

	/////// add by zhq,2014-05-07
	m_pTracker->ClearStrokeList();
	////////////////////////////

	int nCount = m_QuadList.GetSize();
	for( int i = 0; i < nCount; i++ )
	{
		SELQUAD SQD = m_QuadList.GetAt(i);
		if ( TRUE == SQD.m_bSel )
		{
			CGroupObj *pGroupObj = (CGroupObj *)(m_QuadList[i].m_pGroupObj);
			pGroupObj->SetSelect(TRUE);
			pGroupObj->SetStrokeList(&m_pTracker->m_StrokeList);
		}
	}
	
	SetTrackerQuad(GetLimit());
}


void CHLDoc::MoveCmd( double moveX,double moveY, double moveZ )
{
	m_chain.Move(moveX,moveY,moveZ);
	SetModifiedFlag();
	
	RefreshSelectQuadList();
}


void CHLDoc::Move(double moveX, double moveY, double moveZ )
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		MoveCmd(moveX,moveY,moveZ);
	else
		m_cmdProc.Submit(new CCmdMove(this,moveX,moveY,moveZ));
}

//
void CHLDoc::ScaleCmd(CDot dot, double ratioX, double ratioY)
{	
	m_chain.Scale(dot,ratioX,ratioY);
	SetModifiedFlag();
	
	SetTrackerQuad(m_chain.GetLimit());
}

//
void CHLDoc::Scale(CDot dot, double ratioX, double ratioY)
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		ScaleCmd(dot,ratioX,ratioY);
	else
		m_cmdProc.Submit(new CCmdScale(this,dot,ratioX,ratioY));
}

void CHLDoc::RotateCmd(CDot dot, double angle, int nPlane )
{
	CPoint pointtemp(0,13);
  	CQuad quad = m_chain.GetLimit();

	m_chain.Rotate(dot,angle,nPlane);
	SetModifiedFlag();

	RefreshSelectQuadList();
}

void CHLDoc::Rotate(CDot dot, double angle, int nPlane )
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	if ( m_cmdProc.GetMaxCommands() == 0 )
		RotateCmd(dot,angle,nPlane);
	else
		m_cmdProc.Submit(new CCmdRotate(this,dot,angle,nPlane ));
}



void CHLDoc::ShearCmd(CDot dot, double dbAngleX, double dbAngleY)
{
	m_chain.Shear(dot, dbAngleX, dbAngleY);
	SetModifiedFlag();

	RefreshSelectQuadList();
}


void CHLDoc::Shear(CDot dot, double dbAngleX, double dbAngleY)
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	if ( m_cmdProc.GetMaxCommands() == 0 )
		ShearCmd(dot,dbAngleX,dbAngleY);
	else
		m_cmdProc.Submit(new CCmdShear(this,dot,dbAngleX,dbAngleY));
}

void CHLDoc::Copy()//阵列
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		CopyCmd();
	else
		m_cmdProc.Submit(new CCmdCopy(this));
}


void CHLDoc::CopyCmd()//阵列时chain内部新增变量
{
	m_chain.Copy();
	/////
	SetModifiedFlag();
	RefreshSelectQuadList();
}



void CHLDoc::OnEditCopy()
{
	BeginWaitCursor();
	/////

	FILE *fp = NULL;
	if ( ( fp = _tfopen(g_StrCopyTmpFile,_T("wb")))  !=  NULL )
	{
		m_chain.m_MgrText.Clear();

		g_bClipBoardCopyNode = TRUE;
		m_chain.Clipboard(fp,TRUE);
		g_bClipBoardCopyNode = FALSE;
		fclose( fp );
	}
	EndWaitCursor();
}


void CHLDoc::PasteCmd()
{
	BeginWaitCursor();

	FILE *fp = NULL;
	//
	if ( ( fp = _tfopen(g_StrCopyTmpFile,_T("rb")))  !=  NULL )
	{
		m_chain.Clipboard(fp,FALSE);
		fclose( fp );
	}
	
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
	UpdateAllViews(NULL);

	EndWaitCursor();
}


void CHLDoc::OnEditPaste()
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		PasteCmd();
	else
		m_cmdProc.Submit(new CCmdPaste(this));

	if ( !CheckSetTrack() )
		return;
	
	m_pTracker->ClearAll();
	ReLoadStrokeList();
}

void CHLDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if ( m_nStatus == CChain::selectNull || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(!m_b3D);
	}
}


void CHLDoc::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	CFileStatus st;
	BOOL bEnable = CFile::GetStatus(g_StrCopyTmpFile,st);
	bEnable  &=  !m_b3D;
	
	pCmdUI->Enable(bEnable && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) );
}

void CHLDoc::OnEditCut()
{
	m_bEditCut = TRUE;
	OnEditCopy();
	Delete();
	m_bEditCut = FALSE;
}

void CHLDoc::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	if ( m_nStatus == CChain::selectNull || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT)  )
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(!m_b3D);
	}
}

void CHLDoc::OnEditDelete() 
{
	Delete();
}

void CHLDoc::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	if ( m_nStatus == CChain::selectNull || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(!m_b3D);
	}
}

void CHLDoc::OnEditRedo() 
{
	if ( m_b3D )
	{
		AfxGetApp()->BeginWaitCursor();
		m_chain3d.Redo();
		SetModifiedFlag(TRUE);
		AfxGetApp()->EndWaitCursor();

		m_chain3d.Draw();
	}
	else
	{
		m_cmdProc.Redo();
		RefreshSelectQuadList();
	}
}

void CHLDoc::OnEditUndo() 
{	
	if ( m_b3D )
	{
		AfxGetApp()->BeginWaitCursor();
		m_chain3d.Undo();
		SetModifiedFlag(TRUE);
		AfxGetApp()->EndWaitCursor();
		m_chain3d.Draw();
	}
	else
	{
		m_cmdProc.Undo();
		RefreshSelectQuadList();

		CHLView*  pView = GetCurActiveView();
		// add by zhq
		if( pView->m_nDrawTool == DRAWMODEL_DRAG )
			pView->m_nDrawTool = DRAWMODEL_CLICK;

		if ( pView == NULL )
			return;
		
		pView->DispSelObjProp();
	}
}


void CHLDoc::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	if ((g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT))
	{
		if ( m_b3D ) 
			pCmdUI->Enable(m_chain3d.CanRedo());
		else 
			pCmdUI->Enable(m_cmdProc.CanRedo());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


void CHLDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	if ( (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		if ( m_b3D ) 
			pCmdUI->Enable(m_chain3d.CanUndo());
		else 
			pCmdUI->Enable(m_cmdProc.CanUndo());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


CChain & CHLDoc::GetChain()
{
	return m_chain;
}

void CHLDoc::SetChain( const CChain &chain )
{
	m_chain.ReStoreChain(chain);
	/////
	ReLoadStrokeList();
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
}


void CHLDoc::OnEditSelectAll()
{
	if ( m_chain.GetCount()  <=  0 )
		return;
	
	m_chain.SetSelect();
	RefreshSelectQuadList();
}

//
void CHLDoc::FillCmd(FILLDATA fill)
{
	if ( !CheckSetTrack() )
		return;
	
	m_pTracker->ClearAll();
	m_chain.SetFillStyle(fill,m_pTracker);
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
}


//
void CHLDoc::OnDrawFill() 
{
	if ( m_nStatus == CChain::selectNull )
	{
		AfxMessageBox(IDS_NOOBJECT,MB_ICONWARNING);
		return;
	}

	BOOL bCanFill = TRUE;
	if ( m_nStatus == CChain::selectSingle )
	{
		CNode *pNode = GetSelNode(-1);
		if ( pNode == NULL )
			return;
		
		int nType = pNode->GetType();
		if (	(nType == NODE_DELAY)			
			|| (nType == NODE_INPUT) 
			|| (nType == NODE_OUTPUT)			
			|| (nType == NODE_MOTORMOVE)
			|| (nType == NODE_MOTORPOS)		
			|| (nType == NODE_CONDITIONS)
			|| (nType == NODE_CONDITIONE)	
			|| (nType == NODE_BMP)
			|| (nType == NODE_DOT)
			|| (nType == NODE_BUFFERLASER)
			|| (nType == NODE_CCD)
			|| (nType == NODE_POSCALI)
			)
		{
			bCanFill = FALSE;
		}
		else if ( nType == NODE_CODEBAR )
		{
			CNodeCodebar *pBarcode = (CNodeCodebar*)pNode;
			if ( pBarcode->m_bAutoFill )
				bCanFill = FALSE;
		}
	}
	
	if ( bCanFill )
	{
		FILLDATA fillOld,fillNew;
		m_chain.GetFillStyle(fillOld);
		
		CDrawFillDlg dlg;
		dlg.m_fillData1  = fillOld.newFileData1;
		dlg.m_fillData2  = fillOld.newFileData2;
		dlg.m_bFillFirst  = fillOld.bFillFirst;
		dlg.m_bFillAlign  = fillOld.bFillAlign;
		
		if ( dlg.DoModal() == IDOK )
		{
			fillNew.newFileData1  = dlg.m_fillData1;
			fillNew.newFileData2  = dlg.m_fillData2;
			fillNew.bFillFirst		 = dlg.m_bFillFirst;
			fillNew.bFillAlign		 = dlg.m_bFillAlign;
			
			if ( m_cmdProc.GetMaxCommands() == 0 )
				FillCmd(fillNew);
			else
			{
				CCmdFill* pFillCmd = new CCmdFill(this,fillNew,fillOld);
				m_cmdProc.Submit(pFillCmd);
			}
			
			CHLView   *pView = GetCurActiveView();
			if ( pView ) 
			{
				pView->DispSelObjProp();
			}
		}
	}
}


//
void CHLDoc::OnExtendObj(BOOL bIsKillOldObj)
{
	if ( m_nStatus == CChain::selectNull )
	{
		AfxMessageBox(IDS_NOOBJECT, MB_ICONWARNING);
	}
	else
	{
		if ( m_cmdProc.GetMaxCommands() == 0 )
			ExtendObjCmd(g_SysCfg.m_dbExtendSpace, bIsKillOldObj);
		else
			m_cmdProc.Submit(new CCmdExtendObj(this, g_SysCfg.m_dbExtendSpace, bIsKillOldObj));
	}
}

//
void CHLDoc::ExtendObjCmd(double dbSpace, BOOL bIsKillOldObj)
{
	CHLView   *pView = GetCurActiveView();
	if ( pView == NULL )
		return;

	if ( NULL == m_pTracker )
	{
		m_pTracker = &(pView->m_track);
	}

	m_pTracker->ClearAll();
	m_chain.ExtendObj(dbSpace,bIsKillOldObj, m_pTracker, m_bExternOpt);
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
	
	pView->Invalidate();
}

//
void CHLDoc::OnChangeCenter() 
{
	//居中
	CDot dot = m_quadTracker.GetBaseDot();
	double moveX = CENTER-dot.x;
	double moveY = CENTER-dot.y;

	Drag(dot,1,1,moveX,moveY);	
	RefreshSelectQuadList();
	DispSelObjProp();
}


void CHLDoc::OnUpdateChangeCenter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( m_nStatus == CChain::selectNull || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CHLDoc::OnChangeCcw() 
{
	// TODO: Add your command handler code here
	CDot dot = m_quadTracker.GetBaseDot();
	Rotate(dot,90);
}

//
void CHLDoc::OnUpdateChangeCcw(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( m_nStatus == CChain::selectNull  || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

//
void CHLDoc::OnChangeCw() 
{
	// TODO: Add your command handler code here
	CDot dot = m_quadTracker.GetBaseDot();
	Rotate(dot, -90);
}

//
void CHLDoc::OnUpdateChangeCw(CCmdUI* pCmdUI) 
{
	if (m_nStatus == CChain::selectNull || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

//
void CHLDoc::OnChangeMirrorX() 
{
	CDot dot = m_quadTracker.GetBaseDot();
	Drag(dot,-1,1,0,0);
}

///////////////////////////////
void CHLDoc::OnUpdateChangeMirrorX(CCmdUI* pCmdUI) 
{
	if ( m_nStatus == CChain::selectNull || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CHLDoc::OnChangeMirrorY() 
{
	CDot dot = m_quadTracker.GetBaseDot();
	Drag(dot,1,-1,0,0);
}

void CHLDoc::OnUpdateChangeMirrorY(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( m_nStatus == CChain::selectNull || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

//
void CHLDoc::LayChange(int mode,CLayer &ly)
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		SetLayCmd(mode,ly);
	else
		m_cmdProc.Submit(new CCmdSetLay(this,mode,ly));
}


//
void CHLDoc::SetLayCmd(int mode,CLayer &ly)
{
	switch(mode)
	{
	case PTY_ADD:
		m_lay.AddNew();
		break;
	case PTY_LAYER:
		m_lay.Set(mode,ly);
		m_chain.SetProperty(mode,ly.GetProperty());
		break;
	case PTY_PARA:
		m_lay.Set(mode,ly);
		break;
	case PTY_OUTLINE:
		g_DAT.SetLay(m_lay);
		m_chain.SetLayColor(ly.GetProperty().m_nLayer, -1, -1);
		break;
	case PTY_FILL:
	    g_DAT.SetLay(m_lay);
		m_chain.SetLayColor(-1, ly.GetProperty().m_nLayer, ly.GetProperty().m_nLayer);
		break;
	case PTY_DEL:
		//将被删层所对应的图形对象移至第0层
		m_chain.SetProperty(mode,ly.GetProperty());
		m_lay.Del(ly.m_nLayer);
		break;
	case PTY_CHANGE:
		m_lay.SetCur(ly.m_nLayer);
		break;
	}

	SetModifiedFlag();
}

//
CLayer CHLDoc::GetCurLayer()
{
	return m_lay.GetCurLayer();
}


CLayer CHLDoc::GetLayer(int index)
{
	return m_lay.GetLayer(index);
}


void CHLDoc::SetLayer(CLayer &layer)
{
	m_lay.SetLayer(layer);

	CMainFrame *pFrame = (CMainFrame *)theApp.m_pMainWnd;
	if (pFrame)
	{
		pFrame->m_PropDlg.DoUndoLayerPara();
	}
}


//
void CHLDoc::SetLayerColor(int index, COLORREF color)
{
	m_lay.SetLayerColor(index, color);
}

BOOL CHLDoc::GetLockFlg(int index)
{
	return m_lay.GetLockFlg(index);
}


int CHLDoc::GetLaySize()
{
	return m_lay.GetSize();
}

//
void CHLDoc::SetCurLayer(int cur)
{
	m_lay.SetCur(cur);
}


//
int CHLDoc::GetCur()
{
	return m_lay.GetCur();
}


//
CLay & CHLDoc::GetLay()
{
	return m_lay;
}


//
void CHLDoc::SetLay(const CLay &lay)
{
	m_lay = lay;
	SetModifiedFlag();
	
	CMainFrame *pFrame = (CMainFrame *)theApp.m_pMainWnd;
	if ( pFrame )
	{
		pFrame->m_PropDlg.UpdateProperty();
	}
}

//
void CHLDoc::OnMarkPreview() 
{	
	CMarkPreviewDlg dlg;		
	dlg.m_bPathOptimize = m_bPathOptimize;

	if ( m_bPathOptimize )
	{	
		StrokeList listPreview;
		listPreview.AddTail(&(m_chain.m_listStroke));
		m_chain.LoadStrokeList(&listPreview, FALSE, -1);
		
		dlg.SetStrokeList(&listPreview);
	}
	else
	{
		g_DAT.SetLay(m_lay);
		m_chain.PreprocessMark();
		dlg.m_pChain = (CChain *)&m_chain;
	}
	
	dlg.DoModal();
}

//
int CHLDoc::GetMenuIndex()
{	
	//根据当前选中的对象确定右键菜单
	return m_chain.GetMenuIndex();
}

void CHLDoc::DrawsetCmd() 
{
	CProperty pty =  m_lay.GetCurLayer().GetProperty();
	CNode *pNode = m_chain.GetNode();
	if ( pNode == NULL )
		return;

	if ( !CheckSetTrack() )
		return;

	if ( pNode->GetType() == NODE_HLTEXT )
	{		
		EditTextNode(pNode);
	}

	//条码设置在此处处理
	if ( pNode->GetType() == NODE_CODEBAR )
	{
		//文本方式则在此处处理
		pNode->Create(CRect(0,0,0,0));
		//用于刷新文本内容
		m_chain.PreprocessMark(0x04);
		SetModifiedFlag();
		SetTrackerQuad(m_chain.GetLimit());
		DispSelObjProp();
	}
}

void CHLDoc::OnDrawSet() 
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		DrawsetCmd();
	else
		m_cmdProc.Submit(new CCmdDrawSet(this));

	if ( !CheckSetTrack() )
		return;

	m_pTracker->ClearAll();
	ReLoadStrokeList();
}

void CHLDoc::ArrayCmd(CArray<OBJPOS, OBJPOS> &dtArray)
{
	m_chain.m_MgrText.SetCopySts(TRUE);
	///
	m_chain.Array(dtArray);
	if (m_chain.HaveSelectObject())
	{
		SetModifiedFlag();
	}
	RefreshSelectQuadList();
	////
	m_chain.m_MgrText.SetCopySts(FALSE);
}

void CHLDoc::OnUpdateArray(CCmdUI* pCmdUI) 
{
	if ( m_nStatus == CChain::selectNull  || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


void CHLDoc::OnArray()
{
	CArrayDlg dlg;
	CDot dot = m_quadTracker.GetBaseDot();
	dlg.m_dotBase = dot;

	dlg.m_bAuto = ::g_sys.GetProfileInt(_T("OPTION"),_T("ARRAY"),0);
	if ( dlg.DoModal() == IDOK )
	{	
		if ( m_nStatus == CChain::selectNull )
		{
			AfxMessageBox(IDS_NOOBJECT,MB_ICONWARNING);
		}
		else
		{
			if ( m_cmdProc.GetMaxCommands() == 0 )
			{
				g_dwJumpCnt = dlg.m_bAuto ? 0 : -1;
				ArrayCmd(dlg.m_dtArray);
			}
			else
				m_cmdProc.Submit(new CCmdArray(this,dlg.m_dtArray,dlg.m_bAuto));
		}
		
		::g_sys.WriteProfileInt(_T("OPTION"),_T("ARRAY"),dlg.m_bAuto);
	}
}

void CHLDoc::GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray)
{
	m_chain.GetArrayDot(dtArray);
}

void CHLDoc::SetSelectFlag(BOOL *pFlag)
{
    SetTrackerQuad(m_chain.SetSelectFlag(pFlag));
}

void CHLDoc::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_chain.GetCount() && !m_b3D);
}


void CHLDoc::Mark(HWND hWnd,int repeat)
{
	m_chain.Mark(hWnd);
}

//
BOOL CHLDoc::PreprocessMark()
{
	g_DAT.SetLay(m_lay);
	//////
	if ( !m_chain.PreprocessMark() )
		return FALSE;
	
	return TRUE;
}




//
int CHLDoc::GetMarkUnit()
{
	return m_chain.GetCount();
}


//
void CHLDoc::OnDrawBmp()
{
	CNodeDib *pNode = (CNodeDib *)(m_chain.GetNode());
	if ( pNode && (pNode->GetType() == NODE_BMP) )
	{
		CDibDlg dlg;
		CString str			 =  pNode->m_strBmpName;
		dlg.m_strBmpName	 =  str;
		dlg.m_dbSpaceX		 =  pNode->m_dbSpaceX;
		dlg.m_dbSpaceY		 =  pNode->m_dbSpaceY;
		dlg.m_dbDotTime		 =  pNode->m_dbDotTime;
		dlg.m_bOppsitioncol = pNode->m_bOppsitionCol;
		dlg.m_bLoop			 =  pNode->m_bLoop;
		dlg.m_nDibType		 =  pNode->m_nDibType;
		dlg.m_dbLoopCompate = pNode->m_dbLoopCompate;
		dlg.m_nAccTime      = pNode->m_nAccTime;
		dlg.m_dbBrightness  = pNode->m_dbBrightness;
		dlg.m_dbContrast    = pNode->m_dbContrast;
		dlg.m_nMinFDL		= pNode->m_nMinFDL;
		dlg.m_nMaxFDL		= pNode->m_nMaxFDL;
		
		PARA para			 =  m_lay.GetPara(pNode->m_property.m_nLayer);
		dlg.m_dbMarkSpeed	 =  para.emcc.dbMarkV/(1e6);
		if ( dlg.DoModal() == IDOK )
		{
			pNode->m_bOppsitionCol	 =  dlg.m_bOppsitioncol;
			pNode->m_dbSpaceX		 =  dlg.m_dbSpaceX; 
			pNode->m_dbSpaceY		 =  dlg.m_dbSpaceY;
			pNode->m_dbDotTime		 =  dlg.m_dbDotTime;
			pNode->m_bLoop			 =  dlg.m_bLoop;
			pNode->m_nDibType		 =  dlg.m_nDibType;
			pNode->m_dbLoopCompate   =  dlg.m_dbLoopCompate;
			pNode->m_nAccTime		 =  dlg.m_nAccTime;
			pNode->m_dbBrightness	 =  dlg.m_dbBrightness;
			pNode->m_dbContrast		 =  dlg.m_dbContrast;
			pNode->m_nMinFDL		 =  dlg.m_nMinFDL;
			pNode->m_nMaxFDL		 =  dlg.m_nMaxFDL;
			
			pNode->DataChange();
			m_chain.CalLimit(CDot(0,0));
			SetTrackerQuad(m_chain.GetLimit());
		}
	}
	else
	{
		AfxMessageBox(IDS_NOOBJECT,MB_ICONWARNING);
	}
}


void CHLDoc::SetSelectedObjbeMarked(BOOL bFlag)
{
	m_chain.SetSelectedObjbeMarked(bFlag);
	m_chain3d.SetSelectedObjbeMarked(bFlag);
}

void CHLDoc::SetLayerCmd(int nLayer, int nFillLayer, int nFillScalLayer)
{
	m_chain.SetLayColor( nLayer, nFillLayer, nFillScalLayer);
	SetTrackerQuad(m_chain.GetLimit());
}

BOOL CHLDoc::GetLayerSetFromDlg(int &nLayer, int &nFillLayer, int &nFillScalLayer)
{
	g_DAT.SetLay(m_lay);
	CSetLayer dlg;
	m_chain.GetLayColor(dlg.m_nLayer,dlg.m_nFillLayer, dlg.m_nFillScalLayer);

	if ( IDOK == dlg.DoModal())
	{
		CLayer& lay = GetLayer(dlg.m_nLayer);
		CLayer& layFill = GetLayer(dlg.m_nFillLayer);
		CLayer& layFillScal = GetLayer(dlg.m_nFillScalLayer);
		if ( lay.m_nLayer  !=  dlg.m_nLayer || layFill.m_nLayer  !=  dlg.m_nFillLayer || layFillScal.m_nLayer  !=  dlg.m_nFillScalLayer )
		{
			AfxMessageBox(IDS_LAYER_LIMIT);
			return FALSE;
		}

		nLayer = dlg.m_nLayer;
		nFillLayer = dlg.m_nFillLayer;
		nFillScalLayer = dlg.m_nFillScalLayer;
		return TRUE;
	}

	return FALSE;
}

void CHLDoc::OnSetLayer()
{
	int nLayer = 0;
	int nFillLayer = 0;
	int nFillScalLayer = 0;
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		if( GetLayerSetFromDlg(nLayer,nFillLayer,nFillScalLayer) )
			SetLayerCmd(nLayer,nFillLayer,nFillScalLayer);
	}
	else
	{
		if( GetLayerSetFromDlg(nLayer, nFillLayer, nFillScalLayer) )
			m_cmdProc.Submit(new CCmdChangLay(this,nLayer,nFillLayer,nFillScalLayer));
	}
}


void CHLDoc::SetSelect(BOOL bFlag)
{
	m_chain.SetSelect(bFlag);
}

void CHLDoc::UpdatePerUnit()
{
	m_chain.CalLimit(CDot(0,0));
	SetTrackerQuad(m_chain.GetLimit());
}

void CHLDoc::GetSelectedStrokeList(StrokeList *pStrokeList)
{
	m_chain.GetSelectedStrokeList(pStrokeList);
}

void CHLDoc::AddTree(CTree *pAddTree)
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		AddNodeCmd(pAddTree);
	else
		m_cmdProc.Submit(new CCmdAddNode(this,pAddTree));
}

//建立与条码关联的文本设置
void CHLDoc::OnDrawBartext()
{
	CNode *pNode = m_chain.GetNode();
	if ( pNode == NULL )
		return;
	
	if ( pNode->GetType() == NODE_CODEBAR )
	{
		CNodeCodebar* pNodeBar = (CNodeCodebar*)pNode;
		CNodeText* pNodeText = new CNodeText();
		if ( pNodeText == NULL )
			return;

		pNodeText->SetParent(&m_chain);
		CUIntArray arrayID;
		for ( UINT i = 0 ; i< pNodeBar->GetTextCount(); i++ )
		{
			CText *pText = (CText*)pNodeBar->GetText(i);
			if ( pText  !=  NULL )
				arrayID.Add(pText->GetTextID());
		}
		pNodeText->CreateFromBar(arrayID);
		pNodeText->m_bBarText = TRUE;
		pNodeText->m_nBarType = (int)(pNodeBar->m_BarType);
		pNodeText->m_bParity = pNodeBar->m_bParity;
		pNodeText->m_nErrLev = pNodeBar->m_nErrLev;
		pNodeText->m_nSize = pNodeBar->m_nHPoint;
		pNodeText->m_nCodeType = pNodeBar->m_nCodeType;
		
		if ( pNodeText->Create(CRect(0,0,0,0)) )
		{
			AddNode(pNodeText);
			SetModifiedFlag();
			SetTrackerQuad(m_chain.GetLimit());
			pNodeBar->m_bTextBar = TRUE;
		}
		else
		{
			delete pNodeText;
		}
	}
}

void CHLDoc::OnExportPlt() 
{
	// TODO: Add your command handler code here
	if ( 0 == m_chain.GetCount() )
		return;
	
	CDlgExportPlt fileDlg(FALSE,_T("PLT"),NULL, OFN_OVERWRITEPROMPT,_T("PLT File(*.plt)|*.plt||"));
	if ( IDOK == fileDlg.DoModal() )
	{
		CStdioFileEx file;
		if ( file.Open(fileDlg.GetPathName(),CFile::modeCreate|CFile::modeWrite) )
		{
			file.WriteString( _T("IN;DF;PA;") );
			m_chain.ExportPlt( file,fileDlg.m_bUnExportFill,fileDlg.m_bScale100 );
			file.Close();
		}
		else
		{
			AfxMessageBox(IDS_PLT_EXPORT_ERR);
		}
	}
}

void CHLDoc::AddRect(CQuad &quad,CWnd *pWnd)
{
	m_chain.AddRect(quad,pWnd);
}

void CHLDoc::Drag(CDot &dot,double ratioX,double ratioY,double moveX,double moveY, double moveZ)
{	
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	if ( m_nStatus == CChain::selectNull )
	    return;
	
	if ( g_SysCfg.m_nUndoCount > 0 )
		m_cmdProc.Submit( new CCmdNewDrag(this,dot,ratioX,ratioY,moveX,moveY,moveZ) );
	else
		DragCmd(dot,ratioX,ratioY,moveX,moveY,moveZ);
	
	SetModifiedFlag();

	RefreshSelectQuadList();
}

void CHLDoc::DragFrame(CQuadRangle& frame, CDot &dot,double ratioX,double ratioY,double moveX,double moveY)
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	if ( m_nStatus == CChain::selectNull )
	    return;
	
	if ( g_SysCfg.m_nUndoCount > 0 )
		m_cmdProc.Submit(new CCmdDragFrame(this, frame, dot,ratioX,ratioY,moveX,moveY));
	else
		DragFrameCmd(frame, dot,ratioX,ratioY,moveX,moveY);
}


void CHLDoc::DotEdit(XTracker *pTrack, CPoint &point, CWnd *pWnd)
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		CHLView *pView = (CHLView*)pWnd;
		pView->m_track.SingleModify(point,pWnd);
		pView->ReplaceDot();
	}
	else
	{
		m_cmdProc.Submit(new CCmdDotEdit(this,point,pWnd));
	}
}


void CHLDoc::DelDot(CWnd *pWnd)
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		CHLView *pView = (CHLView*)pWnd;
		pView->DelDot();
	}
	else
	{
		m_cmdProc.Submit(new CCmdDelDot(this,pWnd));
	}
}

BOOL CHLDoc::SetStrokeList(StrokeList *pStrokeList, CPoint &point, CWnd *pWnd)
{
	return m_chain.SetStrokeList(pStrokeList,point,pWnd);
}


void CHLDoc::CaptureObjectCmd(CQuad &quad, BOOL bUniSel)
{
	if ( !CheckSetTrack() )
		return;
	
	m_chain.CaptureObject(quad, m_pTracker, bUniSel);
}


void CHLDoc::MultiSelect(CQuad &quad, BOOL bUniSel)
{
	CaptureObjectCmd(quad, bUniSel);
	SetTrackerQuad(GetLimit());
}

//
void CHLDoc::DragCmd(CDot &dot, double dbratioX, double dbratioY, double dbmoveX, double dbmoveY, double dbMoveZ )
{
	m_chain.DragSelectGroupObj(dot,dbratioX,dbratioY,dbmoveX,dbmoveY, dbMoveZ);
	SetObjProp();
}

//
void CHLDoc::DragFrameCmd( CQuadRangle& frame, CDot &dot,double ratioX,double ratioY,double moveX,double moveY )
{
	if ( fabs(ratioX-1)  <=  EPSILON && fabs(ratioY-1)  <=  EPSILON )
	{
		DragCmd(dot,ratioX,ratioY,moveX,moveY);
	}
	else
	{
		double dbRA0 = frame.CalRotateAngle();
		double dbSA0 = frame.CalShearAngle();
		RotateCmd(frame.GetBaseDot(BASE_MM), -dbRA0);
		ShearCmd(frame.GetBaseDot(BASE_MM), -dbSA0, 0);

		CDot dotTmp = frame.GetBaseDot(BASE_MM);
		DragCmd(dot,ratioX,ratioY,moveX,moveY);
		ShearCmd(dotTmp, dbSA0, 0);
		RotateCmd(frame.GetBaseDot(BASE_MM), dbRA0);
	}

	UpdateAllViews(NULL);
	SetModifiedFlag();
	RefreshSelectQuadList();
}

//
void CHLDoc::SelectDraw( CGroupObj *pGroupObj )
{
	if ( NULL == m_pTracker )
	{
		CFrameWnd *pFWnd = (CFrameWnd*)theApp.m_pMainWnd;
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if ( NULL == pView )
			return;
		
		m_pTracker = &(pView->m_track);
	}
	
	m_pTracker->ClearStrokeList();
	m_chain.SelectDraw( pGroupObj, m_pTracker );
}


BOOL CHLDoc::HaveSelectObject()
{
	return m_chain.HaveSelectObject();
}

//
BOOL CHLDoc::HitOtherObject(CPoint &point, CWnd *pWnd)
{
	BOOL bflag = m_chain.HitOtherObject(point,pWnd);
	if ( bflag )
	{
		ChangeQuadCmd(point,pWnd);
		CQuad quad = GetLimit();
		SetTrackerQuad(quad);
	}
	else
	{
		CHLView *pView = (CHLView*)pWnd;
		if ( DRAWMODEL_CLICK == pView->m_nDrawTool ) 
		{
			if ( GetKeyState(VK_SHIFT) & SHIFTED )
			{
				m_chain.SetSelInQuadListByDot(m_QuadList, point, pWnd);
			}
			else if ( GetKeyState(VK_CONTROL) & SHIFTED )
			{
				m_chain.SetUnSelInQuadListByDot(m_QuadList, point, pWnd);
				///////
				RefreshSelectQuadList();
				bflag = TRUE;
			}
			else
			{
				m_QuadList.RemoveAll();
			}
		}
	}
	
	return bflag;
}

//
void CHLDoc::AddPoint(CPoint &point,CWnd *pWnd)
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		CHLView *pView = (CHLView*)pWnd;
		if ( pView )
			pView->AddPoint(point);
	}
	else
	{
		m_cmdProc.Submit(new CCmdAddPoint(this,point,pWnd));
	}
}

//
void CHLDoc::ReLoadStrokeList()
{
	if ( !CheckSetTrack() )
		return;
	
	m_pTracker->m_StrokeList.RemoveAll();
	m_chain.ReLoadStrokeList(&m_pTracker->m_StrokeList);
}

//
void CHLDoc::ChangeQuadCmd(CPoint &point, CWnd *pWnd)
{
	if ( !CheckSetTrack() )
		return;
	
	m_chain.ChangeQuad(point, m_pTracker, pWnd, m_QuadList);
}


CQuad CHLDoc::GetAllLimit()
{
	return m_chain.GetAllLimit();
}


CQuad CHLDoc::GetLimit()
{
	return m_chain.GetLimit();
}

CQuadRangle CHLDoc::GetQuadRangle()
{
	return m_chain.GetQuadRangle();
}

BOOL CHLDoc::GetFrameDots(CArray<CDot,CDot>& frameDots)
{
	return m_chain.GetFrameDots(frameDots);
}

void CHLDoc::SplitDot(CWnd *pWnd, double dbPoint)
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		CHLView *pView = (CHLView*)pWnd;
		if ( pView )
			pView->SplitPoint(dbPoint);
	}
	else
	{
		m_cmdProc.Submit(new SplitPointCmd(this,pWnd,dbPoint));
	}
}

void CHLDoc::Changeconvert(BOOL bPopMsg) //转为曲线，适用于将TREE，CODEBAR，CTEXT转为NODEPLT
{
	// TODO: Add your command handler code here
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		ConvertCmd(bPopMsg);
	}
	else
	{
		m_cmdProc.Submit(new CCmdConvert(this, bPopMsg));
	}
}

//
void CHLDoc::ConvertCmd(BOOL bPopMsg)
{	
	CHLView   *pView = GetCurActiveView();
	if ( NULL == pView )
		return;
	
	if ( NULL == m_pTracker )
	{	
		m_pTracker = &(pView->m_track);
	}
	
	m_chain.Convert(bPopMsg);
	m_pTracker->ClearAll();
	
	ReLoadStrokeList();
	
	CQuad quad = GetLimit();
	SetTrackerQuad(quad);
	
	pView->DispSelObjProp();
}

//
void CHLDoc::JoinStrokeCmd(BOOL bPopMsg)
{
	CHLView   *pView = GetCurActiveView();
	if ( NULL == pView )
		return;
	
	if ( NULL == m_pTracker )
	{	
		m_pTracker = &(pView->m_track);
	}
	
	m_chain.JoinStroke(bPopMsg,g_SysCfg.m_dbJoinDistance);
	m_pTracker->ClearAll();
	
	ReLoadStrokeList();
	
	CQuad quad = GetLimit();
	SetTrackerQuad(quad);
	
	pView->DispSelObjProp();
}

//
void CHLDoc::JoinPoint(CWnd *pWnd)
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		if ( pWnd != NULL )
			((CHLView*)pWnd)->JoinPoint();
	}
	else
	{
		m_cmdProc.Submit(new CJoinPointCmd(this,pWnd));
	}
}

//
void CHLDoc::MoveAccurate(double moveX, double moveY)
{	
	CHLView *pView = GetCurActiveView();
	if ( NULL == pView )
		return;
	
	if ( pView->m_nDrawTool == DRAWMODEL_DRAG ) 
	{
		if ( m_cmdProc.GetMaxCommands() == 0 )
		{
			m_pTracker->MoveDot(moveX,moveY);
		}
		else
		{
			m_cmdProc.Submit(new CMoveAccCmd(m_pTracker,moveX,moveY));
		}
	}
	else
	{	
		Drag(CDot(0,0),1.0,1.0,moveX, moveY);
		SetTrackerQuad(GetLimit());
	}
}

void CHLDoc::OnModifyZ()
{
	if ( NULL == m_pTracker )
		return;

	CDlgModifyZ dlg;
	dlg.m_bEditXY = (m_pTracker->GetSelCnt() == 1 ); //Only one dot
	m_pTracker->GetSelDot(dlg.m_dbX, dlg.m_dbY, dlg.m_dbZ);
	if ( dlg.DoModal()  !=  IDOK )
		return;

	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		m_pTracker->ModifyDot(dlg.m_dbX,dlg.m_dbY,dlg.m_dbZ);
	}
	else
	{
		m_cmdProc.Submit(new CModifyZCmd(m_pTracker,dlg.m_dbX,dlg.m_dbY,dlg.m_dbZ));
	}
}

BOOL CHLDoc::OnLockLayer(int nLayer)
{	
	m_lay.SetLockFlg(nLayer, TRUE);
	
	return TRUE;
}


//
BOOL CHLDoc::OnUnLockLayer(int nLayer)
{
	if ( m_bPathOptimize )
	{
		return FALSE;
	}
	
	m_lay.SetLockFlg(nLayer, FALSE);
	return TRUE;
}

//
void CHLDoc::FilterPoint()
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		FilterPointCmd();
	}
	else
	{
		m_cmdProc.Submit(new CCmdFilterDocPoint(this));
	}
}

//
void CHLDoc::FilterPointCmd()
{
	if ( !CheckSetTrack() )
		return;
	
	m_chain.FilterPoint(g_SysCfg.m_dbDotFilterPrecis, g_SysCfg.m_dbDotFilterAngle);
	m_pTracker->ClearAll();	
	ReLoadStrokeList();
	
	CQuad quad = GetLimit();
	SetTrackerQuad(quad);
}


void CHLDoc::OnKillAngle()
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		KillAngleCmd();
	}
	else
	{
		m_cmdProc.Submit(new CCmdKillAngle(this));
	}
}


void CHLDoc::KillAngleCmd()
{	
	CHLView *pView = GetCurActiveView();
	if ( pView == NULL )
		return;

	if ( m_pTracker == NULL )
		m_pTracker = &pView->m_track;
	
	m_pTracker->KillAngle(pView);
	m_pTracker->ClearAll();

	ReLoadStrokeList();

	CQuad quad = GetLimit();
	SetTrackerQuad(quad);
}

//
void CHLDoc::IniSort(CWnd* pWnd)
{
	if ( NULL == pWnd )
		return;
	
	m_chain.InitSort(m_Sort, pWnd);
	m_SortListObj.InitSortPro();

	pWnd->Invalidate();
}


void CHLDoc::DrawMark(CDC* pDC, CWnd *pWnd)
{
	if ( NULL == pDC )
		return;
	
	CHLView *pView = (CHLView*)pWnd;
	CPen   pen(PS_SOLID, 1, RGB(255,0,0));
	CPen  *pOldPen = pDC->SelectObject(&pen);

	CFont fontSort;
    fontSort.CreateFont(13, 0,0,0, 50, FALSE, FALSE, 0,
                         ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                         DEFAULT_PITCH | FF_SWISS, _T("Arial") );
    CFont* pOldFont = pDC->SelectObject(&fontSort);
	pDC->SetTextColor(RGB(255, 255, 255));

	int nCount = m_Sort.GetSize();
	for (int i = 0 ; i < nCount; i++) 
	{
		SORTNODE sNode;
		sNode = m_Sort.GetAt(i);
		
	    double x = sNode.m_SPt.x;
		double y = sNode.m_SPt.y;
		
		pView->PhysicToLogic(x, y);
		
		int nOff = sNode.m_MNum * 21;
		
		CRect rt((int)(nOff + x), (int)(y - 10), (int)(nOff +x + 20),(int)y);

		pDC->FillSolidRect(rt, sNode.m_Color);

		CString strIndex;
		strIndex.Format(_T("%d"), sNode.m_nSort);
		pDC->DrawText(strIndex, &rt, DT_CENTER);
	}
	
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);

}

void CHLDoc::SortAllObj()
{
	m_chain.SortAllObj(m_Sort);
}

void CHLDoc::CreateNewRect(CRect &lRt, CRect &cRt, CRect &nRt)
{
	CRect r;
	BOOL bRet = IntersectRect(r, lRt, cRt);
	if (bRet)
	{
		nRt = lRt;
		nRt.left   +=  20;
		nRt.right  +=  20;
	}
}

BOOL CHLDoc::InitASort(CWnd* pWnd)
{	
	if ( pWnd == NULL )
		return FALSE;

	m_ASortListObj.m_dwSelObj = 0;
	m_chain.InitASort(m_ASort);
	int nCount = m_ASort.GetSize();
	if (nCount < 6)
	{
		AfxMessageBox(IDS_AOTUSORTLESSSIXOBJ);
		m_ASort.RemoveAll();
		return FALSE;
	}

	((CHLView*)pWnd)->Invalidate();
	if ( IDYES == AfxMessageBox(IDS_IFSETSTARTDOT, MB_YESNO) ) 
	{   
		if (!m_ASortListObj.TrackSelPt(pWnd, m_ASort))
		{
			m_ASort.RemoveAll();
			return FALSE;
		}
	}
	
	m_ASortListObj.RunSort(pWnd);
	AfxGetMainWnd()->SetFocus();
	m_ASortListObj.Track(pWnd);
	
	return TRUE;
}

//
void CHLDoc::ExitASort()
{
	m_ASortListObj.ExitSort();
	if ( IDYES  == AfxMessageBox(IDS_APPLYALLOBJECT, MB_YESNO) ) 
	{
		m_chain.AutoSortAllObj(m_ASort, m_ASortListObj.m_dwSelObj);
		m_ASortListObj.m_dwSelObj = 0;
	}
}

void CHLDoc::SetGroupNum(UINT nChar) 
{
	int nNum = -1;
	switch(nChar) 
	{
	case VK_0 :
		nNum = 0;
		break;
	case VK_1 :
		nNum = 1;
		break;
	case VK_2 :
		nNum = 2;
		break;
	case VK_3 :
		nNum = 3;
		break;
	case VK_4 :
		nNum = 4;
		break;
	case VK_5 :
		nNum = 5;
		break;
	case VK_6 :
		nNum = 6;
		break;
	case VK_7 :
		nNum = 7;
		break;
	case VK_8 :
		nNum = 8;
		break;
	case VK_9 :
		nNum = 9;
		break;
	default:
		return;
	}
	m_chain.SetGroupNum(nNum);
}


void CHLDoc::InitSelSort(CWnd* pWnd)
{
	m_chain.InitSelSort(m_GSort);

	CSelSort selSort;
	BOOL bSel = selSort.Track(pWnd, m_GSort);
	if ( bSel )
	{
		m_chain.GSortAllObj(m_GSort);
	}
	
	SetTrackerQuad(GetLimit());
	
	if ( pWnd == NULL )
		return;

	((CHLView*)pWnd)->Invalidate();
}

//
BOOL CHLDoc::isIntersect(CDot&p1 , CDot& p2 , CDot& p3 , CDot& p4 )
{
	double dblMulti		 =  0;
	double dblTmp1	 =  0;
	double dblTmp2	 =  0;

	// 顶点相交
	if ( p1 == p3 || p1 == p4 || p2 == p3 || p2 == p4 )
	{
		return TRUE;
	}

    // 判断两条线段外接矩形是否相交
	if (		min(p1.x,p2.x) > max(p3.x,p4.x) || max(p1.x,p2.x)	< min(p3.x,p4.x) 
		|| min(p1.y,p2.y) > max(p3.y,p4.y)
		|| max(p1.y,p2.y) < min(p3.y,p4.y) )
	{
		return FALSE;
	}

    //计算叉积  
	dblTmp1 = (double)((p1.x - p3.x)*(p4.y-p3.y) - (p4.x-p3.x)*(p1.y - p3.y));
	dblTmp2 = (double)((p4.x -p3.x)*(p2.y - p3.y) - (p2.x-p3.x)*(p4.y-p3.y));
	dblMulti = dblTmp1 * dblTmp2;
	
	if ( dblMulti  >=  0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

double CHLDoc::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return dbD;
}

// 求两点之间的角度
double CHLDoc::GetAngle(CDot &dtB, CDot &dtC)
{
	double dbL = 0;
	double dbK = 0;
	double ang = 0;
	
	// 如果两点重合时弧度为零
	if (fabs(dtB.x - dtC.x) < EPSILON && 
		fabs(dtB.y - dtC.y) < EPSILON)  
	{
		return 0;
	}

	// 如果两点在一条垂线上
	if ( fabs(dtB.x - dtC.x) < EPSILON )       
	{
		if (dtC.y > dtB.y)            // 90度
		{
			ang = PAI / 2;
		}
		else
		{
			ang = PAI * 3 / 2;	      // 270度
		}
	}
	else// 如果两点不在一条垂线上
	{
		dbL  = Distance(dtB,dtC);      // 两点距离
		dbK  = (dtC.x - dtB.x) / dbL;  // 余弦
		ang  = acos(dbK);               //  反余弦求弧度   
		
		if ( dtC.y < dtB.y )
		{
			ang = PAI * 2 - ang;       // 弧度在三 四象限
		}
	}
	
	return ang;
}


void CHLDoc::ConverStrokeCmd()
{
	g_DAT.SetLay(m_lay);
	m_chain.ConverStrokeCmd();
	
	SetModifiedFlag();
	RefreshSelectQuadList();
	m_pTracker->ClearAll();
	ReLoadStrokeList();
}

void CHLDoc::ConvertSelToOneTree()
{
	// TODO: Add your command handler code here
	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		ConverStrokeCmd();
	}
	else
	{
		m_cmdProc.Submit(new CCmdConverStroke(this));
	}
}


BOOL CHLDoc::SelGroupByNum(UINT nChar, CWnd *pWnd)
{
	int nNum = -1;
	switch(nChar) 
	{
	case VK_0 :
		nNum = 0;
		break;
	case VK_1 :
		nNum = 1;
		break;
	case VK_2 :
		nNum = 2;
		break;
	case VK_3 :
		nNum = 3;
		break;
	case VK_4 :
		nNum = 4;
		break;
	case VK_5 :
		nNum = 5;
		break;
	case VK_6 :
		nNum = 6;
		break;
	case VK_7 :
		nNum = 7;
		break;
	case VK_8 :
		nNum = 8;
		break;
	case VK_9 :
		nNum = 9;
		break;
	default:
		return FALSE;
	}

	if ( m_cmdProc.GetMaxCommands() == 0 )
	{
		SelGroupByNumCmd(nNum,pWnd);
	}
	else
	{
		m_cmdProc.Submit(new CCmdSelGroupByNum(this,nNum,pWnd));
	}
	RefreshSelectQuadList();

	return TRUE;
}


void CHLDoc::SelGroupByNumCmd(int nNum, CWnd *pWnd)
{
	if ( !CheckSetTrack() )
		return;
	
	m_chain.SelGroupByNum(nNum, m_pTracker, pWnd);
}


void CHLDoc::SetGroupObjSel(CGroupObj *pGroupObj)
{
	if ( NULL == pGroupObj  )	
		return;
	
	//
	if ( !CheckSetTrack() )
		return;
	
	m_chain.SetGroupObjSel(pGroupObj, m_pTracker);
	
	CQuad quad = GetLimit();
	SetTrackerQuad(quad);
}

//
void CHLDoc::DelObj(CGroupObj *pGroupObj, BOOL bRefresh)
{
	if ( pGroupObj == NULL || !CheckSetTrack() )
		return;

	if ( pGroupObj->GetLockFlg() )
		return;
	
	DelObjCmd((DWORD)pGroupObj);	
	
	if ( !bRefresh )
		return;
	
	if ( m_cmdProc.GetMaxCommands() == 0 )
		DeleteCmd();
	else
		m_cmdProc.Submit(new CCmdDelete(this));
	m_pTracker->ClearAll();

	ReLoadStrokeList();
	DispSelObjProp();
}


//
void CHLDoc::DelObjCmd(DWORD dwGroupObj)
{
	CQuad quad;
	m_chain.CalLimit(CDot(0,0),TRUE);
	m_quadTracker = m_chain.GetLimit();
	m_chain.DelObj(dwGroupObj);
	SetModifiedFlag();
	
	SetTrackerQuad(m_quadTracker);
	quad.SetEmpty();
	SetTrackerQuad(quad);
}

//
void CHLDoc::ObjListLoad()
{
	m_chain.ObjListLoad();
}

void CHLDoc::ObjListUnLoad()
{
	m_chain.ObjListUnLoad();
}

void CHLDoc::OnMAlignCmd(int nType) 
{
	// TODO: Add your command handler code here
	if ( m_cmdProc.GetMaxCommands() == 0 )
		MAlignCmd(nType);
	else
		m_cmdProc.Submit(new CCmdMAlign(this, nType));
}

void CHLDoc::MAlignCmd(int nType)
{
	m_chain.OnMAlign(m_QuadList, nType);

	if ( m_nStatus & CChain::selectMul )
		SetModifiedFlag();
}

double CHLDoc::GetSelTotalLength()
{
	double total = 0;
	int count = m_QuadList.GetSize();
	if ( count == 0 ) 
		return total;

	for (int i = 0; i<count; i++)
	{
		CGroupObj* pGroupObj = (CGroupObj *)(m_QuadList[i].m_pGroupObj);
		total  +=  pGroupObj->GetLength();
	}
	
	return total;
}

void CHLDoc::SetObjProp()
{
	if ( !CheckSetTrack() )
		return;
	
	CQuad quad = GetLimit();
	CDot  dt		 =  quad.GetBaseDot(m_nPosType);
	/////////
	CMainFrame *pMFrame = (CMainFrame*)theApp.m_pMainWnd;
	if ( pMFrame  )
	{
		pMFrame->SetPos(dt.x,dt.y, dt.z);
		pMFrame->SetSize(quad.right-quad.left,quad.top-quad.bottom);
	}

	m_pTracker->ClearAll();
	ReLoadStrokeList();
	SetTrackerQuad(quad);
}

void CHLDoc::OnStatusButton() 
{
	CDcpMsg dlgMsg;
	dlgMsg.m_bLaserSatus = TRUE;
	dlgMsg.DoModal();
}

//
BOOL CHLDoc::IsHaveTextOrCodeBar()
{
	return m_chain.IsHaveTextOrCodeBar();
}

//
void CHLDoc::OnMarkFast()
{	
#ifdef _SOFTMODE_EDITOR_
	return;
#endif

	if (!(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	if ( !IsSelectedObj() )
		return;
	
	if ( !g_DAT.IsCheckStatusOK() )
		return;
		
	g_DAT.SetLay(m_lay);
	CMainFrame *pFrame = (CMainFrame *)theApp.m_pMainWnd;
	if ( NULL == pFrame )
		return;

	pFrame->m_FillEditDlg.CheckFastmark();
}


//
void CHLDoc::OnUpdateMarkFast(CCmdUI* pCmdUI) 
{
	BOOL bEnable = g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK;

#ifdef _SOFTMODE_EDITOR_
	bEnable = FALSE;
#endif
	pCmdUI->Enable(bEnable);
}

//
void CHLDoc::OnDownLoad()
{
#ifdef	_SOFTMODE_EDITOR_
	return;
#endif
	
	CMainFrame *pFrame = (CMainFrame *)theApp.m_pMainWnd;
	if ( NULL == pFrame )
		return;

	if (!(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	pFrame->m_FillEditDlg.DownOfflinedata();
}

//
void CHLDoc::OnClearDownLoad() 
{	
	if (!(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	if ( IDYES == AfxMessageBox(IDS_ISCLEARDOWNLOAD, MB_YESNO) )
	{
		int nStatus = -1;
		UINT nRet = g_DAT.GetWorkStatus(&nStatus);
		if ( nRet  !=  HLOK )
			return;
		
		AfxGetMainWnd()->BeginWaitCursor();

		if ( 0 == nStatus )
			g_DAT.SetWorkStatus(1);
		
		AfxLockTempMaps();
		g_DAT.ClearOfflineFlag();
		AfxUnlockTempMaps(FALSE);

		AfxGetMainWnd()->EndWaitCursor();
	}
}


//
void CHLDoc::OnUpdateMarkSpc(CCmdUI* pCmdUI) 
{
	BOOL bEnable = g_CurUserInfo.dwRight1 & USER_RIGHT_MULMARK;

#ifdef _SOFTMODE_EDITOR_
	bEnable = FALSE;
#endif
	pCmdUI->Enable(bEnable);
}

//
void CHLDoc::OnMarkSpc() 
{
	if (!(g_CurUserInfo.dwRight1 & USER_RIGHT_MULMARK))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	if ( !g_DAT.IsCheckStatusOK() )
		return;

	// TODO: Add your command handler code here
	CHLApp *pApp = (CHLApp *)AfxGetApp();

	CHLDoc *pDoc = NULL;
	CMultiDocTemplate *pDocTem = NULL;
	POSITION posTem = theApp.GetFirstDocTemplatePosition();
	while( posTem  !=  NULL )
	{
		pDocTem = (CMultiDocTemplate *)theApp.GetNextDocTemplate(posTem);
		POSITION posDoc = pDocTem->GetFirstDocPosition();
		while( posDoc  !=  NULL )
		{
			pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
			pDoc->PreprocessMark();
			if ( pDoc->OutOfLimit() )
			{
				AfxMessageBox(IDS_ERROT_OUTAREA);
				return;
			}
		}
	}
	
	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}
	else if ( g_bIsSelMark ) 
	{
		pApp->m_SelMark.StopSelMark();
	}
	else if ( g_bIsFastMark ) 
	{
		pApp->m_FastMark.StopFastMark();
	}

	CMarkSpecialDlg dlg;
	if ( g_DAT.EnterMarkEnv() )
	{
		//硬件部分准备就绪时才能进入此操作
		dlg.DoModal();
		g_DAT.LeaveMarkEnv();
	}
}

BOOL CHLDoc::GetFillProp(FILLDATA &fillOld) 
{
	if ( m_chain.GetStatus() == CChain::selectNull )
	{
		return FALSE;
	}
	else
	{
 		m_chain.GetFillStyle(fillOld);
	}

	return TRUE;
}



void CHLDoc::UpdateFill(FILLDATA &fillNew) 
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		FillCmd(fillNew);
	else
	{
		FILLDATA fillOld;
		//
		m_chain.GetFillStyle(fillOld);
		CCmdFill* pFillCmd = new CCmdFill(this,fillNew,fillOld);
		m_cmdProc.Submit(pFillCmd);
	}
}

void CHLDoc::UpdateObjMarkSet(OBJMARKSET &ObjMarkSet)
{
	m_chain.UpdateObjMarkSet(ObjMarkSet);
}

BOOL CHLDoc::GetObjMarkSet(OBJMARKSET &ObjMarkSet) 
{
	if ( m_nStatus == CChain::selectNull)
	{
		return FALSE;
	}
	else
	{
 		m_chain.GetObjMarkSet(ObjMarkSet);
	}

	return TRUE;
}


void CHLDoc::OnEditDBCopy() 
{
	m_chain.GetSelTreeDoubleOutPara(m_DBCopyPara);
}

void CHLDoc::OnEditBDPaste() 
{
	CQuad quad = GetLimit();
	m_chain.SetAllSelTreeDoubleOutPara(m_DBCopyPara);
	
	CDot pos = quad.Center();
	double dbMoveX = m_DBCopyPara.pos.x - pos.x;
	double dbMoveY = m_DBCopyPara.pos.y - pos.y;
	Drag(CDot(0,0),1.0,1.0,dbMoveX,dbMoveY);
	SetTrackerQuad(GetLimit());

	memset(&m_DBCopyPara, 0, sizeof(DOUBLEOUTPARA));
}


void CHLDoc::OnUpdateDownLoad(CCmdUI* pCmdUI) 
{
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(m_nStatus  !=  CChain::selectNull && (g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK));
#endif
}

//
CNode* CHLDoc::GetSelNode(int nNodeType)
{
	CNode *pNode = m_chain.GetNode();
	if ( pNode == NULL || pNode->GetLockFlg() )
		return NULL;
	
	if ( nNodeType == -1 || nNodeType == pNode->GetType() )
		return pNode;
	
	return NULL;
}

void CHLDoc::EditTextNode(CNode* pNode)
{
	if ( !pNode || pNode->GetType()  !=  NODE_HLTEXT || !pNode->IsSelect() )
		return;

	m_chain.m_MgrText.Clear();
	//文本方式则在此处处理
	CNodeText *pNodeText = (CNodeText*)pNode; 
	pNodeText->AdvanceSet(CRect(0,0,0,0));

	FILLDATA fill;
	m_chain.GetFillStyle(fill);
	m_chain.SetFillStyle(fill,m_pTracker);
	SetModifiedFlag();
	SetTrackerQuad( m_chain.GetLimit() );	
	
	DispSelObjProp();
}

void CHLDoc::EditBarNode( CNode* pNode )
{
	if ( !pNode || pNode->GetType()  !=  NODE_CODEBAR || !pNode->IsSelect()  )
		return;
	
	CNodeCodebar *pNodeBar = (CNodeCodebar *)pNode;
	if ( FALSE == pNodeBar->Create( CRect(0,0,0,0) ) )
		Delete();
	
	//用于刷新文本内容
	m_chain.PreprocessMark(0x04);
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
	
	DispSelObjProp();
}

void CHLDoc::OnConvertOneTree() 
{
	ConvertSelToOneTree();

	DispSelObjProp();	
}


//
void CHLDoc::OnUpdateConvertOneTree(CCmdUI* pCmdUI) 
{
	if ( m_nStatus == CChain::selectNull  || !(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(!m_b3D);
	}
}

//
void CHLDoc::OnPathOptimize()
{	
	if ( m_bPathOptimize == FALSE )
	{
		if (m_chain.HaveFunctionNode())
		{
			AfxMessageBox(IDS_ERROR_OBJPRO);
			return;
		}
		
		CDlgPathOptimizeSet dlg;
		if (dlg.DoModal() == IDOK)
		{
			m_nPathOptimizeType = dlg.m_nOptimizeType;
			if ( m_cmdProc.GetMaxCommands() == 0 )
				PathOptimizeCmd();
			else
				m_cmdProc.Submit(new CCmdPathOptimize(this));
		}
	}
	else
	{
		if ( m_cmdProc.GetMaxCommands() == 0 )
			PathOptimizeCmd();
		else
			m_cmdProc.Submit(new CCmdPathOptimize(this));
	}
	
	CHLView *pView = (CHLView*)GetCurActiveView();
	if ( pView  !=  NULL )
		pView->Invalidate();
}

void CHLDoc::PathOptimizeCmd()
{
	if (m_bPathOptimize == FALSE)
	{
		PathOptimize();
		SetModifiedFlag();
	}
	else
	{
		NormalPath();
	}
}

//
void CHLDoc::PathOptimize()
{
	AfxGetApp()->BeginWaitCursor();	
	
	g_DAT.SetLay(m_lay);
	
	m_chain.PreprocessMark();
	m_chain.m_listStroke.RemoveAll();
	StrokeList listTemp;
	
	if ( m_nPathOptimizeType == 0) 
	{
		listTemp.RemoveAll();
		//
		m_chain.LoadStrokeList(&listTemp,TRUE,-1);
		m_MgrPathOptimize.OptimizeProcess(&listTemp);
		m_MgrPathOptimize.LoadStrokelist(&(m_chain.m_listStroke));
	}
	else if ( m_nPathOptimizeType == 1 )
	{
		int nLayCount = m_lay.GetSize();
		
		for(int i = 0;i<nLayCount;i++)
		{
			listTemp.RemoveAll();
			
			m_chain.LoadStrokeList(&listTemp,TRUE,i);
			m_MgrPathOptimize.OptimizeProcess(&listTemp);
			m_MgrPathOptimize.LoadStrokelist(&(m_chain.m_listStroke));
		}
	}
	
	if ( m_chain.m_listStroke.GetCount() > 0)
	{
		m_bPathOptimize = TRUE;
		m_chain.m_bPathOptimize = m_bPathOptimize;
		m_chain.m_nPathOptimizeType = m_nPathOptimizeType;
		
		LockAllLayer();
	}
	
	//
	AfxGetApp()->EndWaitCursor();
}

//
void CHLDoc::OnNormalPath()
{	
	NormalPath();
	SetModifiedFlag();
}

//
void CHLDoc::NormalPath()
{
	AfxGetApp()->BeginWaitCursor();
	
	//
	m_MgrPathOptimize.NormalPath(&(m_chain.m_listStroke));
	m_chain.m_listStroke.RemoveAll();
	
	m_bPathOptimize = FALSE;
	m_chain.m_bPathOptimize = m_bPathOptimize;
	m_chain.SetPathOptimizeState(FALSE);
	UnLockAllLayer();
	
	//
	AfxGetApp()->EndWaitCursor();
}


//
void CHLDoc::OnSelectLayer(int nItem)
{
	m_chain.SetSelect(FALSE);
	m_chain.SetLayerSelect(nItem,TRUE);
	SetTrackerQuad(GetLimit());

	DispSelObjProp();
}

//
void CHLDoc::OnUnselectLayer(int nItem)
{	
	m_chain.SetSelect(TRUE);
	m_chain.SetLayerSelect(nItem,FALSE);
	SetTrackerQuad(GetLimit());
	
	CHLView *pView = (CHLView*)GetCurActiveView();
	if ( pView  !=  NULL )
	{
		pView->DispSelObjProp();
		pView->Invalidate();
	}
}

//
BOOL CHLDoc::HasObjForProcess()
{
	return m_chain.HasObjForProcess();
}


void CHLDoc::AdjustObjectOrder(int nFrom, int nTo)
{
	m_chain.AdjustObjectOrder(nFrom, nTo);
	
	m_pTracker->ClearAll();
	ReLoadStrokeList();
	
	DispSelObjProp();
}

//
void CHLDoc::OnUpdateJointstrokeCurdoc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(	!( GetStatus() & CChain::selectNull ) &&  
								(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) && !m_b3D );
}


void CHLDoc::DispSelObjProp()
{
	CHLView   *pView = (CHLView*)GetCurActiveView();
	if ( NULL == pView )
		return;
	
	if ( !m_pTracker )
		m_pTracker = &(pView->m_track);
	
	pView->DispSelObjProp();
}


void CHLDoc::LockAllLayer()
{
	int iCount = m_lay.GetSize();

	for(int i = 0; i<iCount; i++)
		m_lay.SetLockFlg(i, TRUE);

	CMainFrame *pMainWnd = (CMainFrame*)theApp.m_pMainWnd;
	if ( pMainWnd )
	{
		pMainWnd->m_PropDlg.OnDocChange();
	}
}


void CHLDoc::UnLockAllLayer()
{
	if ( m_bPathOptimize )
		return;
	
	int iCount = m_lay.GetSize();
	for( int i = 0; i < iCount; i++)
		m_lay.SetLockFlg(i, FALSE);
	
	CMainFrame *pMainWnd = (CMainFrame*)theApp.m_pMainWnd;
	if ( pMainWnd )
	{
		pMainWnd->m_PropDlg.OnDocChange();
	}
}


//
void CHLDoc::OnUpdatePathOptimize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && !m_b3D);

	if ( m_bPathOptimize == TRUE )
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

//
BOOL CHLDoc::GetSelObjLockFlg()
{
	return m_chain.GetSelObjLockFlg();
}

void CHLDoc::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE);	
}

void CHLDoc::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_OPEN);
}

void CHLDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SAVE);
}

void CHLDoc::OnUpdateFileImport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !m_b3D);
}

void CHLDoc::OnUpdateFileImportVector(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !m_b3D);
}

void CHLDoc::OnUpdateFileImportBmp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !m_b3D);
}

void CHLDoc::OnUpdateFileImportNc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_CREATE && !m_b3D);
}


void CHLDoc::LabelTo3D(BOOL bForce)
{
	if ( !m_b3D )
		return;

	g_DAT.SetLay(m_lay);
	m_chain3d.ClearLabel();
	LABEL_DATA data;
	CQuad quad;
	LABEL_DOT dotBase;
	int n(0), i(0);

	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_chain.m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_chain.m_listGroup.GetNext(pos);
		if ( pGroupObj  !=  NULL )
		{
			pGroupObj->LabelTo3D(data);
			
			//
			quad = pGroupObj->GetLimit();
			dotBase.x = quad.Center().x;
			dotBase.y = quad.Center().y;
			m_chain3d.LabelText( &data, g_SysCfg.m_db3dTrancePrecision, dotBase, bForce );
			
			n = data.GetSize();
			for (int i = 0; i<n; ++i)
			{
				LABEL_NODE* pNode = data[i];
				if ( NULL == pNode )
					continue;
	 
 				int nStk = pNode->list.GetCount();
 				for (int j = 0; j<nStk; ++j)
 				{
 					LABEL_STROKE* pStk = pNode->list[j];
 					pStk->RemoveAll();
 					delete pStk;
 				}
 				pNode->list.RemoveAll();
 				delete pNode;
				pNode = NULL;
 			}
			data.RemoveAll();
		}
	}
	
	m_chain3d.Draw();
}

void CHLDoc::LabelTo3DOptimize(BOOL bForce)
{
	if ( !m_b3D )
		return;

	g_DAT.SetLay(m_lay);
	m_chain3d.ClearLabel();

	LABEL_DATA data;
	CQuad quad;
	LABEL_DOT dotBase;
	int n(0);

	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_chain.m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj*)m_chain.m_listGroup.GetNext(pos);
		if ( pGroupObj  !=  NULL )
		{
			pGroupObj->LabelTo3D(data);

			quad = pGroupObj->GetLimit();
			dotBase.x = quad.Center().x;
			dotBase.y = quad.Center().y;

			double dx(0),dy(0),dz(0);
			if( m_chain3d.LabelNormal(quad.left,quad.right,quad.bottom,quad.top, g_SysCfg.m_db3dTrancePrecision,dx,dy,dz) )
			{
				if( (sqrt(dx*dx+dy*dy+dz*dz) > 1.0E-6) && 
					fabs(dz) > 1.0E-3  &&
					(fabs(dx) > 1.0E-3 || fabs(dy) > 1.0E-3)
					)
				{
					m_chain3d.ConvertDataByNormal(&data,dx,dy,dz,dotBase);

					////////////测试试用，看平面图形按法向量旋转的情况
					double xAngle = m_chain3d.GetVectorAngle(dz,dy);
					double yAngle = m_chain3d.GetVectorAngle(sqrt(dy*dy+dz*dz),dx);
					xAngle = -xAngle;

					//CString str;
					//str.Format(_T("rotate y:%.3lf,rotate x:%.3lf\n"),yAngle,xAngle);
					//::OutputDebugString(str);
					// 						CDot basePt(dotBase.x,dotBase.y,0);
					// 						pGroupObj->Rotate(basePt,yAngle,1); //绕y
					// 						pGroupObj->Rotate(basePt,xAngle,2); //绕x
					////////////////////////////////////////////////
				}
			}
			m_chain3d.LabelText( &data, g_SysCfg.m_db3dTrancePrecision, dotBase, bForce );

			n = data.GetSize();
			for (int i = 0; i<n; ++i)
			{
				LABEL_NODE* pNode = data[i];
				if ( NULL == pNode )
					continue;

				int nStk = pNode->list.GetCount();
				for (int j = 0; j<nStk; ++j)
				{
					LABEL_STROKE* pStk = pNode->list[j];
					pStk->RemoveAll();
					delete pStk;
				}
				pNode->list.RemoveAll();
				delete pNode;
				pNode = NULL;
			}
			data.RemoveAll();
		}
	}

	m_chain3d.Draw();
}


void CHLDoc::OnUpdateModuleSwitch(CCmdUI* pCmdUI) 
{
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(TRUE);
#else
	pCmdUI->Enable( g_CurUserInfo.dwRight1 & USER_RIGHT_OPEN && TRUE == g_DAT.m_btExtendInfo.FunData.bNormal3d );
#endif
	pCmdUI->SetCheck( m_b3D );
}



void CHLDoc::OnModuleSwitch()
{
	m_b3D = !m_b3D;
	POSITION pos = GetFirstViewPosition();
	while(pos  !=  NULL)
	{
		CView* pView = GetNextView(pos);
		pView->SetWindowPos(NULL, 0,0,0,0, 
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
	}
	
	if( g_SysCfg.m_n3DOptimize )
	{
		m_b3D ? LabelTo3DOptimize() : UpdateAllViews(NULL);
	}
	else
	{
		m_b3D ? LabelTo3D() : UpdateAllViews(NULL);
	}
	
	CMainFrame *pFWnd = (CMainFrame*)theApp.m_pMainWnd;
	pFWnd->UpdateProperty();
}



//
void CHLDoc::OnUpdate3dViewXYZ(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && TRUE == m_b3D);
}



//
void CHLDoc::On3dViewXYZ()
{
	m_chain3d.RawView(0);
	m_chain3d.Draw();
	UpdateAllViews(NULL);
}


//
void CHLDoc::OnUpdate3dViewXY(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && TRUE == m_b3D );
}

//
void CHLDoc::On3dViewXY()
{
	m_chain3d.RawView(1);
	m_chain3d.Draw();
	UpdateAllViews(NULL);
}


//
void CHLDoc::OnUpdate3dViewYZ(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && TRUE == m_b3D);
}


//
void CHLDoc::On3dViewYZ()
{
	m_chain3d.RawView(2);
	m_chain3d.Draw();
	UpdateAllViews(NULL);
}


//
void CHLDoc::OnUpdate3dViewXZ(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT && TRUE == m_b3D);
}

//
void CHLDoc::On3dViewXZ()
{
	m_chain3d.RawView(3);
	m_chain3d.Draw();
	UpdateAllViews(NULL);
}

void CHLDoc::OnUpdateModuleLoad3D(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_b3D);
}

void CHLDoc::OnModuleLoad3D()
{
	CDlg3DLoad dlg(&m_chain3d.m_3dpara);
	dlg.m_bNew = m_chain3d.GetObjCount() <= 0;
	if (dlg.DoModal()  !=  IDOK)
		return;

	AfxGetApp()->BeginWaitCursor();

	if ( FALSE == m_chain3d.Build(dlg.m_bNew) )
	{
		AfxMessageBox(_T("Create 3D model failure!"));
		return;
	}
	
	SetModifiedFlag(TRUE);
	AfxGetApp()->EndWaitCursor();
}

void CHLDoc::OnUpdateModuleTransform3D(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_b3D);
}


void CHLDoc::OnModuleTransform3D()
{
	CDlg3DTransform dlg(this);
	dlg.DoModal();
}


void CHLDoc::OnUpdateModuleAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_b3D);
}



void CHLDoc::OnModuleAlign()
{
	CDlg3DAlign dlg;
	dlg.DoModal();
}


void CHLDoc::OnUpdateMarkNormal(CCmdUI* pCmdUI)
{
	BOOL bEnable = g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK;

#ifdef _SOFTMODE_EDITOR_
	bEnable = FALSE;
#endif

	pCmdUI->Enable(bEnable);
}


void CHLDoc::OnUpdateDownMul(CCmdUI* pCmdUI)
{
	BOOL bEnable = g_CurUserInfo.dwRight1 & USER_RIGHT_MULMARK;

#ifdef _SOFTMODE_EDITOR_
	bEnable = FALSE;
#endif

	pCmdUI->Enable(bEnable);
}

void CHLDoc::OnUpdateClearDownload(CCmdUI* pCmdUI)
{
	BOOL bEnable = g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK;

#ifdef _SOFTMODE_EDITOR_
	bEnable = FALSE;
#endif

	pCmdUI->Enable(bEnable);
}



void CHLDoc::OnUpdateExportPlt(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SAVE);
}

void CHLDoc::UnionCmd() 
{
	m_chain.UnionGroup();

	SetModifiedFlag();

	RefreshSelectQuadList();
}


void CHLDoc::OnUnion() 
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		UnionCmd();
	else
		m_cmdProc.Submit(new CCmdUnion(this));
}


void CHLDoc::OnUpdateUnion(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nStatus & CChain::selectMul && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) );
}

void CHLDoc::UnunionCmd()
{
	m_chain.UnUnionGroup();
	SetModifiedFlag();

	RefreshSelectQuadList();
}

void CHLDoc::OnUnunion()
{
	if ( m_cmdProc.GetMaxCommands() == 0 )
		UnunionCmd();
	else
		m_cmdProc.Submit(new CCmdUnunion(this));
}



void CHLDoc::OnUpdateUnunion(CCmdUI* pCmdUI) 
{
	if ( (m_nStatus & CChain::selectUnion) && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}


void CHLDoc::OnMotioninfoSet()
{
	// TODO: Add your command handler code here
	CDlgMotionInfoSet dlg;
	MOTIONPARA para;
	
	for( int i = 0; i<4; i++ )
	{
		g_Motion.GetMotionPara(i, para);
		dlg.m_para[i] = para;
	}
	
	dlg.DoModal();
}


void CHLDoc::OnRotatewheelSet() 
{
	// TODO: Add your command handler code here
	CDlgRotateWheelSet dlg;
	dlg.DoModal();
}

void CHLDoc::OnMotionMarkSet()
{
	CDlgMoveMarkParam dlg;
	dlg.DoModal();
}

void CHLDoc::OnUpdateMotionMarkSet(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable( (g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET) &&  TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark && FALSE == m_b3D);
#endif
}


//
void CHLDoc::OnEditWheel()
{
	// TODO: Add your command handler code here
	if ( m_cmdProc.GetMaxCommands() == 0 )
		EditWheelCmd();
	else
		m_cmdProc.Submit(new CCmdAutoSegment(this));
}


void CHLDoc::EditWheelCmd()
{
	if (m_chain.HaveFunctionNode())
	{
		AfxMessageBox(IDS_ERROR_OBJPRO);
		return;
	}
	
	m_chain.HandleAutoSegment(g_SysCfg.m_dbDiameter/2.0);
	
	SetModifiedFlag();
	RefreshSelectQuadList();
}

//
void CHLDoc::OnUpdateEditWheel(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) && !m_b3D);
}


void CHLDoc::OnMarkWheel()
{
	// TODO: Add your command handler code here
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK) )
	{	
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	if (m_b3D)
		return;
	
	if ( !g_DAT.IsCheckStatusOK() )
		return;
	
	if ( OutOfLimit(FALSE) )
	{
		AfxMessageBox(IDS_ERROT_OUTAREA);
		return;
	}
	
	//判断数据中是否存在功能性对象
	if (m_chain.HaveFunctionNode())
	{
		AfxMessageBox(IDS_ERROR_OBJPRO);
		return;
	}
	
	//打标参数更新
	g_DAT.SetLay(m_lay);
	
	//没有打标内容
	if ( m_chain.GetCount() == 0 )
		return;
	
	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
		return;
	}
	
	if ( g_bIsSelMark )
	{
		theApp.m_SelMark.StopSelMark();
		return;
	}
	
	if ( g_bIsFastMark )
	{
		theApp.m_FastMark.StopFastMark();
		return;
	}
	
	CMarkWheelDlg dlg;
	dlg.m_pDoc = this;
	dlg.m_pChain = (CChain *)&m_chain;
	if ( g_DAT.EnterMarkEnv() )
	{
		//硬件部分准备就绪时才能进入此操作
		dlg.DoModal();
		g_DAT.LeaveMarkEnv();
	}
	g_DAT.m_hMarkWnd = NULL;
	UpdateAllViews(NULL,NULL);
}

void CHLDoc::OnUpdateMarkWheel(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = (g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK) && TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark && !m_b3D;

#ifdef _SOFTMODE_EDITOR_
		bEnable = FALSE;
#endif
	
	pCmdUI->Enable(bEnable);
}

void CHLDoc::OnMoveMark()
{
	// TODO: Add your command handler code here
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK) )
	{	
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	if ( m_b3D )
		return;

	if ( !g_DAT.IsCheckStatusOK() )
		return;

	if ( OutOfLimit(FALSE) )
	{
		AfxMessageBox(IDS_ERROT_OUTAREA);
		return;
	}

	//判断数据中是否存在功能性对象
	if (m_chain.HaveFunctionNode())
	{
		AfxMessageBox(IDS_ERROR_OBJPRO);
		return;
	}

	//打标参数更新
	g_DAT.SetLay(m_lay);

	//没有打标内容
	if ( m_chain.GetCount() == 0 )
		return;

	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
		return;
	}

	if ( g_bIsSelMark )
	{
		theApp.m_SelMark.StopSelMark();
		return;
	}

	if ( g_bIsFastMark )
	{
		theApp.m_FastMark.StopFastMark();
		return;
	}

	// 动态运动打标初始化
 	if( !InitMotionMarkParam() )
 		return;
	////////////////////////////

	CMarkWheelDlg2 dlg;
	dlg.m_pDoc = this;
	dlg.m_pChain = (CChain *)&m_chain;
	if ( g_DAT.EnterMarkEnv() )
	{
		//硬件部分准备就绪时才能进入此操作
		dlg.DoModal();
		g_DAT.LeaveMarkEnv();
	}
	g_DAT.m_hMarkWnd = NULL;
	UpdateAllViews(NULL,NULL);
}

void CHLDoc::OnUpdateMoveMark(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = ((g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK) && (TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark && !m_b3D));

#ifdef _SOFTMODE_EDITOR_
	bEnable = FALSE;
#endif

	pCmdUI->Enable(bEnable);
}

//
// 动态运动打标初始化
BOOL CHLDoc::InitMotionMarkParam()
{
	if ( g_SysCfg.m_sMoveMarkParam.m_nMode == 0 ) // 非圆周
	{
		AfxMessageBox(_T("Error mode, Please reset the work mode! "));
		return FALSE;
	}

	double dbBoxX = 0;
	double dbBoxY = 0;
	g_DAT.GetMarkRect(dbBoxX,dbBoxY);

	double dbMinX,dbMinY,dbMaxX,dbMaxY;
	double dbInput,dbOutput;
	MOTIONPARA	motionPara;
	g_Motion.GetMotionPara(0,motionPara);
	if ( motionPara.nPPR == 0 || motionPara.bEnabled == FALSE )
	{
		AfxMessageBox(_T("Motion parameter Error!"));
		return FALSE;
	}

	double dbWidth = 0.5*g_SysCfg.m_sMoveMarkParam.m_dbWheelDiameter*sin(g_SysCfg.m_sMoveMarkParam.m_dbDivAngle*PAI/360);
	dbMinX  = g_SysCfg.m_sMoveMarkParam.m_dbExcursionX - dbWidth;
	dbMaxX = g_SysCfg.m_sMoveMarkParam.m_dbExcursionX + dbWidth;
	dbMinY = -dbBoxY/2;
	dbMaxY = dbBoxY/2;

	dbInput   = PAI*g_SysCfg.m_sMoveMarkParam.m_dbWheelDiameter/g_SysCfg.m_sMoveMarkParam.m_nEncodePPR;
	dbOutput = motionPara.nPPR*1.0 / (PAI*g_SysCfg.m_sMoveMarkParam.m_dbWheelDiameter);

/*	else
	{
		if( fabs(motionPara.dbPitch) < EPSILON  )
		{
			AfxMessageBox(_T("Motion parameter Error!"));
			return FALSE;
		}

		dbMinX = CENTER - g_SysCfg.m_sMoveMarkParam.m_dbDivDx/2;
		dbMaxX = CENTER + g_SysCfg.m_sMoveMarkParam.m_dbDivDx/2;
		dbMinY = CENTER - g_SysCfg.m_sMoveMarkParam.m_dbDivDy/2;
		dbMaxY = CENTER + g_SysCfg.m_sMoveMarkParam.m_dbDivDy/2;

		if ( dbMinX < x1 || dbMaxX > x2 || dbMinY < y1 || dbMaxY > y2 )
		{
			AfxMessageBox(IDS_ERROT_OUTAREA);
			return FALSE;
		}

		dbInput = motionPara.nPPR;
		dbOutput = motionPara.nPPR*1.0 / motionPara.dbPitch;
	}
*/
	if ( 0 != g_DAT.SetLimitBox(dbMinX,dbMaxX,dbMinY,dbMaxY) )
	{
		AfxMessageBox(IDS_SET_LIMITBOX_FAIL);
		return FALSE;
	}
	
	if( 0 != g_DAT.SetMotorRatio(dbInput,dbOutput) )
	{
		AfxMessageBox(IDS_SET_MOTIONROTIO_FAIL);
		return FALSE;
	}


	double dbLen = PAI*g_SysCfg.m_sMoveMarkParam.m_dbWheelDiameter/4;
	dbMinX = g_SysCfg.m_sMoveMarkParam.m_dbExcursionX - dbLen;
	dbMaxX = g_SysCfg.m_sMoveMarkParam.m_dbExcursionX + dbLen;

	// 传整个幅面
	if( 0 != g_DAT.SetCircleCali(g_SysCfg.m_sMoveMarkParam.m_dbWheelDiameter/2,dbMinX,dbMaxX) )
	{
		AfxMessageBox(IDS_SET_LIMITBOX_FAIL);
		return FALSE;
	}
	if( 0 != g_DAT.SetMotorDelay(g_SysCfg.m_sMoveMarkParam.m_nMotorDelay) )
	{
		AfxMessageBox(_T("Set Motor Delay Failed!"));
		return FALSE;
		//::OutputDebugString(_T("Set Motor Delay Failed!"));
	}

	// 电机回零
	CDlgMotorMoveWait dlg;
	dlg.SetData(3, 0, 0, 0);
	dlg.DoModal(); 

	return TRUE;
}

void CHLDoc::OnBMPIntensity()
{
	// TODO: Add your command handler code here
	CNodeDib *pNode = (CNodeDib *)(m_chain.GetNode());
	if ( pNode && (pNode->GetType()  == NODE_BMP))
	{
		CDlgIntensity dlgPara; 
		dlgPara.m_lpDIBBits = (pNode->m_pImageSrc); 
		dlgPara.m_lWidth = (pNode->m_nWidth); 
		dlgPara.m_lHeight = (pNode->m_nHeight); 
		dlgPara.m_iLowGray = 0;
		dlgPara.m_iUpGray = 255;
		
		//显示对话框，提示用户设定平移量
		if ( dlgPara.DoModal()  !=  IDOK )
			return;
	}
	else
	{
		AfxMessageBox(IDS_NOOBJECT,MB_ICONWARNING);
	}
}


void CHLDoc::OnUpdateBMPIntensity(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT);
}


void CHLDoc::OnEditRuler()
{
	// TODO: Add your command handler code here
	if ( GetStatus() == CChain::selectNull )
	{
		AfxMessageBox(IDS_NOOBJECT,MB_ICONWARNING);
		return;
	}
	
	if ( AfxMessageBox(IDS_RULER_EDITWARNING, MB_YESNO)  !=  IDYES )
	{
		return;
	}
	
	CDlgRulerEdit dlg;
	CGroupObj *pGroup = NULL;
	POSITION pos = m_chain.m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroup = (CGroupObj *)m_chain.m_listGroup.GetNext(pos);
		
		if (pGroup->IsSelect())
		{
			dlg.AddObj(pGroup);
		}
	}
	
	if (dlg.DoModal() == IDOK)
	{
		m_chain.DeleteAll();
		m_chain.m_listGroup.AddTail(&dlg.m_listGroup);

		
		SetModifiedFlag();
		RefreshSelectQuadList();
	}
}



void CHLDoc::OnUpdateEditRuler( CCmdUI* pCmdUI )
{
	//TODO: Add your command update UI handler code here
	pCmdUI->Enable( (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) && !m_b3D );
}


//取Track
BOOL CHLDoc::CheckSetTrack(void)
{
	if ( NULL == m_pTracker )
	{
		CHLView   *pView = GetCurActiveView();
		if ( NULL == pView )
			return FALSE;
		
		m_pTracker = &(pView->m_track);
	}
	
	return TRUE;
}




void CHLDoc::RefreshSelectQuadList()
{
	m_chain.RefreshSelectQuadList(m_QuadList);
	
	m_quadTracker.SetEmpty();
	for(int  i = 0; i<m_QuadList.GetSize(); i++ )
	{
		m_quadTracker.Compose(m_QuadList.GetAt(i).m_QD);
	}
	
	m_nStatus = m_chain.GetStatus();
	if ( !m_b3D )
		UpdateAllViews(NULL);
}





//
void CHLDoc::RefreshTrackerList()
{
	if ( !CheckSetTrack() )
		return;
	
	m_pTracker->ClearAll();
	ReLoadStrokeList();
}


void CHLDoc::OnUpdateSaveAS(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SAVE);
}

void CHLDoc::SetModifiedFlag(BOOL bModified)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_b3D)
	{
		if( g_SysCfg.m_n3DOptimize )
		{
			LabelTo3DOptimize(m_b3dTransForce);
		}
		else
		{
			LabelTo3D(m_b3dTransForce);
		}
		m_bModified = bModified;
		return;
	}
	
	return CDocument::SetModifiedFlag(bModified);
}

void CHLDoc::OnFileSaveAs()
{
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_SAVE) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return ;
	}
	
	SaveMarkFile(_T(""));
}

void CHLDoc::SaveMarkFile(CString strFileName)
{
	BOOL bSaveTip = strFileName.IsEmpty();
	if ( !bSaveTip )
	{
		CString strExt = strFileName.Right(3);
		strExt.MakeLower();
		bSaveTip = m_b3D ? ( strExt  !=  _T(".hd") ) : ( strExt  !=  _T(".hs") );
	}	
	
	if ( bSaveTip )
	{
		CString strFileType;
		if ( TRUE == m_b3D )
			strFileType = _T("HD Files(*.hd)|*.hd|HS Files(*.hs)|*.hs||");
		else
			strFileType = _T("HS Files(*.hs)|*.hs|");
		
		CFileDialog dlg(FALSE, m_b3D ? _T("hd") : _T("hs"), GetTitle(),OFN_EXPLORER|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strFileType);
		
		if ( dlg.DoModal()  !=  IDOK )
			return;
		
		g_OldFileName  = m_strPathName;
		SetPathName(dlg.GetPathName());
		g_OpenFileName = m_strPathName;
		strFileName = dlg.GetPathName();
	}
	
	m_cmdProc.Commit();
	
	//用于去除多的文本对象
	m_chain.m_MgrText.Clear();
	//
	if ( m_bModified == TRUE && m_bPathOptimize == TRUE )
		PathOptimize();
	
	//采用自制的序列化流程
	FILE *fp = NULL;
	if ( ( fp = _tfopen(strFileName,_T("wb")))  !=  NULL )
	{
		if ( strFileName.Right(3) == _T(".hd") )
		{
			HDFILEHEAD head;
			strcpy(head.szFlag,"^3D^");
			strcpy(head.szVersion,"1.0");
			head.n2DOffset = sizeof(HDFILEHEAD);
			fwrite(&head, sizeof(HDFILEHEAD), 1, fp);
			fseek(fp,sizeof(HDFILEHEAD),0);
			/////
			Serialize(fp,TRUE,TRUE);
			head.n3DOffset = ftell(fp);
			m_chain3d.Serialize(fp, TRUE);
		}
		else
		{
			Serialize(fp,TRUE,TRUE);
		}
		fclose( fp );
	}
	
	SetModifiedFlag(FALSE);
}


void CHLDoc::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	if ( !(g_CurUserInfo.dwRight1 & USER_RIGHT_SAVE) )
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return ;
	}

	SaveMarkFile(m_strPathName);
}



void CHLDoc::SelSwitich(BOOL bNext)
{
	int n = m_chain.GetCount();
	if (n  <=  1) return;
	
	BOOL* pFlag = new BOOL[n];
	m_chain.GetSelectFlag(pFlag);
	
	int i(0);
	if ( bNext )
	{
		for(i = n-1; i >= 0; --i)
			if (*(pFlag+i)) 
				break;
		i++;
		if (i >= n) i = 0;
	}
	else
	{
		for(i = 0; i <= n-1; ++i)
			if (*(pFlag+i)) 
				break;
		i--;
		if (i<0) i = n-1;
	}
	
	memset(pFlag, 0, sizeof(BOOL)*n);
	*(pFlag+i) = 1;

	m_chain.SetSelectFlag(pFlag);
	delete []pFlag;
	
	if ( m_pTracker )
	{
		m_pTracker->ClearAll();
		ReLoadStrokeList();
	}

	GetCurActiveView()->DispSelObjProp();
	if(g_SysCfg.m_n3DOptimize )
		LabelTo3DOptimize();
	else
		LabelTo3D();
}




BOOL CHLDoc::SaveModified()
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( !IsModified() )
		return TRUE;        // ok to continue

	// get name/title of document
	CString name;
	if (m_strPathName.IsEmpty())
	{
		// get name based on caption
		name = m_strTitle;
		if ( name.IsEmpty() )
			ENSURE(name.LoadString(AFX_IDS_UNTITLED));
	}
	else
	{
		// get name based on file title of path name
		name = m_strPathName;
		name.ReleaseBuffer();
	}
	
	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue
		
	case IDYES:
		// If so, either Save or Update, as appropriate
		OnFileSave();
		return !IsModified();
		break;
		
	case IDNO:
		// If not saving changes, revert the document
		break;
		
	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}

void CHLDoc::OnCheckFocus()
{
	// TODO: 在此添加命令处理程序代码
	if ( !g_DAT.IsCheckStatusOK() )
		return;
	
	if ( g_bIsRedPreview )
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview(); 
		return;
	}
	if ( g_bIsSelMark ) 
	{
		theApp.m_SelMark.StopSelMark();
		return;
	}
	if ( g_bIsFastMark ) 
	{
		theApp.m_FastMark.StopFastMark();
		return;
	}
	
	if ( OutOfLimit() )
	{
		AfxMessageBox(IDS_ERROT_OUTAREA);
		return;
	}
	
	if ( 20  !=  m_chain.GetCount() )
	{
		AfxMessageBox(IDS_ERROR_FILECOUNT);
		return;
	}
	
	int  i(1);
	BOOL bOk(TRUE);
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_chain.m_listGroup.GetHeadPosition();
	while( pos  !=  NULL )
	{
		CString strTxt,strNum;
		strNum.Format(_T("%d"),i);
		pGroupObj = (CGroupObj*)m_chain.m_listGroup.GetNext(pos);
		pGroupObj->GetTextValue(strTxt);

		if ( strTxt  !=  strNum )
			bOk = FALSE;
		
		if ( !bOk )
		{
			AfxMessageBox(IDS_ERROR_TESTFILE);
			return;
		}
		
		i++;
	}
	
	//打标参数更新
	g_DAT.SetLay(m_lay);
	if ( g_DAT.EnterMarkEnv() )
	{
		CLocaleFocusDlg dlg;
		dlg.m_pDoc = this;
		dlg.DoModal();
		g_DAT.LeaveMarkEnv();
	}
	g_DAT.m_bEnableZOffset = TRUE;
	g_DAT.m_hMarkWnd = NULL;
}


//
void CHLDoc::OnUpdateCheckFocus(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	BOOL bEnable = (g_CurUserInfo.dwRight1 & USER_RIGHT_NORMALMARK) && TRUE == g_DAT.m_btExtendInfo.FunData.bNormal3d;
	pCmdUI->Enable(bEnable);
#endif
	
}


void CHLDoc::OnUpdateMotioninfoSet(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable((g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET) && TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark && FALSE == m_b3D);
#endif
}

void CHLDoc::OnUpdateRotatewheelSet(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable( (g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET) &&  TRUE == g_DAT.m_btExtendInfo.FunData.bMotorMark && FALSE == m_b3D);
#endif
}

//
void CHLDoc::TransPreprocess( TRANSFORM &tsf, int nTransType )
{
	if ( nTransType  !=  1 && nTransType  !=  2 )
		return;
	
	CArray<StrokeList*, StrokeList*> arrayList;
	
	if ( 1 == nTransType )
	{
		CGroupObj *pGroupObj = NULL;
		POSITION pos = m_chain.m_listGroup.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pGroupObj = (CGroupObj*)m_chain.m_listGroup.GetNext(pos);
			if ( pGroupObj  !=  NULL )
			{
				if ( TRUE == pGroupObj->GetSelect() )
				{
					pGroupObj->LoadWheelList(arrayList, 0);
				}
			}
		}
	}
	else if ( 2 == nTransType )
	{
		LABEL_3DLIST list3D;
		m_chain3d.Get3DStrokeList(&list3D);

		if ( list3D.GetSize() < 1 )
			return;

		int nLayer = -1;
		StrokeList *pListStroke = NULL;
		LABEL_3DSTROKE *p3dStroke = NULL;
		for( int i = 0; i<list3D.GetSize(); i++)
		{
			p3dStroke = list3D.GetAt(i);

			if ( p3dStroke  !=  NULL)
			{
				CStroke *pStroke = new CStroke();
				CreateStrokeFrom3D(pStroke, p3dStroke);

				if ( nLayer  !=  p3dStroke->iLayer )
				{
					if ( pListStroke  !=  NULL)
						arrayList.Add(pListStroke);

					nLayer = p3dStroke->iLayer;
					pListStroke = new StrokeList;
				}

				pListStroke->AddTail(pStroke);
			}
		}

		if ( pListStroke  !=  NULL)
			arrayList.Add(pListStroke);

		m_chain3d.Free3DStrokeList(&list3D);
	}
	
	if ( arrayList.GetSize() < 1 )
	{
		return;
	}
	
	//
	TransByTransform(arrayList, tsf);	
	
	if ( 1 == nTransType )
	{
		if(g_SysCfg.m_n3DOptimize )
			LabelTo3DOptimize(m_b3dTransForce);
		else
			LabelTo3D(m_b3dTransForce);
	}	
}




void CHLDoc::TransByTransform( CArray<StrokeList*, StrokeList*> &arrayList, TRANSFORM &tsf )
{
	if ( arrayList.GetSize() < 1 )
		return;

	//
	CMatrix matT, matR, mat;

	CQuad quad = GetLimit();
	CDot	dotOld =  quad.GetBaseDot();

	double dbMoveX = tsf.dx;
	double dbMoveY = tsf.dy;
	double dbMoveZ = tsf.dz;

	double arrayT[16] = {	1, 0, 0, dbMoveX,
							0, 1, 0, dbMoveY,
							0, 0, 1, dbMoveZ,
							0, 0, 0, 1 };
	matT.InitData(4,4,arrayT);
	
	//
	int nAxis = tsf.nDirect;
	double dbAngle = tsf.dbAngle*PAI/180;

	//
	CDot dot1, dot2;
	double dbCx, dbCy, dbCZ;
	m_chain3d.GetCenterPos(dbCx, dbCy, dbCZ);
	dot1.x = dbCx;
	dot1.y = dbCy;
	dot1.z = dbCZ;

	dot2 = dot1;
	if ( 0 == nAxis ) //Z
	{		
		dot2.z  +=  100;
	}
	else if ( 1 == nAxis ) //X
	{
		dot2.x  +=  100;
	}
	else if ( 2 == nAxis ) //Y
	{
		dot2.y  +=  100;
	}

	g_dataMng.CalTransMat(dot1, dot2, dbAngle, matR);

	//
	mat = matT*matR;
	g_dataMng.TransByMat(arrayList, mat);

	//
	DeleteCmd();
	m_chain.AddArrayList(arrayList);
	g_dataMng.DeleteArrayList(arrayList);
}



void CHLDoc::CreateStrokeFrom3D( CStroke *pStroke,  LABEL_3DSTROKE *p3dStroke)
{
	if ( pStroke == NULL || p3dStroke == NULL )
		return;

	for(int i = 0; i<p3dStroke->dotArray.GetSize(); i++)
	{
		LABEL_DOT dot3D = p3dStroke->dotArray.GetAt(i);
		
		CDot dot;
		dot.x = dot3D.x;
		dot.y = dot3D.y;
		dot.z = dot3D.z;

		pStroke->m_dotArray.Add(dot);
	}
}


void CHLDoc::OnEditWeld()
{
	// TODO: 在此添加命令处理程序代码
	if ( m_cmdProc.GetMaxCommands() == 0 )
		EditWeldCmd();
	else
		m_cmdProc.Submit(new CCmdEditWeld(this));	
}


void CHLDoc::OnUpdateEditWeld( CCmdUI *pCmdUI )
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable( !( GetStatus() & CChain::selectNull ) && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) && !m_b3D );
}


void CHLDoc::EditWeldCmd() 
{
	CArray<StrokeList*, StrokeList*> arrayList;	
	CWeldEditPro weldEditPro;

	m_chain.LoadWheelList(arrayList, 8);

	StrokeList listRet;
	weldEditPro.Weld(arrayList, listRet);

	//
	CNodePlt *pNode = new CNodePlt();

	POSITION pos = listRet.GetHeadPosition();
	CStroke *pStroke = NULL;
	CStrokeB *pStrokeB = NULL;
	while( pos  !=  NULL )
	{
		pStrokeB = listRet.GetNext(pos);	
		if ( pStrokeB == NULL )
			continue;

		pStroke = new CStroke();
		(*pStroke) = (*pStrokeB);

		pStroke->SetParent((LPVOID*)pNode);
		pNode->m_list.AddTail(pStroke);

		delete pStrokeB;
		pStrokeB = NULL;
	}

	CTree *pTree = new CTree(pNode);
	CGroupObj *pGroupObj = new CGroupObj();

	pGroupObj->Add(pTree);
	pGroupObj->SetSelect(TRUE);

	m_chain.Delete();
	m_chain.m_listGroup.AddTail(pGroupObj);

	CHLView   *pView = GetCurActiveView();
	if ( pView == NULL )
		return;

	pView->DispSelObjProp();
}

void CHLDoc::OnUpdateChgCard(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
#ifdef _SOFTMODE_EDITOR_
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & USER_RIGHT_SYSTEMSET);
#endif
}


BOOL CHLDoc::GetAllStrokeList(CArray<StrokeList*, StrokeList*> &arrayList, StrokeList* pStrokeList)
{
	if ( NULL == pStrokeList )
		return FALSE;

	POSITION pos;;
	CStrokeB *pStroke    = NULL;
	CStroke *pNewStroke = NULL; 
	StrokeList* pTmpStrokeList = NULL;
	for(int i = 0; i < arrayList.GetCount(); i++)
	{
		pTmpStrokeList = arrayList.GetAt(i);
		if ( NULL == pTmpStrokeList )
			continue;

		pos = pTmpStrokeList->GetHeadPosition();
		while(pos  !=  NULL)
		{	
			pStroke = pTmpStrokeList->GetNext(pos);
			pNewStroke = new CStroke();
			*pNewStroke = *pStroke;
			pStrokeList->AddTail(pNewStroke);
			delete pStroke;
			pStroke = NULL;
		}
		pTmpStrokeList->RemoveAll();
	}

	arrayList.RemoveAll();

	return TRUE;
}

void CHLDoc::EditInterfilter()
{
	CArray<StrokeList*, StrokeList*> arrayList;	
	StrokeList listRet;
	m_chain.LoadWheelList(arrayList, 8);
	GetAllStrokeList(arrayList, &listRet);

	CDotInterFilter dotInterFilter;
	dotInterFilter.SetInterSpace(g_SysCfg.m_dbInterSpace);
	dotInterFilter.ScanFilterInterDot((void *)(&listRet));

	CNodePlt *pNode = new CNodePlt();

	POSITION pos = listRet.GetHeadPosition();
	CStroke *pStroke = NULL;
	CStrokeB *pStrokeB = NULL;
	while( pos  !=  NULL )
	{
		pStrokeB = listRet.GetNext(pos);	
		if ( pStrokeB == NULL )
			continue;

		pStroke = new CStroke();
		(*pStroke) = (*pStrokeB);

		pStroke->SetParent((LPVOID*)pNode);
		pNode->m_list.AddTail(pStroke);

		delete pStrokeB;
		pStrokeB = NULL;
	}

	CTree *pTree = new CTree(pNode);
	CGroupObj *pGroupObj = new CGroupObj();

	pGroupObj->Add(pTree);
	pGroupObj->SetSelect(TRUE);

	m_chain.Delete();
	m_chain.m_listGroup.AddTail(pGroupObj);

	CHLView   *pView = GetCurActiveView();
	if ( pView == NULL )
		return;

	pView->DispSelObjProp();
}


// add by zhq,交点过滤
void CHLDoc::OnEditInterfilter()
{
	CDlgInterspaceSet dlg;
	dlg.m_dbInterSpace = g_SysCfg.m_dbInterSpace;
	if ( IDOK  !=  dlg.DoModal() )
		return;

	g_SysCfg.m_dbInterSpace = dlg.m_dbInterSpace;
	if ( m_cmdProc.GetMaxCommands() == 0 )
		EditInterfilter();
	else
		m_cmdProc.Submit(new CCmdDotInterFilter(this));	
}

void CHLDoc::OnUpdateEditInterfilter(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable( !( GetStatus() & CChain::selectNull ) && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) && !m_b3D );
}

void   CHLDoc::LoadAssistantPoints()
{
	CArray<DOT,DOT> pointArray;
	m_chain.LoadAssistantPoints(pointArray);
	if ( g_bEnableAssist )
		g_ObjDrawAssist.LoadAssistantPoints(pointArray);
}

/// add by zhq,启动铺助点功能
void CHLDoc::OnAssistantPoint()
{
	g_bEnableAssist = !g_bEnableAssist;
	if ( g_bEnableAssist )
	{
		double x(0.0),y(0.0);
		double dbMarkRange;
		g_DAT.GetMarkRect(x,y);
		dbMarkRange = (x > y ? x : y);
		UINT nCatchBox = UINT(dbMarkRange+10);
		g_bEnableAssist = g_ObjDrawAssist.SetAssistantPara(nCatchBox,nCatchBox, g_SysCfg.m_nCatchPrecision);
		LoadAssistantPoints();
		g_ObjDrawAssist.m_anchorPoint.bEnable = FALSE; // 锚点没选中
	}
}


void CHLDoc::OnUpdateAssistantPoint(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) && !m_b3D);
	pCmdUI->SetCheck(g_bEnableAssist);
}


BOOL CHLDoc::IsEnable3DEdit()
{
#ifdef _SOFTMODE_EDITOR_
	return  m_chain.IsEnable3DEdit(); 
#else
	return ( m_chain.IsEnable3DEdit() && g_DAT.m_btExtendInfo.FunData.bNormal3d );
#endif
}

void CHLDoc::BroadenStrokeList()
{
	CArray<StrokeList*, StrokeList*> arrayList;	
	StrokeList listRet;
	m_chain.LoadWheelList(arrayList, 8);
	GetAllStrokeList(arrayList, &listRet);

	StrokeList listBroaden;
	CBroadenStroke broadenStroke(g_SysCfg.m_dbLineWidth/2,g_SysCfg.m_dbBroadenD,1);
	broadenStroke.SetCirclePara(::g_SysCfg.m_dbPrecision,::g_SysCfg.m_nSampleCount);
	broadenStroke.BroadenStrokeList(&listRet,&listBroaden);

	POSITION pos = listRet.GetHeadPosition();
	CStroke *pStroke = NULL;
	CStrokeB *pStrokeB = NULL;
	while( pos  !=  NULL )
	{
		pStrokeB = (CStrokeB*)listRet.GetNext(pos);	
		if ( pStrokeB )
		{
			delete pStrokeB;
			pStrokeB = NULL;
		}
			
	}

	CNodePlt *pNode = new CNodePlt();
	pos = listBroaden.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStrokeB = (CStrokeB*)listBroaden.GetNext(pos);	
		if ( pStrokeB == NULL )
			continue;

		pStroke = new CStroke;
		if( pStroke == NULL )
			continue;
		(*pStroke) = (*pStrokeB);
		pStroke->SetParent((LPVOID*)pNode);
		pNode->m_list.AddTail(pStroke);
		pStroke = NULL;
	}

	broadenStroke.FreeStrokeList(&listBroaden);

	CTree *pTree = new CTree(pNode);
	CGroupObj *pGroupObj = new CGroupObj();

	pGroupObj->Add(pTree);
	pGroupObj->SetSelect(TRUE);

	m_chain.Delete();
	m_chain.m_listGroup.AddTail(pGroupObj);

	CHLView   *pView = GetCurActiveView();
	if ( pView == NULL )
		return;

	pView->DispSelObjProp();
}

void CHLDoc::OnEditBroaden()
{
	CDlgBroadenSet dlg;
	dlg.m_dbLineWidth = g_SysCfg.m_dbLineWidth;
	dlg.m_dbDistance = g_SysCfg.m_dbBroadenD;
	if ( IDOK  !=  dlg.DoModal() )
		return;

	g_SysCfg.m_dbLineWidth = dlg.m_dbLineWidth;
	g_SysCfg.m_dbBroadenD = dlg.m_dbDistance;
	if ( m_cmdProc.GetMaxCommands() == 0 )
		BroadenStrokeList();
	else
		m_cmdProc.Submit(new CCmdBroadenStroke(this));	
}

void CHLDoc::OnUpdateEditBroaden(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable( !( GetStatus() & CChain::selectNull ) && (g_CurUserInfo.dwRight1 & USER_RIGHT_EDIT) && !m_b3D );
}

void CHLDoc::OnComunication()
{
	// TODO: 在此添加命令处理程序代码
	CComMode dlg;
	dlg.m_pDoc = this;
	dlg.m_pChain = (CChain*)&m_chain;
	dlg.DoModal();
}

void CHLDoc::OnSetdatabase()
{
	// TODO: 在此添加命令处理程序代码
	CDataSourceDlg dlg;
	dlg.DoModal();
}

void CHLDoc::OnDatabase()
{
	// TODO: 在此添加命令处理程序代码
	CQueryDBDlg dlg;
	dlg.m_pDoc = this;
	dlg.m_pChain = (CChain*)&m_chain;
	dlg.DoModal();
}
