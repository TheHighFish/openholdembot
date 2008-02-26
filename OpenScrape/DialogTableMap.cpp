// DialogTableMap.cpp : implementation file
//
#include "stdafx.h"
#include <math.h>

#include "DialogTableMap.h"
#include "resource.h"
#include "OpenScrape.h"
#include "OpenScrapeDoc.h"
#include "OpenScrapeView.h"
#include "Registry.h"
#include "Transform.h"
#include "MainFrm.h"
#include "DialogEdit.h"
#include "DialogEditSizes.h"
#include "DialogEditSymbols.h"
#include "DialogEditRegion.h"
#include "DialogEditFont.h"
#include "DialogEditHashPoint.h"
#include "DialogEditGrHashPoints.h"
#include "DialogEditHash.h"
#include "debug.h"

// CDlgTableMap dialog
CDlgTableMap::CDlgTableMap(CWnd* pParent /*=NULL*/)	: CDialog(CDlgTableMap::IDD, pParent)
{
	black_pen.CreatePen(PS_SOLID, 1, COLOR_BLACK);
	green_pen.CreatePen(PS_SOLID, 1, COLOR_GREEN);
	red_pen.CreatePen(PS_SOLID, 1, COLOR_RED);
	blue_pen.CreatePen(PS_SOLID, 1, COLOR_BLUE);
	white_dot_pen.CreatePen(PS_DOT, 1, COLOR_WHITE);
	null_pen.CreatePen(PS_NULL, 0, COLOR_BLACK);

	white_brush.CreateSolidBrush(COLOR_WHITE);
	lt_gray_brush.CreateSolidBrush(COLOR_LT_GRAY);
	gray_brush.CreateSolidBrush(COLOR_GRAY);
	red_brush.CreateSolidBrush(COLOR_RED);
	yellow_brush.CreateSolidBrush(COLOR_YELLOW);

	separation_font_size = 6;
	lf_fixed.lfWidth = 0;
	lf_fixed.lfHeight = separation_font_size;
	lf_fixed.lfEscapement = 0;
	lf_fixed.lfOrientation = 0;
	lf_fixed.lfItalic = 0;
	lf_fixed.lfUnderline = 0;
	lf_fixed.lfStrikeOut = 0;
	lf_fixed.lfCharSet = 0;
	lf_fixed.lfOutPrecision = 0;
	lf_fixed.lfClipPrecision = 0;
	lf_fixed.lfQuality = PROOF_QUALITY;
	lf_fixed.lfPitchAndFamily = 0;
	strcpy(lf_fixed.lfFaceName, "Courier New");

	separation_font.CreateFontIndirect(&lf_fixed);

	nudge_font.CreatePointFont(100, "Wingdings");

	picker_cursor = false;
	hCurPicker = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_PICKERCURSOR));
	hCurStandard = ::LoadCursor(NULL, IDC_ARROW);
}

CDlgTableMap::~CDlgTableMap()
{
}

void CDlgTableMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABLEMAP_TREE, m_TableMapTree);
	DDX_Control(pDX, IDC_CURRENTREGION, m_BitmapFrame);
	DDX_Control(pDX, IDC_LEFT, m_Left);
	DDX_Control(pDX, IDC_LEFT_SPIN, m_LeftSpin);
	DDX_Control(pDX, IDC_TOP, m_Top);
	DDX_Control(pDX, IDC_TOP_SPIN, m_TopSpin);
	DDX_Control(pDX, IDC_BOTTOM, m_Bottom);
	DDX_Control(pDX, IDC_BOTTOM_SPIN, m_BottomSpin);
	DDX_Control(pDX, IDC_RIGHT, m_Right);
	DDX_Control(pDX, IDC_RIGHT_SPIN, m_RightSpin);
	DDX_Control(pDX, IDC_TRANSFORM, m_Transform);
	DDX_Control(pDX, IDC_ALPHA, m_Alpha);
	DDX_Control(pDX, IDC_RED, m_Red);
	DDX_Control(pDX, IDC_GREEN, m_Green);
	DDX_Control(pDX, IDC_BLUE, m_Blue);
	DDX_Control(pDX, IDC_RED_AVG, m_RedAvg);
	DDX_Control(pDX, IDC_GREEN_AVG, m_GreenAvg);
	DDX_Control(pDX, IDC_BLUE_AVG, m_BlueAvg);
	DDX_Control(pDX, IDC_PICKER, m_Picker);
	DDX_Control(pDX, IDC_RADIUS, m_Radius);
	DDX_Control(pDX, IDC_RESULT, m_Result);
	DDX_Control(pDX, IDC_NEW, m_New);
	DDX_Control(pDX, IDC_DELETE, m_Delete);
	DDX_Control(pDX, IDC_EDIT, m_Edit);
	DDX_Control(pDX, IDC_CREATE_IMAGE, m_CreateImage);
	DDX_Control(pDX, IDC_CREATE_FONT, m_CreateFont);
	DDX_Control(pDX, IDC_CREATE_HASH, m_CreateHash);
	DDX_Control(pDX, IDC_PIXELSEPARATION, m_PixelSeparation);
	DDX_Control(pDX, IDC_FONTPLUS, m_FontPlus);
	DDX_Control(pDX, IDC_FONTMINUS, m_FontMinus);
	DDX_Control(pDX, IDC_DRAWRECT, m_DrawRect);
	DDX_Control(pDX, IDC_ZOOM, m_Zoom);
	DDX_Control(pDX, IDC_RADIUS_SPIN, m_RadiusSpin);
	DDX_Control(pDX, IDC_NUDGE_TALLER, m_NudgeTaller);
	DDX_Control(pDX, IDC_NUDGE_SHORTER, m_NudgeShorter);
	DDX_Control(pDX, IDC_NUDGE_WIDER, m_NudgeWider);
	DDX_Control(pDX, IDC_NUDGE_NARROWER, m_NudgeNarrower);
	DDX_Control(pDX, IDC_NUDGE_BIGGER, m_NudgeBigger);
	DDX_Control(pDX, IDC_NUDGE_SMALLER, m_NudgeSmaller);
	DDX_Control(pDX, IDC_NUDGE_UPLEFT, m_NudgeUpLeft);
	DDX_Control(pDX, IDC_NUDGE_UP, m_NudgeUp);
	DDX_Control(pDX, IDC_NUDGE_UPRIGHT, m_NudgeUpRight);
	DDX_Control(pDX, IDC_NUDGE_RIGHT, m_NudgeRight);
	DDX_Control(pDX, IDC_NUDGE_DOWNRIGHT, m_NudgeDownRight);
	DDX_Control(pDX, IDC_NUDGE_DOWN, m_NudgeDown);
	DDX_Control(pDX, IDC_NUDGE_DOWNLEFT, m_NudgeDownLeft);
	DDX_Control(pDX, IDC_NUDGE_LEFT, m_NudgeLeft);
}


BEGIN_MESSAGE_MAP(CDlgTableMap, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TABLEMAP_TREE, &CDlgTableMap::OnTvnSelchangedTablemapTree)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_TRANSFORM, &CDlgTableMap::OnRegionChange)
	ON_EN_KILLFOCUS(IDC_ALPHA, &CDlgTableMap::OnRegionChange)
	ON_EN_KILLFOCUS(IDC_RED, &CDlgTableMap::OnRegionChange)
	ON_EN_KILLFOCUS(IDC_GREEN, &CDlgTableMap::OnRegionChange)
	ON_EN_KILLFOCUS(IDC_BLUE, &CDlgTableMap::OnRegionChange)
	ON_EN_KILLFOCUS(IDC_RADIUS, &CDlgTableMap::OnRegionChange)
	ON_EN_KILLFOCUS(IDC_LEFT, &CDlgTableMap::OnRegionChange)
	ON_EN_KILLFOCUS(IDC_TOP, &CDlgTableMap::OnRegionChange)
	ON_EN_KILLFOCUS(IDC_BOTTOM, &CDlgTableMap::OnRegionChange)
	ON_EN_KILLFOCUS(IDC_RIGHT, &CDlgTableMap::OnRegionChange)
	ON_CBN_SELCHANGE(IDC_ZOOM, &CDlgTableMap::OnZoomChange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_LEFT_SPIN, &CDlgTableMap::OnDeltaposLeftSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TOP_SPIN, &CDlgTableMap::OnDeltaposTopSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_BOTTOM_SPIN, &CDlgTableMap::OnDeltaposBottomSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RIGHT_SPIN, &CDlgTableMap::OnDeltaposRightSpin)

	ON_BN_CLICKED(IDC_NEW, &CDlgTableMap::OnBnClickedNew)
	ON_BN_CLICKED(IDC_DELETE, &CDlgTableMap::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_EDIT, &CDlgTableMap::OnBnClickedEdit)
	ON_BN_CLICKED(IDC_CREATE_IMAGE, &CDlgTableMap::OnBnClickedCreateImage)
	ON_BN_CLICKED(IDC_CREATE_FONT, &CDlgTableMap::OnBnClickedCreateFont)
	ON_BN_CLICKED(IDC_CREATE_HASH, &CDlgTableMap::OnBnClickedCreateHash)
	ON_BN_CLICKED(IDC_FONTPLUS, &CDlgTableMap::OnBnClickedFontplus)
	ON_BN_CLICKED(IDC_FONTMINUS, &CDlgTableMap::OnBnClickedFontminus)
	ON_BN_CLICKED(IDC_NUDGE_TALLER, &CDlgTableMap::OnBnClickedNudgeTaller)
	ON_BN_CLICKED(IDC_NUDGE_SHORTER, &CDlgTableMap::OnBnClickedNudgeShorter)
	ON_BN_CLICKED(IDC_NUDGE_WIDER, &CDlgTableMap::OnBnClickedNudgeWider)
	ON_BN_CLICKED(IDC_NUDGE_NARROWER, &CDlgTableMap::OnBnClickedNudgeNarrower)
	ON_BN_CLICKED(IDC_NUDGE_BIGGER, &CDlgTableMap::OnBnClickedNudgeBigger)
	ON_BN_CLICKED(IDC_NUDGE_SMALLER, &CDlgTableMap::OnBnClickedNudgeSmaller)
	ON_BN_CLICKED(IDC_NUDGE_UPLEFT, &CDlgTableMap::OnBnClickedNudgeUpleft)
	ON_BN_CLICKED(IDC_NUDGE_UP, &CDlgTableMap::OnBnClickedNudgeUp)
	ON_BN_CLICKED(IDC_NUDGE_UPRIGHT, &CDlgTableMap::OnBnClickedNudgeUpright)
	ON_BN_CLICKED(IDC_NUDGE_RIGHT, &CDlgTableMap::OnBnClickedNudgeRight)
	ON_BN_CLICKED(IDC_NUDGE_DOWNRIGHT, &CDlgTableMap::OnBnClickedNudgeDownright)
	ON_BN_CLICKED(IDC_NUDGE_DOWN, &CDlgTableMap::OnBnClickedNudgeDown)
	ON_BN_CLICKED(IDC_NUDGE_DOWNLEFT, &CDlgTableMap::OnBnClickedNudgeDownleft)
	ON_BN_CLICKED(IDC_NUDGE_LEFT, &CDlgTableMap::OnBnClickedNudgeLeft)

	ON_REGISTERED_MESSAGE(WM_STICKYBUTTONDOWN, OnStickyButtonDown)
	ON_REGISTERED_MESSAGE(WM_STICKYBUTTONUP, OnStickyButtonUp)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(UDN_DELTAPOS, IDC_RADIUS_SPIN, &CDlgTableMap::OnDeltaposRadiusSpin)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
END_MESSAGE_MAP()


