// NodeText.cpp: implementation of the CNodeText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeText.h"
#include "Text.h"
#include "HLView.h"
#include "FntShx.h"
#include "FntTTF.h"
#include "TextBaseDlg.h"
#include "NodeLine.h"
#include "FntHzk.h"
#include "SetBarTextDlg.h"
#include "baseMacro.h"
#include "FontUser.h"
#include "WeldEditPro.h"
#include "DotInterFilter.h"
#include "BroadenStroke.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CNodeText,CObject,2000)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodeText::CNodeText()
{
	m_type = NODE_HLTEXT;
	//初始化值
	m_dotBase = CDot(0,0);
	m_nPath	 = PATH_LINE;
	m_dbDirection = 0;
	m_dbRadius = 20; 
	m_bCCW = FALSE;
	m_bInside = FALSE;
	m_dbStart = 0;
	m_bPrintInverse = FALSE;
	
	m_nFont = FONT_TTF;
	m_strHzkType = FONT_HZK_12;
	m_strAscType = FONT_ASC_8_16;
	
	m_dbCharSpace	 =  0;
	m_dbWidthFactor = 1;
	m_dbHeight		 =  5;
	m_dbLineSpace	 =  0.5;
	
	m_nAlign = ALIGN_MID;
	m_strText = _T("");
	
	m_pFnt = NULL;
	m_strFontCHS	 =  _T("简体宋体");
	m_strFontEN	 =  _T("romans");
	m_strFontNUM	 =  _T("romans");
	m_strFontUser	 =  _T("hans001");
	m_dbFixSpace	 =  5.0;
	m_bFixSpace	 =  FALSE;

	// add by zhq
	m_bWeld = FALSE;
	m_bInterFilter = FALSE;
	m_dbInterSpace = 0.1;

	m_bBroaden = FALSE;
	m_dbLineWidth = 0.1;
	m_dbBroadenD = 0.05;
	
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight	 =  -24;
	m_lf.lfHeight   = FW_NORMAL;
	m_lf.lfCharSet = DEFAULT_CHARSET;
	_tcscpy_s(m_lf.lfFaceName,32,_T("Arial"));
	
	m_bCustomFormat = FALSE;
	m_strFormat	 =  _T("XXX-XXX");
	m_strPrecisK	 =  _T("200");
	
	m_bParity		 =  FALSE;
	m_bBarText		 =  FALSE;
	m_strBarString	 =  _T("");
	m_nErrLev = 1;
	m_nSize = 1;
	m_nCodeType = 5;

	m_dbMatrixR = 0 ;
	m_dbMatrixFillD = 0.05;
	
	m_quadChar = CQuadRangle(CDot(0,0), CDot(0,1), CDot(1,1), CDot(1,0));
}

//
CNodeText::~CNodeText()
{
	if ( m_pFnt )
	{
		delete m_pFnt;
		m_pFnt = NULL;
	}
}

//
BOOL CNodeText::Track(CWnd* pWnd, CPoint ptOrg)
{
	return Create(CRect(0,0,0,0));
}

//
void CNodeText::Draw(XDC *pDC)
{
	if ( m_pFnt != NULL )
	{
		m_pFnt->Draw(pDC);
	}
}



CQuad CNodeText::CalLimit()
{
	m_quadLimit.SetEmpty();
	
	if ( m_pFnt )
		m_quadLimit.Union(m_pFnt->CalLimit());
	
	return m_quadLimit;
}



CQuad CNodeText::GetMarkRange()
{
	CQuad quad;
	quad.SetEmpty();
	if ( m_pFnt )
		quad.Union(m_pFnt->GetMarkRange());
	
	return quad;
}


void CNodeText::SetProperty(int mode,CProperty pty, XDC *pDc)
{
	m_property.Set(mode,pty);

	if ( m_pFnt  !=  NULL)
		m_pFnt->SetProperty(mode,pty);
}


BOOL CNodeText::Click(CPoint &point,CWnd *pWnd)
{
	return m_quadLimit.Include(point,pWnd);
}

BOOL CNodeText::CanGroup()
{
	return TRUE;
}

BOOL  CNodeText::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( pSrc == NULL || pSrc->GetType()  !=  NODE_HLTEXT )
		return FALSE;

	CNodeText *pNode = (CNodeText*)pSrc;

	m_property			 =  pNode->m_property;
	m_bSelect			 =  pNode->m_bSelect;
	m_nXPos				 =  pNode->m_nXPos;
	m_nAlign				 =  pNode->m_nAlign;
	
	m_dotBase			 =  pNode->m_dotBase;
	m_nPath				 =  pNode->m_nPath;
	m_dbDirection		 =  pNode->m_dbDirection;
	m_dbRadius			 =  pNode->m_dbRadius;
	m_bCCW				 =  pNode->m_bCCW;
	m_bInside			 =  pNode->m_bInside;
	m_dbStart			 =  pNode->m_dbStart;
	
	m_nFont				 =  pNode->m_nFont;
	m_strFontCHS		 =  pNode->m_strFontCHS;
	m_strFontEN		 =  pNode->m_strFontEN;
	m_strFontNUM		 =  pNode->m_strFontNUM;
	m_lf					 =  pNode->m_lf;
	
	m_strFontUser		 =  pNode->m_strFontUser;
	m_dbFixSpace		 =  pNode->m_dbFixSpace;
	m_dbCharSpace	 =  pNode->m_dbCharSpace;
	m_dbWidthFactor	 =  pNode->m_dbWidthFactor;
	m_dbHeight			 =  pNode->m_dbHeight;
	m_dbLineSpace	 =  pNode->m_dbLineSpace;
	
	m_bWeld				 =  pNode->m_bWeld;
	m_bInterFilter		 =  pNode->m_bInterFilter;
	m_dbInterSpace	 =  pNode->m_dbInterSpace;

	// 
	m_bPrintInverse		= pNode->m_bPrintInverse;

	m_bBroaden			=  pNode->m_bBroaden;
	m_dbLineWidth		=  pNode->m_dbLineWidth;
	m_dbBroadenD		=  pNode->m_dbBroadenD;
	
	m_strHzkType		 =  pNode->m_strHzkType;
	m_strAscType		 =  pNode->m_strAscType;
	
	m_bFixSpace		 =  pNode->m_bFixSpace;
	m_quadChar		 =  pNode->m_quadChar;
	m_quadRangle		 =  pNode->m_quadRangle;
	
	m_bParity			 =  pNode->m_bParity;
	m_bBarText			 =  pNode->m_bBarText;
	m_nBarType         =  pNode->m_nBarType;
	m_nErrLev			 =  pNode->m_nErrLev;
	m_nSize				 =  pNode->m_nSize;
	m_nCodeType		 =  pNode->m_nCodeType;

	m_dbMatrixR		=   pNode->m_dbMatrixR;
	m_dbMatrixFillD	=	pNode->m_dbMatrixFillD;
	
	// CopyText中dwStyle用法说明如下，使用时需谨慎使用
	// 0x00 : 同一个chain中重新创建新TEXT，ID需不同
	// 0x01 : 不同Chain之间互相拷贝，ID相同但两者不相关
	// 0x02 : 同一Chain中增加引用，ID不增加
	if ( !CopyText(pNode,dwStyle,g_dwJumpCnt) )
		return FALSE;

	m_strText			 =  pNode->m_strText;
	m_bCustomFormat = pNode->m_bCustomFormat;
	m_strFormat		=  pNode->m_strFormat;
    m_strPrecisK		=  pNode->m_strPrecisK;
	CreateString(FALSE);

	return TRUE;
}

