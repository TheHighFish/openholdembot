//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************


// OpenScrapeDoc.h : interface of the COpenScrapeDoc class
//
#pragma once
#include "../CTransform/CTransform.h"

int	const num_z$strings = 3;
char * const z$strings[num_z$strings] = 
    { "clientsize", "clientsizemin", "clientsizemax" };

int	const num_s$strings = 64;
char * const s$strings[num_s$strings] = 
    { "titletext", "titletext0", "titletext1", "titletext2", "titletext3", "titletext4", "titletext5", "titletext6",
	  "titletext7", "titletext8", "titletext9", 
	  "!titletext", "!titletext0", "!titletext1", "!titletext2", "!titletext3", "!titletext4", "!titletext5", "!titletext6",
	  "!titletext7", "!titletext8", "!titletext9", 
	  "ttlimits", "ttlimits0", "ttlimits1", "ttlimits2", "ttlimits3", "ttlimits4", "ttlimits5", "ttlimits6",
	  "ttlimits7", "ttlimits8", "ttlimits9", 
	  "c0limits", "c0limits0", "c0limits1", "c0limits2", "c0limits3", "c0limits4", "c0limits5", "c0limits6", "c0limits7", 
	  "c0limits8", "c0limits9", 
	  "sitename", "network", "nchairs", "chairconfig", "swagtextmethod", "potmethod",  
	  "defaultlabels", "t0type", "t1type", "t2type", "t3type", 
	  "swagselectionmethod", "swagdeletionmethod", "swagconfirmationmethod", "buttonclickmethod", 
	  "allinmethod", "balancenumbersonly", "scraperdll", "scraperpreprocessordll",
	  "chipscrapemethod"};

int	const num_r$strings = 383;

