// NodeDib.cpp: implementation of the CNodeDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeDib.h"
#include "baseMacro.h"
#include "DlgFileReplyRename.h"
#include "Error.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern void GenImportFullName(CString&);
extern CString g_OpenFileName;
extern CString g_OldFileName;
extern BOOL g_bClipBoardCopyNode;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
CNodeDib::CNodeDib()
{
	m_type = NODE_BMP;
	m_dbMoveX = m_dbMoveY = 0;
	m_bOppsitionCol = 0;//正常
	m_dbSpaceX = 0.05;
	m_dbSpaceY = 0.05;
	m_nWidth = m_nHeight = 0;
	m_strBmpName.Empty();
	m_pImageSrc = NULL;
	m_dbDotTime = 1000;
	m_dbAngle = 0.0;
	m_iDispWidth = m_iDispHeight = 0;
	m_nDibType = 1;
	m_bXFlip = FALSE;
	m_bYFlip = FALSE;
	m_bLoop = FALSE;
	m_dbLoopCompate = 0.0;
	m_nAccTime = 0;
	m_dbBrightness = 0.;
	m_dbContrast = 0.0;
	m_pDC = NULL;
	m_dbSpaceXR = 0.05;
	m_dbSpaceYR = 0.05;

	m_nMinFDL   = 50;
	m_nMaxFDL   = 815;
}

CNodeDib::CNodeDib(CString filename)
{
	m_type = NODE_BMP;
	m_dbMoveX = m_dbMoveY = 0;
	m_bOppsitionCol = 0;//正常
	m_strBmpName = filename;
	m_dbSpaceX = 0.05;
	m_dbSpaceY = 0.05;
	m_nWidth = m_nHeight = 0;
	m_pImageSrc = NULL;
	m_dbDotTime = 1000;
	m_dbAngle = 0.0;
	m_iDispWidth = m_iDispHeight = 0;
	m_nDibType = 1;
	m_bXFlip = FALSE;
	m_bYFlip = FALSE;
	m_bLoop = FALSE;
	m_dbLoopCompate = 0.0;
	m_nAccTime = 0;
	m_dbBrightness = 0.;
	m_dbContrast = 0.0;
	m_pDC = NULL;
	m_dbSpaceXR = 0.05;
	m_dbSpaceYR = 0.05;
	//
	m_nMinFDL   = 50;
	m_nMaxFDL   = 815;
}
                
CNodeDib::~CNodeDib()
{
	Delete();
}                                                         

void CNodeDib::Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode)
{
	//本标准采用文本方式进行数据录入
	CMySerialize fs(fp,bUnicode);
	CString str;

	CHLView *pView = (CHLView*)GetCurActiveView();
	if ( pView )
	{
		m_pDC = &(pView->m_DC);
	}

	if ( bFlag )
	{
		//写入头信息
		UpdateName(); //执行图片Copy等操作
		fs <<  _T("NDIB_B");
		fs << 100;
		fs << m_strBmpName;
		fs << 101;
		fs << m_dbMoveX;
		fs << 102;
		fs << m_dbMoveY;
		fs << 103;
		fs << m_dbSpaceX;
		fs << 105;
		fs << m_nHeight;
		fs << 106;
		fs << m_nWidth;
		fs << 107;
		fs << m_dbHeight;
		fs << 108;
		fs << m_dbWidth;
		fs << 113;
		fs << m_dbDotTime;
		fs << 114;
		fs << m_bLoop;
		fs << 115;
		fs << m_dbAngle;
		fs << 116;
		fs << m_nDibType;
		fs << 117;
		fs << m_bXFlip;
		fs << 118;
		fs << m_bYFlip;
		fs << 119;
		fs << m_dbSpaceY;
		fs << 120;
		fs << m_dbLoopCompate;
		fs << 121;
		fs << m_nAccTime;
		fs << 122;
		fs << m_dbBrightness;
		fs << 123;
		fs << m_dbContrast;
		// 增加二值打标关延时
		fs << 124;
		fs << m_nMinFDL;
		fs << 125;
		fs << m_nMaxFDL;
		//打标属性
		//////////////////////
		fs << 109;
		fs << m_bOppsitionCol;//下一个节点从112开始
		
		m_property.Serialize(fp,bFlag,bUnicode);
		m_quadRangle.Serialize(fp,bFlag,bUnicode);
		
		//写入尾信息
		fs << _T("NDIB_E");
	}
	else
	{
		while( fs >> str )
		{
			if ( str == _T("PRO_B") )
			{
				m_property.Serialize(fp,bFlag,bUnicode);
				continue;
			}

			if ( str == _T("QUAD_RANG_B") ){
				m_quadRangle.Serialize(fp,bFlag,bUnicode);
				continue;
			}
			
			switch( _ttoi(str) )
			{
			case 100:
				fs >> m_strBmpName;
				break;
			case 101:
				fs >> m_dbMoveX;
				break;
			case 102:
				fs >> m_dbMoveY;
				break;
			case 103:
				fs >> m_dbSpaceX;
				break;
			case 105:
				fs >> m_nHeight;
				break;
			case 106:
				fs >> m_nWidth;
				break;
			case 107:
				fs >> m_dbHeight;
				break;
			case 108:
				fs >> m_dbWidth;
				break;
			case 109:
				fs >> m_bOppsitionCol;//下一个节点从112开始
				break;
			case 113:
				fs >> m_dbDotTime;
				break;
			case 114:
				fs >> m_bLoop;
				break;
				//add by henry_ye
			case 115:
				fs >> m_dbAngle;
				break;
			case 116:
				fs >> m_nDibType;
				break;
			case 117:
				fs >> m_bXFlip;
				break;
			case 118:
				fs >> m_bYFlip;
				break;
			case 119:
				fs >> m_dbSpaceY;
				break;
			case 120:
				fs >> m_dbLoopCompate;
				break;
			case 121:
				fs >> m_nAccTime;
				break;
			case 122:
				fs >> m_dbBrightness;
				break;
			case 123:
				fs >> m_dbContrast;
				break;
			case 124:
				fs >> m_nMinFDL;
				break;
			case 125:
				fs >> m_nMaxFDL;
				break;
			}
			//结束点退出
			if ( str == _T("NDIB_E") )
				break;
		}
		m_dbSpaceXR =  m_dbWidth/(m_nWidth-1);
		m_dbSpaceYR =  m_dbHeight/(m_nHeight-1);
		DataProc();
	}
}