//
void CNodeText::CopyProperty(CNodeText *pSrc)
{
	if ( pSrc == NULL )
		return;

	//COPY相关属性，不COPY文本ID
	m_property			 =  pSrc->m_property;
	m_bSelect			 =  pSrc->m_bSelect;
	m_nXPos				 =  pSrc->m_nXPos;
	m_nAlign				 =  pSrc->m_nAlign;
	m_dotBase			 =  pSrc->m_dotBase;
	m_nPath				 =  pSrc->m_nPath;
	m_dbDirection		 =  pSrc->m_dbDirection;
	m_dbRadius			 =  pSrc->m_dbRadius;
	m_bCCW				 =  pSrc->m_bCCW;
	m_bInside			 =  pSrc->m_bInside;
	m_dbStart			 =  pSrc->m_dbStart;
	m_nFont				 =  pSrc->m_nFont;
	m_strFontCHS		 =  pSrc->m_strFontCHS;
	m_strFontEN		 =  pSrc->m_strFontEN;
	m_strFontNUM		 =  pSrc->m_strFontNUM;
	m_lf					 =  pSrc->m_lf;
	m_strFontUser		 =  pSrc->m_strFontUser;
	m_dbFixSpace		 =  pSrc->m_dbFixSpace;
	m_dbCharSpace	 =  pSrc->m_dbCharSpace;

	m_bWeld				 =  pSrc->m_bWeld;
	m_bInterFilter		 =  pSrc->m_bInterFilter;
	m_dbInterSpace	 =  pSrc->m_dbInterSpace;
	
	m_dbWidthFactor	 =  pSrc->m_dbWidthFactor;
	m_dbHeight			 =  pSrc->m_dbHeight;
	m_dbLineSpace	 =  pSrc->m_dbLineSpace;
	m_strHzkType		 =  pSrc->m_strHzkType;
	m_strAscType		 =  pSrc->m_strAscType;
	
	m_bCustomFormat	 =  pSrc->m_bCustomFormat;
	m_strFormat			 =  pSrc->m_strFormat;
    m_strPrecisK			 =  pSrc->m_strPrecisK;
	m_bFixSpace			 =  pSrc->m_bFixSpace;
	
	m_quadChar			 =  pSrc->m_quadChar;
	m_bParity				 =  pSrc->m_bParity;
	m_bBarText				 =  pSrc->m_bBarText;
	m_nBarType			 =  pSrc->m_nBarType;
	m_nErrLev				 =  pSrc->m_nErrLev;
	m_nSize					 =  pSrc->m_nSize;
	m_nCodeType			 =  pSrc->m_nCodeType;

	//
	m_dbMatrixR		=   pSrc->m_dbMatrixR;
	m_dbMatrixFillD	=	pSrc->m_dbMatrixFillD;
}


//
void CNodeText::Rotate(CDot dot, double angle, int nPlane)
{
	m_quadChar.Rotate(dot,angle);
	m_quadRangle.Rotate(dot,angle);
	m_dotBase.Rotate(dot,nPlane,angle);

	if ( PATH_LINE == m_nPath ) 
	{
		m_dbDirection  +=  angle;
		m_dbDirection  = fmod(m_dbDirection, 360);
	}
	else
	{
		m_dbStart   +=  angle;
		m_dbStart  = fmod(m_dbStart, 360);
	}
	
	if ( m_pFnt ) 
	{
		m_pFnt->Rotate(dot, angle, 0);
		m_pFnt->m_dotBase.Rotate(dot, 0, angle);
		CalLimit();
	}
}


//
BOOL CNodeText::Move(double moveX, double moveY, double moveZ)
{	
	m_dotBase.Move(moveX,moveY,0);
	m_quadChar.Move(moveX,moveY);
	m_quadRangle.Move(moveX,moveY);
	
	if ( m_pFnt ) 
	{
		m_pFnt->Move(moveX,moveY,0);
		m_pFnt->m_dotBase.Move(moveX,moveY,0);
		CalLimit();
	}

	return TRUE;
}

////
// 点阵字体填充放大时可能存在问题，需要重新CreateFnt
void CNodeText::Scale(CDot dot, double ratioX, double ratioY)
{
	if ( PATH_ROUND == m_nPath )
	{
		if ( fabs(ratioX-ratioY) > EPSILON ) 
			return;
		
		double rt = fabs(ratioX);
		m_dbRadius  *=  rt;
		m_dbHeight  *=  rt;
		
		m_pFnt->m_dbRadius = m_dbRadius;
		m_quadRangle.Scale(dot,ratioX,ratioY);
	}
	else
	{	
		CQuadRangle q1 = m_quadChar;
		m_quadRangle.Scale(dot,ratioX,ratioY);
		m_quadChar.Scale(dot,ratioX,ratioY);
		CQuadRangle q2 = m_quadChar;

		double rx(1.0), ry(1.0);
		CalcRatio(q1, q2, rx, ry);
		
		m_dbHeight  *=  ry;
		m_bFixSpace ? (m_dbFixSpace  *=  rx) : (m_dbCharSpace  *=  rx);
		m_dbLineSpace  *=  ry;
		m_dbWidthFactor  *=  rx/ry;
		m_dbDirection = m_quadChar.CalRotateAngle();
	}

	m_dotBase.Scale(dot,ratioX,ratioY);
	
	if ( m_pFnt ) {
		m_pFnt->m_quadInit.Scale(dot,ratioX,ratioY);
		m_pFnt->Scale(dot,ratioX,ratioY);
		m_pFnt->m_dotBase.Scale(dot,ratioX,ratioY);

		CalLimit();
	}	
}

//
BOOL CNodeText::Shear(double dbBaseX, double dbBaseY, double angleX, double angleY)
{
	if ( PATH_ROUND == m_nPath )
		return FALSE;

	CQuadRangle q1 = m_quadChar;
	m_quadChar.Shear(dbBaseX, dbBaseY, angleX, angleY);
	m_quadRangle.Shear(dbBaseX,dbBaseY,angleX,angleY);
	CQuadRangle q2 = m_quadChar;

	double rx(1.0), ry(1.0);
	CalcRatio(q1, q2, rx, ry);

	m_dbHeight  *=  ry;	
	m_bFixSpace ? (m_dbFixSpace  *=  rx) : (m_dbCharSpace  *=  rx);
	m_dbLineSpace  *=  ry;
	m_dbWidthFactor  *=  rx/ry;
	m_dbDirection = m_quadChar.CalRotateAngle();
	m_dotBase.Shear(dbBaseX, dbBaseY, angleX, angleY);

	if ( m_pFnt ) 
	{
		m_pFnt->Shear(dbBaseX, dbBaseY, angleX, angleY);
		m_pFnt->m_dotBase.Shear(dbBaseX, dbBaseY, angleX, angleY);
		CalLimit();
	}

	return TRUE;
}

