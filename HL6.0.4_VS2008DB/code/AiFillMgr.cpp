// AiFillMgr.cpp : 实现文件
//
#include "stdafx.h"
#include "HL.h"
#include "AiFillMgr.h"

// CAiFillMgr

CAiFillMgr::CAiFillMgr()
{
	m_pBmp = NULL;
	m_pBmpOrg = NULL;
	m_pBmpGroup = NULL;
	m_pBmpMark = NULL;
	m_pBmpMat = NULL;
	m_nXR = 0;
	m_nDirection = 0;
	m_bLoop = FALSE;
	m_nBmpWidth = 500;
	m_nBmpHeight = 500;
	m_dbSpaceX = 0.1;
	m_dbSpaceY = 0.1;
	m_nScanType = 0;
	m_dbScanDis = 0.005;
}

CAiFillMgr::~CAiFillMgr()
{
	DeleteLineList();
	ReleaseImageData();
}


// CAiFillMgr 成员函数
BOOL CAiFillMgr::DeCode( const CString& file, StrokeList &listLineData )  
{
	CFileFind fFind;
	CString strMsg;
	if( !fFind.FindFile(file) )
	{
		strMsg.Format(_T("Sorry, not find file %s!"),file);
		AfxMessageBox( strMsg );
		return FALSE;
	}


	//解析AI文件
	int nRet = m_aiTrnslate.Translate( (LPTSTR)(LPCTSTR)file, 0.002 );
	if ( 0 != nRet )
	{
		if( 1 == nRet )
			strMsg = _T("AI: Parameter error!");
		else if ( 2 == nRet )
			strMsg = _T("AI: File format error!");
		else if ( 3 == nRet )
			strMsg = _T("AI: File version must below V8.0!");
		else if ( 4 == nRet )
			strMsg = _T("AI:Allocate memory error!");
		
		AfxMessageBox(strMsg);
		return FALSE;
	}

	//计算点间距和图像大小
	m_dbSpaceX = ::g_sys.GetProfileDouble( _T("SysCfg"), _T("AiBmpSpace"), 0.01 );
	m_dbSpaceY = m_dbSpaceX;
	
	double dbLeft = m_aiTrnslate.m_rcRange.left - 2;
	double dbBottom = m_aiTrnslate.m_rcRange.bottom - 2;
	m_dbWidth = m_aiTrnslate.m_rcRange.Width() + 4;
	m_dbHeight = m_aiTrnslate.m_rcRange.Height() + 4;
	m_nBmpWidth = (int)( m_dbWidth / m_dbSpaceX );
	m_nBmpHeight = (int)( m_dbHeight / m_dbSpaceY );
	
	if( m_nBmpWidth > 10000 || m_nBmpHeight > 4000 )
	{
		AfxMessageBox(_T("Sorry,The size of vector over range!!"));
		return FALSE;
	}
	
	//创建图像
	CreateImageData( m_nBmpWidth, m_nBmpHeight );
	
	//
	AfxGetApp()->BeginWaitCursor();
	CAILayerOut* pLayer = NULL;
	CAIStroke* pAiStroke = NULL;
	
	POSITION pos = NULL;
	POSITION posStroke = NULL;
	pos = m_aiTrnslate.m_layerList.GetHeadPosition();
	while( pos )
	{
		pLayer = (CAILayerOut*)m_aiTrnslate.m_layerList.GetNext(pos);
		if( !pLayer )
			continue;
		
		posStroke = pLayer->m_strokeList.GetHeadPosition();
		while( posStroke )	
		{
			pAiStroke = (CAIStroke*)pLayer->m_strokeList.GetNext(posStroke);
			if( !pAiStroke )
				continue;

			DrawPath( pAiStroke, dbLeft, dbBottom );
		}
	}
	
	//
	cvCopyImage( m_pBmp, m_pBmpMark );
	cvNot( m_pBmpMark, m_pBmp );
	
	CreateLineList();
	listLineData.AddTail(&m_listLineData);
	m_listLineData.RemoveAll();
	
	ReleaseImageData();
	AfxGetApp()->EndWaitCursor();
	
	return TRUE;
}

