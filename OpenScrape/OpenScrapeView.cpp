// OpenScrapeView.cpp : implementation of the COpenScrapeView class
//
#include "stdafx.h"
#include "OpenScrape.h"
#include "OpenScrapeDoc.h"
#include "OpenScrapeView.h"
#include "MainFrm.h"
#include "debug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenScrapeView

IMPLEMENT_DYNCREATE(COpenScrapeView, CView)

BEGIN_MESSAGE_MAP(COpenScrapeView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// COpenScrapeView construction/destruction

COpenScrapeView::COpenScrapeView()
{
    __SEH_SET_EXCEPTION_HANDLER

	__SEH_HEADER
		
	black_pen.CreatePen(PS_SOLID, 1, COLOR_BLACK);
	green_pen.CreatePen(PS_SOLID, 1, COLOR_GREEN);
	red_pen.CreatePen(PS_SOLID, 1, COLOR_RED);
	blue_pen.CreatePen(PS_SOLID, 1, COLOR_BLUE);
	white_dot_pen.CreatePen(PS_DOT, 1, COLOR_WHITE);
	black_dot_pen.CreatePen(PS_DOT, 1, COLOR_BLACK);
	null_pen.CreatePen(PS_NULL, 0, COLOR_BLACK);

	white_brush.CreateSolidBrush(COLOR_WHITE);
	gray_brush.CreateSolidBrush(COLOR_GRAY);
	red_brush.CreateSolidBrush(COLOR_RED);
	yellow_brush.CreateSolidBrush(COLOR_YELLOW);

	dragging = false;
	dragged_region = -1;

	drawing_rect = drawing_started = false;
	hCurDrawRect = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DRAWRECTCURSOR));
	hCurStandard = ::LoadCursor(NULL, IDC_ARROW);

    __SEH_LOGFATAL("COpenScrapeView::COpenScrapeView : \n");
}

COpenScrapeView::~COpenScrapeView()
{
}

BOOL COpenScrapeView::PreCreateWindow(CREATESTRUCT& cs)
{
	__SEH_HEADER
		
	return CView::PreCreateWindow(cs);

    __SEH_LOGFATAL("COpenScrapeView::PreCreateWindow : \n");
}

// COpenScrapeView drawing

void COpenScrapeView::OnDraw(CDC* pDC)
{
	__SEH_HEADER
		
	COpenScrapeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPen		*pTempPen, oldpen;
	CBrush		*pTempBrush, oldbrush;	
	HDC			hdc = *pDC;
	HDC			hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); 
	HDC			hdcCompatible = CreateCompatibleDC(hdcScreen);
	HBITMAP		hbmp = CreateCompatibleBitmap(hdcScreen, 
										      pDoc->attached_rect.right - pDoc->attached_rect.left, 
										      pDoc->attached_rect.bottom - pDoc->attached_rect.top);
	HBITMAP		old_bitmap1, old_bitmap2;
	int			i, N;
	CMainFrame	*pmyframe = (CMainFrame*)AfxGetMainWnd(); 
	RECT		crect;

	// Draw attached window's bitmap as background
	if (pDoc->attached_bitmap)
	{
		old_bitmap1 = (HBITMAP) SelectObject(hdcCompatible, pDoc->attached_bitmap);
		old_bitmap2 = (HBITMAP) SelectObject(hdc, hbmp);
		BitBlt(hdc, 0, 0, 
			   pDoc->attached_rect.right - pDoc->attached_rect.left,
			   pDoc->attached_rect.bottom - pDoc->attached_rect.top,
			   hdcCompatible, 0, 0, SRCCOPY);
		SelectObject(hdc, old_bitmap2);
		SelectObject(hdcCompatible, old_bitmap1);
	}
	else
	{
		pTempPen = (CPen*)pDC->SelectObject(null_pen);
		oldpen.FromHandle((HPEN)pTempPen);
		pTempBrush = (CBrush*)pDC->SelectObject(white_brush);
		oldbrush.FromHandle((HBRUSH)pTempBrush);

		GetClientRect(&crect);
		pDC->Rectangle(&crect);

		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}

	// Draw all region rectangles
	if (pmyframe->show_regions)
	{
		N = (int) pDoc->trans.map.r$.GetSize();
		
		for (i=0; i<N; i++)
		{
			if ( (i==dragged_region && dragging) || (i==drawrect_region && drawing_rect && drawing_started) )
			{
				// Set pen and brush
				pTempPen = (CPen*)pDC->SelectObject(black_dot_pen);
				oldpen.FromHandle((HPEN)pTempPen);
				pTempBrush = (CBrush*)pDC->SelectObject(GetStockObject(NULL_BRUSH));
				oldbrush.FromHandle((HBRUSH)pTempBrush);

				pDC->Rectangle(pDoc->trans.map.r$[i].left-1, pDoc->trans.map.r$[i].top-1, pDoc->trans.map.r$[i].right+1, pDoc->trans.map.r$[i].bottom+1);

				pDC->SelectObject(oldpen);
				pDC->SelectObject(oldbrush);
			}

			else
			{
				// Set pen and brush
				pTempPen = (CPen*)pDC->SelectObject(red_pen);
				oldpen.FromHandle((HPEN)pTempPen);
				pTempBrush = (CBrush*)pDC->SelectObject(GetStockObject(NULL_BRUSH));
				oldbrush.FromHandle((HBRUSH)pTempBrush);

				pDC->Rectangle(pDoc->trans.map.r$[i].left-1, pDoc->trans.map.r$[i].top-1, pDoc->trans.map.r$[i].right+1, pDoc->trans.map.r$[i].bottom+1);

				pDC->SelectObject(oldpen);
				pDC->SelectObject(oldbrush);
			}
		}
	}

	// Clean Up
	DeleteObject(hbmp);
	DeleteDC(hdcCompatible);
	DeleteDC(hdcScreen);

    __SEH_LOGFATAL("COpenScrapeView::OnDraw : \n");
}