// CDlgTableMap message handlers
BOOL CDlgTableMap::OnInitDialog()
{
	Registry		reg;
	int				max_x, max_y;
	CString			text;

	CDialog::OnInitDialog();

	create_tree();

	// Setup text entry fields and spinners
	m_Left.SetWindowText("0");
	m_LeftSpin.SetRange(0, 1600);
	m_LeftSpin.SetPos(0);
	m_LeftSpin.SetBuddy(&m_Left);

	m_Top.SetWindowText("0");
	m_TopSpin.SetRange(0, 1600);
	m_TopSpin.SetPos(0);
	m_TopSpin.SetBuddy(&m_Top);

	m_Right.SetWindowText("0");
	m_RightSpin.SetRange(0, 1600);
	m_RightSpin.SetPos(0);
	m_RightSpin.SetBuddy(&m_Right);

	m_Bottom.SetWindowText("0");
	m_BottomSpin.SetRange(0, 1600);
	m_BottomSpin.SetPos(0);
	m_BottomSpin.SetBuddy(&m_Bottom);

	// Set bitmap on front of draw rect button
	drawrect_bitmap.LoadBitmap(IDB_DRAWRECTBITMAP);
	h_drawrect_bitmap = (HBITMAP) drawrect_bitmap.GetSafeHandle();
	m_DrawRect.SetBitmap(h_drawrect_bitmap);

	m_Transform.AddString("Color");
	m_Transform.AddString("Image");
	m_Transform.AddString("Text0");
	m_Transform.AddString("Text1");
	m_Transform.AddString("Text2");
	m_Transform.AddString("Text3");
	m_Transform.AddString("Hash0");
	m_Transform.AddString("Hash1");
	m_Transform.AddString("Hash2");
	m_Transform.AddString("Hash3");
	m_Transform.AddString("None");
	m_Transform.SetWindowPos(NULL, 0, 0, 72, 200, SWP_NOMOVE | SWP_NOZORDER);

	m_Alpha.SetWindowText("");
	m_Red.SetWindowText("");
	m_Green.SetWindowText("");
	m_Blue.SetWindowText("");
	m_RedAvg.SetWindowText("");
	m_GreenAvg.SetWindowText("");
	m_BlueAvg.SetWindowText("");

	m_Radius.SetWindowText("");
	m_RadiusSpin.SetRange(-441, 441);
	m_RadiusSpin.SetPos(0);
	m_RadiusSpin.SetBuddy(&m_Radius);

	m_Result.SetWindowText("");

	m_PixelSeparation.SetFont(&separation_font);

	m_Zoom.AddString("None");
	m_Zoom.AddString("2x");
	m_Zoom.AddString("4x");
	m_Zoom.AddString("8x");
	m_Zoom.AddString("16x");
	m_Zoom.SetCurSel(2);
	m_Zoom.SetWindowPos(NULL, 0, 0, 72, 200, SWP_NOMOVE | SWP_NOZORDER);

	// Set bitmap on front of picker button
	picker_bitmap.LoadBitmap(IDB_PICKERBITMAP);
	h_picker_bitmap = (HBITMAP) picker_bitmap.GetSafeHandle();
	m_Picker.SetBitmap(h_picker_bitmap);

	// Set text on nudge buttons
	m_NudgeTaller.SetFont(&nudge_font);
	m_NudgeShorter.SetFont(&nudge_font);
	m_NudgeWider.SetFont(&nudge_font);
	m_NudgeNarrower.SetFont(&nudge_font);
	m_NudgeBigger.SetFont(&nudge_font);
	m_NudgeSmaller.SetFont(&nudge_font);
	m_NudgeUpLeft.SetFont(&nudge_font);
	m_NudgeUp.SetFont(&nudge_font);
	m_NudgeUpRight.SetFont(&nudge_font);
	m_NudgeRight.SetFont(&nudge_font);
	m_NudgeDownRight.SetFont(&nudge_font);
	m_NudgeDown.SetFont(&nudge_font);
	m_NudgeDownLeft.SetFont(&nudge_font);
	m_NudgeLeft.SetFont(&nudge_font);


	text.Format("%c", 0xD9);
	m_NudgeTaller.SetWindowText(text.GetString());
	text.Format("%c", 0xDA);
	m_NudgeShorter.SetWindowText(text.GetString());
	text.Format("%c", 0xD8);
	m_NudgeWider.SetWindowText(text.GetString());
	text.Format("%c", 0xD7);
	m_NudgeNarrower.SetWindowText(text.GetString());
	text.Format("%c", 0xB2);
	m_NudgeBigger.SetWindowText(text.GetString());
	text.Format("%c", 0xB3);
	m_NudgeSmaller.SetWindowText(text.GetString());
	text.Format("%c", 0xE3);
	m_NudgeUpLeft.SetWindowText(text.GetString());
	text.Format("%c", 0xE1);
	m_NudgeUp.SetWindowText(text.GetString());
	text.Format("%c", 0xE4);
	m_NudgeUpRight.SetWindowText(text.GetString());
	text.Format("%c", 0xE0);
	m_NudgeRight.SetWindowText(text.GetString());
	text.Format("%c", 0xE6);
	m_NudgeDownRight.SetWindowText(text.GetString());
	text.Format("%c", 0xE2);
	m_NudgeDown.SetWindowText(text.GetString());
	text.Format("%c", 0xE5);
	m_NudgeDownLeft.SetWindowText(text.GetString());
	text.Format("%c", 0xDF);
	m_NudgeLeft.SetWindowText(text.GetString());

	// Restore window location and size
	reg.read_reg();
	max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
	max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
	::SetWindowPos(m_hWnd, HWND_TOP, min(reg.tablemap_x, max_x), min(reg.tablemap_y, max_y),
		reg.tablemap_dx, reg.tablemap_dy, SWP_NOCOPYBITS);

	picker_cursor = false;
	ignore_changes = false;

	EnableToolTips(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgTableMap::DestroyWindow()
{
	Registry		reg;
	WINDOWPLACEMENT wp;

	// Save window position
	reg.read_reg();
	GetWindowPlacement(&wp);

	reg.tablemap_x = wp.rcNormalPosition.left;
	reg.tablemap_y = wp.rcNormalPosition.top;
	reg.tablemap_dx = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	reg.tablemap_dy = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
	reg.write_reg();

	return CDialog::DestroyWindow();
}

void CDlgTableMap::OnOK()
{
	// prevents enter key from closing dialog
	//CDialog::OnOK();
}

void CDlgTableMap::OnCancel()
{
	// prevents esc key from closing dialog
	//CDialog::OnCancel();
}

void CDlgTableMap::OnPaint()
{
	CPaintDC			dc(this);
	CString				sel = m_TableMapTree.GetItemText(m_TableMapTree.GetSelectedItem());
	HTREEITEM			parent = m_TableMapTree.GetParentItem(m_TableMapTree.GetSelectedItem());
	CString				selected_parent_text = "";

	if (parent != NULL) 
		selected_parent_text = m_TableMapTree.GetItemText(parent);

	// A root item was selected, or nothing has been selected yet
	if (parent==NULL || m_TableMapTree.GetSelectedItem()==NULL) 
	{
		clear_bitmap_control();
		m_BitmapFrame.SetWindowPos(NULL, 0, 0, BITMAP_WIDTH, BITMAP_HEIGHT, SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS);
	}

	// A leaf item was selected
	else {

		// Display currently selected region bitmap from saved bitmap
		if (selected_parent_text == "Regions")
		{
			draw_region_bitmap();
		}

		else if (selected_parent_text == "Images")
		{
			draw_image_bitmap();
		}

		else
		{
			m_BitmapFrame.SetBitmap(NULL);
			m_BitmapFrame.SetWindowPos(NULL, 0, 0, BITMAP_WIDTH, BITMAP_HEIGHT, SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS);
		}
	}

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgTableMap::clear_bitmap_control(void)
{
	CDC				*pDC = m_BitmapFrame.GetDC();
	CPen			*pTempPen, oldpen;
	CBrush			*pTempBrush, oldbrush;	
	RECT			rect;

	pTempPen = (CPen*)pDC->SelectObject(&null_pen);
	oldpen.FromHandle((HPEN)pTempPen);
	pTempBrush = (CBrush*)pDC->SelectObject(&lt_gray_brush); 
	oldbrush.FromHandle((HBRUSH)pTempBrush);
	m_BitmapFrame.GetWindowRect(&rect);
	
	pDC->Rectangle(1, 1, rect.right-rect.left, rect.bottom-rect.top);
	
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);
	ReleaseDC(pDC);
}

void CDlgTableMap::draw_region_bitmap(void)
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	CDC					*pDC;
	HDC					hdcControl, hdcScreen;
	HDC					hdc_bitmap_orig, hdc_bitmap_subset;
	HBITMAP				old_bitmap_orig, bitmap_subset, old_bitmap_subset, bitmap_control, old_bitmap_control;
	int					left, right, top, bottom, zoom;
	CPen				*pTempPen, oldpen;
	CBrush				*pTempBrush, oldbrush;	
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		pDC = m_BitmapFrame.GetDC();
		hdcControl = *pDC;
		hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);

		// Select saved bitmap into memory hdc
		hdc_bitmap_orig = CreateCompatibleDC(hdcScreen);
		old_bitmap_orig = (HBITMAP) SelectObject(hdc_bitmap_orig, pDoc->attached_bitmap);

		// Figure out the size of the subset bitmap
		left = sel_region_ptr->left - 5;
		top = sel_region_ptr->top - 5;
		right = sel_region_ptr->right + 5;
		bottom = sel_region_ptr->bottom + 5;
		zoom = m_Zoom.GetCurSel()==0 ? 1 :
			   m_Zoom.GetCurSel()==1 ? 2 :
			   m_Zoom.GetCurSel()==2 ? 4 :
			   m_Zoom.GetCurSel()==3 ? 8 :
			   m_Zoom.GetCurSel()==4 ? 16 : 1;

		// Create a memory DC to hold only the subset bitmap and copy
		hdc_bitmap_subset = CreateCompatibleDC(hdcScreen);
		bitmap_subset = CreateCompatibleBitmap(hdcScreen, right-left, bottom-top);
		old_bitmap_subset = (HBITMAP) SelectObject(hdc_bitmap_subset, bitmap_subset);
		BitBlt(hdc_bitmap_subset, 0, 0, right-left, bottom-top, hdc_bitmap_orig, left, top, SRCCOPY);

		// Draw selection rectangle on the copy of the bitmap
		pTempPen = (CPen*) SelectObject(hdc_bitmap_subset, red_pen);
		oldpen.FromHandle((HPEN)pTempPen);
		pTempBrush = (CBrush*) SelectObject(hdc_bitmap_subset, GetStockObject(NULL_BRUSH));
		oldbrush.FromHandle((HBRUSH)pTempBrush);
		Rectangle(hdc_bitmap_subset, 4, 4, 
				  6 + sel_region_ptr->right - sel_region_ptr->left,  
				  6 + sel_region_ptr->bottom - sel_region_ptr->top);
		SelectObject(hdc_bitmap_subset, oldpen);
		SelectObject(hdc_bitmap_subset, oldbrush);

		// Resize bitmap control to fit
		m_BitmapFrame.SetWindowPos(NULL, 0, 0, ((right-left)*zoom)+2, ((bottom-top)*zoom)+2, SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS);

		// Copy from bitmap subset to our control and stretch it
		bitmap_control = CreateCompatibleBitmap(hdcScreen, (right-left)*zoom, (bottom-top)*zoom);
		old_bitmap_control = (HBITMAP) SelectObject(hdcControl, bitmap_control);
		if (sel_region_ptr->right >= sel_region_ptr->left && 
			sel_region_ptr->bottom >= sel_region_ptr->top)
		{
			StretchBlt(hdcControl, 1, 1, (right-left)*zoom, (bottom-top)*zoom,
				   hdc_bitmap_subset, 0, 0, right-left, bottom-top,
				   SRCCOPY);
		}

		// Clean up
		SelectObject(hdcControl, old_bitmap_control);
		DeleteObject(bitmap_control);

		SelectObject(hdc_bitmap_subset, old_bitmap_subset);
		DeleteObject(bitmap_subset);
		DeleteDC(hdc_bitmap_subset);

		SelectObject(hdc_bitmap_orig, old_bitmap_orig);
		DeleteDC(hdc_bitmap_orig);

		DeleteDC(hdcScreen);
		ReleaseDC(pDC);
	}
}

