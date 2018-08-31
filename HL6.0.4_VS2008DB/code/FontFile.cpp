// FontFile.cpp: implementation of the CFontFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FontFile.h"
#include "HL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFontFile::CFontFile()
{
	m_head.uCode=NUMCODE;
	m_head.uAscNum=NUM_ASCII;
	m_head.dbVersion=1.3;
	m_head.uDotNum=NUM_DOTOFCHAR;
	m_strFileName="\\dotfont.dat";
	m_strFilePath=_T("");
	m_lOffPos=sizeof(FontFileHead);
	m_nFileIsReading=-1;
	m_pFile=NULL;
}

CFontFile::~CFontFile()
{
	if(m_pFile)
	{
		m_pFile->Close();
		delete m_pFile;
		m_pFile=NULL;
	}
	PntStruct *pChar;
	int nSize=m_arDotChar.GetSize();
	if(nSize>0)
	{
		for(int i=0;i<nSize;i++)
		{
			pChar = (PntStruct *)m_arDotChar.GetAt(i);
			delete pChar;
		}
		m_arDotChar.RemoveAll();
		ASSERT(0==m_arDotChar.GetSize()); // Yes it is.
	}
	pChar=NULL;
}

CFontFile::LookUpAllData(CStringArray* pAr)
{
	if(ChangFileOpenFlag())
	{
		int nAsc=0;
		int nPoint=0;
		int ArrayStyle=0;
		long lOff=0;
		for(int i=0;i<NUM_ASCII;i++)
		{
			nAsc=0;
	    	nPoint=0;
			lOff=m_lOffPos+i*sizeof(PntStruct);
			m_pFile->Seek(lOff,CFile::begin);
	        m_pFile->Read(&nAsc,sizeof(int));
			m_pFile->Read(&ArrayStyle,sizeof(int));
		    m_pFile->Read(&nPoint,sizeof(int));
		    if(!nPoint)
			{				
			    continue;
			}
			char Char[3]={0,0,0};
		    if(32>nAsc)
			{			
			    Char[0]='\\';
			    Char[1]=m_head.uCode+nAsc;
			}
		    else
			{
			   Char[0]=nAsc;
			}
			CString str=CString(Char);
			str.TrimRight();
			pAr->Add(str);
		}
	}
	else
	{
		delete pAr;
		pAr = NULL;
	}
}

PntStruct* CFontFile::GetDataOfChar(char c)
{
	if(c<0) return NULL;
	if(m_pFile==NULL||m_nFileIsReading!=1)
	{
		if(!ChangFileOpenFlag()) return NULL;
	}
	int offSize=c*sizeof(PntStruct)+m_lOffPos;    
	m_pFile->Seek(offSize,CFile::begin);
	PntStruct* pnt=new PntStruct;
	int temp;
	int ntemp;
	BOOL bOn;
	int height=7;
	m_pFile->Read(&temp,sizeof(int));
	pnt->ASCNum=temp;
	m_pFile->Read(&temp,sizeof(int));
	pnt->ArrayStyle=temp;
	switch(temp)
	{
	case 0:
		height=7;
		break;
	case 1:
		height=9;
		break;
	case 2:
		height=12;
		break;
	default:
		height=7;
	}
	m_pFile->Read(&temp,sizeof(int));
	pnt->nPoint=temp;
	if(temp==0)
	{
		pnt->nPoint=1;
		pnt->point[0].bOn=FALSE;
		pnt->point[0].x=0;
		pnt->point[0].y=0;
		m_arDotChar.Add(pnt);
		return NULL;
	}

	for(int i=0;i<temp;i++)
	{
		m_pFile->Read(&bOn,sizeof(BOOL));
		pnt->point[i].bOn=bOn;
		m_pFile->Read(&ntemp,sizeof(int));
		pnt->point[i].x=ntemp;
		m_pFile->Read(&ntemp,sizeof(int));
		pnt->point[i].y=ntemp-height;
		/*
		if(ntemp>=0)
		{
			pnt->point[i].y=ntemp-height;
		}
		else
		{
			pnt->point[i].y=ntemp;
		}
		*/
	}
	
	return pnt;
}