BOOL CNodeDib::CanGroup()
{
	//位图不允许组合
	return FALSE;
}

void CNodeDib::ProcMinusRatioX()
{
	int angle = Pai2Deg(m_dbAngle);
	angle %= 360;
	while (angle < 0 )
	{
		angle += 360;
	}
	while (angle >= 360)
	{
		angle -= 360;
	}
	if ((angle >= 315||angle <= 45)||(angle >= 135&&angle <= 225))
	{
		m_bXFlip = !m_bXFlip;	
	}
	else if ((angle >= 45&&angle <= 135)||(angle <= 315&&angle >= 225))
	{
		m_bYFlip = !m_bYFlip;
	}
}

void CNodeDib::ProcMinusRatioY()
{
	int angle = Pai2Deg(m_dbAngle);
	angle %= 360;
	while ( angle < 0 )
	{
		angle += 360;
	}
	while (angle >= 360)
	{
		angle -= 360;
	}

	if ((angle >= 315||angle <= 45)||(angle >= 135&&angle <= 225))
	{
		m_bYFlip = !m_bYFlip;	
	}
	else if ((angle >= 45&&angle <= 135)||(angle <= 315&&angle >= 225))
	{
		m_bXFlip = !m_bXFlip;
	}
}

void CNodeDib::Scale(CDot dot,double ratioX,double ratioY)
{
	int   iBckHeight =  m_nHeight;
	int   iBckWidth = m_nWidth;
	double dbBckAngle = m_dbAngle;
	double dbMovex  = m_dbMoveX;
	double dbMovey  = m_dbMoveY;
	
	if ((fabs(ratioX-1.0)>0.00001) || (fabs(ratioY-1.0)>0.00001))
	{
		//proc angle		
		if ( ratioX < 0.0 )
		{
			ProcMinusRatioX();
		}
		if (ratioY<0.0)
		{
			ProcMinusRatioY();
		}

		//proc center& weigth
		double tx,ty, dbRatioX = 1, dbRatioY = 1;
		
		if (fabs(ratioX - 1) > 0.001 && fabs(ratioY - 1) > 0.001)
		{
			dbRatioX = dbRatioY = (fabs(ratioX) - fabs(ratioY) > 0) ? ratioX : ratioY;
		}
		else if (fabs(ratioX - 1) > 0.001)
		{
			if ( fabs(m_dbAngle) < EPSILON )
			{
				dbRatioX = ratioX;
				dbRatioY = 1;
			}
			else if ( fabs(m_dbAngle - PAI/2) < EPSILON )
			{
				dbRatioX = 1;
				dbRatioY = ratioX;
			}
			else if ( fabs(m_dbAngle - PAI) < EPSILON )
			{
				dbRatioX = ratioX;
				dbRatioY = 1;
			}
			else if ( fabs(m_dbAngle - 3*PAI/2) < EPSILON )
			{
				dbRatioX = 1;
				dbRatioY = ratioX;
			}
			else
			{
				dbRatioX = ratioX ;
				dbRatioY = ratioX ;
			}
		}
		else if (fabs(ratioY - 1) > 0.001)
		{
			if ( fabs(m_dbAngle) < EPSILON )
			{
				dbRatioX = 1;
				dbRatioY = ratioY;
			}
			else if ( fabs(m_dbAngle - PAI/2) < EPSILON )
			{
				dbRatioX = ratioY;
				dbRatioY = 1;
			}
			else if ( fabs(m_dbAngle - PAI) < EPSILON )
			{
				dbRatioX = 1;
				dbRatioY = ratioY;
			}
			else if ( fabs(m_dbAngle - 3*PAI/2) < EPSILON )
			{
				dbRatioX = ratioY;
				dbRatioY = 1;
			}
			else
			{
				dbRatioX = ratioY ;
				dbRatioY = ratioY ;
			}
		}
				
		tx = m_dbSpaceXR*(m_nWidth-1)*dbRatioX;
		ty = m_dbSpaceYR*(m_nHeight-1)*dbRatioY;
				
		m_nWidth = int(tx/m_dbSpaceX+0.5)+1;
		m_nHeight = int(ty/m_dbSpaceY+0.5)+1;
		
		m_dbSpaceXR =  tx/(m_nWidth-1);
		m_dbSpaceYR =  ty/(m_nHeight-1);
		//
		tx = m_dbMoveX-dot.x;
		m_dbMoveX = ratioX*tx+dot.x;
			
		ty = m_dbMoveY-dot.y;
		m_dbMoveY = ratioY*ty+dot.y;
		
		if (m_nHeight  <=  2 || m_nWidth  <=  2)
		{	
			m_nHeight = iBckHeight;
			m_nWidth = iBckWidth;
			m_dbAngle = dbBckAngle;
			m_dbMoveX = dbMovex;
			m_dbMoveY = dbMovey;
			AfxMessageBox(IDS_PROMPT_SCALE_MINIMUM);
			return;
		}
		DeCode(m_strBmpName);
	}

	InitQuadRange();
}