//
void CNodeText::Drag(CDot dot, double ratioX, double ratioY,double moveX,double moveY,double mvoeZ)
{	
	if (fabs(ratioX) < EPSILON) ratioX = 0.000000001;
	if (fabs(ratioY) < EPSILON) ratioY = 0.000000001;
	
	if ( PATH_ROUND == m_nPath )
	{	
		if ( fabs(ratioX-ratioY) > EPSILON ) 
			return;
	}

	if ( fabs(ratioX-1)  <=  EPSILON && fabs(ratioY-1)  <=  EPSILON )
	{
		Move(moveX,moveY,mvoeZ);
	}
	else
	{
		Scale(dot, ratioX, ratioY);
	}
}

//
void CNodeText::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{	
	CMySerialize fs(fp,bUnicode);
	CString str;
	
	if ( bFlag )
	{	
		CalLimit();
		CDot dot1,dot2;
		//写入头信息
		fs <<  _T("NTXT_B");
		fs <<  100;

		GetSerializeString(str);
		fs <<  str;					//文本属性ID
		fs <<  118;
		fs <<  m_nPath;			//路径方式
		fs <<  119;
		fs <<  m_dbDirection;	//直线方式下的方向
		fs <<  120;
		fs <<  m_dbRadius;		//半径
		fs <<  121;
		fs <<  m_bCCW;			//正逆时针方向
		fs <<  122;
		fs <<  m_dbStart;		//开始角度
		fs <<  123;
		fs <<  m_bInside;		//文字内侧显示
		fs <<  124;
		fs <<  m_nFont;			//文字类型shx或ttf
		fs <<  125;
		fs <<  m_dbCharSpace;	//字间距
		fs <<  126;
		fs <<  m_dbWidthFactor;	//宽度系数
		fs <<  127;
		fs <<  m_dbHeight;			//字高
		fs <<  128;
		fs <<  m_dbLineSpace;		//行间距
		fs <<  129;
		fs <<  m_strFontCHS;		//SHX字体的中文字体名
		fs <<  130;
		fs <<  m_strFontEN;
		fs <<  131;
		fs <<  m_strFontNUM;
		
		dot1 = m_quadLimit.GetBaseDot(BASE_TL);
		dot1.TrueToLog();
		fs <<  132;
		fs <<  dot1.x;				//左上点坐标
		fs <<  133;
		fs <<  dot1.y;
		dot1.LogToTrue();
		dot2 = m_quadLimit.GetBaseDot(BASE_BR);
		dot2.TrueToLog();
		fs <<  134;
		fs <<  dot2.x;				//右下点坐标
		fs <<  135;
		fs <<  dot2.y;
		fs <<  136;
		dot2.LogToTrue();
		fs <<  m_nAlign;			//对齐方式;
		
		fs <<  137;
		//写入文本信息到一行文本中
		CString strFontInfo;
		strFontInfo.Format(_T("%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s"),
			m_lf.lfHeight,
			m_lf.lfWeight,
			m_lf.lfEscapement,
			m_lf.lfOrientation,
			m_lf.lfWeight,
			m_lf.lfItalic,
			m_lf.lfUnderline,
			m_lf.lfStrikeOut,
			m_lf.lfCharSet,
			m_lf.lfOutPrecision,
			m_lf.lfClipPrecision,
			m_lf.lfQuality,
			m_lf.lfPitchAndFamily,
			m_lf.lfFaceName);
		fs <<  strFontInfo;
		//
		m_dotBase.TrueToLog();
		fs <<  140;
		fs <<  m_dotBase.x;
		fs <<  141;
		fs <<  m_dotBase.y;
		m_dotBase.LogToTrue();
		fs <<  142;
		fs <<  m_bFixSpace;
		fs <<  143;
		fs <<  m_bCustomFormat;
		fs <<  144;
		fs <<  m_strFormat;
		fs <<  145;
		fs <<  m_strPrecisK;
		fs <<  146;
		fs <<  m_strHzkType;
		fs <<  147;
		fs <<  m_strAscType;

		fs <<  148;  //是否为条码文本
		fs <<  m_bBarText;

		fs <<  149;  //条码文本是否启用校验
		fs <<  m_bParity;

		//
		fs <<  200;          
		fs <<  m_strFontUser;    

		fs <<  202;
		fs <<  m_dbFixSpace;

		fs <<  203;
		m_quadChar.Serialize(fp, bFlag,bUnicode);

		fs <<  204;
		fs <<  m_nBarType;

		//
		fs <<  300;
		fs <<  m_bWeld;
		fs <<  301;
		fs <<  m_bInterFilter;
		fs <<  302;
		fs <<  m_dbInterSpace;
		fs <<  303;
		fs << m_bPrintInverse;
		fs <<  304;
		fs << m_bBroaden;
		fs <<  305;
		fs << m_dbLineWidth;
		fs <<  306;
		fs << m_dbBroadenD;
		//点阵字体参数
		fs << 307;
		fs << m_dbMatrixR;
		fs << 308;
		fs << m_dbMatrixFillD;
		///////////
		m_property.Serialize(fp,bFlag,bUnicode);
		//写入尾信息
		fs <<  _T("NTXT_E");
	}
	else
	{
		CString strRead;
		while( fs >> str )
		{
			str.TrimRight();
			if ( str == _T("PRO_B") )
			{
				m_property.Serialize(fp,bFlag,bUnicode);
				continue;
			}
			switch(_ttoi(str))
			{
			case 100:
				{	
					ClearAllText();
					/////
					fs  >>  strRead;
					int nFind = strRead.Find(_T("|"));
					while( nFind  >=  0 )
					{
						int  nID = _ttoi((LPCTSTR)strRead);
						LoadTextByID(nID,g_arryIDMap);
						/////////
						strRead = strRead.Mid(nFind+1);
						nFind = strRead.Find(_T("|"));
					}
				}
				break;

			case 118:
				fs >> m_nPath;
				break;
			case 119:
				fs >> m_dbDirection;		//直线方式下的方向
				break;
			case 120:
				fs >> m_dbRadius;			//半径
				break;
			case 121:
				fs >> m_bCCW;				//正逆时针方向
				break;
			case 122:
				fs >> m_dbStart;			//开始角度
				break;
			case 123:
				fs >> m_bInside;			//文字内侧显示
				break;
			case 124:
				fs >> m_nFont;			//文字类型shx或ttf
				break;
			case 125:
				fs >> m_dbCharSpace;		//字间距
				break;
			case 126:
				fs >> m_dbWidthFactor;	//宽度系数
				break;
			case 127:
				fs >> m_dbHeight;			//字高
				break;
			case 128:
				fs >> m_dbLineSpace;		//行间距
				break;
			case 129:
				fs >> m_strFontCHS;		//SHX字体的中文字体名
				break;
			case 130:
				fs >> m_strFontEN;
				break;
			case 131:
				fs >> m_strFontNUM;
				break;				
			case 132:
				fs >> m_dotLT.x;
				break;
			case 133:
				fs >> m_dotLT.y;
				break;
			case 134:
				fs >> m_dotRB.x;
				break;
			case 135:
				fs >> m_dotRB.y;
				break;
			case 136:
				fs >> m_nAlign;
				break;
			case 137:
				{
					fs  >>  strRead;

					int nCount(0);
					m_lf.lfHeight = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);

					m_lf.lfWeight = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);
					
					m_lf.lfEscapement = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);

					m_lf.lfOrientation = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);
					m_lf.lfWeight = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);
					m_lf.lfItalic = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);
					m_lf.lfUnderline = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);
					m_lf.lfStrikeOut = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);
					m_lf.lfCharSet = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);
					m_lf.lfOutPrecision = _ttoi(strRead);
					nCount  = strRead.Find(_T("|"));
					strRead = strRead.Mid(nCount+1);
					m_lf.lfClipPrecision = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead = strRead.Mid(nCount+1);
					m_lf.lfQuality = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead = strRead.Mid(nCount+1);
					m_lf.lfPitchAndFamily = _ttoi(strRead);
					nCount = strRead.Find(_T("|"));
					strRead =  strRead.Mid(nCount+1);
					strRead.TrimRight(_T("\r\n"));
					_tcscpy_s(m_lf.lfFaceName,32,strRead.GetBuffer(strRead.GetLength()));
					strRead.ReleaseBuffer();
					break;
				}
			case 140:
				fs >> m_dotBase.x;
				break;
			case 141:
				fs >> m_dotBase.y;
				break;
			case 142:
				fs >> m_bFixSpace;
				break;
			case 143:
				fs >> m_bCustomFormat;
				break;
			case 144:
				fs >> m_strFormat;
				break;
            case 145:
				fs >> m_strPrecisK;
				break;
            case 146:
				fs >> m_strHzkType;
				break;
            case 147:
				fs >> m_strAscType;
				break;
			case 148:
				fs >> m_bBarText;
				break;
			case 149:
				fs >> m_bParity;
				break;
			case 200:              
				fs >> m_strFontUser;     //自定义字库
				break;
			case 202:
				fs >> m_dbFixSpace;
				break;
			case 203:
				m_quadChar.Serialize(fp, bFlag,bUnicode);
				break;
			case 204:
				fs >> m_nBarType;
				break;
			case 300:
				fs >> m_bWeld;
				break;
			case 301:
				fs >> m_bInterFilter;
				break;
			case 302:
				fs >> m_dbInterSpace;
				break;
			case 303:
				fs >> m_bPrintInverse;
				break;
			case 304:
				fs >> m_bBroaden;
				break;
			case 305:
				fs >> m_dbLineWidth;
				break;
			case 306:
				fs >> m_dbBroadenD;
				break;
			case 307:
				fs >> m_dbMatrixR;
				break;
			case 308:
				fs >> m_dbMatrixFillD;
				break;
			}
			//结束点退出
			if ( str == _T("NTXT_E") )
				break;
		}
		m_dotRB.LogToTrue();
		m_dotBase.LogToTrue();
		m_dotLT.LogToTrue();
		CreateString(FALSE);
		CalLimit();

		double moveX(0),moveY(0);
		switch( m_nAlign )
		{
		case ALIGN_LEFT:
			moveX = m_dotLT.x-m_quadLimit.left;
			break;
		case ALIGN_RIGHT:
			moveX = m_dotRB.x-m_quadLimit.right;
			break;
		case ALIGN_MID:
			moveX = (m_dotLT.x+m_dotRB.x)/2-(m_quadLimit.left+m_quadLimit.right)/2;
			break;
		}
		moveY = (m_dotLT.y+m_dotRB.y)/2-(m_quadLimit.top+m_quadLimit.bottom)/2;
		Move(moveX,moveY,0);
	}
}

