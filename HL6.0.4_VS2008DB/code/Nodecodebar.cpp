// NodeCodebar.cpp: implementation of the CNodeCodebar class.
//
/////////////////
#include "stdafx.h"
#include "hl.h"
#include "Text.h"
#include "NodeCodebar.h"
#include "BarCodeDlg.h"
#include "baseMacro.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CNodeCodebar,CObject,2000)

CNodeCodebar::CNodeCodebar()
{
	m_type = NODE_CODEBAR;

	m_pByte = NULL;
	InitCodebar();
	
	m_dbRatioX = m_dbRatioY = 1;
	m_dotDragPt.x = 0;
	m_dotDragPt.y = 0;
	m_dbDotRadius = 0.05;
	m_bTextBar = FALSE;
	m_nVersion = 0;

	m_nCodePage = -1;
	m_quadRangle = CQuadRangle(CDot(0,0), CDot(0,1), CDot(1,1), CDot(1,0));
}

CNodeCodebar::~CNodeCodebar()
{
	if ( m_pByte )
	{
		delete []m_pByte;
		m_pByte = NULL;
	}
}

//
void CNodeCodebar::InitCodebar()
{
	if ( m_pByte )
	{
		delete[] m_pByte;
		m_pByte = NULL;
	}

	m_pByte = new BYTE[256*256];
	if ( m_pByte )
		memset(m_pByte,0,256*256);

	m_dbSpace = 0.6;
	m_dbcodeRate = 2.75;
	m_bRev = FALSE;
	m_bGoBack  = TRUE;
	m_bConnect = TRUE;
	m_dbRevWidth = 0.5;
	m_BarType = CODE39;
	m_dbFillSpace = 0.05;
	m_dbAngle = 0;
	m_bMatrix = FALSE;
	m_dbHeightRev = 0.0;
	
	m_nErrLev = 3;
	m_nHPoint = 10;
	m_nWPoint = 10;
	m_nCodeType = 5;
	m_nSymbolNo = -1;
	m_nShapeNo  = -1;
	m_nOutline = 0;
	
	m_nSingleLineMarkCount = 1;
	m_nNarrowLineNum = 2;
	
	m_dbHeightBar = 10.0f;
	m_dbshrink  = 0.0f;
	m_bAutoFill = TRUE;
	m_bAutoSet = FALSE;
	
	m_nAlign = 2;
	m_narrowW = (double)basenar;
	
	m_dotBase = CDot(CENTER,CENTER);
	m_bParity  = FALSE;

	m_bHollow = FALSE;
	m_dbHollowWidth = 0;
	m_dbHollowHeight = 0;

	m_nCodePage = -1;
}

void CNodeCodebar::Serialize( FILE* fp, BOOL bFlag, BOOL bUnicode )
{
	CMySerialize fs(fp,bUnicode);
	CString str;
	CDot dot1,dot2;
	
	if ( bFlag )
	{
		CQuad quad = CalLimit();
		//写入头信息
		fs << _T("NBAR_B");
		fs << 102;
		fs << (UINT)m_BarType;		//条码类型
		fs << 104;
		fs << m_dbHeightBar;			//条码高度
		fs << 106;
		fs << m_narrowW ;				//条宽比
		fs << 107;
		fs << m_dbshrink;				//条码缩进量
		fs << 110;
		fs << m_nAlign;					//对齐方式
		fs << 111;
		fs << m_nOutline;				//PDF417的模块个数
		fs << 113;
		fs << m_nErrLev;				//纠错级别
		fs << 115;
		fs << m_nHPoint;				//垂直方向点个数
		fs << 116;
		fs << m_nWPoint;				//水平方向点个数
		fs << 117;
		fs << m_bAutoSet;				//设置条码长宽属性;//true 自动设置;FALSE 用户设置
		
		dot1 = quad.GetBaseDot(BASE_BL);
		dot2 = quad.GetBaseDot(BASE_TR);
		
		dot1.TrueToLog();
		dot2.TrueToLog();
		
		fs << 118;
		fs << dot1.x;
		fs << 119;
		fs << dot1.y;
		fs << 120;
		fs << dot2.x;
		fs << 121;
		fs << dot2.y;
		fs << 130;
		fs << m_bRev;				// 确定条码是否反填
		fs << 131;
		fs << m_dbRevWidth;		// 反相条码时的宽度
		fs << 132;
		fs << (m_dbAngle * -1);	// 条码角度
		fs << 133;
		fs << m_bAutoFill;			// 是否自动填充
		
		m_dotBase.TrueToLog();
		fs << 134;
		fs << m_dotBase.x;
		fs << 135;
		fs << m_dotBase.y;
		fs << 136;
		fs << m_dbFillSpace;
		fs << 137;
		fs << m_bMatrix;
		fs << 138;
		fs << m_dbHeightRev;
		
		fs << 140;
		fs << m_dbSpace;
		fs << 141;
		fs << m_dbcodeRate;
		
		fs << 142;
		fs << m_bGoBack;
		fs << 143;
		fs << m_bConnect;
		fs << 144;
		fs << m_nSingleLineMarkCount;
        fs << 145;
		fs << m_nNarrowLineNum;
		fs << 146;
		fs << m_bParity;
		
		fs << 147;
		int n = GetTextCount();
		fs << n;
		CText *pText = NULL;
		for ( int i = 0; i<n; i++ )
		{
			pText = (CText*)GetText(i);
			if ( pText  !=  NULL )
				fs << pText->GetTextID();
			else
				fs << 9999999;
		}
		
		fs << 148;
		fs << m_bTextBar;
		fs << 149;
		fs << m_dbDotRadius;
		fs << 150;
		fs << m_nCodeType;
		fs << 151;
		fs << m_nSymbolNo;
		fs << 152;
		fs << m_nShapeNo;
		// 镂空参数
		fs << 153;
		fs << m_bHollow;
		fs << 154;
		fs << m_dbHollowWidth;
		fs << 155;
		fs << m_dbHollowHeight;
		fs << 156;
		fs << m_nCodePage;
		
		m_dotBase.LogToTrue();
		m_property.Serialize(fp,bFlag,bUnicode);
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		
		//写入尾信息
		fs << _T("NBAR_E");
	}
	else
	{
		BOOL  bOldBar = TRUE;
		while( fs >> str )
		{
			if ( str == _T("PRO_B") )
			{
				m_property.Serialize(fp,bFlag,bUnicode);
				continue;
			}
			
			if ( str == _T("QUAD_RANG_B") )
			{
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
				continue;
			}
			
			//结束点退出
			if ( str == _T("NBAR_E") )
				break;
			
			switch( _ttoi(str) )
			{
			case 100:		//兼容6.0前的单段条码
				{
					int  nTextID(0);
					fs >> nTextID;
					////
					LoadTextByID(nTextID,g_arryIDMap);
					break;
				}
			case 102:
				{
					UINT tmp;
					fs >> tmp;
					m_BarType = (BARTYPE)tmp;
					break;
				}
			case 104:
				fs >> m_dbHeightBar;
				break;
			case 106:
				fs >> m_narrowW;
				break;
			case 107:
				fs >> m_dbshrink;				//条码缩进量
				break;
			case 110:
				fs >> m_nAlign;
				break;
			case 111:
				fs >> m_nOutline;				//PDF417的模块个数
				break;
			case 113:
				fs >> m_nErrLev;				//纠错级别
				break;
			case 115:
				fs >> m_nHPoint;				//垂直方向点个数
				break;
			case 116:
				fs >> m_nWPoint;				//水平方向点个数
				break;
			case 117:
				fs >> m_bAutoSet;				//设置条码长宽属性;//true 自动设置;FALSE 用户设置
				break;
			case 118:
				fs >> dot1.x;
				break;
			case 119:
				fs >> dot1.y;
				break;
			case 120:
				fs >> dot2.x;
				break;
			case 121:
				fs >> dot2.y;
				break;
			case 130:
				fs >> m_bRev;				//确定条码是否反填
				break;
			case 131:
				fs >> m_dbRevWidth;		//反相条码时的宽度
				break;
			case 132:
				{
					fs >> m_dbAngle;		//条码角度
					m_dbAngle  *=  -1;
				    break;
				}
			case 133:
				fs >> m_bAutoFill;			//是否显示文本
				break;
			case 134:
				fs >> m_dotBase.x;
				break;
			case 135:
				fs >> m_dotBase.y;
				break;
			case 136:
				fs >> m_dbFillSpace;
				break;
			case 137:
				fs >> m_bMatrix;
				break;
			case 138:
				fs >> m_dbHeightRev;
				break;
				
			case 140:
				fs >> m_dbSpace;
				break;
			case 141:
				fs >> m_dbcodeRate;
				break;
            case 142:
				fs >> m_bGoBack;
				break;
			case 143:
				fs >> m_bConnect;
				break;
            case 144:
				fs >> m_nSingleLineMarkCount;
				break;
			case 145:
				fs >> m_nNarrowLineNum;
				break;
			case 146:
				fs >> m_bParity;
				break;
			case 147:
				{
					ClearAllText();
					///////
					int n(0), nID(0);
					fs >> n;
					for( int i = 0; i < n; i++ )
					{
						fs >> nID;
						LoadTextByID(nID,g_arryIDMap);
					}
				}
				break;
			case 148:
				fs >> m_bTextBar;
				break;
			case 149:
				fs >> m_dbDotRadius;
				break;
			case 150:
				fs >> m_nCodeType;
				break;
			case 151:
				bOldBar = FALSE;
				fs >> m_nSymbolNo;
				break;
			case 152:
				fs >> m_nShapeNo;
				break;
			case 153:
				fs >> m_bHollow;
				break;
			case 154:
				fs >> m_dbHollowWidth;
				break;
			case 155:
				fs >> m_dbHollowHeight;
				break;
			case 156:
				fs >> m_nCodePage;
				break;
			}
		}
		
		if ( m_BarType == DATAMATRIX && bOldBar )//旧版本格式转换
		{
			m_nSymbolNo = -1;//编码格式转自动
			m_nShapeNo = -1;
		}
		
		dot1.LogToTrue();
		dot2.LogToTrue();
		m_dotBase.LogToTrue();
		
		GenBarCode(0x00);
		CalLimit();
		double moveX,moveY;
		switch( m_nAlign )	//目前不起作用
		{
		case ALIGN_LEFT:
			moveX = dot1.x - m_quadLimit.left;
			break;
		case ALIGN_RIGHT:
			moveX = dot2.x - m_quadLimit.right;
			break;
		case ALIGN_MID:
			moveX = (dot1.x+dot2.x)/2 - ( m_quadLimit.left + m_quadLimit.right )/2;
			break;
		}
		moveY = dot1.y - m_quadLimit.bottom;
		Move( moveX, moveY, 0 );
	}
}