void CNodeDib::Rotate(CDot dot, double angle, int nPlane)
{
	angle = angle*PAI/180;

	double dbSine = sin(angle);
	double dbCos = cos(angle);
	double a,b,r,Araf,newx,newy;	
	CDot dotBase = m_quadLimit.GetBaseDot(BASE_MM);

 	a = dotBase.x - dot.x;
 	b = dotBase.y - dot.y;
	r = sqrt(a*a + b*b);

	if( fabs(a) > EPSILON )
	{
		Araf = atan(b/a);
		if ( a < 0 )
			Araf += PAI;
	}
	else
	{
		Araf = PAI/2;
		if( b < 0 )
			Araf = -PAI/2;
	}
	Araf += angle;

	newx = dot.x + r*cos(Araf) - dotBase.x;
	newy = dot.y + r*sin(Araf) - dotBase.y;
	m_dbAngle  +=  angle;

	DeCode(m_strBmpName);
	Move(newx,newy);

	InitQuadRange();
}


BOOL CNodeDib::Move(double moveX,double moveY,double moveZ)
{
	m_dbMoveX  +=  moveX;
	m_dbMoveY  +=  moveY;

	CalLimit();
	m_quadRangle.Move(moveX,moveY);

	return TRUE;
}


void CNodeDib::Draw(XDC *pDC)
{
	if( !m_pImageSrc )
	{
		DataProc();
	}
	
	//
	m_pDC = pDC;
	if ( m_pImageSrc )
	{
		if ( pDC->IsPrinting() )
		{
			double x,y;
			BYTE tmp;
			double dbCenterX, dbCenterY;
			CDot tmpDot;
			dbCenterX = m_dbSpaceXR*(m_nWidth-1)/2.0;
			dbCenterY = m_dbSpaceYR*(m_nHeight-1)/2.0;
			double dSin = sin(m_dbAngle);
			double dCos = cos(m_dbAngle);	
			BOOL bDraw = FALSE;
			for( int j = 0; j<m_nHeight; j++ )//每行
			{
				for( int i = 0; i<m_nWidth; i++ )//每列
				{
					if( 0 == (j%2) )
					{//正向扫描
						tmp = *(m_pImageSrc+j*m_nWidth+i);
						x = i*m_dbSpaceXR-dbCenterX;
						y = j*m_dbSpaceYR-dbCenterY;
					}
					else
					{//逆向扫描
						tmp = *(m_pImageSrc+j*m_nWidth+((m_nWidth-1-i)));
						x = (m_nWidth-1-i)*m_dbSpaceXR-dbCenterX;
						y = j*m_dbSpaceYR-dbCenterY;
					}
					if (m_nDibType != 1)
					{
						tmp = m_bOppsitionCol?~tmp:tmp;
						{
							tmpDot.x = x*dCos-y*dSin+m_dbMoveX;
							tmpDot.y = x*dSin+y*dCos+m_dbMoveY;
							pDC->SetPixelV((int)tmpDot.x,(int)tmpDot.y,RGB(tmp,tmp,tmp));
						}
					} 
					else
					{
						bDraw = m_bOppsitionCol?tmp:!tmp;
						if( bDraw )
						{
							tmpDot.x = x*dCos-y*dSin+m_dbMoveX;
							tmpDot.y = x*dSin+y*dCos+m_dbMoveY;
							pDC->SetPixelV((int)tmpDot.x,(int)tmpDot.y,m_property.m_color);
						}
					}
				}
			}
		}
		else
		{
			//char pbuf[256];
			BITMAP   bm;
			m_bmp.GetBitmap(&bm); 
			CDC   dcCompatible; 
			dcCompatible.CreateCompatibleDC(pDC->m_pDC); 
			dcCompatible.SelectObject(&m_bmp);
			
			CQuad quad;
			CRgn   cRgn;
			CPoint point[4];
			CalDibRgn(quad,point,4);
			pDC->m_pDC->LPtoDP(point,4);
			VERIFY(cRgn.CreatePolygonRgn(point,4,ALTERNATE));
			pDC->m_pDC->SelectClipRgn(&cRgn,RGN_COPY);
			SetStretchBltMode(pDC->m_pDC->m_hDC,COLORONCOLOR);	
			pDC->m_pDC->StretchBlt((int)quad.left,(int)quad.bottom,int(quad.right-quad.left),int(quad.top-quad.bottom),&dcCompatible,0,0, bm.bmWidth,bm.bmHeight,SRCCOPY); 
			cRgn.DeleteObject();
			pDC->m_pDC->SelectClipRgn(NULL,RGN_COPY);
		}
	}
}



BOOL CNodeDib::Track(CWnd* pWnd, CPoint ptOrg)
{
	return DeCode(m_strBmpName);
}