void CNodeText::Scan(StrokeList *pList)
{
	if ( m_pFnt )
	{
		m_pFnt->Scan(pList);
	}
}

//
BOOL CNodeText::AdvanceSet( CRect &rect )
{
	UINT nTextID(0);
	CText* pText = NULL;
	
	CString strText;
	BOOL bBarText = FALSE;
	int nCount = GetTextCount();
	for( int i = 0; i<nCount; i++ )
	{
		pText  = (CText*)GetText(i);
		if ( pText == NULL )
			continue;

		if ( !bBarText && pText->m_bBarCode )//是条码所属文本
			bBarText = TRUE;

		strText  +=  pText->GetString();
	}
	
	if( bBarText )
	{	
		CSetBarTextDlg dlg;
		dlg.m_strInfo = strText;
		//字体设置部分
		dlg.m_PageFnt.m_nFont						 =  m_nFont;
		dlg.m_PageFnt.m_dbCharSpace			 =  m_dbCharSpace;
		dlg.m_PageFnt.m_dbWidthFactor			 =  m_dbWidthFactor*100;
		dlg.m_PageFnt.m_dbHeight					 =  m_dbHeight;
		dlg.m_PageFnt.m_dbLineSpace			 =  m_dbLineSpace;
		dlg.m_PageFnt.m_strFontCHS				 =  m_strFontCHS;
		dlg.m_PageFnt.m_strFontEN				 =  m_strFontEN;
		dlg.m_PageFnt.m_strFontNUM				 =  m_strFontNUM;
		dlg.m_PageFnt.m_strFontUser				 =  m_strFontUser;
		dlg.m_PageFnt.m_bFixSpace				 =  m_bFixSpace;
		dlg.m_PageFnt.m_dbFixSpace				 =  m_dbFixSpace;
		dlg.m_PageFnt.m_lf							 =  m_lf;
        dlg.m_PageFnt.m_strPrecisK				 =  m_strPrecisK;
		dlg.m_PageFnt.m_strHzkType				 =  m_strHzkType;
		dlg.m_PageFnt.m_strAscType				 =  m_strAscType;

		// add by zhq,增加字体自动焊接和过滤交点选项
		dlg.m_PageFnt.m_bWeld					 =  m_bWeld;
		dlg.m_PageFnt.m_bInterFilter				 =  m_bInterFilter;
		dlg.m_PageFnt.m_dbInterSpace			 =  m_dbInterSpace;

		// 加粗
		dlg.m_PageFnt.m_bBroaden				 =  m_bBroaden;
		dlg.m_PageFnt.m_dbLineWidth				 =  m_dbLineWidth;
		dlg.m_PageFnt.m_dbBroadenD				 =  m_dbBroadenD;

		//排布设置部分
		dlg.m_PageRound.m_nPath					 =  m_nPath;
		dlg.m_PageRound.m_dbRadius				 =  m_dbRadius;
		dlg.m_PageRound.m_dbStart				 =  m_dbStart;
		dlg.m_PageRound.m_bCCW					 =  m_bCCW;
		dlg.m_PageRound.m_bInside				 =  m_bInside;
		dlg.m_PageRound.m_nAlign				 =  m_nAlign;
		dlg.m_PageRound.m_dbDirection			 =  m_dbDirection;
		dlg.m_PageRound.m_bCustomFormat			 =  m_bCustomFormat;
		dlg.m_PageRound.m_strFormat				 =  m_strFormat;
		dlg.m_PageRound.m_bPrintInverse			 =  m_bPrintInverse;

		//点阵字体参数
		dlg.m_PageFnt.m_dbMatrixR				 =	m_dbMatrixR;
		dlg.m_PageFnt.m_dbMatrixFillD			 =	m_dbMatrixFillD;
		
		if ( dlg.DoModal() == IDCANCEL )
			return FALSE;

		//字体设置部分
		m_nFont					 =  dlg.m_PageFnt.m_nFont;
		m_dbCharSpace		 =  dlg.m_PageFnt.m_dbCharSpace;
		m_dbWidthFactor 	 =  dlg.m_PageFnt.m_dbWidthFactor/100;
		m_dbHeight				 =  dlg.m_PageFnt.m_dbHeight;
		m_dbLineSpace		 =  dlg.m_PageFnt.m_dbLineSpace;
		m_strFontCHS			 =  dlg.m_PageFnt.m_strFontCHS;
		m_strFontEN			 =  dlg.m_PageFnt.m_strFontEN;	
		m_strFontNUM			 =  dlg.m_PageFnt.m_strFontNUM;
		m_strFontUser			 =  dlg.m_PageFnt.m_strFontUser;    //自定义字库
		m_bFixSpace			 =  dlg.m_PageFnt.m_bFixSpace;
		m_dbFixSpace			 =  dlg.m_PageFnt.m_dbFixSpace;     //自定义字库
		m_lf						 =  dlg.m_PageFnt.m_lf;
        m_strPrecisK			 =  dlg.m_PageFnt.m_strPrecisK;
		m_strHzkType			 =  dlg.m_PageFnt.m_strHzkType;
		m_strAscType			 =  dlg.m_PageFnt.m_strAscType;

		// add by zhq,增加字体自动焊接和过滤交点选项
		m_bWeld					 =  dlg.m_PageFnt.m_bWeld ;
		m_bInterFilter			 =  dlg.m_PageFnt.m_bInterFilter;
		m_dbInterSpace			 = dlg.m_PageFnt.m_dbInterSpace ;

		// 加粗
		m_bBroaden				 = dlg.m_PageFnt.m_bBroaden;
		m_dbLineWidth			 = dlg.m_PageFnt.m_dbLineWidth;
		m_dbBroadenD			 = dlg.m_PageFnt.m_dbBroadenD;

		//排布设置部分
		m_nPath					 =  dlg.m_PageRound.m_nPath;
		m_dbRadius				 =  dlg.m_PageRound.m_dbRadius;
		m_dbStart				 =  dlg.m_PageRound.m_dbStart;
		m_bCCW					 =  dlg.m_PageRound.m_bCCW;
		m_bInside				 =  dlg.m_PageRound.m_bInside;
		m_nAlign				 =  dlg.m_PageRound.m_nAlign;
		m_dbDirection			 =  dlg.m_PageRound.m_dbDirection;
		m_bCustomFormat			 =  dlg.m_PageRound.m_bCustomFormat;
		m_strFormat				 =  dlg.m_PageRound.m_strFormat;
		m_bPrintInverse			 =  dlg.m_PageRound.m_bPrintInverse;

		//点阵字体参数
		m_dbMatrixR				 =	dlg.m_PageFnt.m_dbMatrixR;
		m_dbMatrixFillD			 =	dlg.m_PageFnt.m_dbMatrixFillD;
	}
	else
	{
		CTextBaseDlg dlg;
		dlg.m_pNode = this;
		dlg.DoModal();
	}

	CreateString(FALSE);

	return TRUE;
}



