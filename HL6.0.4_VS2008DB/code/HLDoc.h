// HLDoc.h : interface of the CHLDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLDOC_H__B8BC184F_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_HLDOC_H__B8BC184F_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DocPipe.h"
#include "MgrText.h"
#include "XDC.h"
#include "XTracker.h"
#include "Quad.h"	// Added by ClassView
#include "stdafx.h"
#include "SortListObj.h"
#include "AutoSortListObj.h"

#include "MgrPathOptimize.h"
#include "NodeCircle.h"
#include "Chain3D.h"

#include "Dlg3DTransform.h"
#include "AssistantObj.h"

// 铺助对象
extern CAssistantObj g_ObjDrawAssist;
// 是否启动
extern BOOL g_bEnableAssist;

struct HDFILEHEAD
{
	HDFILEHEAD()
	{
		memset(szFlag,0,4);
		memset(szVersion,0,6);
		n2DOffset = n3DOffset = 0;
	}
	char szFlag[4];
	char szVersion[6];
	int   n2DOffset;
	int   n3DOffset;
};

class CHLDoc : public CDocument
{
protected: // create from serialization only
	CHLDoc(); 
	DECLARE_DYNCREATE(CHLDoc)

// Attributes
public:
// Operations
public:
    inline CCommandProcessor& GetCommandProcessor() { return m_cmdProc; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHLDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public: 
	CChain		m_chain;	//所有绘图对象列表
	CChain3D	m_chain3d;
	BOOL			m_b3D;

	void AddRect(CQuad &quad,CWnd *pWnd);
	//设置或取出chain数据
	void SetChain(const CChain &chain );
	CChain & GetChain();

	//增加节点
	void AddNodeCmd(CNode *pNode, int nObjType = -1);
	void AddNode(CNode *pNode,    int nObjType = -1);

	//拉变
	void Drag(CDot &dot,double ratioX,double ratioY,double moveX,double moveY, double moveZ = 0);
	void DragCmd(CDot &dot,double ratioX,double ratioY,double moveX,double moveY, double moveZ = 0);

	//拉变边框
	void DragFrame(CQuadRangle& frame, CDot &dot,double ratioX,double ratioY,double moveX,double moveY);
	void DragFrameCmd(CQuadRangle& frame, CDot &dot,double ratioX,double ratioY,double moveX,double moveY);
	
	//缩放
	void ScaleCmd(CDot dot,double ratioX,double ratioY);
	void Scale(CDot dot,double ratioX,double ratioY);

	//移动
	void MoveCmd(double moveX,double moveY,double moveZ);
	void Move(double moveX,double moveY, double moveZ);

	//旋转
	void RotateCmd(CDot dot, double angle, int nPlane = 0);
	void Rotate(CDot dot, double angle, int nPlane = 0);

	//切变
	void ShearCmd(CDot dot, double dbAngleX, double dbAngleY);
	void Shear(CDot dot, double dbAngleX, double dbAngleY);

	//删除
	void DeleteCmd();
	void Delete();

	//拷贝
	void CopyCmd();
	void Copy();

	//框选
	//右键选中0.
	//右键属性修改
	void DrawsetCmd();

	//成组
	void GroupCmd();

	//取消成组
	void UngroupCmd();

	//群组
	void UnionCmd();

	//取消群组
	void UnunionCmd();

	//PASTE
	void   PasteCmd();
	void   FillCmd(FILLDATA fill);

	//阵列
	void    ArrayCmd(CArray<OBJPOS, OBJPOS> &dtArray);

	CQuad GetTrackerQuad();
	void    SetTrackerQuad(CQuad &quad,double angle = 0);
    CQuad GetAllLimit();
	CQuad GetLimit();

	CQuadRangle GetQuadRangle();
	//预览边框
	BOOL GetFrameDots(CArray<CDot,CDot>& frameDots);
	void   SetQuad(CQuad quad);
	void   SetSelect(BOOL bFlag);
	BOOL  IsSelectedObj();
	void   SetGroupNum(UINT nChar);

	void   Draw(XDC *pDC);
	int     GetStatus(){return m_nStatus;};

	void   SetLayerColor(int index, COLORREF color);
	BOOL OnLockLayer(int nLayer);
    BOOL OnUnLockLayer(int nLayer);

	void   ConvertSelToOneTree();
	void   ConverStrokeCmd();

	BOOL SelGroupByNum(UINT nChar, CWnd *pWnd);
	void   SelGroupByNumCmd(int nNum, CWnd *pWnd);
	void   SetGroupObjSel(CGroupObj *pGroupObj);

	BOOL  isIntersect(CDot&p1 , CDot& p2 , CDot& p3 , CDot& p4 );

	void   DelObjCmd(DWORD dwGroupObj);
	void   DelObj(CGroupObj *pGroupObj,BOOL bRefresh = TRUE);
	void   OnMAlignCmd(int nType);
	void   EditWheelCmd();
	
	double GetSelTotalLength();

	BOOL GetFillProp(FILLDATA &fillOld);
	void	UpdateFill(FILLDATA &fillNew);
	void	InitSelRowSort(CWnd* pWnd);
	BOOL HasObjForProcess();
	void	EditWeldCmd();

	virtual ~CHLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	void Serialize(FILE* fp,BOOL bFlag,BOOL bUnicode);
	void LabelTo3D(BOOL bForce = FALSE);

	void LabelTo3DOptimize(BOOL bForce = FALSE);
	BOOL InitMotionMarkParam();
	
// Generated message map functions
protected:
	//{{AFX_MSG(CHLDoc)
	afx_msg void OnGroup();
	afx_msg void OnUnGroup();
	afx_msg void OnUpdateGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUnGroup(CCmdUI* pCmdUI);
	afx_msg void OnMarkNormal();
    afx_msg void OnRedPreview();
    afx_msg void OnUpdateRedPreview(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnDrawFill();
	afx_msg void OnChangeCenter();
	afx_msg void OnUpdateChangeCenter(CCmdUI* pCmdUI);
	afx_msg void OnChangeCcw();
	afx_msg void OnUpdateChangeCcw(CCmdUI* pCmdUI);
	afx_msg void OnChangeCw();
	afx_msg void OnUpdateChangeCw(CCmdUI* pCmdUI);
	afx_msg void OnChangeMirrorX();
	afx_msg void OnUpdateChangeMirrorX(CCmdUI* pCmdUI);
	afx_msg void OnChangeMirrorY();
	afx_msg void OnUpdateChangeMirrorY(CCmdUI* pCmdUI);
	afx_msg void OnMarkPreview();
	afx_msg void OnDrawSet();
	afx_msg void OnArray();
	afx_msg void OnUpdateArray(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnDrawBmp();
	afx_msg void OnSetLayer();
	afx_msg void OnDrawBartext();
	afx_msg void OnExportPlt();
    afx_msg void OnStatusButton();
	afx_msg void OnMarkSpc();
	afx_msg void OnUpdateMarkSpc(CCmdUI* pCmdUI);
    afx_msg void OnMarkFast();
	afx_msg void OnUpdateMarkFast(CCmdUI* pCmdUI);
	afx_msg void OnDownLoad();
	afx_msg void OnUpdateDownLoad(CCmdUI* pCmdUI);
	afx_msg void OnClearDownLoad();
	afx_msg void OnEditDBCopy();
	afx_msg void OnEditBDPaste();
	afx_msg void OnUpdateConvertOneTree(CCmdUI* pCmdUI);
    afx_msg void OnConvertOneTree();
	afx_msg void OnPathOptimize();
	afx_msg void OnNormalPath();
	afx_msg void OnUpdateJointstrokeCurdoc(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePathOptimize(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileImport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileImportVector(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileImportBmp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileImportNc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModuleSwitch(CCmdUI* pCmdUI);
	afx_msg void OnModuleSwitch();
	afx_msg void OnUpdate3dViewXYZ(CCmdUI* pCmdUI);
	afx_msg void On3dViewXYZ();
	afx_msg void OnUpdate3dViewXY(CCmdUI* pCmdUI);
	afx_msg void On3dViewXY();
	afx_msg void OnUpdate3dViewYZ(CCmdUI* pCmdUI);
	afx_msg void On3dViewYZ();
	afx_msg void OnUpdate3dViewXZ(CCmdUI* pCmdUI);
	afx_msg void On3dViewXZ();
	afx_msg void OnUpdateModuleLoad3D(CCmdUI* pCmdUI);
	afx_msg void OnModuleLoad3D();
	afx_msg void OnUpdateModuleTransform3D(CCmdUI* pCmdUI);
	afx_msg void OnModuleTransform3D();
	afx_msg void OnUpdateMarkNormal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDownMul(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClearDownload(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawDrag(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExportPlt(CCmdUI* pCmdUI);
	afx_msg void OnUnion();
	afx_msg void OnUpdateUnion(CCmdUI* pCmdUI);
	afx_msg void OnUnunion();
	afx_msg void OnUpdateUnunion(CCmdUI* pCmdUI);
	afx_msg void OnMotioninfoSet();
	afx_msg void OnRotatewheelSet();
	afx_msg void OnMotionMarkSet();  // 动态运动打标设置
	afx_msg void OnEditWheel();
	afx_msg void OnUpdateEditWheel(CCmdUI* pCmdUI);
	afx_msg void OnMarkWheel();
	afx_msg void OnUpdateMarkWheel(CCmdUI* pCmdUI);
	afx_msg void OnMoveMark();
	afx_msg void OnUpdateMoveMark(CCmdUI* pCmdUI);
	afx_msg void OnBMPIntensity();
	afx_msg void OnUpdateBMPIntensity(CCmdUI* pCmdUI);
	afx_msg void OnEditRuler();
	afx_msg void OnUpdateEditRuler(CCmdUI* pCmdUI);
	afx_msg void OnModifyZ();
	afx_msg void OnUpdateMotioninfoSet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRotatewheelSet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMotionMarkSet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSaveAS(CCmdUI *pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnCheckFocus();
	afx_msg void OnUpdateCheckFocus(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int			m_nStatus;				//当前选择状态
	CQuad	m_quadPrevFram;   //预览边框
	BOOL		m_bEditCut;
	CCommandProcessor m_cmdProc;	//命令处理器
	CLay		m_lay;

	DOUBLEOUTPARA m_DBCopyPara;

public:
	MOTOR	m_motor;			//打标结束后电机移动量
	
	int		GetMarkUnit();
	BOOL PreprocessMark();
	void	SaveMarkFile(CString strFileName);
	void	Mark(HWND hWnd,int repeat);
	void	SetSelectFlag(BOOL *pFlag);
	void	GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray);	//阵列时从屏幕取位置
	int		GetMenuIndex();
	void	SetCurLayer(int cur);
	int		GetLaySize();

    CLayer	GetCurLayer();
	CLayer	GetLayer(int index = 0);
	void	SetLayer(CLayer &layer);

	CLay	&GetLay();
	void	SetLay(const CLay &lay);	
	void	LayChange(int mode,CLayer &ly);
	void	SetLayCmd(int mode,CLayer &ly);

	int		GetCur();
	CQuad GetQuadTracker(){return m_quadTracker;};

	void SetSelectedObjbeMarked(BOOL bFlag);
	void SetObjSelInQuadList();
	void RedPreview(BOOL bIsRed = TRUE, BOOL bIsArray = FALSE);

public:	
	void MoveAccurate(double MoveX,double MoveY);
	void JoinPoint(CWnd *pWnd);
	void ConvertCmd(BOOL bPopMsg = TRUE);
	void SplitDot(CWnd *pWnd,double dbPoint);
	void ChangeQuadCmd(CPoint &point,CWnd *pWnd);
	void ReLoadStrokeList();
	void AddPoint(CPoint &point,CWnd *pWnd);
	BOOL HitOtherObject(CPoint &point,CWnd *pWnd);
	BOOL HaveSelectObject();
	void MultiSelect(CQuad &quad, BOOL bUniSel = FALSE);
	void CaptureObjectCmd(CQuad &quad, BOOL bUniSel = FALSE);
	BOOL SetStrokeList(StrokeList *pStrokeList,CPoint &point,CWnd *pWnd);
	void DelDot(CWnd *pWnd);
	void DotEdit(XTracker *pTrack,CPoint &point,CWnd *pWnd);
	void AddNodeCmd(CTree *pTree);
	void AddTree(CTree *pAddTree);
	void GetSelectedStrokeList(StrokeList *pStrokeList);
	void UpdatePerUnit();
	void SetLayerCmd(int nLayer, int nFillLayer, int nFillScalLayer);
	BOOL GetLayerSetFromDlg(int &nLayer, int &nFillLayer, int &nFillScalLayer);
	BOOL OutOfLimit(BOOL bCheckX = TRUE,BOOL bCheckY = TRUE);
	void SelectDraw(CGroupObj *pObj);
	void Changeconvert(BOOL bPopMsg = TRUE);
	void FilterPoint();
	void FilterPointCmd();

    void OnKillAngle();
	void KillAngleCmd();
	void OnExtendObj(BOOL bIsKillOldObj);
	void ExtendObjCmd(double dbSpace, BOOL bIsKillOldObj);
	void IniSort(CWnd* pWnd);
	void DrawMark(CDC* pDC, CWnd *pWnd);
	void SortAllObj();

	BOOL InitASort(CWnd* pWnd);
	void ExitASort();
	void SetObjProp();
	void InitSelSort(CWnd* pWnd);
	
	double GetAngle(CDot &dtS, CDot &dtE);
	double Distance(CDot &sDot, CDot &eDot);
	void CreateNewRect(CRect &lRt, CRect &cRt, CRect &nRt);

	void ObjListLoad();
	void ObjListUnLoad();

	void MAlignCmd(int nType);
	BOOL GetLockFlg(int index);
	BOOL IsHaveTextOrCodeBar();

	CMgrText* GetMgrTextObj(){return &m_chain.m_MgrText;};

	void UpdateObjMarkSet(OBJMARKSET &ObjMarkSet);
	BOOL GetObjMarkSet(OBJMARKSET &ObjMarkSet);

	void EditTextNode(CNode* pNode);
	void EditBarNode(CNode* pNode);
	
	void OnSelectLayer(int nItem);
	void OnUnselectLayer(int nItem);
	void PathOptimizeCmd();

public:
	BOOL		GetSelObjLockFlg();
	CQuad	m_quadTracker;	//当前选中的对象的范围大小（逻辑坐标）
	XTracker *m_pTracker;
	DOCEXINFO  m_docExtent;

	int		m_nPosType;
	CArray<SORTNODE,SORTNODE>  m_Sort;
    CArray<ASNODE,ASNODE>			m_ASort;
	CArray<GNODE,GNODE>				m_GSort;
    CArray<SELQUAD ,SELQUAD>		m_QuadList;
	CSortListObj							m_SortListObj;
	CAutoSortListObj						m_ASortListObj;

	BOOL m_bExternOpt;
	inline void DispSelObjProp();
	void AdjustObjectOrder(int nFrom, int nTo);
	
	void JoinStrokeCmd(BOOL bPopMsg = TRUE);  //
	void PathOptimize();
	void NormalPath();
	CNode* GetSelNode(int nNodeType);
	
	void LockAllLayer();
	void UnLockAllLayer();
	
	CMgrPathOptimize m_MgrPathOptimize;
	BOOL m_bPathOptimize;
	int		m_nPathOptimizeType;
	BOOL m_bPathModify;         //打标内容变动，需重新进行快速排序

	BOOL m_bInit3D;

	void InitChain3D();

	//取Track
	BOOL CheckSetTrack(void);
	void RefreshSelectQuadList();
	void RefreshTrackerList();
	void SelSwitich(BOOL bNext);

	//是否允许进行三维变换
	BOOL IsEnable3DEdit();

protected:
	virtual BOOL SaveModified();
	
public:
	void TransPreprocess(TRANSFORM &tsf, int nType);
	void TransByTransform( CArray<StrokeList*, StrokeList*> &arrayList, TRANSFORM &tsf );
	void CreateStrokeFrom3D( CStroke *pStroke,  LABEL_3DSTROKE *p3dStroke);

	BOOL m_b3dTransForce;
	afx_msg void OnEditWeld();
	afx_msg void OnUpdateEditWeld(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChgCard(CCmdUI *pCmdUI);

	// add by zhq
	BOOL GetAllStrokeList(CArray<StrokeList*, StrokeList*> &arrayList, StrokeList* pStrokeList);
	void EditInterfilter();
	afx_msg void OnEditInterfilter();
	afx_msg void OnUpdateEditInterfilter(CCmdUI *pCmdUI);

	


	void LoadAssistantPoints();
	afx_msg void OnAssistantPoint();
	afx_msg void OnUpdateAssistantPoint(CCmdUI *pCmdUI);

	// 3d模型移动
	afx_msg void OnUpdateModuleAlign(CCmdUI* pCmdUI);
	afx_msg void OnModuleAlign();

	//////////////////////////////////////////////////////////////////////////
	void BroadenStrokeList();
	afx_msg void OnEditBroaden();
	afx_msg void OnUpdateEditBroaden(CCmdUI *pCmdUI);
	afx_msg void OnComunication();
	afx_msg void OnSetdatabase();
	afx_msg void OnDatabase();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HLDOC_H__B8BC184F_DE24_11D3_845D_444553540001__INCLUDED_)