//
void CNodeCodebar::Rotate(CDot dot,double angle, int nPlane)
{
	nPlane = 0;
	/////////
	m_dbAngle  +=  angle;
	m_dotBase.Rotate(dot,nPlane,angle);
	
	CNode::Rotate(dot,angle, nPlane);
}

//
BOOL CNodeCodebar::Move( double moveX,double moveY, double moveZ )
{
	moveZ = 0;
	////////
	m_dotBase.Move(moveX,moveY,moveZ);
	CNode::Move(moveX, moveY,moveZ);

	CalLimit();
	return TRUE;
}


//
void CNodeCodebar::Drag(CDot dot, double ratioX, double ratioY,double moveX,double moveY,double moveZ)
{	
	moveZ = 0;

	if ( fabs(ratioX-1.0) < 0.0001 && fabs(ratioY-1.0) < 0.0001 )
	{
		Move(moveX,moveY,moveZ);
		return;
	}
	
	m_dbRatioX = ratioX;
	m_dbRatioY = ratioY;
	m_dotDragPt = dot;
	
	int nReg = (int)(m_dbAngle/90);
	if ( fabs(m_dbAngle-nReg*90) < 0.01 )
	{
		m_dotBase.Move(moveX,moveY,moveZ);
		if ( ((int)(m_dbAngle)%180) == 0 )
			Scale(dot,ratioX,ratioY);
		else
			Scale(dot,ratioY,ratioX);
	}
}


//
void CNodeCodebar::Scale( CDot dot,double ratioX,double ratioY )
{	
	if ( fabs(ratioX) < 0.0000001 ) ratioX = 1;
	if ( fabs(ratioY) < 0.0000001 ) ratioY = 1;
	
	//所有条码均允许不成比例拉升
/*	if ( m_BarType >=  DATAMATRIX && m_BarType  <=  MicroQR )
	{
		//二维条码缩放一定要XY比例一样
		if ( fabs(ratioX-1) > 0.001 || fabs(ratioY-1) > 0.001 )
		{
			ratioX = fabs(ratioY-1)>fabs(ratioX-1)?ratioY:ratioX;
			ratioY = ratioX;
			m_dbHeightBar = (int)(fabs(m_dbHeightBar*ratioX)*1000)/1000.0;
			GenBarCode(0x00);
		}
	}
	else
*/	{
		//一维条码及PDF417缩放比例XY可以不一样
		m_narrowW = ratioX*m_narrowW;
		m_dbHeightBar = m_dbHeightBar*ratioY;
		GenBarCode(0x00);
	}
}



void CNodeCodebar::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke )
			pStroke->ExportPlt(file, bUnExportFill, bScall100);
	}
}