void CAiFillMgr::CreateImageData( int nWidth, int nHeight )
{
	ReleaseImageData();
	m_pBmpOrg = cvCreateImage( cvSize(nWidth,nHeight), IPL_DEPTH_8U, 1 );	
	cvSet( m_pBmpOrg, CV_RGB(0, 0, 0) );
	
	m_pBmp = cvCloneImage(m_pBmpOrg);
	m_pBmpGroup = cvCloneImage(m_pBmpOrg);
	m_pBmpMark = cvCloneImage(m_pBmpOrg);
	m_pBmpMat = cvCloneImage(m_pBmpOrg);
}


void CAiFillMgr::ReleaseImageData()
{
	if( m_pBmp )
	{
		cvReleaseImage(&m_pBmp);
	}

	if( m_pBmpOrg )
	{
		cvReleaseImage(&m_pBmpOrg);
	}

	if( m_pBmpGroup )
	{
		cvReleaseImage(&m_pBmpGroup);
	}
	
	if( m_pBmpMark )
	{
		cvReleaseImage(&m_pBmpMark);
	}
	
	if( m_pBmpMat )
	{
		cvReleaseImage(&m_pBmpMat);
	}
	
	m_pBmp = NULL;
	m_pBmpOrg = NULL;
	m_pBmpGroup = NULL;
	m_pBmpMark = NULL;
	m_pBmpMat = NULL;
}

//
BOOL CAiFillMgr::DrawPath( CAIStroke* pAiStroke, double dbLeft, double dbBottom )
{
	if ( !pAiStroke )
		return FALSE;
	
	int nType = 0;
	if( pAiStroke->m_nID < 0 )
	{
		nType = 0;
	}
	else
	{
		if( pAiStroke->m_bCompoundPathEnd )
		{
			nType = 2;
		}
		else
		{
			nType = 1;
		}
	}
	
	//
	int nCount = pAiStroke->m_ptArray.GetCount();
	CvPoint *pPointData = new CvPoint[nCount];
	GetPointArray( pAiStroke->m_ptArray, pPointData, dbLeft, dbBottom );
	
	//获取颜色
	CvScalar colorFill, colorLine, colorMark;
	colorFill = RgbToCvcolor( pAiStroke->m_fillColor );
	colorLine = RgbToCvcolor( pAiStroke->m_wireColor );	
	colorMark = cvScalar(1, 1, 1);
	
	//线宽、是否封闭
	BOOL bClosed = pAiStroke->m_bCLosed;
	int nPenwidth = (int)(pAiStroke->m_dbStrokeWidth/m_dbSpaceX);
	nPenwidth = ( nPenwidth < 0 ? 1 : nPenwidth );
	if( !(pAiStroke->m_bWired) )
	{
		nPenwidth = 0;
	}

	m_nXR = 1; //pPath->m_nXR;
	m_nDirection = pAiStroke->m_nDirection;
	
	//------------绘图
	//填充
	if( pAiStroke->m_bFilled )
	{
		if( 0 == nType )
		{
			cvFillPoly( m_pBmp, &pPointData, &nCount, 1, colorFill );
		}
		else
		{
			cvSet( m_pBmpMark, CV_RGB(0, 0, 0) );	
			cvFillPoly( m_pBmpGroup, &pPointData, &nCount, 1, colorFill );
			cvFillPoly( m_pBmpMark, &pPointData, &nCount, 1, colorMark );
			ErodeFillPart(nPenwidth);
		}
	}
	
	//
	if( nPenwidth > 0 )
	{
		cvPolyLine( m_pBmp, &pPointData, &nCount, 1, bClosed, colorLine, nPenwidth, 8, 0 );
	}

	//
	if( 0 == nType )
	{
	}
	else if( 1 == nType )
	{
		AddGroupCount();
	}
	else if( 2 == nType )
	{
		AddGroupCount();
		AddBmp(m_nXR);
		
		cvSet( m_pBmpMat, CV_RGB(0, 0, 0) );
		cvSet( m_pBmpGroup, CV_RGB(0, 0, 0) );
	}
	
	//
	delete []pPointData;
	pPointData = NULL;
	return TRUE;
}