void CDlgTableMap::draw_image_bitmap(void)
{
	int					x, y, width, height, zoom;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	CDC					*pDC;
	HDC					hdcControl, hdcScreen, hdc_image;
	HBITMAP				bitmap_image, old_bitmap_image, bitmap_control, old_bitmap_control;
	BYTE				*pBits, alpha, red, green, blue;
	Stablemap_image		*sel_image = NULL;
	
	// Get pointer to selected image record
	if (m_TableMapTree.GetSelectedItem())
		sel_image = (Stablemap_image *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_image)
	{
		// Get bitmap size
		width = sel_image->width;
		height = sel_image->height;

		// Copy saved bitmap into a memory dc so we can get the bmi
		pDC = m_BitmapFrame.GetDC();
		hdcControl = *pDC;
		hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); 

		// Create new memory DC and new bitmap
		hdc_image = CreateCompatibleDC(hdcScreen);
		bitmap_image = CreateCompatibleBitmap(hdcScreen, width, height);
		old_bitmap_image = (HBITMAP) SelectObject(hdc_image, bitmap_image);

		// Setup BITMAPINFO
		BITMAPINFO	bmi;
		ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
		bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = -height;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB; //BI_BITFIELDS;
		bmi.bmiHeader.biSizeImage = width * height * 4;

		// Copy saved image info into pBits array
		pBits = new BYTE[bmi.bmiHeader.biSizeImage];
		for (y=0; y < (int) height; y++) {
			for (x=0; x < (int) width; x++) {
				// image record is stored internally in ABGR format
				alpha = (sel_image->pixel[y*width + x] >> 24) & 0xff;
				red = (sel_image->pixel[y*width + x] >> 0) & 0xff;
				green = (sel_image->pixel[y*width + x] >> 8) & 0xff;
				blue = (sel_image->pixel[y*width + x] >> 16) & 0xff;

				// SetDIBits format is BGRA
				pBits[y*width*4 + x*4 + 0] = blue;
				pBits[y*width*4 + x*4 + 1] = green;
				pBits[y*width*4 + x*4 + 2] = red;
				pBits[y*width*4 + x*4 + 3] = alpha; 
			}
		}
		::SetDIBits(hdc_image, bitmap_image, 0, height, pBits, &bmi, DIB_RGB_COLORS);

		// Figure size of stretched bitmap and resize control to fit
		zoom = m_Zoom.GetCurSel()==0 ? 1 :
			m_Zoom.GetCurSel()==1 ? 2 :
			m_Zoom.GetCurSel()==2 ? 4 :
			m_Zoom.GetCurSel()==3 ? 8 :
			m_Zoom.GetCurSel()==4 ? 16 : 1;

		m_BitmapFrame.SetWindowPos(NULL, 0, 0, (width*zoom)+2, (height*zoom)+2, SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS);

		// Copy from saved bitmap DC copy to our control and stretch it
		bitmap_control = CreateCompatibleBitmap(hdcScreen, width*zoom, height*zoom);
		old_bitmap_control = (HBITMAP) SelectObject(hdcControl, bitmap_control);
		StretchBlt(hdcControl, 1, 1, width*zoom, height*zoom,
				   hdc_image, 0, 0, width, height,
				   SRCCOPY);

		// Clean up
		delete []pBits;

		SelectObject(hdcControl, old_bitmap_control);
		DeleteObject(bitmap_control);

		SelectObject(hdc_image, old_bitmap_image);
		DeleteObject(bitmap_image);
		DeleteDC(hdc_image);

		DeleteDC(hdcScreen);
		ReleaseDC(pDC);
	}
}

void CDlgTableMap::OnRegionChange()
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	CString				text, alpha, red, green, blue;
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (!sel_region_ptr)  return;

	if (ignore_changes)  return;

	// left/top/right/bottom
	m_Left.GetWindowText(text);
	sel_region_ptr->left = atoi(text.GetString());
	m_Top.GetWindowText(text);
	sel_region_ptr->top = atoi(text.GetString());
	m_Right.GetWindowText(text);
	sel_region_ptr->right = atoi(text.GetString());
	m_Bottom.GetWindowText(text);
	sel_region_ptr->bottom = atoi(text.GetString());

	// color/radius (color is stored internally and in the .tm file in ABGR (COLORREF) format)
	m_Alpha.GetWindowText(alpha);
	m_Red.GetWindowText(red);
	m_Green.GetWindowText(green);
	m_Blue.GetWindowText(blue);
	sel_region_ptr->color = ((strtoul(alpha.GetString(), NULL, 16))<<24) +  // alpha
								 ((strtoul(blue.GetString(), NULL, 16))<<16) +  // red
								 ((strtoul(green.GetString(), NULL, 16))<<8) +   // green
								 (strtoul(red.GetString(), NULL, 16));         // blue
	m_Radius.GetWindowText(text);
	sel_region_ptr->radius = atoi(text.GetString());

	// transform type
	m_Transform.GetLBText(m_Transform.GetCurSel(), text);
	sel_region_ptr->transform = text == "Color" ? "C" :
								text == "Image" ? "I" :
								text == "Text0" ? "T0" :
								text == "Text1" ? "T1" :
								text == "Text2" ? "T2" :
								text == "Text3" ? "T3" : 
								text == "Hash0" ? "H0" :
								text == "Hash1" ? "H1" :
								text == "Hash2" ? "H2" :
								text == "Hash3" ? "H3" :
								text == "None" ? "N" : 
								"";

	update_display();
	theApp.m_pMainWnd->Invalidate(false);
	Invalidate(false);

	pDoc->SetModifiedFlag(true);
}

void CDlgTableMap::OnZoomChange()
{
	theApp.m_pMainWnd->Invalidate(false);
	Invalidate(false);
}

void CDlgTableMap::OnTvnSelchangedTablemapTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW	pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	update_display();

	theApp.m_pMainWnd->Invalidate(false);
	Invalidate(false);

	*pResult = 0;
}