//
BOOL CNodeCodebar::Create( CRect &rect )
{
	CBarCodeDlg dlg;
	/////////
	dlg.m_BarPage.m_bRev = m_bRev;
    dlg.m_BarPage.m_nSingleLineMarkCount = m_nSingleLineMarkCount;
    dlg.m_BarPage.m_nNarrowLineNum = m_nNarrowLineNum;
    dlg.m_BarPage.m_bGoBack		 =  m_bGoBack;
    dlg.m_BarPage.m_bConnect	 =  m_bConnect;
	dlg.m_BarPage.m_dbHeight		 =  m_dbHeightBar;
	dlg.m_BarPage.m_dbNarrow	 =  m_narrowW;
	dlg.m_BarPage.m_dbShrink		 =  m_dbshrink;
	dlg.m_BarPage.m_dbWidth		 =  m_dbRevWidth;
	dlg.m_BarPage.m_dbHeightRev = m_dbHeightRev;
	dlg.m_nBarType = (UINT)m_BarType;
	dlg.m_BarPage.m_dbSpace		 =  m_dbFillSpace;
	dlg.m_BarPage.m_dbAngle		 =  m_dbAngle;
	dlg.m_BarPage.m_bMatrix		 =  m_bMatrix;
	dlg.m_BarPage.m_bAutoFill		 =  m_bAutoFill;
	dlg.m_BarPage.m_dbHeightRev = m_dbHeightRev;
	dlg.m_BarPage.m_bParity		 =  m_bParity;
	dlg.m_BarPage.m_dbDotRadius =  m_dbDotRadius;
	
	dlg.m_nOutline = m_nOutline;
	dlg.m_nErrLev  = m_nErrLev;
	dlg.m_nCodeType = m_nCodeType;
	dlg.m_nHPoint  = m_nHPoint;
	dlg.m_nWPoint = m_nWPoint;
	dlg.m_nSymbolNo = m_nSymbolNo;
	dlg.m_nShapeNo = m_nShapeNo;
	dlg.m_bAutoSet  = m_bAutoSet;
	dlg.m_nVersion = m_nVersion;
	// 条码镂空参数
	dlg.m_BarPage.m_bHollow = m_bHollow;
	dlg.m_BarPage.m_dbHollowWidth = m_dbHollowWidth;
	dlg.m_BarPage.m_dbHollowHeight = m_dbHollowHeight;

	dlg.m_BarPage.m_nCodePage = m_nCodePage;
	//
	dlg.m_TextPage.m_pNodeCodeBar = this;
	if ( dlg.DoModal() == IDOK )
	{
		CNodeCodebar *pBarTmp = new CNodeCodebar();//使用临时变量用于检查参数合法性，如果失败，原有条码不会丢失
		if ( pBarTmp == NULL )
			return FALSE;

		pBarTmp->CopyToMe(this,0x01);
		pBarTmp->SetParent(GetParent());
		pBarTmp->m_bRev					 =  dlg.m_BarPage.m_bRev;
        pBarTmp->m_nSingleLineMarkCount	 =  dlg.m_BarPage.m_nSingleLineMarkCount;
        pBarTmp->m_nNarrowLineNum	 =  dlg.m_BarPage.m_nNarrowLineNum;
        pBarTmp->m_bGoBack				 =  dlg.m_BarPage.m_bGoBack;
        pBarTmp->m_bConnect				 =  dlg.m_BarPage.m_bConnect;
		pBarTmp->m_dbHeightBar			 =  dlg.m_BarPage.m_dbHeight;
		pBarTmp->m_narrowW				 =  dlg.m_BarPage.m_dbNarrow;
		pBarTmp->m_dbshrink				 =  dlg.m_BarPage.m_dbShrink;
		pBarTmp->m_dbRevWidth			 =  dlg.m_BarPage.m_dbWidth;
		pBarTmp->m_dbHeightRev			 =  dlg.m_BarPage.m_dbHeightRev;
		pBarTmp->m_BarType				 =  (BARTYPE)dlg.m_nBarType;
		pBarTmp->m_dbFillSpace			 =  dlg.m_BarPage.m_dbSpace;
		pBarTmp->m_dbAngle				 =  dlg.m_BarPage.m_dbAngle;
		pBarTmp->m_bMatrix				 =  dlg.m_BarPage.m_bMatrix;
		pBarTmp->m_dbDotRadius			 =  dlg.m_BarPage.m_dbDotRadius;
		pBarTmp->m_bAutoFill			 =  ( m_bMatrix && m_dbDotRadius <0.008 ) ? FALSE : dlg.m_BarPage.m_bAutoFill;
		pBarTmp->m_dbHeightRev			 =  dlg.m_BarPage.m_dbHeightRev;
		pBarTmp->m_bParity					 =  dlg.m_BarPage.m_bParity;
		
		pBarTmp->m_nOutline				 =  dlg.m_nOutline;
		pBarTmp->m_nErrLev				 =  dlg.m_nErrLev;
		pBarTmp->m_nHPoint				 =  dlg.m_nHPoint;
		pBarTmp->m_nWPoint				 =  dlg.m_nWPoint;
		pBarTmp->m_nSymbolNo			 =  dlg.m_nSymbolNo;
		pBarTmp->m_nShapeNo				 =  dlg.m_nShapeNo;
		pBarTmp->m_nCodeType			 =  dlg.m_nCodeType;
		pBarTmp->m_bAutoSet				 =  dlg.m_bAutoSet;
		pBarTmp->m_nVersion				 =  dlg.m_nVersion;
		// 条码镂空参数
		pBarTmp->m_bHollow = dlg.m_BarPage.m_bHollow;
		pBarTmp->m_dbHollowWidth = dlg.m_BarPage.m_dbHollowWidth;
		pBarTmp->m_dbHollowHeight = dlg.m_BarPage.m_dbHollowHeight;

		pBarTmp->m_nCodePage			 =	dlg.m_BarPage.m_nCodePage;
		if( pBarTmp->m_BarType != QR && pBarTmp->m_BarType != DATAMATRIX && pBarTmp->m_BarType != MicroQR && pBarTmp->m_BarType != HANXIN )
		{
			pBarTmp->m_bHollow = FALSE;
		}
		
		
		pBarTmp->ClearAllText();
		POSITION pos = dlg.m_TextPage.m_list.GetHeadPosition();
		while( pos  !=  NULL )
		{
			CText *pText = dlg.m_TextPage.m_list.GetNext(pos);
			if ( pText  !=  NULL )
			{
				CText *pNew = new CText();
				if ( pNew  !=  NULL )
				{
					pNew->CopyExceptID(pText);
					pBarTmp->AddTextAll_NewID(pNew);//向MgrText和Node同时添加TEXT,重新分配ID
				}
			}
		}
		if ( pBarTmp->GenBarCode(0x00) )//创建成功，改变原有条码
		{
			CopyToMe(pBarTmp,0x03);
		}

		delete pBarTmp;
		pBarTmp = NULL;
	}

	SetSelect(TRUE);
	CalLimit();
	
	return TRUE;
}

//注意：条码ID存在一对多情况，为了保障跳号的一致性，
//文本预处理CChain::PreProcessMark()已经对跳号进行了处理，
//此处nStyle应该使用0x00,nStyle保留是维持虚函数的一致性。
BOOL CNodeCodebar::PreprocessMark(DWORD nStyle)
{
	GenBarCode(0x00);//此处nStyle必须用0x00
	CalLimit();
	
	return TRUE;
}

CQuad CNodeCodebar::GetMarkRange()
{
	CQuad quad;
    quad.SetEmpty();
	
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke )
			quad.Union(pStroke->CalLimit());
	}
	quad.Normal();
	
	return quad;
}

/*
nStyle = 0x00时不进行下一个跳号
0x01时进行下一个数的跳号
*/
BOOL CNodeCodebar::GenBarCode(UINT nStyle)
{
	FlashText(nStyle);
	//////
	CString strText = m_strText;
	if ( strText.IsEmpty() )
		return FALSE;
	
	TCHAR szFlag1 = 0x0A;
	TCHAR szFlag2 = 0x0D;
	strText.TrimRight(szFlag1);
	strText.TrimRight(szFlag2);

	BOOL bRtn = g_BarCodeMgr.GenBarCode(this, strText, m_nCodePage);
	if ( TRUE == bRtn )
	{
		CreateStrokeList();
	}
    return bRtn;
}