BOOL CNodeText::Create(CRect &rect)
{
	FlashText(0x00);
	CreateString(FALSE);
	
	return TRUE;
}



UINT CNodeText::Mark(HWND hWnd)
{
	if ( m_pFnt )
		return m_pFnt->Mark(hWnd);
	else
		return HLOK;
}


UINT CNodeText::SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsRevMark)
{
	if ( m_pFnt )
		return m_pFnt->SelMark(hWnd,  hSelStop, nLayer, bIsRevMark);
	else
		return HLOK;
}


void CNodeText::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	if ( m_pFnt )
		m_pFnt->ExportPlt(file,bUnExportFill,bScall100);	
}

BOOL CNodeText::CreateString(BOOL bNext)
{
	FlashText( bNext ? 0x01 : 0x00);

	if ( TRUE == m_bBarText )
	{
		GenBarString();
		FormatString(m_strBarString);
	}
	else
	{
		FormatString(m_strText);
	}
	
	CreateFnt();
 	CalLimit();

	return TRUE;
}

// 点阵字体处理
BOOL CNodeText::ProcessMatrixFont(StrokeList& listStroke)
{
	if( m_dbMatrixR < 0.01 )
		return TRUE;

	CStroke* pStroke = NULL;
	CStroke* pNew = NULL;
	StrokeList listTmp;
	
	list<CStroke*> listRet;
	list<CStroke*>::const_iterator iter;

	CGArc arcObj;
	arcObj.m_dbPrecision = g_SysCfg.m_dbDotmatrixCirPrec;
	arcObj.m_nSampleCount = 10;
	CDot  dot(0,0),dot2(0,0);
	POSITION pos = listStroke.GetHeadPosition();
	while( NULL != pos )
	{
		pStroke = (CStroke*)listStroke.GetNext(pos);
		if( pStroke )
		{
			pNew = new CStroke(m_property);
			dot = pStroke->GetAt(0);
			dot2.x = dot.x + m_dbMatrixR;
			dot2.y = dot.y;
			arcObj.CreateArcline(dot, dot2, dot2, pNew, 2);
			if( m_dbMatrixFillD > EPSILON )
				FillStroke(&listRet,pNew);
			listRet.push_back(pNew);

			if ( listRet.size() ) 
			{
				for( iter = listRet.begin(); iter  !=  listRet.end(); ++iter ) 
				{
					listTmp.AddTail(*iter);
				}
				listRet.clear();
			}
		}
	}

	pos = listStroke.GetHeadPosition();
	while( NULL != pos )
	{
		pStroke = (CStroke*)listStroke.GetNext(pos);
		if( pStroke )
		{
			delete pStroke;
			pStroke = NULL;
		}
	}
	listStroke.RemoveAll();

	pos = listTmp.GetHeadPosition();
	while( NULL != pos )
	{
		pStroke = (CStroke*)listTmp.GetNext(pos);
		if( pStroke )
			listStroke.AddTail(pStroke);
	}

	return TRUE;
}