//
BOOL CAiFillMgr::GetPointArray( CArray<PTOUT,PTOUT> &dtArray,  CvPoint *pPointData, double dbLeft, double dbBottom )
{
	if( !pPointData )
		return FALSE;
	
	for( int i = 0; i < dtArray.GetSize(); i++ )
	{
		PTOUT dotAi = dtArray.GetAt(i);	
		int nX = (int)( ( dotAi.x - dbLeft ) / m_dbSpaceX );
		int nY = (int)( ( dotAi.y - dbBottom ) / m_dbSpaceY );
		
		(*(pPointData+i)).x = nX;
		(*(pPointData+i)).y = nY;
	}
	
	return TRUE;
}


//
CvScalar CAiFillMgr::RgbToCvcolor( COLORREF color)
{
	CvScalar cvColor;
	cvColor.val[0] = ( ( color & 0x0000FF) );
	cvColor.val[1] = ( ((color>>8) & 0x0000FF) );
	cvColor.val[2] = ( ((color>>16) & 0x0000FF) );
	
	if( cvColor.val[0] == 255 && cvColor.val[1] == 255 && cvColor.val[2] == 255 )
	{
		cvColor.val[0] = 0;
		cvColor.val[1] = 0;
		cvColor.val[2] = 0;
	}
	else
	{
		cvColor.val[0] = 255;
		cvColor.val[1] = 255;
		cvColor.val[2] = 255;
	}
	
	return cvColor;
}


void CAiFillMgr::ErodeFillPart( int nPenWidth )
{
	int nValue = 0.5*nPenWidth;
	if( nValue <= 0 )
		return;
	
	IplImage *pBmp0 = cvCloneImage(m_pBmpMark);
	cvErode( pBmp0, m_pBmpMark, NULL,nValue ); 
	cvReleaseImage(&pBmp0);
}


void CAiFillMgr::AddGroupCount()
{
	int nValue = 1;
	if( 0 == m_nXR )
	{
		if( 0 == m_nDirection )
		{
			nValue = 1;
		}
		else
		{
			nValue = -1;
		}
	}
	else
	{
		nValue = 1;
	}
	
	//
	CvScalar color;
	IplImage *pBmp0 = cvCloneImage(m_pBmpMat);
	color.val[0] = color.val[1] = color.val[2] = 1;
	cvAddS( pBmp0, color, m_pBmpMat, m_pBmpMark );
	
	cvReleaseImage(&pBmp0);
}


//0:非零缠绕填充 1:奇偶填充
void CAiFillMgr::AddBmp( int nType )
{
	int nValue = -1;
	for( int i=0; i<m_nBmpWidth; i++ )
	{
		for( int j=0; j<m_nBmpHeight; j++ )
		{
			nValue = m_pBmpMat->imageData[m_pBmpMark->height*i + j];
			
			if( 0 == nType )
			{
				if( nValue != 0 ) //非零填充
				{
					m_pBmp->imageData[m_pBmp->height*i + j] = m_pBmpGroup->imageData[m_pBmpMark->height*i + j];
				}
			}
			else if( 1 == nType )
			{
				if( nValue % 2 == 0 )
				{
					//
				}
				else //奇数填充
				{
					m_pBmp->imageData[m_pBmp->height*i + j] = m_pBmpGroup->imageData[m_pBmpMark->height*i + j];
				}
			}
		}
	}
}