BOOL CNodeCodebar::CreateStrokeList()
{
	ClearBarList();
	Delete();
	
	BOOL bFlag = FALSE;
	// m_nRow = 1为一维码,大于1为二维码
	if ( m_nRow > 1 )
	{
		if ( m_bMatrix ) // 点阵二维码
		{
			bFlag = CreateStrokeList3();
		}
		else
		{
			//if ( m_BarType == PDF417 )
				bFlag = CreatePDFStrokeList();
			//else
			//	bFlag = CreateStrokeList2();
		}
	}
	else
	{
		bFlag = CreateStrokeList1();
	}
	
	//优化处理
	if ( m_bGoBack )
	{
		BetterOneCodeBar();
	}
	
	//连接处理
	if ( m_bConnect )
	{
		ConnectOneCodeBar();
	}
	
	AddToList();
	InitQuadRange();
	
	//增加转动角度
	double dbAngle = m_dbAngle;
	m_dbAngle = 0;
	Rotate(m_dotBase,dbAngle,0);
	CQuad quadNew = CalLimit();
	CDot dotNew = quadNew.GetBaseDot();
	
	CNode::Move(m_dotBase.x-dotNew.x,m_dotBase.y-dotNew.y,0);
	ClearBarList();
	
	return bFlag;
}


//
BOOL CNodeCodebar::CreateStrokeList1()
{
	CStroke* pStroke  = NULL;

	//一维条码建立过程
	double dbHeight   = m_dbHeightBar;	//二维条码宽度
	double dbOffset   = m_dbFillSpace;
	double dbShrink   = m_dbshrink;	    //缩进量
	BOOL   bRev		 =  m_bRev;			//是否反填
    BOOL   bGoBack	 =  m_bGoBack;	//块往复
	BOOL   bConnect	=  m_bConnect;	//块连接
	double dbRevWidth=  m_dbRevWidth;	//反填宽度
	double dbNarrow	=  m_narrowW;	//条宽比
	double dbAOffset	=  0;	//经过计算算出最合理的均匀填充间距

	UINT   nRevNum  = (UINT)((dbRevWidth-2*dbShrink)/dbOffset+0.5);
	nRevNum  = max(1,nRevNum);
	dbAOffset  = (dbRevWidth-2*dbShrink)/nRevNum;

	double dbGoStep = 0, dbGoStep2 = 0;
	double dbCurX = 0,  dbCurX2 = 0;
	int      nRevFillNum = 0;
	
	//
	m_dbSpace = 0;
	switch(m_BarType)
	{
	case EAN8:
	case EAN13:
	case UPCA:
	case UPCE:
		m_dbSpace = 0.6;
	
	case EAN128:
	case CODE128A:
	case CODE128B:
	case CODE128C:
	case CODE128AUTO:
	case CODE93:
	case CODE93EX:
		{
			BYTE tmp(0);
			BOOL bBlack0(FALSE), bBlack1(FALSE);
			int  nBlack = 0;
			BOOL bFirst = TRUE;
			int  nS = 0;
			BOOL bLong = FALSE;
			
			if ( bRev )
			{
				MakeLineList(2, -dbRevWidth+dbShrink, nRevNum, dbAOffset);
			}
			
			bBlack0 = ( *m_pByte & 0x80 ) ? TRUE:FALSE;
			for( UINT i = 0; i<m_nCol; i++ )
			{
				tmp = *(m_pByte+i);
				bBlack1 = (tmp&0x80)?TRUE:FALSE;
				if ( bBlack1 == bBlack0 )
				{
					nBlack++;
					bLong = (tmp&0x40);
				}
				else
				{
					if ( nBlack > 1 )
					{
						//画线条
						double dbBlack = nBlack*dbNarrow*1.4;
						int      nFillNum = (int)((dbBlack-dbShrink*2)/dbOffset + 0.5);
						nFillNum   = max(1,nFillNum);
						dbGoStep = (dbBlack-dbShrink*2) / nFillNum;

						dbCurX  = nS*dbNarrow*1.4+dbShrink;
						if ( nFillNum > 1 ) 
						{
							if ( bBlack0 )
							{
								MakeLineList(bRev, dbCurX, nFillNum, dbGoStep, bLong);
							}
							else
							{
								if ( bRev ) 
								{
									MakeLineList(2, dbCurX, nFillNum, dbGoStep);
								}					
							}
						}
						else
						{
							double x(0);
							x = (nS + nBlack) * dbNarrow * 1.4 - dbShrink;
							x = ( x > dbCurX ) ? dbCurX : x;

							StrokeList* pStrkList = NULL;

							if ( bBlack0 )
							{
								pStrkList = new StrokeList();
								MakeLineToList(pStrkList, dbCurX, bLong ? -m_dbSpace : 0, dbHeight);
								MakeLineToList(pStrkList, x, bLong ? -m_dbSpace : 0, dbHeight);
								m_CodeBarList.Add(pStrkList);
							}
							else
							{
								if ( bRev )
								{
									pStrkList = new StrokeList;
									if ( pStrkList )
									{
										MakeLineToList(pStrkList, dbCurX, -m_dbSpace-m_dbHeightRev, bLong?-m_dbSpace:0);
										MakeLineToList(pStrkList, dbCurX, m_dbHeightBar, m_dbHeightBar+m_dbHeightRev);
										m_CodeBarList.Add(pStrkList);
									}

									pStrkList = new StrokeList;
									if ( pStrkList )
									{
										MakeLineToList(pStrkList, x, -m_dbSpace-m_dbHeightRev, bLong?-m_dbSpace:0);
										MakeLineToList(pStrkList, x, m_dbHeightBar, m_dbHeightBar+m_dbHeightRev);
										m_CodeBarList.Add(pStrkList);
									}
								}
							}
						}
					}
					else if (1 == nBlack)
					{
						m_nNarrowLineNum = ( m_nNarrowLineNum < 2 ) ? 2 : m_nNarrowLineNum;
						int nNarrowLineCnt = m_nNarrowLineNum;
						double dbBlack = nBlack*dbNarrow*1.4;
						int nLineCnt = (int)((dbBlack-dbShrink*2) / dbOffset + 0.5) + 1;
						if (nLineCnt  >=  (int)nNarrowLineCnt )
						{
							nNarrowLineCnt = nLineCnt;
						}
						dbGoStep = ((dbBlack-dbShrink*2) / (nNarrowLineCnt - 1) );
						dbCurX = nS*dbNarrow*1.4+dbShrink;
						if ( bBlack0 )
						{
							MakeLineList(bRev, dbCurX, nNarrowLineCnt-1, dbGoStep, bLong);
						}
						else
						{
							if ( bRev ) {
								MakeLineList(2, dbCurX, nNarrowLineCnt-1, dbGoStep);
							}					
						}
					}
					
					bBlack0 = bBlack1;
					nBlack = 1;
					nS = i;
					bLong = (tmp&0x40);
				}
			}
			if ( nBlack > 0 )
			{
				//画线条
				double dbBlack = nBlack*dbNarrow*1.4;
				int nFillNum  = (int)((dbBlack-dbShrink*2)/dbOffset + 0.5) + 1;
				nFillNum = (nFillNum  <=  (int)m_nSingleLineMarkCount) ? (int)m_nSingleLineMarkCount : nFillNum;
				
				if ( nFillNum  <=  1 )  
					dbGoStep = 0;
				else
					dbGoStep = (dbBlack-dbShrink*2) / (nFillNum - 1);
		 
				dbCurX = nS*dbNarrow*1.4+dbShrink;
				if ( bBlack0 )
				{
					MakeLineList(bRev, dbCurX, nFillNum-1, dbGoStep, bLong);
				}
				else{
					if ( bRev ) 
					{
						MakeLineList(2, dbCurX, nFillNum-1, dbGoStep);
					}					
				}
			}

			if ( bRev )
			{
				dbCurX = m_nCol*dbNarrow*1.4+dbShrink;
				MakeLineList(2, dbCurX, nRevNum, dbOffset);
			}
			break;
		}

	case CODE39:
	case INT25:
	case EXT39:
	case CODE25:
		{
			BYTE tmp;
			BOOL bBlack = TRUE;
			double dbCurX = 0;
			CDot dottmp,dot;
			double dbBlack;
			int nFillNum;

			if ( bRev )
			{
				MakeLineList(2, -dbRevWidth+dbShrink, nRevNum, dbOffset);
			}
			
			for( UINT i = 0; i<m_nCol; i++ )
			{
				tmp = *(m_pByte+i);
				tmp = tmp&0x0F;
				/////////////
				if ( tmp == 0x01 )//窄条
				{
					dbBlack = m_narrowW;
				}
				else if (tmp == 0x00)//位空
				{
					dbBlack  = m_narrowW;
				}
				else//宽条
				{
					dbBlack   = m_dbcodeRate*m_narrowW;
				}

				nFillNum = (int)((dbBlack-dbShrink*2)/dbOffset + 0.5);
				nFillNum = ( nFillNum  <=  (int)m_nSingleLineMarkCount ) ? (int)m_nSingleLineMarkCount : nFillNum;
				dbAOffset =  (dbBlack-dbShrink*2)/nFillNum;
				if ( bBlack )
				{
					MakeLineList(bRev, dbCurX+dbShrink, nFillNum, dbAOffset, 0);
				}
				else
				{
					if ( bRev ) 
					{
						MakeLineList(2, dbCurX+dbShrink, nFillNum, dbAOffset);
					}					
				}
				dbCurX  +=  dbBlack;
				
				//下一个条相反
				bBlack = !bBlack;
			}

			if ( bRev )
			{
				MakeLineList(2, dbCurX+dbShrink, nRevNum, dbOffset);
			}
			break;
		}
	}
	
	return TRUE;
}