void CNodeText::CreateFnt()
{
	CFnt *p = NULL;
	if ( m_nFont == FONT_SHX )
	{
		CFntShx *tmp = new CFntShx();	
		if ( tmp == NULL )
			return;

		tmp->m_strChineseFont	 =  m_strFontCHS;
		tmp->m_strEnglishFont	 =  m_strFontEN;
		tmp->m_strNumberFont = m_strFontNUM;
		p = tmp;
	}
	else if ( m_nFont == FONT_TTF )
	{
		CFntTTF *tmp = new CFntTTF();
		if ( tmp == NULL )
			return;

		tmp->m_lf = m_lf;
		p = tmp;
	}
	else if ( m_nFont == FONT_USER )
	{
		CFontUser *tmp = new CFontUser();
		if ( tmp == NULL )
			return;

		tmp->m_strFileName = g_sys.GetInstallPath() + _T("Font\\") + m_strFontUser + _T(".font");
		p = tmp;
	}
	else if ( m_nFont == FONT_MATRIX )
	{
		CFntHzk *tmp = new CFntHzk();
		if ( tmp == NULL )
			return;

		tmp->m_strHzkType = m_strHzkType;
		tmp->m_strAscType = m_strAscType;
		p = tmp;
	}
	else
	{
		return;
	}
	
	if ( m_pFnt  !=  NULL )
		delete m_pFnt;

	m_pFnt = p;
	
	CDot dot(0,0);
	double dbRotate(0.0), dbShearA(0.0), hr(1.0);
	BOOL bMirror( FALSE );
	if ( PATH_LINE == m_nPath && m_quadChar.GetHeight() )
	{
		bMirror = m_quadChar.IsMirror();
		if ( bMirror  )
		{
			m_quadChar.Scale(CDot(0,0), -1, 1);
			m_dotBase.Scale(CDot(0,0), -1, 1);
		}
		dbRotate = m_quadChar.CalRotateAngle();
		dbShearA = m_quadChar.CalShearAngle();

		dot = m_quadChar.GetBaseDot(BASE_BL);
		m_dotBase.Rotate(dot,-dbRotate);
		m_dotBase.Shear(dot.x, dot.y, -dbShearA, 0);
		
		double h1(1.0), h2(1.0);
		h1 = m_quadChar.GetHeight();
		m_quadChar.Rotate(dot, -dbRotate);
		m_quadChar.Shear(dot.x, dot.y, -dbShearA, 0);
		h2 = m_quadChar.GetHeight();
		hr = h2 / h1;
	}
	m_dbHeight  *=  hr;
	m_dbLineSpace  *=  hr;
	m_dbWidthFactor  /=  hr;
	
	p->SetProperty(PTY_INIT,m_property);
	
	if (TRUE == m_bBarText)
		p->m_strText		= m_strBarString;
	else
		p->m_strText		 =  m_strText;

	p->m_nPath			=  m_nPath;
	p->m_nAlign			=  m_nAlign;
	p->m_dotBase		=  m_dotBase;
	
	//圆弧方式
	p->m_dbRadius		 =  m_dbRadius;
	p->m_dbStart			 =  m_dbStart;
	p->m_bCCW			 =  m_bCCW;
	p->m_bInside			 =  m_bInside;
	p->m_bPrintInverse	= m_bPrintInverse;
	
	//字形
	p->m_dbCharSpace	 =  m_dbCharSpace;
	p->m_dbHeight		 =  m_dbHeight;
	p->m_dbLineSpace	 =  m_dbLineSpace;
	p->m_dbWidthFactor =  m_dbWidthFactor;
	
	p->m_bFixSpace		 =  m_bFixSpace;
	p->m_dbFixSpace		 =  m_dbFixSpace;
    p->m_strPrecisK		 =  m_strPrecisK;
	p->m_bIsCurSortTree	 =  m_bIsCurSortTree;
	
	p->CreateStrokeList();

	//点阵字体处理
	if ( m_nFont == FONT_MATRIX && m_dbMatrixR > 0.01 )
	{
		p->m_nDrawType = FONT_TTF; // 绘图是不再是绘点阵
		ProcessMatrixFont(p->m_list);
	}
	
	//  焊接造型
	if ( m_bWeld )
	{
		CArray<StrokeList*, StrokeList*> arrayList;
		CWeldEditPro weldEditPro;
		StrokeList listRet, listSourc;
		CStroke *pStroke = NULL;
		CStrokeB *pStrokeB = NULL;

		POSITION pos = NULL;
		pos = p->m_list.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pStroke = (CStroke *)(p->m_list.GetNext(pos));
			if ( pStroke  !=  NULL )
			{
				pStrokeB = new CStrokeB( pStroke->m_property );
				pStrokeB->Append( *((CStrokeB *)pStroke) );
				listSourc.AddTail( pStrokeB );

				delete pStroke;
				pStroke = NULL;
				pStrokeB = NULL;
			}
		}
		p->m_list.RemoveAll();

		//
		arrayList.Add( &listSourc );
		weldEditPro.Weld( arrayList, listRet );//arrayList内部清除原始数据
		//
		pos = listRet.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pStrokeB = listRet.GetNext( pos );
			if ( pStrokeB  !=  NULL )
			{
				pStroke = new CStroke( pStrokeB->m_property );				
				pStroke->CopyStrokeB( pStrokeB );
				p->m_list.AddTail( pStroke );

				delete pStrokeB;
				pStrokeB = NULL;
			}
		}
	}

	//  过滤交点
	if ( m_bInterFilter )
	{
		StrokeList listRet, listSourc;
		CStroke  *pStroke = NULL;
		CStrokeB *pStrokeB = NULL;

		POSITION pos = NULL;
		pos = p->m_list.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pStroke = (CStroke *)(p->m_list.GetNext(pos));
			if ( pStroke  !=  NULL )
			{
				pStrokeB = new CStrokeB( pStroke->m_property );
				pStrokeB->Append( *((CStrokeB *)pStroke) );
				listSourc.AddTail( pStrokeB );

				delete pStroke;
				pStroke = NULL;
				pStrokeB = NULL;
			}
		}
		p->m_list.RemoveAll();

		CDotInterFilter dotInterFilter;
		dotInterFilter.SetInterSpace(m_dbInterSpace);
		dotInterFilter.ScanFilterInterDot((void *)(&listSourc));

		pos = listSourc.GetHeadPosition();
		while( pos  !=  NULL )
		{
			pStrokeB = listSourc.GetNext( pos );
			if ( pStrokeB  !=  NULL )
			{
				pStroke = new CStroke( pStrokeB->m_property );				
				pStroke->CopyStrokeB( pStrokeB );
				p->m_list.AddTail( pStroke );

				delete pStrokeB;
				pStrokeB = NULL;
			}
		}
	}

	if( m_bBroaden )
	{
		StrokeList listBroaden;
		CBroadenStroke broadenStroke(m_dbLineWidth/2,m_dbBroadenD,1);
		broadenStroke.SetCirclePara(::g_SysCfg.m_dbPrecision,::g_SysCfg.m_nSampleCount);
		broadenStroke.BroadenStrokeList(&(p->m_list),&listBroaden);
	
		// 清除中m_list
		p->Delete();

		// 拷出库中处理的线条数据
		CStrokeB *pStrokeB = NULL;
		CStroke	 *pStroke = NULL;
		POSITION pos = listBroaden.GetHeadPosition();
		while( pos != NULL )
		{
			pStrokeB = (CStrokeB*)listBroaden.GetNext(pos);	
			if ( pStrokeB == NULL )
				continue;

			pStroke = new CStroke;
			if( pStroke == NULL )
				continue;
			(*pStroke) = (*pStrokeB);
			p->m_list.AddTail(pStroke);
		}
		// 清除库中分配的线条数据内存
		broadenStroke.FreeStrokeList(&listBroaden);
	}

	InitQuadRange();
	if ( PATH_LINE == m_nPath ) 
	{
		dot = m_quadChar.GetBaseDot(BASE_BL);
		Shear(dot.x, dot.y, dbShearA, 0);
		Rotate(dot,dbRotate,0);

		if ( bMirror )
		{
			Scale(CDot(0,0), -1, 1);
		}
	}
}