UINT CNodeDib::Mark( HWND hWnd )
{
	CLay *pLay = g_DAT.GetCurLay();	
	CString strPath = ::g_sys.GetInstallPath();
	///////
	int nDACycle = GetPrivateProfileInt(_T("Machine"),_T("DA_CYCLE"),10,strPath+_T("\\Emcc\\SystemSet.ini"));
	if ( nDACycle < 2 )  
		nDACycle = 2;

	////////
	BMPSPEC bmpSpec = { 0 };
	PARA  para  = pLay->GetPara(m_property.m_nLayer);
	int  nSteps = (int)(fabs(m_dbSpaceX)/para.emcc.dbMarkV*1000000/nDACycle);//跳转步数
	if ( nSteps < 1 ) 	nSteps = 1;

	bmpSpec.nDotSteps = nSteps;
	bmpSpec.dbStepLen = fabs(m_dbSpaceXR)/nSteps;//单步长度(mm/Step)
	bmpSpec.dbQFreq   = 100.0/nSteps;	//Q频(KHZ)
	g_DAT.BmpStart(&bmpSpec);

	BOOL bStop = FALSE;
	UINT nMarkRet = 0;
	int  nNum = pLay->GetRepeat(m_property.m_nLayer);	
	for( int i =  0; i<nNum; i++ )
	{		
		if ( m_pImageSrc )
		{
			double  x,y;
			BYTE  	tmp;
			double	dbCenterX = m_dbSpaceXR*(m_nWidth-1)/2.0;
			double  dbCenterY = m_dbSpaceYR*(m_nHeight-1)/2.0;
			double	dSin = sin(m_dbAngle);
			double	dCos = cos(m_dbAngle);
			UINT	nType = (m_nDibType == 0 ) ? 2 : 1 ;
			DOT6*	pDot6 = new DOT6[m_nWidth+m_nAccTime];
			int		index = 0;
	
			// 灰度位图打标
			if ( m_nDibType == 0  )
			{
				for( int j = 0; j<m_nHeight; j++ )
				{
					index = 0;
					memset(pDot6,0,sizeof(DOT6)*(m_nWidth+m_nAccTime));
					//////
					//添加加速点
					for ( int k = 0; k < m_nAccTime; k++ )
					{										
						if ( m_bLoop )
						{
							x = ( ( (j%2) == 0 ) ? (k-m_nAccTime) : (m_nWidth-1+m_nAccTime-k+m_dbLoopCompate) )*m_dbSpaceXR-dbCenterX;
							y = j*m_dbSpaceYR-dbCenterY;
						}
						else
						{
							x = (k-m_nAccTime)*m_dbSpaceXR-dbCenterX;
							y = j*m_dbSpaceYR-dbCenterY;
						}
						pDot6[k].x = x*dCos-y*dSin+m_dbMoveX;
						pDot6[k].y = x*dSin+y*dCos+m_dbMoveY;
						pDot6[k].i =  999;	
					}
					for ( int i = 0; i<m_nWidth; i++ )
					{
						if ( m_bLoop )
						{
							if ( ( j%2 ) == 0 )
							{
								//正向
								tmp = *(m_pImageSrc+j*m_nWidth+i);
								x = i*m_dbSpaceXR-dbCenterX;
								y = j*m_dbSpaceYR-dbCenterY;
							}
							else
							{
								//反向
								tmp = *(m_pImageSrc+j*m_nWidth+(m_nWidth-i-1));
								x = (m_nWidth-i-1+m_dbLoopCompate)*m_dbSpaceXR-dbCenterX;
								y = j*m_dbSpaceYR-dbCenterY;
							}
						}
						else
						{
							//每列
							tmp = *(m_pImageSrc+j*m_nWidth+i);
							x = i*m_dbSpaceXR-dbCenterX;
							y = j*m_dbSpaceYR-dbCenterY;
						}
					
						pDot6[index+m_nAccTime].x = x*dCos-y*dSin+m_dbMoveX;
						pDot6[index+m_nAccTime].y = x*dSin+y*dCos+m_dbMoveY;

						tmp = m_bOppsitionCol ? (255-tmp) : tmp;
						pDot6[index+m_nAccTime].i = g_sys.m_GrayCompensate[tmp];
						index++;
					}
					if ( index > 0 )
					{
						nMarkRet = g_DAT.Mark_BMP(nType, LPVOID(pDot6),index+m_nAccTime,m_property.m_nLayer);
						if ( nMarkRet  !=  FUN_OK )
						{
							bStop = TRUE;
							break;
						}
					}			
				}
				
			} 
			else
			{
				//二值图打标或灰度点阵打标
				BOOL bMark = FALSE;
				for(int j = 0;j<m_nHeight;j++)
				{
					index = 0;//每行
					for( int i = 0;i<m_nWidth;i++ )
					{
						if ( m_bLoop )
						{
							if ((j%2) == 0)
							{
								//正向
								tmp = *(m_pImageSrc+j*m_nWidth+i);
								x = i*m_dbSpaceXR-dbCenterX;
								y = j*m_dbSpaceYR-dbCenterY;
							}
							else
							{
								//反向
								tmp = *(m_pImageSrc+j*m_nWidth+(m_nWidth-i-1));
								x = (m_nWidth-i-1)*m_dbSpaceXR-dbCenterX;
								y = j*m_dbSpaceYR-dbCenterY;
							}
						}
						else
						{
							//每列
							tmp = *(m_pImageSrc+j*m_nWidth+i);
							x = i*m_dbSpaceXR-dbCenterX;
							y = j*m_dbSpaceYR-dbCenterY;
						}

						// (IPG激光器使用灰度点阵方式,控制不同的关延时)
						if( m_nDibType == 2  )
						{
							tmp = m_bOppsitionCol ? (255-tmp) : tmp;
							tmp = g_sys.m_GrayCompensate[tmp]; // 灰度非线性补偿
							if( tmp != 255 )
							{
								pDot6[index].x = x*dCos-y*dSin+m_dbMoveX;
								pDot6[index].y = x*dSin+y*dCos+m_dbMoveY;
								pDot6[index].i = m_nMinFDL+(255-tmp)*(m_nMaxFDL-m_nMinFDL)/255.0;
								index++;
							}
						}
						else
						{
							bMark = m_bOppsitionCol ? tmp: !tmp;
							if ( bMark )
							{
								pDot6[index].x = x*dCos-y*dSin+m_dbMoveX;
								pDot6[index].y = x*dSin+y*dCos+m_dbMoveY;
								index++;
							}
						}
					}
					if ( index >0 )
					{
						nMarkRet = g_DAT.Mark_BMP(nType, LPVOID(pDot6),index,m_property.m_nLayer);
						if (nMarkRet != FUN_OK)
						{
							bStop = TRUE;
							break;
						}
					}			
				}		
			}
			delete[] pDot6;
		}
		if ( bStop )
			break;
	}

	return HLOK;
}