BOOL CNodeCodebar::CreatePDFStrokeList()
{	
	double dbBlockH  = m_dbHeightBar/m_nRow;	//二维条码高度
	double dbBlockW = 10.0/m_nRow*m_narrowW/basenar;
	double dbOffset  = m_dbFillSpace;
	double dbRevWidth = m_dbRevWidth;	//反填宽度

	UINT   nFillNum = (UINT)((dbBlockW-m_dbshrink*2)/dbOffset+0.5);	
	nFillNum = max(1,nFillNum);
	dbOffset = (dbBlockW-m_dbshrink*2)/nFillNum;

	UINT nRevNum = (UINT)((dbRevWidth-m_dbshrink*2)/dbOffset+0.5);
	nRevNum = max(1,nRevNum);
	dbRevWidth   = nRevNum*dbOffset+2*m_dbshrink;

	// 计算镂空参数
	int nHollowColS = 0;
	int nHollowRowS = 0;
	int nHollowColCount = 0;
	int nHollowRowCount = 0;

	BOOL bHollow = m_bHollow;
	if( bHollow )
	{
		if( m_dbHollowWidth <=0 
			|| m_dbHollowWidth >= m_dbHeightBar  
			|| m_dbHollowHeight <= 0 
			|| m_dbHollowHeight >= m_dbHeightBar )
		{
			bHollow = FALSE;
		}
		else
		{
			nHollowColCount = int(m_dbHollowWidth*m_nCol/m_dbHeightBar);
			nHollowRowCount = int(m_dbHollowHeight*m_nRow/m_dbHeightBar);

			nHollowColS = m_nCol/2-nHollowColCount/2;
			nHollowRowS = m_nRow/2-nHollowRowCount/2;
		}
	}
	
	if ( m_bRev )
	{
		MakeLineList(-dbRevWidth+m_dbshrink, -(m_nRow*dbBlockH+dbRevWidth)+m_dbshrink, dbRevWidth-m_dbshrink, nRevNum, dbOffset);
	}

	BYTE s,e;	//标准点，用于反填时确定位置
	s = m_bRev?0:1;
	e = m_bRev?1:0;

	for( UINT i = 0; i<m_nCol; i++ )
	{
		UINT nS(0),nE(0);
		CUIntArray nArray;
		BOOL bS(FALSE),bE(FALSE);
		BOOL bEnter(FALSE);
		BYTE byte;
		
		for( UINT j = 0; j<m_nRow; j++ )
		{
			byte = *(m_pByte+j*m_nCol+i);

			/// 镂空
			if( bHollow )
			{	
				if( (j >= nHollowRowS && j < nHollowRowS+nHollowRowCount) && (i>=nHollowColS && i < nHollowColS+nHollowColCount) )
					byte = m_bRev?1:0;
			}
			//////////////////////////////////////////////////////////////////////////

			if ( (s == byte) && (FALSE == bEnter) )
			{
				bEnter = TRUE;
				nS = j;
			}
			else if ( (e == byte) && bEnter )
			{
				bEnter = FALSE;
				nE = j;
				nArray.Add(nS);
				nArray.Add(nE);
			}
		}
		
		if ( bEnter )
		{	
			nE = m_nRow;
			nArray.Add(nS);
			nArray.Add(nE);
		}

		if ( m_bRev )
		{
			if ( nArray.GetSize() == 0 || nArray.GetAt(0)  !=  0 )  
				MakeLineList(i*dbBlockW+m_dbshrink, m_dbshrink, dbRevWidth-m_dbshrink, nFillNum, dbOffset);
		}
		
		int nSize = nArray.GetSize();
		for ( int m = 0; m<nSize/2; m++)
		{
			nS = nArray.GetAt(m*2);
			nE = nArray.GetAt(m*2+1);

			bS = (nS == 0 && m_bRev);
			bE = (nE == m_nRow && m_bRev);

			MakeLineList(	i*dbBlockW+m_dbshrink, 
							bE ? -(m_nRow*dbBlockH+dbRevWidth-m_dbshrink) : -(nE*dbBlockH-m_dbshrink),
							bS ? (dbRevWidth-m_dbshrink) : -(nS*dbBlockH+m_dbshrink),
							nFillNum, 
							dbOffset);	
		}

		if ( m_bRev )
		{
			
			if ( nArray.GetSize() == 0 || nArray.GetAt(nArray.GetSize()-1)  !=  m_nRow )
			{
				MakeLineList(	i*dbBlockW+m_dbshrink, 
								-(m_nRow*dbBlockH+dbRevWidth-m_dbshrink), 
								-(m_nRow*dbBlockH+m_dbshrink), 
								nFillNum, 
								dbOffset);
			}
			
		}
	}

	if ( m_bRev )
	{
		MakeLineList(m_nCol*dbBlockW+m_dbshrink, -(m_nRow*dbBlockH+dbRevWidth)+m_dbshrink, dbRevWidth-m_dbshrink, nRevNum, dbOffset);
	}
	
	return TRUE;
}