//注意：条码ID存在一对多情况，为了保障跳号的一致性，
//文本预处理CChain::PreProcessMark()已经对跳号进行了处理，
//此处nStyle应该使用0x00,nStyle保留是维持虚函数的一致性。
BOOL CNodeText::PreprocessMark(DWORD nStyle)
{
	BOOL bFlag = FALSE;
	CString strText(_T(""));
	BOOL bOverMark = FALSE;
	GetTextString(0x00,bOverMark,strText);

	if ( strText == m_strText )
	{
		if ( !bOverMark )
			bFlag = TRUE;
	}
	else
	{
		bFlag = TRUE;
		m_strText = strText;

		if ( TRUE == m_bBarText )
		{
			GenBarString();
			FormatString(m_strBarString);
		}
		else
		{
			FormatString(m_strText);
		}

		CreateFnt();
		CalLimit();
	}

	return bFlag;
}



void CNodeText::SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{
	m_property.m_color  = color;
	m_property.m_nLayer  = nLayer;
	
	CreateFnt();
	CalLimit();
}


void CNodeText::FormatString(CString &strText)
{
	if ( !m_bCustomFormat )
		return;
	
	//格式化文本输出
	CString strNext;
	
	TCHAR tmp(0);
	int    pos = 0;
	int    nSize = strText.GetLength();
	int    nCount = m_strFormat.GetLength();
	
	strNext.Empty();
	
	for( int i = 0; i<nCount; i++)
	{
		tmp = m_strFormat.GetAt(i);
		if ( tmp == _T('X') )
		{
			if ( nSize > pos ){
				tmp = strText.GetAt(pos);
			}
			else{
				//超出信息量的范围则不用进行相应处理直接退出
				break;
			}
			pos++;
		}
		else if ( tmp == _T('N') )
		{
			pos++;
			continue;
		}
		strNext  +=  tmp;
	}

	strText = strNext;
}

void CNodeText::CreateFromBar(UINT nText)
{
	CText *pText = (CText*)GetTextByID(nText);
	AddText(pText);
}

void CNodeText::CreateFromBar(CUIntArray& ArrayTextID)
{
	for ( int i = 0; i < ArrayTextID.GetSize(); ++i )
		CreateFromBar(ArrayTextID.GetAt(i));
}

void CNodeText::SetStrokeList(StrokeList *pStrokeList)
{
	POSITION pos = m_pFnt->m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_pFnt->m_list.GetNext(pos);
		if ( pStroke  !=  NULL )
			pStrokeList->AddTail(pStroke);
	}
}

CNode * CNodeText::CopyStrokeOnly()
{
	CNode *pNodeTmp = new CNodeV();
	if ( pNodeTmp == NULL )
		return NULL;

	pNodeTmp->m_property =  m_property;
	pNodeTmp->m_bSelect = m_bSelect;

	CStrokeB *pStroke = NULL,*pNew = NULL;
	POSITION pos = m_pFnt->m_list.GetHeadPosition();
	while( pos )
	{
		pStroke = m_pFnt->m_list.GetNext(pos);
		if ( pStroke  && ( pNew = pStroke->Copy() ) )
			pNodeTmp->m_list.AddTail(pNew);
	}

	return pNodeTmp;
}


void CNodeText::AddToPlt(CNode *pNodePlt)
{
	if ( NULL == pNodePlt ) 
		return;

	POSITION pos = m_pFnt->m_list.GetHeadPosition();
	CStrokeB *pStroke = NULL,*pStrokeNew = NULL;
	while( pos )
	{
		pStroke = (CStroke *)m_pFnt->m_list.GetNext(pos);
		if ( !pStroke )
			continue;

		pStrokeNew = pStroke->Copy();
		if ( pStrokeNew )
			pNodePlt->GetList().AddTail(pStrokeNew);
	}

	pNodePlt->m_property = m_property;
}