void CDlgTableMap::update_display(void)
{
	CString				text, hashname, type, charstr, sel_ch, sel_type, sel_temp;
	CString				sel = m_TableMapTree.GetItemText(m_TableMapTree.GetSelectedItem());
	HTREEITEM			parent = m_TableMapTree.GetParentItem(m_TableMapTree.GetSelectedItem());
	CString				num, x, y, selected_parent_text;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();

	Stablemap_size		*sel_size_ptr = NULL;
	Stablemap_symbol	*sel_symbol_ptr = NULL;
	Stablemap_hash_value	*sel_hash_value_ptr = NULL;

	// Don't trigger OnChange messages
	ignore_changes = true;

	// A root item was selected
	if (parent == NULL) 
	{
		disable_and_clear_all();

		if (sel == "Sizes")
		{
			m_New.EnableWindow(true);
		}

		else if (sel == "Symbols")
		{
			m_New.EnableWindow(true);
		}

		else if (sel == "Regions")
		{
			m_New.EnableWindow(true);
		}

		else if (sel == "Fonts")
		{
		}

		else if (sel == "Hash Points")
		{
			m_New.EnableWindow(true);
			m_Edit.EnableWindow(true);
		}

		else if (sel == "Hashes")
		{
		}

		else if (sel == "Images")
		{
		}
	}

	// A leaf item was selected
	else 
	{

		selected_parent_text = m_TableMapTree.GetItemText(parent);

		if (selected_parent_text == "Sizes")
		{
			disable_and_clear_all();
			m_New.EnableWindow(true);
			m_Delete.EnableWindow(true);
			m_Edit.EnableWindow(true);

			if (m_TableMapTree.GetSelectedItem())
				 sel_size_ptr = (Stablemap_size *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

			if (sel_size_ptr)
			{
				text.Format("%d x %d", sel_size_ptr->width, sel_size_ptr->height);
				m_Result.SetWindowText(text.GetString());
			}
		}

		else if (selected_parent_text == "Symbols")
		{
			disable_and_clear_all();
			m_New.EnableWindow(true);
			m_Delete.EnableWindow(true);
			m_Edit.EnableWindow(true);

			if (m_TableMapTree.GetSelectedItem())
				sel_symbol_ptr = (Stablemap_symbol *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

			if (sel_symbol_ptr)
				m_Result.SetWindowText(sel_symbol_ptr->text.GetString());
		}

		else if (selected_parent_text == "Regions")
		{
			m_Left.EnableWindow(true);
			m_Top.EnableWindow(true);
			m_Right.EnableWindow(true);
			m_Bottom.EnableWindow(true);
			m_DrawRect.EnableWindow(true);
			m_Transform.EnableWindow(true);
			m_Alpha.EnableWindow(true);
			m_Red.EnableWindow(true);
			m_Green.EnableWindow(true);
			m_Blue.EnableWindow(true);
			m_Picker.EnableWindow(true);
			m_Radius.EnableWindow(true);
			m_CreateImage.EnableWindow(true);
			m_CreateFont.EnableWindow(true);
			m_CreateHash.EnableWindow(false);
			m_Result.EnableWindow(true);
			m_PixelSeparation.EnableWindow(true);
			m_FontPlus.EnableWindow(true);
			m_FontMinus.EnableWindow(true);
			m_New.EnableWindow(true);
			m_Delete.EnableWindow(true);
			m_Edit.EnableWindow(true);
			m_Zoom.EnableWindow(true);
			m_NudgeTaller.EnableWindow(true);
			m_NudgeShorter.EnableWindow(true);
			m_NudgeWider.EnableWindow(true);
			m_NudgeNarrower.EnableWindow(true);
			m_NudgeBigger.EnableWindow(true);
			m_NudgeSmaller.EnableWindow(true);
			m_NudgeUpLeft.EnableWindow(true);
			m_NudgeUp.EnableWindow(true);
			m_NudgeUpRight.EnableWindow(true);
			m_NudgeRight.EnableWindow(true);
			m_NudgeDownRight.EnableWindow(true);
			m_NudgeDown.EnableWindow(true);
			m_NudgeDownLeft.EnableWindow(true);
			m_NudgeLeft.EnableWindow(true);

			update_r$_display(false);
		}

		else if (selected_parent_text == "Fonts")
		{
			disable_and_clear_all();
			m_PixelSeparation.EnableWindow(true);
			m_FontPlus.EnableWindow(true);
			m_FontMinus.EnableWindow(true);
			m_Delete.EnableWindow(true);
						
			update_t$_display();
		}

		else if (selected_parent_text == "Hash Points")
		{
			disable_and_clear_all();
			m_New.EnableWindow(true);
			m_Delete.EnableWindow(true);
			m_Edit.EnableWindow(true);
		}

		else if (selected_parent_text == "Hashes")
		{
			disable_and_clear_all();
			m_New.EnableWindow(true);
			m_Delete.EnableWindow(true);

			if (m_TableMapTree.GetSelectedItem())
				sel_hash_value_ptr = (Stablemap_hash_value *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

			if (sel_hash_value_ptr)
			{
				text.Format("%08x", sel_hash_value_ptr->hash);
				m_Result.SetWindowText(text.GetString());
			}
		}

		else if (selected_parent_text == "Images")
		{
			disable_and_clear_all();
			m_Delete.EnableWindow(true);
			m_CreateHash.EnableWindow(true);
			m_Zoom.EnableWindow(true);
		}
	}

	// Re-enable triggering of OnChange messages
	ignore_changes = false;
}

void CDlgTableMap::disable_and_clear_all(void)
{
	m_Left.EnableWindow(false);
	m_Left.SetWindowText("");
	m_Top.EnableWindow(false);
	m_Top.SetWindowText("");
	m_Right.EnableWindow(false);
	m_Right.SetWindowText("");
	m_Bottom.EnableWindow(false);
	m_Bottom.SetWindowText("");
	m_DrawRect.EnableWindow(false);

	m_Transform.EnableWindow(false);
	m_Transform.SetCurSel(-1);
	m_Alpha.EnableWindow(false);
	m_Alpha.SetWindowText("");
	m_Red.EnableWindow(false);
	m_Red.SetWindowText("");
	m_Green.EnableWindow(false);
	m_Green.SetWindowText("");
	m_Blue.EnableWindow(false);
	m_Blue.SetWindowText("");
	m_RedAvg.EnableWindow(false);
	m_RedAvg.SetWindowText("");
	m_GreenAvg.EnableWindow(false);
	m_GreenAvg.SetWindowText("");
	m_BlueAvg.EnableWindow(false);
	m_BlueAvg.SetWindowText("");
	m_Picker.EnableWindow(false);
	m_Radius.EnableWindow(false);
	m_Radius.SetWindowText("");

	m_CreateImage.EnableWindow(false);
	m_CreateFont.EnableWindow(false);
	m_CreateHash.EnableWindow(false);

	m_PixelSeparation.EnableWindow(false);
	m_PixelSeparation.SetWindowText("");
	m_FontPlus.EnableWindow(false);
	m_FontMinus.EnableWindow(false);

	m_Result.EnableWindow(false);
	m_Result.SetWindowText("");

	m_New.EnableWindow(false);
	m_Delete.EnableWindow(false);
	m_Edit.EnableWindow(false);
	m_Zoom.EnableWindow(false);

	m_NudgeTaller.EnableWindow(false);
	m_NudgeShorter.EnableWindow(false);
	m_NudgeWider.EnableWindow(false);
	m_NudgeNarrower.EnableWindow(false);
	m_NudgeBigger.EnableWindow(false);
	m_NudgeSmaller.EnableWindow(false);
	m_NudgeUpLeft.EnableWindow(false);
	m_NudgeUp.EnableWindow(false);
	m_NudgeUpRight.EnableWindow(false);
	m_NudgeRight.EnableWindow(false);
	m_NudgeDownRight.EnableWindow(false);
	m_NudgeDown.EnableWindow(false);
	m_NudgeDownLeft.EnableWindow(false);
	m_NudgeLeft.EnableWindow(false);
}

void CDlgTableMap::update_r$_display(bool dont_update_spinners)
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	CString				text, selected_transform, separation;
	CDC					*pDC;
	HDC					hdcControl, hdcScreen, hdc_bitmap_orig, hdc_bitmap_transform;
	HBITMAP				old_bitmap_orig, bitmap_transform, old_bitmap_transform;
	CTransform			ctrans;
	COLORREF			cr_avg;
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr)
	{
		// Left/top/right/bottom edits/spinners
		if (!dont_update_spinners)
		{
			text.Format("%d", sel_region_ptr->left);
			m_Left.SetWindowText(text);
			m_LeftSpin.SetPos(sel_region_ptr->left);

			text.Format("%d", sel_region_ptr->top);
			m_Top.SetWindowText(text);
			m_TopSpin.SetPos(sel_region_ptr->top);

			text.Format("%d", sel_region_ptr->right);
			m_Right.SetWindowText(text);
			m_RightSpin.SetPos(sel_region_ptr->right);

			text.Format("%d", sel_region_ptr->bottom);
			m_Bottom.SetWindowText(text);
			m_BottomSpin.SetPos(sel_region_ptr->bottom);
		}

		// Transform
		if (sel_region_ptr->transform == "C")			selected_transform = "Color";
		else if (sel_region_ptr->transform == "I")		selected_transform = "Image";
		else if (sel_region_ptr->transform == "T0")		selected_transform = "Text0";
		else if (sel_region_ptr->transform == "T1")		selected_transform = "Text1";
		else if (sel_region_ptr->transform == "T2")		selected_transform = "Text2";
		else if (sel_region_ptr->transform == "T3")		selected_transform = "Text3";
		else if (sel_region_ptr->transform == "H0")		selected_transform = "Hash0";
		else if (sel_region_ptr->transform == "H1")		selected_transform = "Hash1";
		else if (sel_region_ptr->transform == "H2")		selected_transform = "Hash2";
		else if (sel_region_ptr->transform == "H3")		selected_transform = "Hash3";
		else if (sel_region_ptr->transform == "N")		selected_transform = "None";
		m_Transform.SelectString(-1, selected_transform);

		// Color/radius  (color is stored internally and in the .tm file in ABGR (COLORREF) format)
		if (selected_transform == "Hash0" ||
			selected_transform == "Hash1" ||
			selected_transform == "Hash2" ||
			selected_transform == "Hash3" ||
			selected_transform == "None" ||
			selected_transform == "Image")
		{
			m_Alpha.EnableWindow(false);
			m_Red.EnableWindow(false);
			m_Green.EnableWindow(false);
			m_Blue.EnableWindow(false);
			m_Picker.EnableWindow(false);
			m_Radius.EnableWindow(false);
		}
		else
		{
			m_Alpha.EnableWindow(true);
			m_Red.EnableWindow(true);
			m_Green.EnableWindow(true);
			m_Blue.EnableWindow(true);
			m_Picker.EnableWindow(true);
			m_Radius.EnableWindow(true);
		}

		text.Format("%02x", (sel_region_ptr->color >> 24) & 0xff);
		m_Alpha.SetWindowText(text);
		text.Format("%02x", (sel_region_ptr->color >> 0) & 0xff);
		m_Red.SetWindowText(text);
		text.Format("%02x", (sel_region_ptr->color >> 8) & 0xff);
		m_Green.SetWindowText(text);
		text.Format("%02x", (sel_region_ptr->color >> 16) & 0xff);
		m_Blue.SetWindowText(text);
		text.Format("%d", sel_region_ptr->radius);
		m_Radius.SetWindowText(text);

		if (selected_transform != "Color")
		{
			m_RedAvg.EnableWindow(false);
			m_GreenAvg.EnableWindow(false);
			m_BlueAvg.EnableWindow(false);
		}
		else
		{
			m_RedAvg.EnableWindow(true);
			m_GreenAvg.EnableWindow(true);
			m_BlueAvg.EnableWindow(true);
		}

		// Go calc the result and display it
		pDC = m_BitmapFrame.GetDC();
		hdcControl = *pDC;
		hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); 

		hdc_bitmap_orig = CreateCompatibleDC(hdcScreen);
		old_bitmap_orig = (HBITMAP) SelectObject(hdc_bitmap_orig, pDoc->attached_bitmap);

		hdc_bitmap_transform = CreateCompatibleDC(hdcScreen);
		bitmap_transform = CreateCompatibleBitmap(hdcScreen, 
									   sel_region_ptr->right-sel_region_ptr->left, 
									   sel_region_ptr->bottom-sel_region_ptr->top);
		old_bitmap_transform = (HBITMAP) SelectObject(hdc_bitmap_transform, bitmap_transform);

		BitBlt(hdc_bitmap_transform, 0, 0, 
			   sel_region_ptr->right - sel_region_ptr->left, 
			   sel_region_ptr->bottom - sel_region_ptr->top,
			   hdc_bitmap_orig, 
			   sel_region_ptr->left, sel_region_ptr->top, 
			   SRCCOPY);

		// result field
		int ret = ctrans.do_transform(&pDoc->tablemap, sel_region_ptr, hdc_bitmap_transform, &text, &separation, &cr_avg);
		if (ret != ERR_GOOD_SCRAPE_GENERAL)
		{
			switch (ret)
			{
			case ERR_FIELD_TOO_LARGE:
				text.Format("Err: Field too large");
				break;
			case ERR_NO_HASH_MATCH:
				text.Format("Err: No hash match");
				break;
			case ERR_TEXT_SCRAPE_NOMATCH:
				if (separation.Find("X") == -1)
					text.Format("Err: No foreground pixels");
				else
					text.Format("Err: No text match");
				break;
			case ERR_NO_IMAGE_MATCH:
				text.Format("Err: No image match");
				break;
			default:
				text.Format("Err: %d", ret);
				break;
			}
		}
		m_Result.SetWindowText(text);

		// pixel separation field
		if (selected_transform.Find("Text") == -1)  separation = "";
		m_PixelSeparation.SetWindowText(separation);

		// avg color fields
		if (selected_transform == "Color")
		{
			text.Format("%02x", GetRValue(cr_avg));
			m_RedAvg.SetWindowText(text);
			text.Format("%02x", GetGValue(cr_avg));
			m_GreenAvg.SetWindowText(text);
			text.Format("%02x", GetBValue(cr_avg));
			m_BlueAvg.SetWindowText(text);
			text.Format("%d", sel_region_ptr->radius);
		}

		// Create font button
		if (selected_transform.Find("Text") != -1)
		{
			m_CreateFont.EnableWindow(true);
		}
		else
		{
			m_CreateFont.EnableWindow(false);
		}

		// Clean up
		SelectObject(hdc_bitmap_transform, old_bitmap_transform);
		DeleteObject(bitmap_transform);
		DeleteDC(hdc_bitmap_transform);

		SelectObject(hdc_bitmap_orig, old_bitmap_orig);
		DeleteDC(hdc_bitmap_orig);

		DeleteDC(hdcScreen);
		ReleaseDC(pDC);
	}
}

void CDlgTableMap::update_t$_display(void)
{
	int					j, x, bit, top;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	CString				separation, text, charstr;
	bool				character[MAX_CHAR_WIDTH][MAX_CHAR_HEIGHT] = { false };	
	Stablemap_font		*sel_font = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_font = (Stablemap_font *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_font)
	{
		// Get set bits
		bit = 0;
		for (j=0; j<sel_font->x_count; j++)
			for (bit=0; bit<MAX_CHAR_HEIGHT; bit++)
				character[j][bit] = (sel_font->x[j] & (int) pow((double) 2, (double) bit)) != 0;

		// Find topmost line with a set pixel
		for (j=MAX_CHAR_HEIGHT-1; j>=0; j--)
		{
			for (x=0; x<sel_font->x_count; x++)
			{
				if (character[x][j])
				{
					top = j;
					x = sel_font->x_count + 1;
					j = -1;
				}
			}
		}

		// Create string of set pixels
		separation = "";
		for (j=top; j>=0; j--)
		{
			for (x=0; x<sel_font->x_count; x++)
			{
				if (character[x][j])
					separation.Append("X");
				else
					separation.Append(" ");
			}
			separation.Append("\r\n");
		}

		// Update pixel separation control
		m_PixelSeparation.SetWindowText(separation);
	}
}

void CDlgTableMap::OnDeltaposLeftSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN			pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (!sel_region_ptr)  return;
	
	if (ignore_changes)  return;

	sel_region_ptr->left = pNMUpDown->iPos + pNMUpDown->iDelta;

	update_r$_display(true);
	Invalidate(false);
	theApp.m_pMainWnd->Invalidate(false);

	pDoc->SetModifiedFlag(true);

	*pResult = 0;
}

void CDlgTableMap::OnDeltaposTopSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN			pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (!sel_region_ptr)  return;

	if (ignore_changes)  return;

	sel_region_ptr->top = pNMUpDown->iPos + pNMUpDown->iDelta;

	update_r$_display(true);
	Invalidate(false);
	theApp.m_pMainWnd->Invalidate(false);

	pDoc->SetModifiedFlag(true);

	*pResult = 0;
}

void CDlgTableMap::OnDeltaposBottomSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN			pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (!sel_region_ptr)  return;

	if (ignore_changes)  return;

	sel_region_ptr->bottom = pNMUpDown->iPos + pNMUpDown->iDelta;

	update_r$_display(true);
	Invalidate(false);
	theApp.m_pMainWnd->Invalidate(false);

	pDoc->SetModifiedFlag(true);

	*pResult = 0;
}

void CDlgTableMap::OnDeltaposRightSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN			pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (!sel_region_ptr)  return;

	if (ignore_changes)  return;

	sel_region_ptr->right = pNMUpDown->iPos + pNMUpDown->iDelta;

	update_r$_display(true);
	Invalidate(false);
	theApp.m_pMainWnd->Invalidate(false);

	pDoc->SetModifiedFlag(true);

	*pResult = 0;
}

void CDlgTableMap::OnDeltaposRadiusSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN			pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (!sel_region_ptr)  return;

	if (ignore_changes)  return;

	sel_region_ptr->radius = pNMUpDown->iPos + pNMUpDown->iDelta;

	update_r$_display(true);
	Invalidate(false);
	theApp.m_pMainWnd->Invalidate(false);

	pDoc->SetModifiedFlag(true);

	*pResult = 0;
}