BOOL CNodeCodebar::CreateStrokeList2()
{	
	double dbBlockH  = m_dbHeightBar/m_nRow;	//二维条码宽度
	double dbOffset  = m_dbFillSpace;
	double dbRevWidth =  m_dbRevWidth;	//反填宽度

	UINT   nFillNum = (UINT)((dbBlockH-m_dbshrink*2)/dbOffset+0.5);	
	nFillNum = max(1,nFillNum);
	dbOffset =  (dbBlockH-m_dbshrink*2)/nFillNum;

	UINT nRevNum = (UINT)((dbRevWidth-m_dbshrink*2)/dbOffset+0.5);
	nRevNum = max(1,nRevNum);
	dbRevWidth = nRevNum*dbOffset+2*m_dbshrink;

	if ( m_bRev )
	{
		MakeLineList(-dbRevWidth+m_dbshrink, -(m_nRow*dbBlockH+dbRevWidth)+m_dbshrink, dbRevWidth-m_dbshrink, nRevNum, dbOffset);
	}

	BYTE s,e;	//标准点，用于反填时确定位置
	s = m_bRev?0:1;
	e = m_bRev?1:0;

	for( UINT i = 0; i<m_nCol; i++ )
	{
		UINT nS(0),nE(0);
		CUIntArray nArray;
		BOOL bS(FALSE),bE(FALSE);
		BOOL bEnter(FALSE);
		BYTE byte;
		
		for( UINT j = 0;j<m_nRow;j++ )
		{	
			byte = *(m_pByte+j*m_nCol+i);
			if ( (s == byte) && (FALSE == bEnter) )
			{
				bEnter = TRUE;
				nS = j;
			}
			else if ( (e == byte) && bEnter )
			{
				bEnter = FALSE;
				nE = j;
				nArray.Add(nS);
				nArray.Add(nE);
			}
		}
		
		if ( bEnter )
		{	
			nE = m_nRow;
			nArray.Add(nS);
			nArray.Add(nE);
		}

		if ( m_bRev )
		{
			if ( nArray.GetSize() == 0 || nArray.GetAt(0)  !=  0 )  
				MakeLineList(i*dbBlockH+m_dbshrink, m_dbshrink, dbRevWidth-m_dbshrink, nFillNum, dbOffset);
		}
		
		int nSize = nArray.GetSize();
		for ( int m = 0; m<nSize/2; m++)
		{
			nS = nArray.GetAt(m*2);
			nE = nArray.GetAt(m*2+1);

			bS = (nS == 0 && m_bRev);
			bE = (nE == m_nRow && m_bRev);

			MakeLineList(	i*dbBlockH+m_dbshrink, 
							bE ? -(m_nRow*dbBlockH+dbRevWidth-m_dbshrink) : -(nE*dbBlockH-m_dbshrink),
							bS ? (dbRevWidth-m_dbshrink) : -(nS*dbBlockH+m_dbshrink),
							nFillNum, 
							dbOffset);	
		}

		if ( m_bRev )
		{
			if ( nArray.GetSize() == 0 || nArray.GetAt(nArray.GetSize()-1)  !=  m_nRow )
			{
				MakeLineList(	i*dbBlockH+m_dbshrink, 
								-(m_nRow*dbBlockH+dbRevWidth-m_dbshrink), 
								-(m_nRow*dbBlockH+m_dbshrink), 
								nFillNum, 
								dbOffset);
			}
		}
	}

	if ( m_bRev )
	{
		MakeLineList(m_nCol*dbBlockH+m_dbshrink, -(m_nRow*dbBlockH+dbRevWidth)+m_dbshrink, dbRevWidth-m_dbshrink, nRevNum, dbOffset);
	}
	
	return TRUE;
}


void CNodeCodebar::FillStroke(list<CStroke*> * pList,CStroke *pStroke)
{
	StrokeList listFill,listData;
	listData.AddTail(pStroke);
	
	if ( !m_bAutoFill )
		return;

	CStroke   *pStrokeList = NULL;
	CStrokeB *pStrokeB = NULL;
	
	BaseHatch  hatch;
	hatch.dbSpace = m_dbFillSpace;
	hatch.bEnable  = TRUE;
	hatch.dbAngle  = 0;
	hatch.dbBeamComp = m_dbshrink*0.7;
	hatch.bEquaDist = TRUE;
	hatch.nFillStyle = (m_bGoBack ? 2 : 0 );		//单向/往复填充
	//
	CFill f1(hatch, m_property);
	f1.Proc(&listData,listFill);
	
	POSITION pos = listFill.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pStrokeB = listFill.GetNext(pos);
		///////
		if ( pStrokeB  !=  NULL )
		{
			pStrokeList = new CStroke(m_property);
			if ( pStrokeList == NULL  )
				return;

			(*pStrokeList) = (*pStrokeB);
			//pStrokeList->SetProperty( PTY_INIT, m_property);
			
			pList->push_back(pStrokeList);
			
			delete pStrokeB;
			pStrokeB = NULL;
		}
	}
	listFill.RemoveAll();
}


BOOL CNodeCodebar::CreateStrokeList3()
{
	double dbBlockH = m_dbHeightBar/m_nRow;
	UINT    nRevNum = (UINT)(m_dbRevWidth/dbBlockH+0.5);

	CDot dot(0,0);
	int i(0), j(0), k(0), n(0), nn(0);
	CStroke* pStroke = NULL;
	BYTE byte(0);
	BOOL bDirect = TRUE;

	CGArc arcObj;
	CDot  dot2(0,0);
	list<CStroke*> listStroke;
	list<CStroke*>::const_iterator iter;
	
	arcObj.m_dbPrecision = g_SysCfg.m_dbDotmatrixCirPrec;
	arcObj.m_nSampleCount = 10;
	
	if ( m_bRev )
	{
		for( n = nRevNum; n > 0; --n )
		{
			dot.x = -n*dbBlockH;
			nn = (-1) * (m_nRow + nRevNum);
			for(k = nRevNum; k>nn; --k)
			{
				dot.y = k*dbBlockH;
				pStroke = new CStroke(m_property);
				if ( ! pStroke  )
					return FALSE;

				if ( m_dbDotRadius >  0.01 )
				{
					dot2.x = dot.x + m_dbDotRadius;
					dot2.y = dot.y;
					arcObj.CreateArcline(dot, dot2, dot2, pStroke, 2);
					if ( m_bAutoFill )
						FillStroke(&listStroke,pStroke);
				}
				else
				{
					pStroke->Add(dot);
				}
				listStroke.push_back(pStroke);
			}

			if ( listStroke.size() ) 
			{
				if ( m_bGoBack && bDirect )
				{
					listStroke.reverse();
				}
				for( iter = listStroke.begin(); iter  !=  listStroke.end(); ++iter ) 
				{
					m_list.AddTail(*iter);
				}
				listStroke.clear();
				bDirect  = !bDirect;
			}
		}
	}
	
	for( i = 0; i < (int)m_nCol; i++ )
	{
		if ( m_bRev )
		{
			dot.x = i*dbBlockH;
			for( k = nRevNum; k > 0; --k )
			{
				dot.y = k*dbBlockH;
				pStroke = new CStroke(m_property);
				if ( ! pStroke  )
					return FALSE;

				if ( m_dbDotRadius >  0.01 )
				{
					dot2.x = dot.x + m_dbDotRadius;
					dot2.y = dot.y;
					arcObj.CreateArcline(dot, dot2, dot2, pStroke, 2);
					FillStroke(&listStroke,pStroke);
				}
				else
				{
					pStroke->Add(dot);
				}
				listStroke.push_back(pStroke);
			}
		}

		dot.x = i*dbBlockH;
		for( j = 0; j < (int)m_nRow; j++ )
		{	
			byte = *(m_pByte+j*m_nCol+i);
			if (byte  !=  m_bRev) 
			{
				dot.y = -j*dbBlockH;
				pStroke = new CStroke(m_property);
				if ( ! pStroke  )
					return FALSE;
				if ( m_dbDotRadius >  0.01 )
				{
					dot2.x = dot.x + m_dbDotRadius;
					dot2.y = dot.y;
					arcObj.CreateArcline(dot, dot2, dot2, pStroke, 2);
					FillStroke(&listStroke,pStroke);
				}
				else
				{
					pStroke->Add(dot);
				}
				listStroke.push_back(pStroke);
			}
		}
		
		if ( m_bRev )
		{
			for( k = 0; k < (int)nRevNum; k++ )
			{
				dot.y = -((m_nRow+k)*dbBlockH);
				pStroke = new CStroke(m_property);
				if  ( m_dbDotRadius >  0.01 )
				{
					dot2.x = dot.x + m_dbDotRadius;
					dot2.y = dot.y;
					arcObj.CreateArcline(dot, dot2, dot2, pStroke, 2);
					FillStroke(&listStroke,pStroke);
				}
				else
				{
					pStroke->Add(dot);
				}
				listStroke.push_back(pStroke);
			}
		}

		if ( listStroke.size() ) 
		{
			if ( m_bGoBack && bDirect ) 
			{
				listStroke.reverse();
			}
			
			for( iter = listStroke.begin(); iter != listStroke.end(); ++iter ) 
			{
				m_list.AddTail(*iter);
			}
			listStroke.clear();
			bDirect = !bDirect;
		}
	}
	
	if ( m_bRev )
	{
		for( n = 0; n<(int)nRevNum; n++ )
		{
			dot.x = (m_nCol+n)*dbBlockH;

			nn = (-1) * (m_nRow + nRevNum);
			for ( k = nRevNum; k > nn; --k ) 
			{
				dot.y = k*dbBlockH;
				pStroke = new CStroke(m_property);
				if ( ! pStroke  )
					return FALSE;
				if  ( m_dbDotRadius >  0.01 )
				{
					dot2.x = dot.x + m_dbDotRadius;
					dot2.y = dot.y;
					arcObj.CreateArcline(dot, dot2, dot2, pStroke, 2);
					FillStroke(&listStroke,pStroke);
				}
				else
				{
					pStroke->Add(dot);
				}
				listStroke.push_back(pStroke);					
			}
			
			if ( listStroke.size() ) 
			{
				if ( m_bGoBack && bDirect ) 
				{
					listStroke.reverse();
				}
				
				for( iter = listStroke.begin();iter != listStroke.end();++iter ) 
				{
					m_list.AddTail(*iter);
				}
				listStroke.clear();
				bDirect = !bDirect;
			}
		}
	}

	return TRUE;
}