char * const r$strings[num_r$strings] = { "c0cardface0", "c0cardface1", "c0cardface2", "c0cardface3", "c0cardface4", 
								"c0cardface0rank", "c0cardface1rank", "c0cardface2rank", "c0cardface3rank", "c0cardface4rank", 
								"c0cardface0suit", "c0cardface1suit", "c0cardface2suit", "c0cardface3suit", "c0cardface4suit", 
								"c0handnumber", "c0handnumber1", "c0handnumber2", "c0handnumber3", "c0handnumber4", "c0handnumber5", "c0handnumber6",
								"c0handnumber7", "c0handnumber8", "c0handnumber9",
								"c0isfinaltable", "c0smallblind", "c0bigblind", "c0bigbet", "c0ante",
								"c0pot0", "c0pot1", "c0pot2", "c0pot3", "c0pot4", 
								"c0pot0chip00", "c0pot0chip01", "c0pot0chip10",
								"c0pot1chip00", "c0pot1chip01", "c0pot1chip10",
								"c0pot2chip00", "c0pot2chip01", "c0pot2chip10",
								"c0pot3chip00", "c0pot3chip01", "c0pot3chip10",
								"c0pot4chip00", "c0pot4chip01", "c0pot4chip10", 
								//50
                "c0cardface0nocard", "c0cardface1nocard", "c0cardface2nocard", "c0cardface3nocard", "c0cardface4nocard",
                // 5
								"c0limits", "c0limits0", "c0limits1", "c0limits2", "c0limits3", "c0limits4", "c0limits5", "c0limits6", "c0limits7", 
								"c0limits8", "c0limits9", 
								"i0button", "i1button", "i2button", "i3button", "i4button", "i5button", "i6button", "i7button", 
								"i8button", "i9button", 
								"i0label", "i1label", "i2label", "i3label", "i4label", "i5label", "i6label", "i7label", 
								"i8label", "i9label", 
								"i0state", "i1state", "i2state", "i3state", "i4state", "i5state", "i6state", "i7state", 
								"i8state", "i9state", 
								"i3handle", "i3slider",
								//41
								"i3edit",
								"i86button", "i860button", "i861button", "i862button", "i863button", "i864button", "i865button", "i866button", 
								"i867button", "i868button", "i869button", 
								"i86state", "i860state", "i861state", "i862state", "i863state", "i864state", "i865state", "i866state", 
								"i867state", "i868state", "i869state", 
								"p0active", "p1active", "p2active", "p3active", "p4active", "p5active", "p6active", "p7active", 
								"p8active", "p9active", 
								"p0balance", "p1balance", "p2balance", "p3balance", "p4balance", "p5balance", "p6balance", "p7balance", 
								"p8balance", "p9balance", 
								"p0bet", "p1bet", "p2bet", "p3bet", "p4bet", "p5bet", "p6bet", "p7bet", 
								"p8bet", "p9bet", 
								//53
								"p0cardback", "p1cardback", "p2cardback", "p3cardback", "p4cardback", "p5cardback", "p6cardback", "p7cardback", 
								"p8cardback", "p9cardback", 
								"p0cardface0", "p1cardface0", "p2cardface0", "p3cardface0", "p4cardface0", "p5cardface0", "p6cardface0", 
								"p7cardface0", "p8cardface0", "p9cardface0", 
								"p0cardface1", "p1cardface1", "p2cardface1", "p3cardface1", "p4cardface1", "p5cardface1", "p6cardface1", 
								"p7cardface1", "p8cardface1", "p9cardface1", 
								"p0cardface0rank", "p1cardface0rank", "p2cardface0rank", "p3cardface0rank", "p4cardface0rank", "p5cardface0rank", 
								"p6cardface0rank", "p7cardface0rank", "p8cardface0rank", "p9cardface0rank", 
								"p0cardface0suit", "p1cardface0suit", "p2cardface0suit", "p3cardface0suit", "p4cardface0suit", "p5cardface0suit", 
								"p6cardface0suit", "p7cardface0suit", "p8cardface0suit", "p9cardface0suit", 
								"p0cardface1rank", "p1cardface1rank", "p2cardface1rank", "p3cardface1rank", "p4cardface1rank", "p5cardface1rank", 
								"p6cardface1rank", "p7cardface1rank", "p8cardface1rank", "p9cardface1rank", 
								"p0cardface1suit", "p1cardface1suit", "p2cardface1suit", "p3cardface1suit", "p4cardface1suit", "p5cardface1suit", 
								"p6cardface1suit", "p7cardface1suit", "p8cardface1suit", "p9cardface1suit", 
								"p0dealer", "p1dealer", "p2dealer", "p3dealer", "p4dealer", "p5dealer", "p6dealer", "p7dealer", 
								"p8dealer", "p9dealer", 
								"p0name", "p1name", "p2name", "p3name", "p4name", "p5name", "p6name", "p7name", 
								"p8name", "p9name", 
								"p0seated", "p1seated", "p2seated", "p3seated", "p4seated", "p5seated", "p6seated", "p7seated", 
								"p8seated", "p9seated", 
								//100
                "p0cardface0nocard", "p1cardface0nocard", "p2cardface0nocard", "p3cardface0nocard", "p4cardface0nocard", 
                "p5cardface0nocard", "p6cardface0nocard", "p7cardface0nocard", "p8cardface0nocard", "p9cardface0nocard", 
								"p0cardface1nocard", "p1cardface1nocard", "p2cardface1nocard", "p3cardface1nocard", "p4cardface1nocard", 
                "p5cardface1nocard", "p6cardface1nocard", "p7cardface1nocard", "p8cardface1nocard", "p9cardface1nocard", 
                // 20
								"p0chip00", "p1chip00", "p2chip00", "p3chip00", "p4chip00", "p5chip00", "p6chip00", "p7chip00", 
								"p8chip00", "p9chip00", 
								"p0chip01", "p1chip01", "p2chip01", "p3chip01", "p4chip01", "p5chip01", "p6chip01", "p7chip01", 
								"p8chip01", "p9chip01", 
								"p0chip10", "p1chip10", "p2chip10", "p3chip10", "p4chip10", "p5chip10", "p6chip10", "p7chip10", 
								"p8chip10", "p9chip10", 
								"u0active", "u1active", "u2active", "u3active", "u4active", "u5active", "u6active", "u7active", 
								"u8active", "u9active", 
								"ubalance", "u0balance", "u1balance", "u2balance", "u3balance", "u4balance", "u5balance", "u6balance", "u7balance", 
								"u8balance", "u9balance",  
								//51
								"u0cardface0", "u1cardface0", "u2cardface0", "u3cardface0", "u4cardface0", "u5cardface0", "u6cardface0", 
								"u7cardface0", "u8cardface0", "u9cardface0", 
								"u0cardface1", "u1cardface1", "u2cardface1", "u3cardface1", "u4cardface1", "u5cardface1", "u6cardface1", 
								"u7cardface1", "u8cardface1", "u9cardface1", 
								"u0bet", "u1bet", "u2bet", "u3bet", "u4bet", "u5bet", "u6bet", "u7bet", 
								"u8bet", "u9bet", 
								"u0dealer", "u1dealer", "u2dealer", "u3dealer", "u4dealer", "u5dealer", "u6dealer", "u7dealer", 
								"u8dealer", "u9dealer", 
								"uname", "u0name", "u1name", "u2name", "u3name", "u4name", "u5name", "u6name", "u7name", 
								"u8name", "u9name", 
								"u0seated", "u1seated", "u2seated", "u3seated", "u4seated", "u5seated", "u6seated", "u7seated", 
								"u8seated", "u9seated"};
								//61


//!  Container class for Tablemap data to aid with loading/saving
class COpenScrapeDoc : public CDocument
{
protected: // create from serialization only
	COpenScrapeDoc();
	DECLARE_DYNCREATE(COpenScrapeDoc)
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

	bool		valid_open;
	bool		is_dirty;
	
public:
	static COpenScrapeDoc* GetDocument();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual ~COpenScrapeDoc();

	// Blink state
	bool		blink_on;

	// Window that we are attached to
	HWND		attached_hwnd;
	RECT		attached_rect;
	HBITMAP		attached_bitmap;
	BYTE		*attached_pBits;


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};