void CDlgTableMap::OnBnClickedNew()
{
	COpenScrapeDoc			*pDoc = COpenScrapeDoc::GetDocument();
	CString					text, sel;
	int						i, j, new_index;
	bool					used_string;
	HTREEITEM				new_hti;
	char					title[200];

	CDlgEditSizes			dlgsizes;
	CDlgEditSymbols			dlgsymbols;
	CDlgEditRegion			dlgregions;
	CDlgEditHashPoint		dlghashpoint;
	Stablemap_size			new_size;
	Stablemap_symbol		new_symbol;
	Stablemap_region		new_region;
	Stablemap_hash_point	new_hashpoint;

	HTREEITEM				parent = m_TableMapTree.GetParentItem(m_TableMapTree.GetSelectedItem());
	CString					selected_parent_text = "";

	if (m_TableMapTree.GetSelectedItem())
		selected_parent_text = m_TableMapTree.GetItemText(m_TableMapTree.GetSelectedItem());

	if (parent != NULL) 
		selected_parent_text = m_TableMapTree.GetItemText(parent);

	if (selected_parent_text == "Sizes")
	{
		// Prep dialog
		dlgsizes.titletext = "New Size record";
		dlgsizes.name = "";
		dlgsizes.width = 0;
		dlgsizes.height = 0;
		dlgsizes.strings.RemoveAll();
		for (i=0; i<num_z$strings; i++)
		{
			used_string = false;
			for (j=0; j<pDoc->tablemap.z$.GetCount(); j++)
				if (pDoc->tablemap.z$[j].name == z$strings[i])  
					used_string=true;

			if (!used_string)
				dlgsizes.strings.Add(z$strings[i]);
		}

		// Show dialog if there are any strings left to add
		if (dlgsizes.strings.GetSize() == 0)
		{
			MessageBox("All Size records are already present.");
		}
		else
		{
			if (dlgsizes.DoModal()==IDOK && dlgsizes.name!="")
			{

				// Add new record to internal structure
				new_size.name = dlgsizes.name;
				new_size.width = dlgsizes.width;
				new_size.height = dlgsizes.height;
				new_index = (int) pDoc->tablemap.z$.Add(new_size);

				// Add new record to tree
				new_hti = m_TableMapTree.InsertItem(dlgsizes.name, parent ? parent : m_TableMapTree.GetSelectedItem());
				m_TableMapTree.SetItemData(new_hti, (DWORD_PTR) &pDoc->tablemap.z$[new_index]);
				m_TableMapTree.SortChildren(parent ? parent : m_TableMapTree.GetSelectedItem());
				m_TableMapTree.SelectItem(new_hti);

				pDoc->SetModifiedFlag(true);
				Invalidate(false);
			}
		}
	}
	
	else if (selected_parent_text == "Symbols")
	{
		// Prep dialog
		dlgsymbols.titletext = "New Symbol record";
		dlgsymbols.name = "";
		dlgsymbols.value = "";
		::GetWindowText(pDoc->attached_hwnd, title, 200);
		dlgsymbols.titlebartext = title;
		dlgsymbols.strings.RemoveAll();

		for (i=0; i<num_s$strings; i++)
		{
			used_string = false;
			for (j=0; j<pDoc->tablemap.s$.GetCount(); j++)
				if (pDoc->tablemap.s$[j].name == s$strings[i])  
					used_string=true;

			if (!used_string)
				dlgsymbols.strings.Add(s$strings[i]);
		}

		// Show dialog if there are any strings left to add
		if (dlgsymbols.strings.GetSize() == 0)
		{
			MessageBox("All Symbol records are already present.");
		}
		else
		{
			if (dlgsymbols.DoModal() == IDOK && dlgsymbols.name!="")
			{
				// Add new record to internal structure
				new_symbol.name = dlgsymbols.name;
				new_symbol.text = dlgsymbols.value;
				new_index = (int) pDoc->tablemap.s$.Add(new_symbol);

				// Add new record to tree
				new_hti = m_TableMapTree.InsertItem(new_symbol.name, parent ? parent : m_TableMapTree.GetSelectedItem());
				m_TableMapTree.SetItemData(new_hti, (DWORD_PTR) &pDoc->tablemap.s$[new_index]);
				m_TableMapTree.SortChildren(parent ? parent : m_TableMapTree.GetSelectedItem());
				m_TableMapTree.SelectItem(new_hti);

				pDoc->SetModifiedFlag(true);
				Invalidate(false);
			}
		}
	}
	
	else if (selected_parent_text == "Regions")
	{
		// Prep dialog
		dlgregions.titletext = "New Region record";
		dlgregions.name = "";
		dlgregions.strings.RemoveAll();
		for (i=0; i<num_r$strings; i++)
		{
			used_string = false;
			for (j=0; j<pDoc->tablemap.r$.GetCount(); j++)
				if (pDoc->tablemap.r$[j].name == r$strings[i] && r$strings[i] != "tablepoint")  
					used_string=true;

			if (!used_string)
				dlgregions.strings.Add(r$strings[i]);
		}

		// Show dialog if there are any strings left to add
		if (dlgregions.strings.GetSize() == 0)
		{
			MessageBox("All Region records are already present.");
		}
		else
		{
			if (dlgregions.DoModal() == IDOK && dlgregions.name!="")
			{
				// Add new record to internal structure
				new_region.name = dlgregions.name;
				new_region.left = 0;
				new_region.top = 0;
				new_region.right = 0;
				new_region.bottom = 0;
				new_region.color = 0;
				new_region.radius = 0;
				new_region.transform = "N";
				new_index = (int) pDoc->tablemap.r$.Add(new_region);

				// Add new record to tree
				new_hti = m_TableMapTree.InsertItem(new_region.name, parent ? parent : m_TableMapTree.GetSelectedItem());
				m_TableMapTree.SetItemData(new_hti, (DWORD_PTR) &pDoc->tablemap.r$[new_index]);
				m_TableMapTree.SortChildren(parent ? parent : m_TableMapTree.GetSelectedItem());
				m_TableMapTree.SelectItem(new_hti);

				pDoc->SetModifiedFlag(true);
				Invalidate(false);
			}
		}
	}
	
	else if (selected_parent_text == "Fonts")
	{
		// Not valid - add font character using "Create Font" button
	}
	
	else if (selected_parent_text == "Hash Points")
	{
		// Prep dialog
		dlghashpoint.titletext = "New Hash Point record";
		dlghashpoint.type = "Type 0";
		dlghashpoint.x = 0;
		dlghashpoint.y = 0;

		// Show dialog
		if (dlghashpoint.DoModal() == IDOK)
		{
			// Add new record to internal structure
			new_hashpoint.number = atoi(dlghashpoint.type.Mid(5,1).GetString());
			new_hashpoint.x = dlghashpoint.x;
			new_hashpoint.y = dlghashpoint.y;
			new_index = (int) pDoc->tablemap.p$.Add(new_hashpoint);

			// Add new record to tree
			text.Format("%d (%d, %d)", new_hashpoint.number, new_hashpoint.x, new_hashpoint.y);
			new_hti = m_TableMapTree.InsertItem(text.GetString(), parent ? parent : m_TableMapTree.GetSelectedItem());
			m_TableMapTree.SetItemData(new_hti, (DWORD_PTR) &pDoc->tablemap.p$[new_index]);
			m_TableMapTree.SortChildren(parent ? parent : m_TableMapTree.GetSelectedItem());
			m_TableMapTree.SelectItem(new_hti);

			pDoc->SetModifiedFlag(true);
			Invalidate(false);
		}
	}
	
	else if (selected_parent_text == "Hashes")
	{
		// Not valid - add hash using "Create Hash" button
	}

	else if (selected_parent_text == "Images")
	{
		// Not valid - add image using "Create Image" button
	}

}

void CDlgTableMap::OnBnClickedDelete()
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	int					i, del_index;
	CString				text;
	CString				sel = m_TableMapTree.GetItemText(m_TableMapTree.GetSelectedItem());
	HTREEITEM			parent = m_TableMapTree.GetParentItem(m_TableMapTree.GetSelectedItem());
	HTREEITEM			node;
	Stablemap_size		*sel_size_ptr = NULL;
	Stablemap_symbol	*sel_symbol_ptr = NULL;
	Stablemap_region	*sel_region_ptr = NULL;
	Stablemap_font		*sel_font_ptr = NULL;
	Stablemap_hash_point	*sel_hash_point_ptr = NULL;
	Stablemap_hash_value	*sel_hash_value_ptr = NULL;
	Stablemap_image		*sel_image_ptr = NULL;
	CString				selected_parent_text = "";

	if (parent != NULL) 
		selected_parent_text = m_TableMapTree.GetItemText(parent);

	if (selected_parent_text == "Sizes")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_size_ptr = (Stablemap_size *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_size_ptr)
		{
			text.Format("Really delete Size record: %s", sel);
			if (MessageBox(text.GetString(), "Delete Size record?", MB_YESNO) == IDYES)
			{
				// Delete record from internal structure and update tree
				del_index = -1;
				for (i=0; i<pDoc->tablemap.z$.GetSize() && del_index==-1; i++)
				{
					if (&pDoc->tablemap.z$[i] == sel_size_ptr)
						del_index = i;
				}

				if (del_index != -1)
				{
					pDoc->tablemap.z$.RemoveAt(del_index, 1);
					node = update_tree("Sizes");
					if (node!=NULL)				
						m_TableMapTree.SelectItem(node);

					Invalidate(false);
					pDoc->SetModifiedFlag(true);
				}
			}
		}
	}
	
	else if (selected_parent_text == "Symbols")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_symbol_ptr = (Stablemap_symbol *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_symbol_ptr)
		{
			text.Format("Really delete Symbol record: %s", sel);
			if (MessageBox(text.GetString(), "Delete Symbol record?", MB_YESNO) == IDYES)
			{
				// Delete record from internal structure and update tree
				del_index = -1;
				for (i=0; i<pDoc->tablemap.s$.GetSize() && del_index==-1; i++)
					if (&pDoc->tablemap.s$[i] == sel_symbol_ptr)
						del_index = i;

				if (del_index != -1)
				{
					pDoc->tablemap.s$.RemoveAt(del_index, 1);
					node = update_tree("Symbols");
					if (node!=NULL)				
						m_TableMapTree.SelectItem(node);

					Invalidate(false);
					pDoc->SetModifiedFlag(true);
				}
			}
		}
	}
	
	else if (selected_parent_text == "Regions")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_region_ptr)
		{
			text.Format("Really delete Region record: %s", sel);
			if (MessageBox(text.GetString(), "Delete Region record?", MB_YESNO) == IDYES)
			{
				// Delete record from internal structure and update tree
				del_index = -1;
				for (i=0; i<pDoc->tablemap.r$.GetSize() && del_index==-1; i++)
					if (&pDoc->tablemap.r$[i] == sel_region_ptr)
						del_index = i;

				if (del_index != -1)
				{
					pDoc->tablemap.r$.RemoveAt(del_index, 1);
					node = update_tree("Regions");
					if (node!=NULL)				
						m_TableMapTree.SelectItem(node);

					Invalidate(false);
					pDoc->SetModifiedFlag(true);
				}
			}
		}
	}
	
	else if (selected_parent_text == "Fonts")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_font_ptr = (Stablemap_font *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_font_ptr)
		{
			text.Format("Really delete Font record: %s", sel);
			if (MessageBox(text.GetString(), "Delete Font record?", MB_YESNO) == IDYES)
			{
				// Delete record from internal structure and update tree
				del_index = -1;
				for (i=0; i<pDoc->tablemap.t$.GetSize() && del_index==-1; i++)
					if (&pDoc->tablemap.t$[i] == sel_font_ptr)
						del_index = i;

				if (del_index != -1)
				{
					pDoc->tablemap.t$.RemoveAt(del_index, 1);
					node = update_tree("Fonts");
					if (node!=NULL)				
						m_TableMapTree.SelectItem(node);

					Invalidate(false);
					pDoc->SetModifiedFlag(true);
				}
			}
		}
	}
	
	else if (selected_parent_text == "Hash Points")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_hash_point_ptr = (Stablemap_hash_point *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_hash_point_ptr)
		{
			text.Format("Really delete Hash Point record: %s", sel);
			if (MessageBox(text.GetString(), "Delete Hash Point record?", MB_YESNO) == IDYES)
			{
				// Delete record from internal structure and update tree
				del_index = -1;
				for (i=0; i<pDoc->tablemap.p$.GetSize() && del_index==-1; i++)
					if (&pDoc->tablemap.p$[i] == sel_hash_point_ptr)
						del_index = i;

				if (del_index != -1)
				{
					pDoc->tablemap.p$.RemoveAt(del_index, 1);
					node = update_tree("Hash Points");
					if (node!=NULL)				
						m_TableMapTree.SelectItem(node);

					Invalidate(false);
					pDoc->SetModifiedFlag(true);
				}
			}
		}
	}
	
	else if (selected_parent_text == "Hashes")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_hash_value_ptr = (Stablemap_hash_value *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_hash_value_ptr)
		{
			text.Format("Really delete Hash record: %s", sel);
			if (MessageBox(text.GetString(), "Delete Hash record?", MB_YESNO) == IDYES)
			{
				// Delete record from internal structure and update tree
				del_index = -1;
				for (i=0; i<pDoc->tablemap.h$.GetSize() && del_index==-1; i++)
					if (&pDoc->tablemap.h$[i] == sel_hash_value_ptr)
						del_index = i;

				if (del_index != -1)
				{
					pDoc->tablemap.h$.RemoveAt(del_index, 1);
					node = update_tree("Hashes");
					if (node!=NULL)				
						m_TableMapTree.SelectItem(node);

					Invalidate(false);
					pDoc->SetModifiedFlag(true);
				}
			}
		}
	}

	else if (selected_parent_text == "Images")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_image_ptr = (Stablemap_image *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_image_ptr)
		{
			text.Format("Really delete Image record: %s", sel);
			if (MessageBox(text.GetString(), "Delete Image record?", MB_YESNO) == IDYES)
			{
				// Delete record from internal structure and update tree
				del_index = -1;
				for (i=0; i<pDoc->tablemap.i$.GetSize() && del_index==-1; i++)
					if (&pDoc->tablemap.i$[i] == sel_image_ptr)
						del_index = i;

				if (del_index != -1)
				{
					pDoc->tablemap.i$.RemoveAt(del_index, 1);
					node = update_tree("Images");
					if (node!=NULL)				
						m_TableMapTree.SelectItem(node);

					Invalidate(false);
					pDoc->SetModifiedFlag(true);
				}
			}
		}
	}
}