//注意dwStyle的用法，详见函数内说明
BOOL CNodeCodebar::CopyToMe(CNode* pSrc,DWORD dwStyle)
{
	if ( pSrc == NULL || pSrc->GetType()  !=  NODE_CODEBAR )
		return FALSE;

	CNodeCodebar *pNode = (CNodeCodebar*)pSrc;

	m_bRev				=  pNode->m_bRev;
	m_dbRevWidth		= pNode->m_dbRevWidth;
	m_BarType			=  pNode->m_BarType;
	m_dbFillSpace		= pNode->m_dbFillSpace;
	
	m_type				=  pNode->m_type;
	m_bAutoSet			=  pNode->m_bAutoSet;
	m_dbHeightBar		=  pNode->m_dbHeightBar ;
	m_dbshrink			=  pNode->m_dbshrink;
	m_nOutline			=  pNode->m_nOutline;
	m_nErrLev			=  pNode->m_nErrLev;
	m_bAutoFill			=  pNode->m_bAutoFill;
	m_nAlign			=  pNode->m_nAlign;
	m_narrowW			=  pNode->m_narrowW;
	m_bSelect			=  pNode->m_bSelect;
	m_nHPoint			=  pNode->m_nHPoint;
	m_nWPoint			=  pNode->m_nWPoint;
	m_dotBase			=  pNode->m_dotBase;
    m_dbAngle			=  pNode->m_dbAngle;
	m_bMatrix			=  pNode->m_bMatrix;
	m_dbHeightRev		=  pNode->m_dbHeightRev;
    m_bGoBack			=  pNode->m_bGoBack;
	m_bConnect			=  pNode->m_bConnect;
	m_nSingleLineMarkCount = pNode->m_nSingleLineMarkCount;
	m_property			=  pNode->m_property;
	m_quadRangle		= pNode->m_quadRangle;
	m_bParity			= pNode->m_bParity;
	m_bTextBar			= pNode->m_bTextBar;
	m_dbDotRadius		=  pNode->m_dbDotRadius;
	m_nSymbolNo			= pNode->m_nSymbolNo;
	m_nShapeNo			= pNode->m_nShapeNo;
	m_nCodeType			=  pNode->m_nCodeType;
	m_nVersion			=  pNode->m_nVersion;
	///镂空参数
	m_bHollow			= pNode->m_bHollow;
	m_dbHollowWidth		= pNode->m_dbHollowWidth;
	m_dbHollowHeight	= pNode->m_dbHollowHeight;

	m_nCodePage			=  pNode->m_nCodePage;


	if ( !CopyText(pNode,dwStyle,g_dwJumpCnt) )
		return FALSE;
	// CopyText将根据设定条件和传入node 生成新的TEXT及建立ID对应关系
	// TextBase原有ID对应关系会被自动删除
	// dwStyle用法说明如下，使用时需谨慎使用
	// 0x00 : 同一个chain中重新创建新TEXT，ID需不同
	// 0x01 : 不同Chain之间互相拷贝，ID相同但两者不相关
	// 0x02 : 同一Chain中增加引用，ID不增加
	// 0x03 : 同一chain中仅互相拷贝数据，原有ID不变,原有ID个数不足时自动增加
	
	GenBarCode(0x00);
	CalLimit();
	
	return TRUE;
}

//
BOOL CNodeCodebar::ClearBarList()
{
	int nCount = m_CodeBarList.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		StrokeList *pSList = m_CodeBarList.GetAt(i);
		if ( NULL == pSList )
			continue;
		
		delete pSList;
		pSList = NULL;
	}
	
	m_CodeBarList.RemoveAll();
	
	return TRUE;
}

void CNodeCodebar::Draw(XDC *pDC)
{
	UINT nFont = FONT_TTF;
	if ( m_nRow > 1 && m_bMatrix && m_dbDotRadius  <=  0.01 ) 
		nFont = FONT_MATRIX;

	POSITION pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos  !=  NULL)
	{
		pStroke = (CStroke*)m_list.GetNext(pos);
		if ( pStroke )
			pStroke->Draw(pDC, nFont, m_bIsCurSortTree);
	}
}

double CNodeCodebar::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return  dbD;
}