void CAiFillMgr::CreateLineList()
{
	if( !m_pBmp )
		return;
	
	DeleteLineList();

	double dbDistance = (0 == m_nScanType ? m_dbHeight : m_dbWidth );
	double dbStep = m_dbScanDis;
	int nCount = dbDistance/dbStep;
	
	//生成数据
	for( int i=0; i<nCount; i++ )
	{
		double dbCurPos = i*dbStep;
		
		if( 0 == m_nScanType ) //X方向
		{
			int nRowPos = dbCurPos/m_dbSpaceY;
			nRowPos = (nRowPos >= m_nBmpHeight ? m_nBmpHeight-1 : nRowPos);
			ScanX( nRowPos, dbCurPos, m_listLineData );
		}
		else
		{
			int nColPos = dbCurPos/m_dbSpaceX;
			nColPos = (nColPos >= m_nBmpWidth ? m_nBmpWidth-1 : nColPos);
			ScanY( nColPos, dbCurPos, m_listLineData );
		}
	}
}


void CAiFillMgr::DeleteLineList()
{
	while( !m_listLineData.IsEmpty() )
	{
		CStroke *pStroke = (CStroke *)m_listLineData.GetHead();

		if( pStroke )
		{
			delete pStroke;
		}
		m_listLineData.RemoveHead();
	}
}


//
void CAiFillMgr::ScanX( int nRowPos, double dbY, StrokeList &listData )
{
	CvScalar color;
	double  x, y;
	int nY, nX;
	CStroke *pStroke = NULL;
	BOOL bFirst = TRUE;
	double dbMove = 0.5*m_dbSpaceX;

	for( int i=0; i<m_nBmpWidth; i++ )
	{
		nX = i;
		nY = m_nBmpHeight - nRowPos - 1;
		color = cvGet2D( m_pBmp, nY, nX );

		if( color.val[0] == 0 )
		{
			x = nX*m_dbSpaceX - 0.5*m_dbWidth;
			y = dbY - 0.5*m_dbHeight;
			
			if( bFirst )
			{
				bFirst = FALSE;
				
				CDot dot(x, y);
				pStroke = new CStroke();
				pStroke->Add(dot);
				listData.AddTail(pStroke);
			}
		}
		else
		{
			if( !bFirst )
			{
				bFirst = TRUE;
				x = nX*m_dbSpaceX - 0.5*m_dbWidth;
				y = dbY - 0.5*m_dbHeight;
				
				CDot dot(x, y);
				pStroke->Add(dot);
				pStroke = NULL;
			}
		}
	}
	
	//
	if( !bFirst && pStroke )
	{
		CDot dot(x, y);
		dot.x += m_dbSpaceX;
		pStroke->Add(dot);
	}
}


void CAiFillMgr::ScanY( int nColPos, double dbX, StrokeList &listData )
{
	CvScalar color;
	double  x, y;
	int nY, nX;
	CStroke *pStroke = NULL;
	BOOL bFirst = TRUE;
	
	for( int i=0; i<m_nBmpHeight; i++ )
	{
		nX = nColPos;
		nY = m_nBmpHeight-i-1;

		color = cvGet2D( m_pBmp, nY, nX );
		if( color.val[0] == 0 )
		{
			x = dbX - 0.5*m_dbWidth;
			y = i*m_dbSpaceY - 0.5*m_dbHeight;

			if( bFirst )
			{
				bFirst = FALSE;
				CDot dot(x, y);
				pStroke = new CStroke();
				pStroke->Add(dot);
				
				listData.AddTail(pStroke);
			}
		}
		else
		{
			if( !bFirst )
			{
				bFirst = TRUE;
				x = dbX - 0.5*m_dbWidth;
				y = i*m_dbSpaceY - 0.5*m_dbHeight;
				
				CDot dot(x, y);
				pStroke->Add(dot);
				pStroke = NULL;
			}
		}
	}

	//
	if( !bFirst && pStroke )
	{
		CDot dot(x, y);
		dot.y += m_dbSpaceY;
		pStroke->Add(dot);
	}
}