HTREEITEM CDlgTableMap::update_tree(CString node_text)
{
	int				i;
	bool			expanded[10];
	HTREEITEM		node;
	CString			text;

	// Save expanded state of each node
	i = 0;
	node = m_TableMapTree.GetChildItem(NULL);
	while (node!=NULL)
	{
		expanded[i++] = (m_TableMapTree.GetItemState(node, TVIS_EXPANDED) & TVIS_EXPANDED) != 0;
		node = m_TableMapTree.GetNextItem(node, TVGN_NEXT);
	}

	// Recreate the tree
	create_tree();

	// Restore expanded state of each node
	i = 0;
	node = m_TableMapTree.GetChildItem(NULL);
	while (node!=NULL)
	{
		m_TableMapTree.Expand(node, expanded[i++] ? TVE_EXPAND : TVE_COLLAPSE);
		node = m_TableMapTree.GetNextItem(node, TVGN_NEXT);
	}

	// Find root node and return it
	node = m_TableMapTree.GetChildItem(NULL);
	text = m_TableMapTree.GetItemText(node);

	while (text!=node_text && node!=NULL)
	{
		node = m_TableMapTree.GetNextItem(node, TVGN_NEXT);
		text = m_TableMapTree.GetItemText(node);
	}

	return node;
}

void CDlgTableMap::OnBnClickedEdit()
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	CString				text, num;
	CString				sel = m_TableMapTree.GetItemText(m_TableMapTree.GetSelectedItem());
	HTREEITEM			parent = m_TableMapTree.GetParentItem(m_TableMapTree.GetSelectedItem());
	HTREEITEM			node;
	int					i, j;
	char				title[200];
	CDlgEditSizes		dlgsizes;
	CDlgEditSymbols		dlgsymbols;
	CDlgEditRegion		dlgregions;
	CDlgEditHashPoint	dlghashpoint;
	CDlgEditGrHashPoints	dlggrhashpoints;
	CString				selected_parent_text = "";
	Stablemap_size		*sel_size_ptr = NULL;
	Stablemap_symbol	*sel_symbol_ptr = NULL;
	Stablemap_region	*sel_region_ptr = NULL;
	Stablemap_hash_point	*sel_hash_point_ptr = NULL, temp_hash_point;

	if (parent != NULL) 
		selected_parent_text = m_TableMapTree.GetItemText(parent);

	if (selected_parent_text == "Sizes")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_size_ptr = (Stablemap_size *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_size_ptr)
		{
			// Prep dialog
			dlgsizes.titletext = "Edit Size record";
			dlgsizes.name = sel_size_ptr->name;
			dlgsizes.width = sel_size_ptr->width;
			dlgsizes.height = sel_size_ptr->height;
			dlgsizes.strings.RemoveAll();
			for (j=0; j<num_z$strings; j++)  dlgsizes.strings.Add(z$strings[j]);

			// Show dialog
			if (dlgsizes.DoModal() == IDOK)
			{
				// Edit record in internal structure
				sel_size_ptr->name = dlgsizes.name;
				sel_size_ptr->width = dlgsizes.width;
				sel_size_ptr->height = dlgsizes.height;

				// Edit record in tree
				m_TableMapTree.SetItemText(m_TableMapTree.GetSelectedItem(), dlgsizes.name.GetString());
				m_TableMapTree.SortChildren(parent ? parent : m_TableMapTree.GetSelectedItem());
				m_TableMapTree.SelectItem(m_TableMapTree.GetSelectedItem());

				update_display();
				Invalidate(false);
				pDoc->SetModifiedFlag(true);
			}
		}
	}
	
	else if (selected_parent_text == "Symbols")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_symbol_ptr = (Stablemap_symbol *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_symbol_ptr)
		{
			// Prep dialog
			dlgsymbols.titletext = "Edit Symbol record";
			dlgsymbols.name = sel_symbol_ptr->name;
			dlgsymbols.value = sel_symbol_ptr->text;
			::GetWindowText(pDoc->attached_hwnd, title, 200);
			dlgsymbols.titlebartext = title;
			dlgsymbols.strings.RemoveAll();
			for (j=0; j<num_s$strings; j++)  dlgsymbols.strings.Add(s$strings[j]);
			
			// Show dialog
			if (dlgsymbols.DoModal() == IDOK)
			{
				// Edit record in internal structure
				sel_symbol_ptr->name = dlgsymbols.name;
				sel_symbol_ptr->text = dlgsymbols.value;

				// Edit record in tree
				m_TableMapTree.SetItemText(m_TableMapTree.GetSelectedItem(), dlgsymbols.name.GetString());
				m_TableMapTree.SortChildren(parent ? parent : m_TableMapTree.GetSelectedItem());
				m_TableMapTree.SelectItem(m_TableMapTree.GetSelectedItem());

				update_display();
				Invalidate(false);
				pDoc->SetModifiedFlag(true);
			}
		}
	}
	
	else if (selected_parent_text == "Regions")
	{
		if (m_TableMapTree.GetSelectedItem())
			sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		if (sel_region_ptr)
		{
			// Prep dialog
			dlgregions.titletext = "Edit Region record";
			dlgregions.name = sel_region_ptr->name;
			dlgregions.strings.RemoveAll();
			for (j=0; j<num_r$strings; j++)  dlgregions.strings.Add(r$strings[j]);
			
			// Show dialog
			if (dlgregions.DoModal() == IDOK)
			{
				// Edit record in internal structure
				sel_region_ptr->name = dlgregions.name;

				// Edit record in tree
				m_TableMapTree.SetItemText(m_TableMapTree.GetSelectedItem(), dlgregions.name.GetString());
				m_TableMapTree.SortChildren(parent ? parent : m_TableMapTree.GetSelectedItem());
				m_TableMapTree.SelectItem(m_TableMapTree.GetSelectedItem());

				update_display();
				Invalidate(false);
				pDoc->SetModifiedFlag(true);
			}
		}
	}
	
	else if (selected_parent_text == "Fonts")
	{
		// Not valid - should delete and add a new one using "Create Font" button
	}
	
	else if (selected_parent_text == "Hash Points" ||
			 parent == NULL && sel == "Hash Points")
	{
		// If parent==NULL, then the root of hash points is selected and we should bring up the graphical hash point editor
		if (parent==NULL)
		{
			// Prep dialog
			for (i=0; i<pDoc->tablemap.p$.GetCount(); i++)
			{
				temp_hash_point.number = pDoc->tablemap.p$[i].number;
				temp_hash_point.x = pDoc->tablemap.p$[i].x;
				temp_hash_point.y = pDoc->tablemap.p$[i].y;
				dlggrhashpoints.working_hash_points.Add(temp_hash_point);
			}

			// Show dialog
			if (dlggrhashpoints.DoModal() == IDOK)
			{
				// Clear all existing hash points
				pDoc->tablemap.p$.RemoveAll();

				// Load new has points from dialog into internal structure
				for (i=0; i<dlggrhashpoints.working_hash_points.GetSize(); i++)
				{
					temp_hash_point.number = dlggrhashpoints.working_hash_points[i].number;
					temp_hash_point.x = dlggrhashpoints.working_hash_points[i].x;
					temp_hash_point.y = dlggrhashpoints.working_hash_points[i].y;
					pDoc->tablemap.p$.Add(temp_hash_point);
				}

				// Rebuild tree
				node = update_tree("Hash Points");
				if (node!=NULL)				
					m_TableMapTree.SelectItem(node);

				update_display();
				Invalidate(false);
				pDoc->SetModifiedFlag(true);
			}
		}

		// else bring up the individual hash point editor
		else
		{
			if (m_TableMapTree.GetSelectedItem())
				sel_hash_point_ptr = (Stablemap_hash_point *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

			if (sel_hash_point_ptr)
			{
				j = 0;
				num = "";
				while (sel.Mid(j, 1) != " ")  num += sel.Mid(j++, 1);

				// Prep dialog
				dlghashpoint.titletext = "Edit Hash Point record";
				text.Format("Type %s", num);
				dlghashpoint.type = text;
				dlghashpoint.x = sel_hash_point_ptr->x;
				dlghashpoint.y = sel_hash_point_ptr->y;
				
				// Show dialog
				if (dlghashpoint.DoModal() == IDOK)
				{

					// Edit record in internal structure
					sel_hash_point_ptr->number = atoi(dlghashpoint.type.Mid(5,1).GetString());
					sel_hash_point_ptr->x = dlghashpoint.x;
					sel_hash_point_ptr->y = dlghashpoint.y;

					// Edit record in tree
					text.Format("%d (%d, %d)", sel_hash_point_ptr->number, sel_hash_point_ptr->x, sel_hash_point_ptr->y);
					m_TableMapTree.SetItemText(m_TableMapTree.GetSelectedItem(), text.GetString());
					m_TableMapTree.SortChildren(parent ? parent : m_TableMapTree.GetSelectedItem());
					m_TableMapTree.SelectItem(m_TableMapTree.GetSelectedItem());

					update_display();
					Invalidate(false);
					pDoc->SetModifiedFlag(true);
				}
			}
		}
	}
	
	else if (selected_parent_text == "Hashes")
	{
		// Not valid - should delete and add a new one using "Create Hash" button
	}

	else if (selected_parent_text == "Images")
	{
		// Not valid - should delete and add a new one using "Create Image" button
	}
}

void CDlgTableMap::OnBnClickedCreateImage()
{
	CDlgEdit			edit;
	Stablemap_image		new_image;
	int					x, y, width, height, pix_cnt, new_index;
	BYTE				alpha, red, green, blue;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	HTREEITEM			new_hti;
	CString				text, node_text;
	HTREEITEM			image_node;
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr)
	{

		edit.m_titletext = "Name of new image";
		edit.m_result = "";
		if (edit.DoModal() == IDOK) 
		{
			// Get bitmap size
			width = pDoc->attached_rect.right - pDoc->attached_rect.left;
			height = pDoc->attached_rect.bottom - pDoc->attached_rect.top;

			// Populate new image record			
			new_image.name = edit.m_result;
			new_image.width = sel_region_ptr->right - sel_region_ptr->left;
			new_image.height = sel_region_ptr->bottom - sel_region_ptr->top;
			pix_cnt = 0;
			for (y=(int) sel_region_ptr->top; y < (int) sel_region_ptr->bottom; y++) {
				for (x=(int) sel_region_ptr->left; x < (int) sel_region_ptr->right; x++) {
					alpha = pDoc->attached_pBits[y*width*4 + x*4 + 3];
					red = pDoc->attached_pBits[y*width*4 + x*4 + 2];
					green = pDoc->attached_pBits[y*width*4 + x*4 + 1];
					blue = pDoc->attached_pBits[y*width*4 + x*4 + 0];

					// image record is stored internally in ABGR format
					new_image.pixel[pix_cnt++] = (alpha<<24) + (blue<<16) + (green<<8) + red;
				}
			}

			// Insert the new record in the existing array of i$ records
			new_index = (int) pDoc->tablemap.i$.Add(new_image);

			// Find root of the Images node
			image_node = m_TableMapTree.GetChildItem(NULL);
			node_text = m_TableMapTree.GetItemText(image_node);

			while (node_text!="Images" && image_node!=NULL)
			{
				image_node = m_TableMapTree.GetNextItem(image_node, TVGN_NEXT);
				node_text = m_TableMapTree.GetItemText(image_node);
			}

			// Insert the new record into the tree
			if (image_node!=NULL)
			{
				text.Format("%s (%dx%d)", new_image.name, new_image.width, new_image.height);
				new_hti = m_TableMapTree.InsertItem(text.GetString(), image_node);
				m_TableMapTree.SetItemData(new_hti, (DWORD_PTR) &pDoc->tablemap.i$[new_index]);
				m_TableMapTree.SortChildren(image_node);
			}

			//Invalidate(false);
			pDoc->SetModifiedFlag(true);
		}
	}
}