// COpenScrapeView diagnostics

#ifdef _DEBUG
void COpenScrapeView::AssertValid() const
{
	CView::AssertValid();
}

void COpenScrapeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenScrapeDoc* COpenScrapeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenScrapeDoc)));
	return (COpenScrapeDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenScrapeView message handlers

void COpenScrapeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	__SEH_HEADER
		
	COpenScrapeDoc		*pDoc = GetDocument();
	int					i;
	CString				sel = theApp.m_TableMapDlg->m_TableMapTree.GetItemText(theApp.m_TableMapDlg->m_TableMapTree.GetSelectedItem());	
	HTREEITEM			temp_hti, imagepar_hti;
	bool				found;
	CString				text;

	// If we are drawing a rectangle for a region, handle that
	if (drawing_rect)
	{
		drawrect_start = point;
		drawing_started = true;
		
		for (i=0; i<pDoc->trans.map.r$.GetSize(); i++)
		{
			if (pDoc->trans.map.r$[i].name == sel)
			{
				drawrect_region = i;

				// Update internal structure
				pDoc->trans.map.r$[drawrect_region].left = point.x;
				pDoc->trans.map.r$[drawrect_region].top = point.y;
				pDoc->trans.map.r$[drawrect_region].right = point.x;
				pDoc->trans.map.r$[drawrect_region].bottom = point.y;

				// Update table map dialog
				text.Format("%d", point.x);
				theApp.m_TableMapDlg->m_Left.SetWindowText(text.GetString());
				theApp.m_TableMapDlg->m_Right.SetWindowText(text.GetString());
				text.Format("%d", point.y);
				theApp.m_TableMapDlg->m_Top.SetWindowText(text.GetString());
				theApp.m_TableMapDlg->m_Bottom.SetWindowText(text.GetString());

				Invalidate(false);
				i = (int) pDoc->trans.map.r$.GetSize() + 1;

			}
		}

	}

	// Otherwise...
	else
	{
		// Shift click means we want to drag the region
		if (nFlags & MK_SHIFT)
		{
			for (i=0; i<pDoc->trans.map.r$.GetSize(); i++)
			{
				if (point.x >= (LONG) pDoc->trans.map.r$[i].left-1 &&
					point.x <= (LONG) pDoc->trans.map.r$[i].right+1 &&
					point.y >= (LONG) pDoc->trans.map.r$[i].top-1 &&
					point.y <= (LONG) pDoc->trans.map.r$[i].bottom+1 &&
					(theApp.m_TableMapDlg->m_TableMapTree.GetSelectedItem() == NULL || pDoc->trans.map.r$[i].name == sel) )
				{
					dragging = true;
					dragged_region = i;
					drag_left_offset = point.x - pDoc->trans.map.r$[i].left;
					drag_top_offset = point.y - pDoc->trans.map.r$[i].top;
					Invalidate(false);
					i = (int) pDoc->trans.map.r$.GetSize() + 1;
				}
			}
		}


		// No shift means just select the region in the tree
		else
		{
			for (i=0; i<pDoc->trans.map.r$.GetSize(); i++)
			{
				if (point.x >= (LONG) pDoc->trans.map.r$[i].left-1 &&
					point.x <= (LONG) pDoc->trans.map.r$[i].right+1 &&
					point.y >= (LONG) pDoc->trans.map.r$[i].top-1 &&
					point.y <= (LONG) pDoc->trans.map.r$[i].bottom+1 &&
					pDoc->trans.map.r$[i].name != sel)
				{

					// Find parent node
					temp_hti = theApp.m_TableMapDlg->m_TableMapTree.GetRootItem();
					imagepar_hti = NULL;
					while (temp_hti && imagepar_hti==NULL)
					{
						if (theApp.m_TableMapDlg->m_TableMapTree.GetItemText(temp_hti) == "Regions")  imagepar_hti = temp_hti;
						temp_hti = theApp.m_TableMapDlg->m_TableMapTree.GetNextItem(temp_hti, TVGN_NEXT);
					}

					// Find correct leaf node
					temp_hti = theApp.m_TableMapDlg->m_TableMapTree.GetNextItem(imagepar_hti, TVGN_CHILD);
					found = false;
					while (temp_hti && !found)
					{
						if (pDoc->trans.map.r$[i].name == theApp.m_TableMapDlg->m_TableMapTree.GetItemText(temp_hti))  found = true;
						else temp_hti = theApp.m_TableMapDlg->m_TableMapTree.GetNextItem(temp_hti, TVGN_NEXT);
					}

					theApp.m_TableMapDlg->m_TableMapTree.SelectItem(temp_hti);

					i = (int) pDoc->trans.map.r$.GetSize() + 1;
				}
			}
		}
	}

	CView::OnLButtonDown(nFlags, point);

    __SEH_LOGFATAL("COpenScrapeView::OnLButtonDown : \n");
}

void COpenScrapeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	__SEH_HEADER
		
	COpenScrapeDoc		*pDoc = GetDocument();

	if (drawing_rect)
	{
		drawing_rect = false;
		drawing_started = false;

		pDoc->trans.map.r$[drawrect_region].left = drawrect_start.x<point.x ? drawrect_start.x : point.x;
		pDoc->trans.map.r$[drawrect_region].top = drawrect_start.y<point.y ? drawrect_start.y : point.y;
		pDoc->trans.map.r$[drawrect_region].right = drawrect_start.x>=point.x ? drawrect_start.x : point.x;
		pDoc->trans.map.r$[drawrect_region].bottom = drawrect_start.y>=point.y ? drawrect_start.y : point.y;

		theApp.m_TableMapDlg->m_DrawRect.OnBnClicked();
		Invalidate(false);
		theApp.m_TableMapDlg->Invalidate(false);
	}


	else if (dragging)
	{
		dragging = false;
		dragged_region = -1;
		Invalidate(false);
		theApp.m_TableMapDlg->Invalidate(false);
	}


	CView::OnLButtonUp(nFlags, point);

    __SEH_LOGFATAL("COpenScrapeView::OnLButtonUp : \n");
}

void COpenScrapeView::OnMouseMove(UINT nFlags, CPoint point)
{
	__SEH_HEADER
		
	COpenScrapeDoc		*pDoc = GetDocument();
	int					width, height;
	CString				text;

	if (drawing_rect && drawing_started)
	{
		// Update internal structure for selected region
		pDoc->trans.map.r$[drawrect_region].left = drawrect_start.x<point.x ? drawrect_start.x : point.x;
		pDoc->trans.map.r$[drawrect_region].top = drawrect_start.y<point.y ? drawrect_start.y : point.y;
		pDoc->trans.map.r$[drawrect_region].right = drawrect_start.x>=point.x ? drawrect_start.x : point.x;
		pDoc->trans.map.r$[drawrect_region].bottom = drawrect_start.y>=point.y ? drawrect_start.y : point.y;

		// Update table map dialog
		text.Format("%d", pDoc->trans.map.r$[drawrect_region].left);
		theApp.m_TableMapDlg->m_Left.SetWindowText(text.GetString());
		text.Format("%d", pDoc->trans.map.r$[drawrect_region].top);
		theApp.m_TableMapDlg->m_Top.SetWindowText(text.GetString());
		text.Format("%d", pDoc->trans.map.r$[drawrect_region].right);
		theApp.m_TableMapDlg->m_Right.SetWindowText(text.GetString());
		text.Format("%d", pDoc->trans.map.r$[drawrect_region].bottom);
		theApp.m_TableMapDlg->m_Bottom.SetWindowText(text.GetString());

		theApp.m_TableMapDlg->update_display();
		theApp.m_TableMapDlg->Invalidate(false);
		Invalidate(false);
		pDoc->SetModifiedFlag(true);
	}

	else if (dragging)
	{
		width = pDoc->trans.map.r$[dragged_region].right - pDoc->trans.map.r$[dragged_region].left;
		height = pDoc->trans.map.r$[dragged_region].bottom - pDoc->trans.map.r$[dragged_region].top;

		// Update internal structure for selected region
		pDoc->trans.map.r$[dragged_region].left = point.x - drag_left_offset;
		pDoc->trans.map.r$[dragged_region].top = point.y - drag_top_offset;
		pDoc->trans.map.r$[dragged_region].right = pDoc->trans.map.r$[dragged_region].left + width;
		pDoc->trans.map.r$[dragged_region].bottom = pDoc->trans.map.r$[dragged_region].top + height;

		// Update table map dialog
		text.Format("%d", pDoc->trans.map.r$[dragged_region].left);
		theApp.m_TableMapDlg->m_Left.SetWindowText(text.GetString());
		text.Format("%d", pDoc->trans.map.r$[dragged_region].top);
		theApp.m_TableMapDlg->m_Top.SetWindowText(text.GetString());
		text.Format("%d", pDoc->trans.map.r$[dragged_region].right);
		theApp.m_TableMapDlg->m_Right.SetWindowText(text.GetString());
		text.Format("%d", pDoc->trans.map.r$[dragged_region].bottom);
		theApp.m_TableMapDlg->m_Bottom.SetWindowText(text.GetString());

		theApp.m_TableMapDlg->update_display();
		theApp.m_TableMapDlg->Invalidate(false);
		Invalidate(false);
		pDoc->SetModifiedFlag(true);
	}

	CView::OnMouseMove(nFlags, point);

    __SEH_LOGFATAL("COpenScrapeView::OnMouseMove : \n");
}