BOOL CNodeText::GetNodeFirstDot(CDot &dt)
{
	int nCount = m_pFnt->m_list.GetCount();
	if ( nCount  <=  0 )
		return FALSE;

	CStroke *pStroke = NULL;
	POSITION pos = m_pFnt->m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pStroke = (CStroke *)m_list.GetNext(pos);
		if ( pStroke  !=  NULL)
		{
			if ( pStroke->m_dotArray.GetSize() > 0 )
			{
				dt = pStroke->m_dotArray.GetAt(0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CNodeText::GetNodeLastDot(CDot &dt)
{
	if ( m_pFnt == NULL || m_pFnt->m_list.GetCount() )
		return FALSE;

	POSITION pos = m_pFnt->m_list.GetTailPosition();
	if (pos  !=  NULL)
	{
		CStroke *pStroke = (CStroke*)m_pFnt->m_list.GetNext(pos);
		if (pStroke  !=  NULL)
		{
			int nDCount = pStroke->m_dotArray.GetSize();
			if (nDCount > 0)
			{
				dt = pStroke->m_dotArray.GetAt(nDCount - 1);
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CNodeText::DrawExtern(XDC *pDC)
{
	Draw(pDC);
}

void CNodeText::CalcRatio(CQuadRangle q1, CQuadRangle q2, double& rx, double& ry)
{
	CDot dot(0,0);
	double dbRotate(0.0);

	dbRotate = q1.CalRotateAngle();
	dot = q1.GetBaseDot(BASE_BL);
	q1.Rotate(dot,-dbRotate);

	dbRotate = q2.CalRotateAngle();
	dot = q2.GetBaseDot(BASE_BL);
	q2.Rotate(dot,-dbRotate);

	double h2 = q2.GetHeight();
	double h1 = q1.GetHeight();
	double w2 = q2.GetWidth();
	double w1 = q1.GetWidth();

	rx = q2.GetWidth() / q1.GetWidth();
	ry = q2.GetHeight() / q1.GetHeight();
}

//
void CNodeText::LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo)
{	
	if ( !m_pFnt || ( (bOptimize == TRUE) && (IsFixSerial() == FALSE) ) )
		return;
	
	m_pFnt->LoadStrokeList(pStrokeList, bOptimize, nLayNo);
	m_bPathOptimize = m_pFnt->m_bPathOptimize;
}



void CNodeText::LoadWheelList(int nType, StrokeList *pListStroke)
{	
	if ( m_pFnt && m_pFnt->m_list.GetCount() > 0 )
	{
		CStrokeB *pStroke = NULL;
		POSITION pos = m_pFnt->m_list.GetHeadPosition();
		while( pos != NULL )
		{	
			pStroke = m_pFnt->m_list.GetNext(pos);
			
			CStrokeB *pNew = new CStrokeB(pStroke->m_property);
			if ( NULL == pNew )
				continue;
			
			pNew->Append(*pStroke);	
			pListStroke->AddTail(pNew);
		}
	}
}


void CNodeText::RefreshBarText(CWnd* pWnd,int nTextID)
{
	if ( IsExistText(nTextID) )//条码对应的ID在本文本中存在
	{
		CQuad quad = CalLimit();
		quad.Inflate(50,5);

		CZoomView *pView = (CZoomView*)pWnd;
		pView->PhysicToLogic(quad);
		pView->InvalidateRect(&(RECT)quad);
	}
}


//
BOOL CNodeText::GetLockFlg()
{
	CHLDoc *pDoc = (CHLDoc*)GetCurActiveDocument();
	if ( pDoc == NULL )
		return FALSE;
	
	int nLayer     = 0;
    int nFillLayer = 0;
	GetLayColor(nLayer, nFillLayer);
		
	return (pDoc->GetLockFlg(nLayer) && IsFixSerial());
}

void CNodeText::SetPathOptimizeState(BOOL bState)
{	
	if ( m_pFnt  !=  NULL )
	{
		m_pFnt->SetPathOptimizeState(bState);
	}
}

//
BOOL CNodeText::GenBarString()
{
	BOOL bRtn = TRUE;
	CString strText = m_strText;

	switch( m_nBarType ) 
	{
	case EAN8:
	case EAN13:
	case EAN128:
	case CODE128A:
	case CODE128B:
	case CODE128C:
	case CODE128AUTO:
	case CODE39:
	case CODE93:
	case CODE93EX:
	case CODE25:
	case INT25:
	case EXT39:
	case UPCA:
	case UPCE:
		{
			CNodeCodebar *pNode = new CNodeCodebar();
			if ( pNode == NULL )
				return FALSE;

			pNode->m_BarType = (BARTYPE)m_nBarType;
			pNode->m_bParity = m_bParity;
			bRtn = g_BarCodeMgr.GenBarCode(pNode, strText,pNode->m_nCodePage);

			delete pNode;
		}
		break;
	}

	m_strBarString = strText;	

	return bRtn;
}


void CNodeText::CaseNum(CString &str)
{
	int nCount = str.GetLength();
	CString tmp = str;
	
	TCHAR ch;
	str.Empty();
	for(int i = 0;i<nCount;i++)
	{
		ch = tmp.GetAt(i);
		if ( ( ch > 0X29 ) && ( ch < 0x40 ))
		{
			str  +=  ch;
		}
	}
}

CQuad CNodeText::GetLimit(BOOL bRef)
{
	if ( bRef && m_pFnt )
	{
		return m_pFnt->CalLimit(&(m_pFnt->m_list));
	}

	return m_quadLimit;
}

// 根据字体及圆弧排布的起始和终止角度计算预览框
BOOL CNodeText::GetFrameDots(CArray<CDot,CDot>& frameDots)
{
	if( NULL == m_pFnt )
		return FALSE;

	frameDots.RemoveAll();
	if( m_nPath != PATH_ROUND )
	{
		CQuadRangle qd = GetQuadRangle();
		frameDots.Add(qd.GetBaseDot(BASE_BL));
		frameDots.Add(qd.GetBaseDot(BASE_TL));
		frameDots.Add(qd.GetBaseDot(BASE_TR));
		frameDots.Add(qd.GetBaseDot(BASE_BR));
		return TRUE;
	}
	else
	{
		CDot dot1,dot2;
		dot1.x = m_pFnt->m_dotBase.x+m_dbRadius*cos(m_pFnt->m_dbStart*PAI/180);
		dot1.y = m_pFnt->m_dotBase.y+m_dbRadius*sin(m_pFnt->m_dbStart*PAI/180);
		dot2.x = m_pFnt->m_dotBase.x+m_dbRadius*cos(m_pFnt->m_dbEndAngle*PAI/180);
		dot2.y = m_pFnt->m_dotBase.y+m_dbRadius*sin(m_pFnt->m_dbEndAngle*PAI/180);

		CGArc  arcCreator;
		arcCreator.m_dbPrecision = 0.1;
		arcCreator.m_nSampleCount = 30;

		CStrokeB *pStroke = new CStrokeB();
		arcCreator.CreateArcline(m_pFnt->m_dotBase,dot2,dot1,pStroke,2);
		if( pStroke->GetSize() <= 2 )
		{
			frameDots.Add(dot2);
			frameDots.Add(dot1);
		}
		else
		{
			for(int i = 0; i<pStroke->GetSize(); i++)
				frameDots.Add(pStroke->GetAt(i));
		}

		pStroke->RemoveAllPoint();
		dot1.x = m_pFnt->m_dotBase.x+m_pFnt->m_dbRadius2*cos(m_pFnt->m_dbStart*PAI/180);
		dot1.y = m_pFnt->m_dotBase.y+m_pFnt->m_dbRadius2*sin(m_pFnt->m_dbStart*PAI/180);
		dot2.x = m_pFnt->m_dotBase.x+m_pFnt->m_dbRadius2*cos(m_pFnt->m_dbEndAngle*PAI/180);
		dot2.y = m_pFnt->m_dotBase.y+m_pFnt->m_dbRadius2*sin(m_pFnt->m_dbEndAngle*PAI/180);

		arcCreator.CreateArcline(m_pFnt->m_dotBase,dot2,dot1,pStroke,2);
		if( pStroke->GetSize() <= 2 )
		{
			frameDots.Add(dot1);
			frameDots.Add(dot2);
		}
		else
		{
			for(int i = pStroke->GetSize()-1; i>=0; i--)
				frameDots.Add(pStroke->GetAt(i));
		}

		delete pStroke;
		pStroke = NULL;
		return TRUE;
	}
}

void CNodeText::FillStroke(list<CStroke*> * pList,CStroke *pStroke)
{
	StrokeList listFill,listData;
	listData.AddTail(pStroke);

	if ( m_dbMatrixFillD < EPSILON )
		return;

	CStroke   *pStrokeList = NULL;
	CStrokeB *pStrokeB = NULL;

	BaseHatch  hatch;
	hatch.dbSpace = m_dbMatrixFillD;
	hatch.bEnable  = TRUE;
	hatch.dbAngle  = 0;
	//hatch.dbBeamComp = m_dbshrink*0.7;
	hatch.bEquaDist = TRUE;
	hatch.nFillStyle = 2;		//单向/往复填充
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