void CDlgTableMap::OnBnClickedCreateFont()
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	CDlgEditFont		dlg_editfont;
	Stablemap_font		new_font;
	CString				text, separation, hex, num;
	int					width, height, pos, x_cnt, scan_pos;
	HDC					hdcScreen, hdc, hdc_region;
	HBITMAP				old_bitmap, old_bitmap2, bitmap_region;
	CTransform			trans;
	bool				character[MAX_CHAR_WIDTH][MAX_CHAR_HEIGHT], background[MAX_CHAR_WIDTH];
	char				*cresult, hexmash[MAX_CHAR_WIDTH*16];
	int					char_field_x_begin, char_field_x_end, char_field_y_begin, char_field_y_end;
	int					i, j, insert_point, new_index;
	char				hexmash_array[512][MAX_SINGLE_CHAR_WIDTH*8 + 1];
	HTREEITEM			new_hti, font_node, region_node, child_node;
	CString				node_text;
	HTREEITEM			parent = m_TableMapTree.GetParentItem(m_TableMapTree.GetSelectedItem());
	CArray <Stablemap_font, Stablemap_font>		new_t$_recs;
	Stablemap_region	*sel_region_ptr = NULL;
	CString				sel_region_name;
	
	if (m_TableMapTree.GetSelectedItem())
	{
		// Get pointer to selected region record
		sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

		// Save currently selected region so we can reselect it at the end
		sel_region_name = m_TableMapTree.GetItemText(m_TableMapTree.GetSelectedItem());
	}

	if (sel_region_ptr)
	{
		// Get bitmap size
		width = sel_region_ptr->right - sel_region_ptr->left;
		height = sel_region_ptr->bottom - sel_region_ptr->top;

		// Select saved bitmap into memory DC
		hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL); 
		hdc = CreateCompatibleDC(hdcScreen);
		old_bitmap = (HBITMAP) SelectObject(hdc, pDoc->attached_bitmap);

		// Create new memory DC, new bitmap, and copy our region into the new dc/bitmap
		hdc_region = CreateCompatibleDC(hdcScreen);
		bitmap_region = CreateCompatibleBitmap(hdcScreen, width, height);
		old_bitmap2 = (HBITMAP) SelectObject(hdc_region, bitmap_region);
		BitBlt(hdc_region, 0, 0, width, height, hdc, sel_region_ptr->left, sel_region_ptr->top, SRCCOPY);

		// Get the pixels
		trans.t_transform(&pDoc->tablemap, sel_region_ptr, hdc_region, &text, &separation, background, character);

		// Clean up
		SelectObject(hdc_region, old_bitmap2);
		DeleteObject(bitmap_region);
		DeleteDC(hdc_region);

		SelectObject(hdc, old_bitmap);
		DeleteDC(hdc);
		DeleteDC(hdcScreen);

		// populate hexmash array
		for (i=0; i<(int) pDoc->tablemap.t$.GetSize(); i++)
			strcpy(hexmash_array[i], pDoc->tablemap.t$[i].hexmash);

		// Scan through background, separate characters by looking for background bands
		new_t$_recs.RemoveAll();
		int start = 0;

		// skip initial background bands
		scan_pos = 0;
		while (background[scan_pos] && scan_pos < width)
			scan_pos++;

		while (scan_pos < width)
		{
			start = scan_pos;

			// scan for next background band
			while (!background[scan_pos] && scan_pos < width)
				scan_pos++;

			// We got a background bar, add element to array
			trans.get_shift_left_down_indexes(start, scan_pos-start, height, background, character, 
											  &char_field_x_begin, &char_field_x_end, &char_field_y_begin, &char_field_y_end);

			// Get individual hex values
			trans.calc_hexmash(char_field_x_begin, char_field_x_end, char_field_y_begin, char_field_y_end, character, hexmash, true);
			hex.Format("%s", hexmash);
			pos = x_cnt = 0;
			num = hex.Tokenize(" ", pos);
			while (pos != -1 && x_cnt<MAX_SINGLE_CHAR_WIDTH)
			{
				new_font.x[x_cnt++] = strtoul(num.GetString(), NULL, 16);
				num = hex.Tokenize(" ", pos);
			}
			new_font.x_count = x_cnt;

			// Get whole hexmash
			trans.calc_hexmash(char_field_x_begin, char_field_x_end, char_field_y_begin, char_field_y_end, character, hexmash, false);
			new_font.hexmash.Format("%s", hexmash);


			// Search for this character in the existing t$ records list
			cresult = (char *) bsearch( hexmash, hexmash_array, pDoc->tablemap.t$.GetSize(), sizeof(hexmash_array[0]),
										(int (*)(const void*, const void*)) compare_font);

			// Populate new font record
			if (cresult!=NULL) 
			{
				//new_font.ch = pDoc->tablemap.t$[((long)cresult - (long)hexmash_array[0]) / sizeof(hexmash_array[0])].ch;
				//new_font.group = pDoc->tablemap.t$[((long)cresult - (long)hexmash_array[0]) / sizeof(hexmash_array[0])].group;

				// Insert the new record in the existing array of i$ records
				//new_t$_recs.Add(new_font);
			}
			else			
			{
				m_Transform.GetLBText(m_Transform.GetCurSel(), text);
				new_font.ch = '?';
				new_font.group = text.GetString()[4]-'0';

				// Insert the new record in the existing array of i$ records
				new_t$_recs.Add(new_font);
			}

			// skip background bands
			while (background[scan_pos] && scan_pos < width)
				scan_pos++;

		}

		if (new_t$_recs.GetCount() == 0)
		{
			MessageBox("No foreground pixels, or no unknown characters found.", "Font creation error");
		}
		else
		{
			// Prepare dialog box
			dlg_editfont.titletext = "Add font characters";
			dlg_editfont.character = "";
			dlg_editfont.type = "Type 0";
			dlg_editfont.new_t$_recs = &new_t$_recs;

			if (dlg_editfont.DoModal() == IDOK) 
			{
				// Find root of the Images node
				font_node = m_TableMapTree.GetChildItem(NULL);
				node_text = m_TableMapTree.GetItemText(font_node);
				while (node_text!="Fonts" && font_node!=NULL)
				{
					font_node = m_TableMapTree.GetNextItem(font_node, TVGN_NEXT);
					node_text = m_TableMapTree.GetItemText(font_node);
				}

				for (i=0; i<new_t$_recs.GetCount(); i++)
				{
					// Populate temp structure
					new_font.x_count = new_t$_recs[i].x_count;
					for (j=0; j<new_font.x_count; j++)
						new_font.x[j] = new_t$_recs[i].x[j];
					new_font.hexmash = new_t$_recs[i].hexmash;
					new_font.group = new_t$_recs[i].group;
					new_font.ch = new_t$_recs[i].ch;

					// Insert into internal structure so it is sorted alphabetically by hexmash
					insert_point = -1;
					for (j=0; j<(int) pDoc->tablemap.t$.GetSize(); j++) 
					{
						if (new_font.hexmash < pDoc->tablemap.t$[j].hexmash) 
						{
							insert_point = j;
							j=(int) pDoc->tablemap.t$.GetSize() + 1;
						}
					}
					if (insert_point==-1) 
					{
						new_index = (int) pDoc->tablemap.t$.Add(new_font);
					}
					else 
					{
						pDoc->tablemap.t$.InsertAt(insert_point, new_font);
						new_index = insert_point;
					}

					// Insert the new record into the tree
					if (font_node!=NULL)
					{
						// Insert the new records into the tree
						text.Format("%c (%d)", new_font.ch, new_font.group);
						new_hti = m_TableMapTree.InsertItem(text.GetString(), font_node);
						m_TableMapTree.SetItemData(new_hti, (DWORD_PTR) &pDoc->tablemap.t$[new_index]);
					}
				}

				region_node = update_tree("Regions");
				m_TableMapTree.SortChildren(font_node);

				// Re-select previously selected region
				child_node = m_TableMapTree.GetChildItem(region_node);
				node_text = m_TableMapTree.GetItemText(child_node);
				while (node_text!=sel_region_name && child_node!=NULL)
				{
					child_node = m_TableMapTree.GetNextItem(child_node, TVGN_NEXT);
					node_text = m_TableMapTree.GetItemText(child_node);
				}

				if (child_node)
					m_TableMapTree.SelectItem(child_node);

				// Update display and set dirty bit 
				update_display();
				Invalidate(false);
				pDoc->SetModifiedFlag(true);
			}
		}
	}
}

void CDlgTableMap::OnBnClickedCreateHash()
{
	CDlgEditHash			dlghash;
	Stablemap_hash_value	new_hash;
	bool					t[4] = { false };
	COpenScrapeDoc			*pDoc = COpenScrapeDoc::GetDocument();
	CString					sel = m_TableMapTree.GetItemText(m_TableMapTree.GetSelectedItem());
	HTREEITEM				parent = m_TableMapTree.GetParentItem(m_TableMapTree.GetSelectedItem());
	HTREEITEM				new_hti, node;
	CString					text, node_text;
	int						j, new_index;

	// Get image name
	j = 0;
	sel = sel.Tokenize(" ", j);

	// See which hash types are already present for this image
	for (j=0; j<pDoc->tablemap.h$.GetSize(); j++)
		if (pDoc->tablemap.h$[j].name == sel)
			t[pDoc->tablemap.h$[j].number] = true;

	// Prepare dialog
	dlghash.titletext = "Name of new font character";
	dlghash.strings.RemoveAll();
	if (!t[0])  dlghash.strings.Add("Hash 0");
	if (!t[1])  dlghash.strings.Add("Hash 1");
	if (!t[2])  dlghash.strings.Add("Hash 2");
	if (!t[3])  dlghash.strings.Add("Hash 3");

	// Show dialog if there are any strings left to add
	if (dlghash.strings.GetSize() == 0)
	{
		MessageBox("All Hash types are already present for this image.");
	}
	else
	{
		if (dlghash.DoModal() == IDOK)
		{
			// Add new record to internal structure
			new_hash.name = sel;
			new_hash.number = atoi(dlghash.type.Mid(5,1).GetString());
			new_hash.hash = 0;
			new_index = (int) pDoc->tablemap.h$.Add(new_hash);

			// Warn to update hashes
			//MessageBox("Hash record created.  Don't forget to 'Edit/Update Hashes'.");

			// Find root of the Hashes node
			node = m_TableMapTree.GetChildItem(NULL);
			node_text = m_TableMapTree.GetItemText(node);

			while (node_text!="Hashes" && node!=NULL)
			{
				node = m_TableMapTree.GetNextItem(node, TVGN_NEXT);
				node_text = m_TableMapTree.GetItemText(node);
			}

			// Insert the new record into the tree
			if (node!=NULL)
			{
				// Add new record to tree
				text.Format("%s (%d)", new_hash.name, new_hash.number);
				new_hti = m_TableMapTree.InsertItem(text.GetString(), node);
				m_TableMapTree.SetItemData(new_hti, (DWORD_PTR) &pDoc->tablemap.h$[new_index]);
				m_TableMapTree.SortChildren(node);
			}

			//Invalidate(false);
			pDoc->SetModifiedFlag(true);
		}
	}
}

void CDlgTableMap::OnBnClickedFontplus()
{
	separation_font_size++;
	lf_fixed.lfHeight = separation_font_size;
	separation_font.CreateFontIndirect(&lf_fixed);
	m_PixelSeparation.SetFont(&separation_font);
	Invalidate(false);
}