BOOL CNodeDib::Create(CRect &rect)
{
	return TRUE;
}




BOOL CNodeDib::DataChange()
{
	//建立打标数据
	double tx = m_dbWidth;
	double ty = m_dbHeight;
	
	//cal m_dbSpaceX&m_dbSpaceY	
	m_nWidth  = (int)(tx/m_dbSpaceX+0.5)+1;
	m_nHeight = (int)(ty/m_dbSpaceY+0.5)+1;
	
	
	m_dbSpaceXR = tx/(m_nWidth-1);
	m_dbSpaceYR = ty/(m_nHeight-1);

	return DeCode(m_strBmpName);
}

BOOL CNodeDib::DataProc()
{
	return DeCode(m_strBmpName);
}

BOOL CNodeDib::Click(CPoint &point, CWnd *pWnd)
{	
	CPoint pcPoint[4] = {0};
	CQuad quad;
	CalDibRgn(quad,pcPoint, 4);

	CDot dot;
	dot.x = point.x;
	dot.y = point.y;

	return quad.DotIn(dot);
}


BOOL CNodeDib::CopyToMe(CNode *pSrc,DWORD dwStyle)
{
	if ( NULL == pSrc || pSrc->GetType()  !=  NODE_BMP )
		return FALSE;

	CNodeDib *pNode = (CNodeDib*)pSrc;
	////////
	m_bLoop = pNode->m_bLoop;
	m_property		 =  pNode->m_property;
	m_bSelect		 =  pNode->m_bSelect;
	m_strBmpName = pNode->m_strBmpName;
	m_bOppsitionCol = pNode->m_bOppsitionCol;
	m_dbDotTime	 =  pNode->m_dbDotTime;
	m_dbSpaceX	 =  pNode->m_dbSpaceX;
	m_dbSpaceY	 =  pNode->m_dbSpaceY;
	m_dbHeight		 =  pNode->m_dbHeight;
	m_dbWidth		 =  pNode->m_dbWidth;
	m_nHeight		 =  pNode->m_nHeight;
	m_nWidth		 =  pNode->m_nWidth;
	m_property		 =  pNode->m_property;
	m_dbMoveX		 =  pNode->m_dbMoveX;
	m_dbMoveY		 =  pNode->m_dbMoveY;
	
	m_dbAngle		 =  pNode->m_dbAngle;
	m_nDibType	 =  pNode->m_nDibType;
	m_bXFlip			 =  pNode->m_bXFlip;
	m_bYFlip			 =  pNode->m_bYFlip;
	m_bSelect		 =  pNode->m_bSelect; //for 阵列
	m_iDispHeight	 =  pNode->m_iDispHeight;
	m_iDispWidth	 =  pNode->m_iDispWidth;
	m_nAccTime	 =  pNode->m_nAccTime;
	m_dbLoopCompate = pNode->m_dbLoopCompate;
	m_dbBrightness =  pNode->m_dbBrightness;
	m_dbContrast	 =  pNode->m_dbContrast;
	m_pDC = pNode->m_pDC;

	m_dbSpaceXR	 =  pNode->m_dbSpaceXR;
	m_dbSpaceYR	 =  pNode->m_dbSpaceYR;
	m_quadRangle	 =  pNode->m_quadRangle;

	if( m_pImageSrc )
	{
		delete []m_pImageSrc;
		m_pImageSrc = NULL;
	}
	int nLen = pNode->m_nWidth*pNode->m_nHeight;
	if( nLen > 0 )
	{
		m_pImageSrc = new BYTE[nLen];
		::memcpy(m_pImageSrc,pNode->m_pImageSrc,pNode->m_nWidth*pNode->m_nHeight);
	}

	return TRUE;
}