//
BOOL CNodeCodebar::MinDsIsFirstDot(CDot &dt, CStroke *pStrk)
{
	if ( NULL == pStrk )
		return FALSE;
	
	int nCount = pStrk->GetSize();
	if ( nCount  <=  1 ) 
		return FALSE;
	
	CDot sDt = pStrk->GetAt(0);
	CDot eDt = pStrk->GetAt(nCount -1);

	if ( Distance(dt, sDt) < Distance(dt, eDt) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}


BOOL CNodeCodebar::ProOneStroke(CDot &dt, CStroke *pStrk)
{
	if ( !pStrk )
		return FALSE;
		
	if ( !MinDsIsFirstDot(dt, pStrk) )
	{
		pStrk->Reverse();
	}

	return TRUE;
}


BOOL CNodeCodebar::BetterOneCodeBar()
{
	int nCount = m_CodeBarList.GetSize();
	if ( nCount  <=  0 ) 
		return FALSE;
	
	BOOL bIsGetFirstDt = FALSE;
	CDot lsDt;
	
	for ( int i = 0; i < nCount; i++ )
	{
		StrokeList *pSList = m_CodeBarList.GetAt(i);
		if ( NULL == pSList )
			continue;
		
		POSITION pos = pSList->GetHeadPosition();
		CStroke *pStroke = NULL;
		while( pos != NULL )
		{
			pStroke = (CStroke *)pSList->GetNext(pos);
			if ( NULL == pStroke ) 
				continue;
		
			int nDtCount = pStroke->GetSize();
			if ( nDtCount  <=  0 ) 
				continue;
			
			if ( !bIsGetFirstDt )
			{
				lsDt = pStroke->GetAt(nDtCount-1);
				bIsGetFirstDt = TRUE;
				continue;
			}
			
			ProOneStroke(lsDt, pStroke);
			lsDt = pStroke->GetAt(nDtCount-1);
		}// while(pos != NULL)
	}// for (int i = 0; i < nCount; i++)
	
	return TRUE;
}



BOOL CNodeCodebar::ConnectOneCodeBar()
{
	int nCount = m_CodeBarList.GetSize();
	if ( nCount  <=  0 ) 
		return FALSE;
	
	for ( int i = 0; i < nCount; i++ )
	{
		StrokeList *pSList = m_CodeBarList.GetAt(i);
		if ( NULL == pSList )
			continue;
		
		POSITION pos  = pSList->GetHeadPosition();
		POSITION prev = NULL;
		CStroke* pPreStroke = (CStroke*)pSList->GetNext(pos);
		if ( NULL == pPreStroke )
			continue;
		
		while (pos  !=  NULL)
		{
			prev = pos;
			CStroke* pStroke = (CStroke*)pSList->GetNext(pos);
			if ( NULL == pStroke ) 
				continue;
			
			int nCnt = pStroke->GetSize();
			for (int k = 0; k < nCnt; k++)
			{
				CDot dt = pStroke->GetAt(k);
				pPreStroke->Add(dt);
			}
			
			pSList->RemoveAt(prev);
			if ( pStroke ) 
			{
				delete pStroke;
				pStroke = NULL;
			}
		}//end while (pos  !=  NULL)
	}

	return TRUE;
}

BOOL CNodeCodebar::AddToList()
{
	int nCount = m_CodeBarList.GetSize();
	if ( nCount  <=  0 ) 
		return FALSE;
	
	for (int i = 0; i < nCount; i++)
	{
		StrokeList *pSList = m_CodeBarList.GetAt(i);
		if ( NULL == pSList )
			continue;

		POSITION pos = pSList->GetHeadPosition();
		CStroke *pStroke = NULL;
		while(pos != NULL)
		{
			pStroke = (CStroke *)pSList->GetNext(pos);
			if ( NULL == pStroke ) 
				continue;

			m_list.AddTail(pStroke);
		}
	}
	
	return TRUE;
}

void CNodeCodebar::DrawExtern(XDC *pDC)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )	
	{
		pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke )
			pStroke->DrawExtern(pDC);
	}
}

//
void CNodeCodebar::RefreshBarText(CWnd* pWnd, int nTextID)
{
	if ( IsExistText(nTextID) )
	{	
		CQuad quad = CalLimit();
		quad.Inflate(50,5);
		///////////
		CZoomView *pView = (CZoomView*)pWnd;
		if ( pView )
		{
			pView->PhysicToLogic(quad);
			pView->InvalidateRect(&(RECT)quad);
		}
	}	
}

//
BOOL CNodeCodebar::GetLockFlg()
{
	CHLDoc *pDoc = (CHLDoc*)GetCurActiveDocument();
	if ( pDoc == NULL )
		return FALSE;
	
	int nLayer    = 0;
    int nFillLayer = 0;
	GetLayColor(nLayer, nFillLayer);
		
	return ( pDoc->GetLockFlg(nLayer) && IsFixSerial() );
}

//
void CNodeCodebar::LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo)
{	
	if ( (bOptimize == FALSE) && (m_bPathOptimize == TRUE) )
		return;
	
	if ( (bOptimize == TRUE) && ( IsFixSerial() == FALSE) )
		return;
	
	int nLay  = m_property.m_nLayer;
	if ( nLay == nLayNo || nLayNo == -1 )
	{	
		POSITION pos = m_list.GetHeadPosition();
		CStroke *pStroke = NULL;
		while( pos )
		{	
			pStroke = (CStroke *)m_list.GetNext(pos);
			if ( !pStroke )
				continue;
			
			pStroke->m_bSelect = m_bSelect;
			pStrokeList->AddTail(pStroke);
		}
		
		m_bPathOptimize = bOptimize;
	}
}

BOOL CNodeCodebar::MakeLineToList(StrokeList *pStrkList, double x, double y0, double y1)
{
	if ( NULL == pStrkList ) 
		return FALSE;
 
	if ( fabs(y0-y1) < EPSILON )  
		return FALSE;
 
	CStroke* pStroke = new CStroke(m_property);
	if ( !pStroke )
		return  FALSE;

	CDot  dot;
	dot.x	=  x;
	dot.y	=  y0;
	pStroke->Add(dot);
	dot.y = y1;
	pStroke->Add(dot);
	
	pStrkList->AddTail(pStroke);

	return TRUE;
}

BOOL CNodeCodebar::MakeLineList(double x, double y0, double y1, int nLineCnt, double dbGoStep)
{
	if ( fabs(y0-y1) < EPSILON || nLineCnt  <=  0 )  
		return FALSE;

	StrokeList *pStrkList = new StrokeList;
	if ( NULL == pStrkList ) 
		return FALSE;
	
	double  x1(0.0);
	if ( !m_bAutoFill )//仅生成外框
	{
		CStroke* pStroke = new CStroke(m_property);
		if ( !pStroke )
			return FALSE;
		CDot  dot;
		x1 = x + nLineCnt*dbGoStep;
		dot.x = x;
		dot.y = y0;
		pStroke->Add(dot);
		dot.x = x1;
		pStroke->Add(dot);
		dot.y = y1;
		pStroke->Add(dot);
		dot.x = x;
		pStroke->Add(dot);
		dot.y = y0;
		pStroke->Add(dot);
		//
		pStrkList->AddTail(pStroke);
	}
	else
	{
		for( int j = 0; j < nLineCnt; j++ )
		{
			x1 = x + j*dbGoStep;
			MakeLineToList(pStrkList, x1, y0, y1);
		}
	}
	
	if ( pStrkList->GetCount() > 0 )
	{
		m_CodeBarList.Add(pStrkList);
	}
	else
	{
		delete pStrkList;
		return FALSE;
	}

	return TRUE;
}

BOOL CNodeCodebar::MakeLineList(int nType, double x, int nLineCnt, double dbGoStep, BOOL bLong)
{
	if ( nLineCnt  <=  0 )  
		return TRUE;

	if ( 0 == nType ) {
		MakeLineList(x, bLong?-m_dbSpace:0, m_dbHeightBar, nLineCnt, dbGoStep);
	}
	else if ( 1 == nType ) {
		MakeLineList(x, -m_dbSpace-m_dbHeightRev, bLong?-m_dbSpace:0, nLineCnt, dbGoStep);
		MakeLineList(x, m_dbHeightBar, m_dbHeightBar+m_dbHeightRev, nLineCnt, dbGoStep);
	}
	else if ( 2 == nType ) {
		MakeLineList(x, -m_dbSpace-m_dbHeightRev, m_dbHeightBar+m_dbHeightRev, nLineCnt, dbGoStep);
	}

	return TRUE;
}