void CDlgTableMap::OnBnClickedFontminus()
{
	separation_font_size--;
	lf_fixed.lfHeight = separation_font_size;
	separation_font.CreateFontIndirect(&lf_fixed);
	m_PixelSeparation.SetFont(&separation_font);
	Invalidate(false);
}

BOOL CDlgTableMap::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	RECT			bmp_rect;
	POINT			point;

	m_BitmapFrame.GetClientRect(&bmp_rect);
	GetCursorPos(&point);
	m_BitmapFrame.ScreenToClient(&point);

	if (picker_cursor &&
		point.x >= bmp_rect.left && point.x <= bmp_rect.right &&
		point.y >= bmp_rect.top && point.y <= bmp_rect.bottom)
	{
		SetCursor(hCurPicker);
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CDlgTableMap::OnStickyButtonDown(WPARAM wp, LPARAM lp)
{
	COpenScrapeView			*pView = COpenScrapeView::GetView();

	if ((HWND) wp == m_DrawRect.GetSafeHwnd())
	{
		pView->drawing_rect = true;
	}

	else if ((HWND) wp == m_Picker.GetSafeHwnd())
	{
		picker_cursor = true;
		SetCursor(hCurPicker);
	}

	return false;
}

LRESULT CDlgTableMap::OnStickyButtonUp(WPARAM wp, LPARAM lp)
{
	COpenScrapeView			*pView = COpenScrapeView::GetView();

	if ((HWND) wp == m_DrawRect.GetSafeHwnd())
	{
		pView->drawing_rect = false;
	}

	else if ((HWND) wp == m_Picker.GetSafeHwnd())
	{
		picker_cursor = false;
		SetCursor(hCurStandard);
		update_display();
		Invalidate(false);
	}

	return false;
}

void CDlgTableMap::OnLButtonDown(UINT nFlags, CPoint point)
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	RECT				bmp_rect;
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr)
	{
		m_BitmapFrame.GetWindowRect(&bmp_rect);
		ScreenToClient(&bmp_rect);

		if (picker_cursor &&
			point.x >= bmp_rect.left && point.x <= bmp_rect.right &&
			point.y >= bmp_rect.top && point.y <= bmp_rect.bottom)
		{
			sel_region_ptr->color = get_color_under_mouse(&nFlags, &point);

			update_display();
			Invalidate(false);
			pDoc->SetModifiedFlag(true);

			m_Picker.OnBnClicked();
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgTableMap::OnMouseMove(UINT nFlags, CPoint point)
{
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	RECT				bmp_rect;
	Stablemap_region	*sel_region_ptr = NULL;
	
	// Get pointer to selected region record
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr)
	{
		m_BitmapFrame.GetWindowRect(&bmp_rect);
		ScreenToClient(&bmp_rect);

		if (picker_cursor &&
			point.x >= bmp_rect.left && point.x <= bmp_rect.right &&
			point.y >= bmp_rect.top && point.y <= bmp_rect.bottom)
		{
			sel_region_ptr->color = get_color_under_mouse(&nFlags, &point);

			update_display();
			Invalidate(false);
			pDoc->SetModifiedFlag(true);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

COLORREF CDlgTableMap::get_color_under_mouse(UINT *nFlags, CPoint *point)
{
	CDC				*pDC = GetDC();
	HDC				hdc = *pDC;
	HDC				hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
	HDC				hdcCompatible = CreateCompatibleDC(hdcScreen); 
	COLORREF		cr;
	int				width, height;
	CString			sel = m_TableMapTree.GetItemText(m_TableMapTree.GetSelectedItem());	
	RECT			crect;
	HBITMAP			hbm, old_bitmap;
	BYTE			*pBits, alpha, red, green, blue;

	// Load TableMap dialog into memory DC
	GetClientRect(&crect);
	width = crect.right - crect.left;
	height = crect.bottom - crect.top;
	hbm = CreateCompatibleBitmap(hdcScreen, width, height);
	old_bitmap = (HBITMAP) SelectObject(hdcCompatible, hbm);
	BitBlt(hdcCompatible, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	SelectObject(hdcCompatible, old_bitmap);

	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	// Populate BITMAPINFOHEADER
	bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
	bmi->bmiHeader.biBitCount = 0;
	GetDIBits(hdcCompatible, hbm, 0, 0, NULL, bmi, DIB_RGB_COLORS);

	// Get the actual BGRA bit information
	bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
	pBits = new BYTE[bmi->bmiHeader.biSizeImage];
	GetDIBits(hdc, hbm, 0, height, pBits, bmi, DIB_RGB_COLORS);

	// Get pixel color under mouse click spot - GetDIBits format is BGRA, COLORREF format is ABGR
	alpha = pBits[point->y*width*4 + point->x*4 + 3];
	red = pBits[point->y*width*4 + point->x*4 + 2];
	green = pBits[point->y*width*4 + point->x*4 + 1];
	blue = pBits[point->y*width*4 + point->x*4 + 0];

	cr = (alpha<<24) + (blue<<16) + (green<<8) + (red);

	// Clean up
	delete []pBits;
	HeapFree(GetProcessHeap(), NULL, bmi);

	// Clean up
	DeleteObject(hbm);
	DeleteDC(hdcCompatible);
	DeleteDC(hdcScreen);
	ReleaseDC(pDC);

	return cr;
}

void CDlgTableMap::create_tree(void)
{
	int							i;
	HTREEITEM					parent, newitem;
	COpenScrapeDoc				*pDoc = COpenScrapeDoc::GetDocument();
	CArray<CString, CString>	strings;
	CString						text;

	m_TableMapTree.DeleteAllItems();

	// z$ records
	parent = m_TableMapTree.InsertItem("Sizes");
	m_TableMapTree.SetItemState(parent, TVIS_BOLD, TVIS_BOLD );
	for (i=0; i<pDoc->tablemap.z$.GetSize(); i++) 
	{
		newitem = m_TableMapTree.InsertItem(pDoc->tablemap.z$[i].name, parent);
		m_TableMapTree.SetItemData(newitem, (DWORD_PTR) &pDoc->tablemap.z$[i]);
	}
	m_TableMapTree.SortChildren(parent);

	// s$ records
	parent = m_TableMapTree.InsertItem("Symbols");
	m_TableMapTree.SetItemState(parent, TVIS_BOLD, TVIS_BOLD );
	for (i=0; i<pDoc->tablemap.s$.GetSize(); i++) 
	{
		newitem = m_TableMapTree.InsertItem(pDoc->tablemap.s$[i].name, parent);
		m_TableMapTree.SetItemData(newitem, (DWORD_PTR) &pDoc->tablemap.s$[i]);
	}
	m_TableMapTree.SortChildren(parent);

	// r$ records
	parent = m_TableMapTree.InsertItem("Regions");
	m_TableMapTree.SetItemState(parent, TVIS_BOLD, TVIS_BOLD );
	for (i=0; i<pDoc->tablemap.r$.GetSize(); i++) 
	{
		newitem = m_TableMapTree.InsertItem(pDoc->tablemap.r$[i].name, parent);
		m_TableMapTree.SetItemData(newitem, (DWORD_PTR) &pDoc->tablemap.r$[i]);
	}
	m_TableMapTree.SortChildren(parent);

	// t$ records
	parent = m_TableMapTree.InsertItem("Fonts");
	m_TableMapTree.SetItemState(parent, TVIS_BOLD, TVIS_BOLD );
	for (i=0; i<pDoc->tablemap.t$.GetSize(); i++) 
	{
		text.Format("%c (%d)", pDoc->tablemap.t$[i].ch, pDoc->tablemap.t$[i].group);
		newitem = m_TableMapTree.InsertItem(text, parent);
		m_TableMapTree.SetItemData(newitem, (DWORD_PTR) &pDoc->tablemap.t$[i]);
	}
	m_TableMapTree.SortChildren(parent);

	// p$ records
	parent = m_TableMapTree.InsertItem("Hash Points");
	m_TableMapTree.SetItemState(parent, TVIS_BOLD, TVIS_BOLD );
	for (i=0; i<pDoc->tablemap.p$.GetSize(); i++) 
	{
		text.Format("%d (%d, %d)", pDoc->tablemap.p$[i].number, pDoc->tablemap.p$[i].x, pDoc->tablemap.p$[i].y);
		newitem = m_TableMapTree.InsertItem(text, parent);
		m_TableMapTree.SetItemData(newitem, (DWORD_PTR) &pDoc->tablemap.p$[i]);
	}
	m_TableMapTree.SortChildren(parent);

	// h$ records
	parent = m_TableMapTree.InsertItem("Hashes");
	m_TableMapTree.SetItemState(parent, TVIS_BOLD, TVIS_BOLD );
	for (i=0; i<pDoc->tablemap.h$.GetSize(); i++) 
	{
		text.Format("%s (%d)", pDoc->tablemap.h$[i].name, pDoc->tablemap.h$[i].number);
		newitem = m_TableMapTree.InsertItem(text, parent);
		m_TableMapTree.SetItemData(newitem, (DWORD_PTR) &pDoc->tablemap.h$[i]);
	}
	m_TableMapTree.SortChildren(parent);

	// i$ records
	parent = m_TableMapTree.InsertItem("Images");
	m_TableMapTree.SetItemState(parent, TVIS_BOLD, TVIS_BOLD );
	for (i=0; i<pDoc->tablemap.i$.GetSize(); i++) 
	{
		text.Format("%s (%dx%d)", pDoc->tablemap.i$[i].name, pDoc->tablemap.i$[i].width, pDoc->tablemap.i$[i].height);
		newitem = m_TableMapTree.InsertItem(text, parent);
		m_TableMapTree.SetItemData(newitem, (DWORD_PTR) &pDoc->tablemap.i$[i]);
	}
	m_TableMapTree.SortChildren(parent);
}

void CDlgTableMap::OnBnClickedNudgeTaller()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->top--;
		sel_region_ptr->bottom++;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeShorter()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->top++;
		sel_region_ptr->bottom--;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeWider()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left--;
		sel_region_ptr->right++;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeNarrower()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left++;
		sel_region_ptr->right--;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeBigger()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left--;
		sel_region_ptr->top--;
		sel_region_ptr->right++;
		sel_region_ptr->bottom++;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeSmaller()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left++;
		sel_region_ptr->top++;
		sel_region_ptr->right--;
		sel_region_ptr->bottom--;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeUpleft()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left--;
		sel_region_ptr->top--;
		sel_region_ptr->right--;
		sel_region_ptr->bottom--;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeUp()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->top--;
		sel_region_ptr->bottom--;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeUpright()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left++;
		sel_region_ptr->top--;
		sel_region_ptr->right++;
		sel_region_ptr->bottom--;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeRight()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left++;
		sel_region_ptr->right++;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeDownright()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left++;
		sel_region_ptr->top++;
		sel_region_ptr->right++;
		sel_region_ptr->bottom++;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeDown()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->top++;
		sel_region_ptr->bottom++;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeDownleft()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left--;
		sel_region_ptr->top++;
		sel_region_ptr->right--;
		sel_region_ptr->bottom++;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

void CDlgTableMap::OnBnClickedNudgeLeft()
{
	Stablemap_region	*sel_region_ptr = NULL;
	COpenScrapeDoc		*pDoc = COpenScrapeDoc::GetDocument();
	
	if (m_TableMapTree.GetSelectedItem())
		 sel_region_ptr = (Stablemap_region *) m_TableMapTree.GetItemData(m_TableMapTree.GetSelectedItem());

	if (sel_region_ptr) 
	{
		sel_region_ptr->left--;
		sel_region_ptr->right--;

		update_r$_display(false);
		Invalidate(false);
		theApp.m_pMainWnd->Invalidate(false);

		pDoc->SetModifiedFlag(true);
	}
}

BOOL CDlgTableMap::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	// allow top level routing frame to handle the message
	if (GetRoutingFrame() != NULL)
		return false;

    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT*)pNMHDR;
    UINT nID = pNMHDR->idFrom;
    if(pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
            pTTT->lpszText = MAKEINTRESOURCE(nID);
            pTTT->hinst = AfxGetResourceHandle();
            return(TRUE);
        }
    }

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|
	SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

	return true;    // message was handled
}