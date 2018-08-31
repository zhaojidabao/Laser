

#include "stdafx.h"
#include "ColorPickerCB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColorPickerCB::CColorPickerCB()
{
}


CColorPickerCB::~CColorPickerCB()
{
}


BEGIN_MESSAGE_MAP(CColorPickerCB, CComboBox)
	//{{AFX_MSG_MAP(CColorPickerCB)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPickerCB message handlers

int CColorPickerCB::OnCreate( LPCREATESTRUCT pCStruct ) 
{
	if ( CComboBox::OnCreate( pCStruct ) == -1 )				
		return( -1 );										

	return( 0 );											
}


//�����б������ɫֵ�������ػ��б��
void CColorPickerCB::DrawItem( LPDRAWITEMSTRUCT pDIStruct )
{
	static		CString	sColor;								

	CDC			dcContext;
	CRect		rItemRect( pDIStruct -> rcItem );
	CRect		rBlockRect( rItemRect );
	CRect		rTextRect( rBlockRect );
	CBrush		brFrameBrush;
	int			iFourthWidth = 0;
	int			iItem = pDIStruct -> itemID;
	int			iAction = pDIStruct -> itemAction;
	int			iState = pDIStruct -> itemState;
	COLORREF	crColor = 0;
	COLORREF	crNormal = GetSysColor( COLOR_WINDOW );
	COLORREF	crSelected = GetSysColor( COLOR_HIGHLIGHT );
	COLORREF	crText = GetSysColor( COLOR_WINDOWTEXT );

	// ��ϵ CDC����
	if ( !dcContext.Attach( pDIStruct -> hDC ) )				
		return;			

	//���1/4���б�����
	iFourthWidth = ( rBlockRect.Width() / 2 );				
	//������ɫ��ˢ
	brFrameBrush.CreateStockObject( BLACK_BRUSH );			

	//�������Ŀ��ѡ��
	if ( iState & ODS_SELECTED )								
	{										
		
		dcContext.SetTextColor(
				( 0x00FFFFFF & ~( crText ) ) );				
		dcContext.SetBkColor( crSelected );					
		dcContext.FillSolidRect( &rBlockRect, crSelected );	
	}
	else//���û�б�ѡ��
	{
		dcContext.SetTextColor( crText );					
		dcContext.SetBkColor( crNormal );					
		dcContext.FillSolidRect( &rBlockRect, crNormal );	
	}
	//���������ڽ���״̬
	if ( iState & ODS_FOCUS )								
		dcContext.DrawFocusRect( &rItemRect );				

	//������������
	rTextRect.left  +=  ( iFourthWidth + 2 );					
	rTextRect.top  +=  2;										
	
	//����ͼ������
	rBlockRect.DeflateRect( CSize( 2, 2 ) );				
	rBlockRect.right = iFourthWidth;						
	
	//��ʾ����(��ɫ��)
	if ( iItem  !=  -1 )										
	{
		GetLBText( iItem, sColor );							

		//������ڲ�����״̬
		if ( iState & ODS_DISABLED )							
		{
			crColor = GetSysColor( COLOR_INACTIVECAPTIONTEXT );
			dcContext.SetTextColor( crColor );				
		}
		else //�������һ��״̬
			crColor = GetItemData( iItem );					

		dcContext.SetBkMode( TRANSPARENT );				

		dcContext.TextOut( rTextRect.left, rTextRect.top,sColor );									

		//�����ɫ����
		dcContext.FillSolidRect( &rBlockRect, crColor );	
		
		//���߿�
		dcContext.FrameRect( &rBlockRect, &brFrameBrush );	
	}
	//����CDC����
	dcContext.Detach();										
}


//����ָ����ɫ���ƺ���ɫֵ���б���
int	CColorPickerCB::AddColor(COLORREF crColor )
{
	int	  iItems = GetCount();
	TCHAR szName[20] = {0};
	if ( 0x804 == g_LangID )
	{
		_stprintf_s(szName,20, _T("��%d"), iItems);
	}
	else
	{
		_stprintf_s(szName,20, _T("Lay%d"), iItems);
	}
	
	int iItem = InsertString( iItems, szName );						
	if ( iItem  !=  LB_ERR )
	{
		SetItemData( iItem, crColor );
	}

	return  iItem;										
}


	