BOOL COpenScrapeView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	__SEH_HEADER
		
	if (drawing_rect)
	{
		::SetCursor(hCurDrawRect);
		return true;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);

    __SEH_LOGFATAL("COpenScrapeView::OnSetCursor : \n");
}

void COpenScrapeView::blink_rect(void)
{
	__SEH_HEADER
		
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	CString				sel = theApp.m_TableMapDlg->m_TableMapTree.GetItemText(theApp.m_TableMapDlg->m_TableMapTree.GetSelectedItem());	

	CDC					*pDC = GetDC();
	CPen				*pTempPen, oldpen;
	CBrush				*pTempBrush, oldbrush;	
	int					i;

	if (dragging)  return;


	// Set pen and brush
	pTempPen = (CPen*)pDC->SelectObject(pDoc->blink_on ? green_pen : red_pen);
	oldpen.FromHandle((HPEN)pTempPen);					// Save old pen
	pTempBrush = (CBrush*)pDC->SelectObject(GetStockObject(NULL_BRUSH));
	oldbrush.FromHandle((HBRUSH)pTempBrush);			// Save old brush

	if (!dragging)
	{
		for (i=0; i<(int) pDoc->trans.map.r$.GetSize(); i++)
		{
			if (pDoc->trans.map.r$[i].name == sel)
			{
				pDC->Rectangle(pDoc->trans.map.r$[i].left-1, pDoc->trans.map.r$[i].top-1, pDoc->trans.map.r$[i].right+1, pDoc->trans.map.r$[i].bottom+1);
				i = (int) pDoc->trans.map.r$.GetSize() + 1;
			}
		}
	}

	// Clean up
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	ReleaseDC(pDC);

    __SEH_LOGFATAL("COpenScrapeView::blink_rect : \n");
}


COpenScrapeView *COpenScrapeView::GetView()
{
	__SEH_HEADER
		
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

	CView * pView = pFrame->GetActiveView();

	if ( !pView )
		return NULL;

	// Fail if view is of wrong kind
	// (this could occur with splitter windows, or additional
	// views on a single document
	if ( ! pView->IsKindOf( RUNTIME_CLASS(COpenScrapeView) ) )
		return NULL;

	return (COpenScrapeView *) pView;

    __SEH_LOGFATAL("COpenScrapeView::GetView : \n");
} 