void CNodeDib::SetProperty(int mode,CProperty pty ,XDC* pDC)
{	
	m_pDC = pDC;
	m_property.Set(mode,pty);
}


BOOL CNodeDib::DeCode(CString strPathName)
{
	if ( m_pDC == NULL )
	{
		CHLView *pView = GetCurActiveView();
		if ( pView )
			m_pDC = &(pView->m_DC);
	}
	
	GenImportFullName(strPathName);

	AfxGetApp()->BeginWaitCursor();
	BOOL bFlag = FALSE;
	
	double dbBoxX = 50;
	double dbBoxY = 50;
	g_DAT.GetMarkRect(dbBoxX,dbBoxY);

	if ( m_pImageSrc )
	{
		delete[] m_pImageSrc;
		m_pImageSrc = NULL;
	}

	int    nWidth,nHeight,nXDpi,nYDpi;
	char  szPathName[MAX_PATH] = {0};

#ifdef _UNICODE
	CStdioFileEx::GetMultiByteFromUnicode(strPathName.GetBuffer(strPathName.GetLength()),szPathName,MAX_PATH);
	strPathName.ReleaseBuffer();
#else
	_tcscpy_s( szPathName, MAX_PATH, (char*)(LPCTSTR) strPathName );
#endif
	
	if ( LoadDib(szPathName,nWidth,nHeight,nXDpi,nYDpi,m_bXFlip,m_bYFlip) == 0 )
	{
		if ( (m_nWidth == 0) || (m_nHeight == 0) )
		{
 			//按X拉伸，保持显示比例不变
 			int nMax = nHeight>nWidth ? nHeight:nWidth;
 			double dbMax = nMax*m_dbSpaceXR;
 			dbMax = dbMax>dbBoxX ? dbBoxX:dbMax;
 			dbMax = dbMax>dbBoxY ? dbBoxY:dbMax;
			nMax = (int)(dbMax/m_dbSpaceXR);
			if ( nHeight > nWidth )
 			{
 				nWidth = (nWidth*nMax/nHeight);
 				nHeight = nMax;
 			}
 			else
 			{
 				nHeight = (nHeight*nMax/nWidth);
 				nWidth = nMax;
 			}
			m_nWidth = nWidth;
			m_nHeight = (int)(m_dbSpaceXR*nHeight/m_dbSpaceYR);
		}
		
		int  nDllRtn = 0;
		m_pImageSrc  = new BYTE[m_nWidth*m_nHeight]; //100段线有101个端点
		if ( m_pImageSrc == NULL )
		{
			AfxGetApp()->EndWaitCursor();
			return FALSE;
		}

		long brightness = long(m_dbBrightness*255);
		brightness = brightness>255 ? 255:brightness;
		brightness = brightness<(-255) ? (-255):brightness;
		long contrast = long(m_dbContrast*100);
		contrast = contrast > 100 ? 100 : contrast;
		contrast = contrast < (-100) ? (-100) : contrast;
		if ( m_nDibType != 1 )
		{
			nDllRtn = ProcGrayDib(m_pImageSrc,m_nWidth,m_nHeight,m_nWidth,int(m_dbSpaceYR*m_nHeight/m_dbSpaceXR),g_sys.m_nDibSampleMode,brightness,contrast) ;
		} 
		else
		{
			nDllRtn =  ProcDib(m_pImageSrc,m_nWidth,m_nHeight,m_nWidth,int(m_dbSpaceYR*m_nHeight/m_dbSpaceXR),g_sys.m_nDibSampleMode,brightness,contrast);
		}

		if ( nDllRtn == 0 )
		{
			m_dbWidth  = (m_nWidth-1)*m_dbSpaceXR;
			m_dbHeight = (m_nHeight-1)*m_dbSpaceYR;
			TCHAR buf[MAX_PATH] = {0};
			GetTempPath(MAX_PATH,buf);
			_tcscat_s(buf,MAX_PATH,_T("HLBMP.bmp"));// 
			m_bmp.DeleteObject();						
			HBITMAP hbmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(),buf,IMAGE_BITMAP,
																	0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);   

			hbmp = GetRotatedBitmapNT( hbmp,m_dbAngle,RGB(255,255,255) );
			if ( hbmp == NULL )
				return bFlag;
			
			m_bmp.DeleteObject();
			m_bmp.Attach(hbmp);
			bFlag = TRUE;
		}
		DelDib();
	}
	else
	{
		CString str;
		str.LoadString(IDS_PROMPT_CANNOTFIND);
		str  +=  strPathName;
		AfxMessageBox(str);
	}

	CalLimit();
	AfxGetApp()->EndWaitCursor();
	return bFlag;
}


void CNodeDib::Delete()
{
	delete []m_pImageSrc;
	m_pImageSrc = NULL;
	m_bmp.DeleteObject();		
}