BOOL CFontFile::WriteData(PntStruct *pPnt)
{
	if(pPnt->ASCNum<0){
		AfxMessageBox("Error character!");
		return NULL;
	}
	if(m_pFile==NULL||m_nFileIsReading!=0){
		if(!ChangFileOpenFlag(0))
			return NULL;
	}
	long loff=sizeof(FontFileHead);
	loff+=(pPnt->ASCNum)*sizeof(PntStruct);
	int nTmp=0;
	nTmp=pPnt->ASCNum;
	m_pFile->Seek(loff,CFile::begin);
	m_pFile->Write(&nTmp,sizeof(int));
	nTmp=pPnt->ArrayStyle;
	m_pFile->Write(&nTmp,sizeof(int));
	nTmp=pPnt->nPoint;
	m_pFile->Write(&nTmp,sizeof(int));
	int nPoint=0;
	BOOL bOn=TRUE;
	for(int i=0;i<nTmp;i++)
	{
		bOn=pPnt->point[i].bOn;
		m_pFile->Write(&bOn,sizeof(BOOL));
		nPoint=pPnt->point[i].x;
		m_pFile->Write(&nPoint,sizeof(int));
		nPoint=pPnt->point[i].y;
		m_pFile->Write(&nPoint,sizeof(int));
	}
	return TRUE;
}

FontFileHead* CFontFile::ReadFileHead()
{
	FontFileHead* head=new FontFileHead;
	m_pFile->SeekToBegin();
	m_pFile->Read(&head->dbVersion,sizeof(double));
	m_pFile->Read(&head->uAscNum,sizeof(UINT));
	m_pFile->Read(&head->uCode,sizeof(UINT));
	m_pFile->Read(&head->uDotNum,sizeof(UINT));
	return head;
}

BOOL CFontFile::WriteFileHead()
{
	m_pFile->SeekToBegin();
	m_pFile->Write(&m_head.dbVersion,sizeof(double));
	m_pFile->Write(&m_head.uAscNum,sizeof(UINT));
	m_pFile->Write(&m_head.uCode,sizeof(UINT));
	m_pFile->Write(&m_head.uDotNum,sizeof(UINT));
	return TRUE;
}

int CFontFile::ReadTextDataFile(CString fileName)
{
	CStdioFile* pFile=new CStdioFile();
	if(fileName==_T("")){
	    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,"HL font data file(*.txt)|*.txt||");
	    if(IDOK!=dlg.DoModal()){
		    return -1;
		}
	    fileName=dlg.GetFileName();
	}
	int handle=pFile->Open(fileName,CFile::modeRead);
	if(!handle){
		AfxMessageBox("Open font data file error!");
		return -1;
	}
	int nTemp=0;
	CString str;
	m_arDotChar.RemoveAll();
	
	while(pFile->GetPosition()!=CFile::end)
	{
		char* pChar=new char[6];
		PntStruct* pPnt=new PntStruct;
		pFile->ReadString(str);
		if(6<str.GetLength()){
			AfxMessageBox("Open font data file error!");
			pFile->Close();
			return 0;
		}
		strcpy(pChar,str);
		nTemp=atoi(pChar);
		pPnt->ASCNum=nTemp;
		pFile->ReadString(str);
		strcpy(pChar,str);
		nTemp=atoi(pChar);
		char* pStr=new char[3];
		BOOL bOn=TRUE;
		pPnt->ArrayStyle=0;
		if(nTemp>4)
		   pPnt->nPoint=nTemp;
		if(nTemp<1)
			break;
		for(int i=0;i<pPnt->nPoint;i++)
		{
			pFile->ReadString(str);
			strcpy(pChar,str);
			char* pXY=new char;
			char* pOn=new char;
			pXY=strchr(pChar,',');
			if(pXY==NULL){
				AfxMessageBox("Open font data file error!");
				pFile->Close();
				return 0;
			}
			memcpy(pStr,pChar,pXY-pChar);
			nTemp=atoi(pStr);
			pPnt->point[i].x=nTemp;
			pOn=strchr(pXY+1,',');
			if(pOn!=NULL)
			{				
				nTemp=atoi(pOn+1);
				bOn=nTemp;
				pStr=new char[3];
				memcpy(pStr,pXY+1,pOn-pXY-1);
				nTemp=atoi(pStr);
			}
			else
			{
				nTemp=atoi(pXY+1);
			}			
			pPnt->point[i].y=nTemp;
			pPnt->point[i].bOn=bOn;

		}
		m_arDotChar.Add(pPnt);
	}
	pFile->Close();
	delete pFile;
	pFile=NULL;
	return 1;
}

BOOL CFontFile::ClearDataFile()
{
	if(!ChangFileOpenFlag(2)){
		AfxMessageBox("Open font data file error!");
		return FALSE;
	}
	WriteFileHead();

	PntStruct pnt;
	pnt.ASCNum=0;
	pnt.ArrayStyle=0;
	pnt.nPoint=0;
	for(int i=0;i<NUM_DOTOFCHAR;i++)
	{
		pnt.point[i].bOn=FALSE;
		pnt.point[i].x=0;
		pnt.point[i].y=0;
	}
	for(int j=0;j<NUM_ASCII;j++)
	{
		m_pFile->Write(&pnt,sizeof(PntStruct));
	}
	return TRUE;

}