//计算位图外包矩阵大小
CQuad CNodeDib::CalLimit()
{
	CDot pCPoint[4];
	//计算画图多边形区域
	{
		double cosine = cos(m_dbAngle);
		double sine    = sin(m_dbAngle);
		double const fX = (m_dbWidth/2.0);
		double const fY = (m_dbHeight/2.0);

		CDot cDot[4];
		cDot[0].x = fX;cDot[0].y = -fY;
		cDot[1].x = fX;cDot[1].y =  fY;
		cDot[2].x  = -fX;cDot[2].y =  fY;
		cDot[3].x  = -fX;cDot[3].y = -fY;
		
		double x(0.0),y(0.0);
		for (int i = 0; i < 4; i++)
		{
			x = cDot[i].x * cosine - cDot[i].y * sine;
			y = cDot[i].x * sine   + cDot[i].y * cosine;
			x = x + m_dbMoveX;
			y = y + m_dbMoveY;
			pCPoint[i].x = x;			
			pCPoint[i].y = y;
		}		
	}
	CQuad quad1;
	quad1.left = min(pCPoint[3].x,min(pCPoint[2].x,min(pCPoint[0].x,pCPoint[1].x)));
	quad1.right = max(pCPoint[3].x,max(pCPoint[2].x,max(pCPoint[0].x,pCPoint[1].x)));
	quad1.bottom = min(pCPoint[3].y,min(pCPoint[2].y,min(pCPoint[0].y,pCPoint[1].y)));
	quad1.top = max(pCPoint[3].y,max(pCPoint[2].y,max(pCPoint[0].y,pCPoint[1].y)));
	m_quadLimit = quad1;
	return m_quadLimit;
}

//
void CNodeDib::SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{
	m_property.m_color  = color;
	m_property.m_nLayer  = nLayer;
}

// GetRotatedBitmapNT	- Create a new bitmap with rotated image
// Returns		- Returns new bitmap with rotated image
// hBitmap		- Bitmap to rotate
// radians		- Angle of rotation in radians
// clrBack		- Color of pixels in the resulting bitmap that do
//			  not get covered by source pixels
HBITMAP CNodeDib::GetRotatedBitmapNT( HBITMAP hBitmap, double radians, COLORREF clrBack )
{
	// Create a memory DC compatible with the display
	CDC sourceDC, destDC;
	sourceDC.CreateCompatibleDC( NULL );
	destDC.CreateCompatibleDC( NULL );

	// Get logical coordinates
	BITMAP bm;
	::GetObject( hBitmap, sizeof( bm ), &bm );

	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);

	int x1 = (int)(bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(-bm.bmWidth * sine);

	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(0,max(x1, max(x2,x3)));
	int maxy = max(0,max(y1, max(y2,y3)));

	int w = maxx - minx;
	int h = maxy - miny;

	// Create a bitmap to hold the result
	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);
	HBITMAP hbmOldSource = (HBITMAP)::SelectObject( sourceDC.m_hDC, hBitmap );
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC.m_hDC, hbmResult );
	// Draw the background color before we change mapping mode
	HBRUSH hbrBack = CreateSolidBrush( clrBack );
	HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC.m_hDC, hbrBack );
	destDC.PatBlt( 0, 0, w, h, PATCOPY );
	::DeleteObject( ::SelectObject( destDC.m_hDC, hbrOld ) );

	// We will use world transform to rotate the bitmap
	SetGraphicsMode(destDC.m_hDC, GM_ADVANCED);
	

	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = (float)-minx;
	xform.eDy = (float)-miny;

	SetWorldTransform( destDC.m_hDC, &xform );

	if (m_bOppsitionCol)
	{
		destDC.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, NOTSRCCOPY );
	}
	else
	{
		destDC.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, SRCCOPY );		
	}
	::SelectObject( sourceDC.m_hDC, hbmOldSource );
	::SelectObject( destDC.m_hDC, hbmOldDest );

	return hbmResult;
}


//计算绘图的矩形框CQuad& quad，CRgn clip窗大小(CPoint* pCPoint,int iCnt)。
void CNodeDib::CalDibRgn(CQuad& quad,CPoint* pCPoint,int iCnt)
{
	if ( iCnt  !=  4 )
		return;

	//计算画图多边形区域
	{
		double cosine = cos(m_dbAngle);
		double sine = sin(m_dbAngle);
		double const fX = (m_dbWidth/2.0);
		double const fY = (m_dbHeight/2.0);
		CDot cDot[4];
		cDot[0].x = fX;cDot[0].y = -fY;
		cDot[1].x = fX;cDot[1].y = fY;
		cDot[2].x = -fX;cDot[2].y = fY;
		cDot[3].x = -fX;cDot[3].y = -fY;	
		double x,y;
		for (int i = 0;i<4;i++)
		{
			x = cDot[i].x*cosine-cDot[i].y*sine;
			y = cDot[i].x*sine+cDot[i].y*cosine;
			x = x+m_dbMoveX;
			y = y+m_dbMoveY;
			//实际坐标转化为log坐标
			m_pDC->PhysicToLogic(x, y);
			pCPoint[i].x = (long)x;			
			pCPoint[i].y = (long)y;
		}		
	}
	quad.left = min(pCPoint[3].x,min(pCPoint[2].x,min(pCPoint[0].x,pCPoint[1].x)));
	quad.right = max(pCPoint[3].x,max(pCPoint[2].x,max(pCPoint[0].x,pCPoint[1].x)));
	quad.bottom = min(pCPoint[3].y,min(pCPoint[2].y,min(pCPoint[0].y,pCPoint[1].y)));
	quad.top = max(pCPoint[3].y,max(pCPoint[2].y,max(pCPoint[0].y,pCPoint[1].y)));
}


void CNodeDib::UpdateName()
{	
	if (g_bClipBoardCopyNode)
	{			
		//复制文件操作
		//如果为绝对路径，返回，否则改为绝对路径。
		if (-1 == m_strBmpName.Find(':'))
		{
			//还原路径
			CString strHLFileName;
			CString strPath;
			CString strHLImportFilePath;	
			if (!g_OpenFileName.IsEmpty())
			{		
				strHLFileName = g_OpenFileName;
				strPath = strHLFileName.Left(strHLFileName.ReverseFind('\\'));
				strHLImportFilePath = strPath+strHLFileName.Mid(strHLFileName.ReverseFind('\\'),strHLFileName.ReverseFind('.')-strHLFileName.ReverseFind('\\'));
				m_strBmpName = strHLImportFilePath+"\\"+m_strBmpName;
			}
			else if (!g_OldFileName.IsEmpty())
			{
				strHLFileName = g_OldFileName;
				strPath = strHLFileName.Left(strHLFileName.ReverseFind('\\'));
				strHLImportFilePath = strPath+strHLFileName.Mid(strHLFileName.ReverseFind('\\'),strHLFileName.ReverseFind('.')-strHLFileName.ReverseFind('\\'));
				m_strBmpName = strHLImportFilePath+"\\"+m_strBmpName;
			}
		}
		return;
	}

	if (0 != g_OldFileName.Compare(g_OpenFileName)&&(!g_OldFileName.IsEmpty()))
	{
		//为SAve as
		//do copy 文件夹
		//复原BMP路径 利用g_OldFileName;
		CString str = g_OldFileName.Left(g_OldFileName.ReverseFind('.'));
		m_strBmpName = str+'\\'+m_strBmpName;
	}
	else if (-1 == m_strBmpName.Find(':'))
	{
		return; //已经是相对路径了
	}	
	CString strHLFileName = g_OpenFileName;
	CString strPath;
	CString strHLImportFilePath;
	CString strDestFile;
	CString strSrcFile = m_strBmpName;
	strPath = strHLFileName.Left(strHLFileName.ReverseFind('\\'));
	strHLImportFilePath = strPath+strHLFileName.Mid(strHLFileName.ReverseFind('\\'),strHLFileName.ReverseFind('.')-strHLFileName.ReverseFind('\\'));
	m_strBmpName = strSrcFile.Mid(strSrcFile.ReverseFind('\\')+1);
	strDestFile = strHLImportFilePath+"\\"+m_strBmpName;
	if (0 == strSrcFile.Compare(strDestFile))
	{
		return;
	}
	//1.能否找到文件夹,不能则新建
	BOOL bHLImportFilePathExist = FALSE;
	CFileFind finder;
	bHLImportFilePathExist = finder.FindFile(strHLImportFilePath);		
	finder.Close();
	if (!bHLImportFilePathExist)
	{
		if (!::CreateDirectory(strHLImportFilePath,NULL))
		{
			AfxMessageBox(IDS_PROMPT_CREATEINPORTFOLDER_FAIL);
			m_strBmpName = strSrcFile;
			return;
		}		
	}
	//2.Copy 导入文件到文件夹
	if (!CopyFile(strSrcFile,strDestFile,TRUE))
	{
		CString	str;
		CString strTemp;
		str.LoadString(IDS_PROMPT_INPORTFILE1); // 导入文件夹已有文件;
		str += m_strBmpName;
		strTemp.LoadString(IDS_PROMPT_INPORTFILE2);
		str += strTemp+strSrcFile; // 原文件:"+strSrcFile+"\n替换或自动重命名?
		strTemp.LoadString(IDS_PROMPT_INPORTFILE3);
		str += strTemp;
		CDlgFileReplyRename dlg;
		dlg.m_strPromptMessage = str;
		if ( IDOK == dlg.DoModal() )
			CopyFile(strSrcFile,strDestFile,FALSE);
		else
		{
			//自动进行加序号处理
			str = m_strBmpName.Left(m_strBmpName.ReverseFind('.'));
			CString bmpExt;
			int len = m_strBmpName.GetLength();
			int j;
			for (j = len-1; j  >=  0; j--)
			{
				if (m_strBmpName[j] == _T('.') )
				{
					bmpExt = m_strBmpName.Mid(j+1);
					break;
				}
			}
			if ( j < 0 )
			{
				bmpExt.Empty();
			}

			CString newName;
			int i = 1;
			newName.Format(_T("_%d."),i);
			newName  +=  bmpExt;
			m_strBmpName = str+newName;
			strDestFile = strHLImportFilePath+_T("\\")+m_strBmpName;
			while (!CopyFile(strSrcFile,strDestFile,TRUE))
			{
				i++;
				newName.Format(_T("_%d."),i);
				newName  +=  bmpExt;
				m_strBmpName = str+newName;
				strDestFile = strHLImportFilePath+_T("\\")+m_strBmpName;
			}
			return;
		}
	}	
}

int CNodeDib::Pai2Deg(double f)
{
	return int(f*180/PAI);
}

double CNodeDib::Deg2Pai(int a)
{
	return double(a)*PAI/180;
}


void CNodeDib::Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY)
{	
	if ( fabs(ratioX -1) < EPSILON && fabs(ratioY-1) < EPSILON )
		Move(moveX,moveY);
	
	Scale(dot,ratioX,ratioY);
}


void CNodeDib::LoadStrokeList(StrokeList *pStrokeList,BOOL bOptimize,int nLayNo)
{
}


void CNodeDib::LoadWheelList(int nType, StrokeList *pListStroke)
{
}

void CNodeDib::AddToPlt(CNode *pNodePlt)
{
}