BOOL CFontFile::SaveTextFile()
{
	if(!ChangFileOpenFlag(2))
		return FALSE;

	int nAr=m_arDotChar.GetSize();
	if(!nAr)
		return FALSE;
	PntStruct* spnt=new PntStruct;;
	for(int h=0;h<nAr;h++)
	{
		spnt=m_arDotChar.GetAt(h);
		WriteData(spnt);
	}
	m_arDotChar.RemoveAll();
	return TRUE;
}

BOOL CFontFile::Close()
{
	if(m_pFile!=NULL)
	{
		m_pFile->Close();
		delete m_pFile;
		m_pFile=NULL;
		m_nFileIsReading=-1;
	}
	return TRUE;
}

CString CFontFile::GetFileName(CString filename)
{
	if(filename==_T(""))
	{
		m_strFilePath = ::g_sys.GetInstallPath();
		filename=m_strFilePath+m_strFileName;
	}
	m_strFilePathName=filename;
	return filename;
}

BOOL CFontFile::InitFontFile(double dbVer)
{
	GetFileName();
	if(!CheckVersion(dbVer,NUM_ASCII,NUMCODE,NUM_DOTOFCHAR)){
		AfxMessageBox("Font file version is error!\rNew file will be created now!");
		ClearDataFile();
	}
	return TRUE;
}

BOOL CFontFile::Open(unsigned int flag)
{
	Close();
	m_pFile=new CFile();
	if(m_pFile->Open(m_strFilePathName,flag)==0)
		return FALSE;
	return TRUE;
}

BOOL CFontFile::ChangFileOpenFlag(int nRead)
{
	if(m_nFileIsReading==nRead)
	{
		return TRUE;	
	}
	Close();
	m_nFileIsReading=nRead;
	m_pFile=new CFile;
	unsigned int flag;
	if(m_strFilePathName==_T(""))
	{
		GetFileName();
	}
	if(nRead==2)//Create a new file
	{
		if(m_pFile->Open(m_strFilePathName,CFile::modeWrite|CFile::modeCreate)==0)
		{
			m_pFile=NULL;
			return FALSE;
		}
		else
			return TRUE;
	}
	flag=(m_nFileIsReading>0)?(CFile::modeRead):(CFile::modeWrite);//m_nFileIsReading==1---Read;m_nFileIsReading==0---Write
	if(m_nFileIsReading>=0)
		if(m_pFile->Open(m_strFilePathName,flag)==0)
		{
			m_pFile=NULL;
			return FALSE;
		}
		return TRUE;
}

BOOL CFontFile::CheckVersion(double dbVer, UINT uAscNum, UINT uCode,UINT uDotNum)
{
	if(ChangFileOpenFlag())	
	{
		FontFileHead* head=ReadFileHead();		
		if(head->uAscNum==uAscNum&&head->uCode==uCode&&head->uDotNum==uDotNum&&head->dbVersion==dbVer)
		   return TRUE;
	}
	return FALSE;
}

BOOL CFontFile::DelChar(char c)
{
	if(c<0){
		AfxMessageBox("Error character!");
		return FALSE;
	}
	if(m_pFile==NULL||m_nFileIsReading!=0)
	{
		if(!ChangFileOpenFlag(0))
			return FALSE;
	}
	long loff=sizeof(FontFileHead);
	loff+=c*sizeof(PntStruct);
	m_pFile->Seek(loff,CFile::begin);
	PntStruct pnt;
	pnt.ASCNum=0;
	pnt.ArrayStyle=0;
	pnt.nPoint=0;
	for(int i=0;i<NUM_DOTOFCHAR;i++)
	{
		pnt.point[i].bOn=FALSE;
		pnt.point[i].x=0;
		pnt.point[i].y=0;
	}	
	m_pFile->Write(&pnt,sizeof(PntStruct));
	return TRUE;
}

BOOL CFontFile::CheckVersion(double dbVer)
{
	if(ChangFileOpenFlag())	
	{
		FontFileHead* head=ReadFileHead();		
		if(head->dbVersion==dbVer)
		   return TRUE;
	}
	return FALSE;
}

BOOL CFontFile::IsCharExist(char c)
{
	if(c<0){
		AfxMessageBox("Error character!");
		return FALSE;
	}
	if(m_pFile==NULL||m_nFileIsReading!=1)
	{
		if(!ChangFileOpenFlag())
			return FALSE;
	}
	long loff=sizeof(FontFileHead);
	loff+=c*sizeof(PntStruct);
	m_pFile->Seek(loff,CFile::begin);
	int n=0;
	m_pFile->Read(&n,sizeof(int));
	return n;

}

