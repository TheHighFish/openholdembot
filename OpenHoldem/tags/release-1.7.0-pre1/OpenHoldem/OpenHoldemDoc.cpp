// OpenHoldemDoc.cpp : implementation of the COpenHoldemDoc class
//

#include "stdafx.h"
#include "OpenHoldem.h"
#include "MainFrm.h"

#include "OpenHoldemDoc.h"
#include "DialogFormulaScintilla.h"
#include "debug.h"
#include "global.h"

//
// Default bot specification follows
//
char * defaultCSnotes = "add your formula notes and comments here";

char * defaultCSalli = "   0                                     // default is do not alli\r\n\
   || [ prwin >= 1.00 ]                  // dead nuts\r\n\
   || [ br==1 && islistalli ]            // listed opening hand\r\n\
";

char * defaultCSswag = "0 // default is no stated wagers";

char * defaultCSsrai = "0 // default is no stated raises";

char * defaultCSrais = "   0                                     // default is do not rais\r\n\
   || [ prwin >= 1.00 ]                  // dead nuts\r\n\
\r\n\
   || [ f$evrais > f$evcall && f$evrais>pot/2 ]  // majority share\r\n\
   || [ br==1 && islistrais ]            // listed opening hand\r\n\
\r\n\
   || [ f0                               // f0 button flag is on\r\n\
        && br==1                         // preflop\r\n\
        && isemptylistrais               // rais list is empty\r\n\
        && handrank2652<=2652            // my hand has\r\n\
           * (1.0 - e**(ln(0.50)/        // 50% chance hand better than all\r\n\
//           nopponentsdealt             // tight (static)\r\n\
             nopponents                  // medium (p formula)\r\n\
//           nopponentsplaying           // loose (dynamic)\r\n\
             ))\r\n\
           * betposition/nplayersplaying // dynamic betting position percentage\r\n\
//         * dealposition/nplayersdealt  // static betting position percentage\r\n\
      ]\r\n\
";

char * defaultCScall = "  0                                     // default is do not call\r\n\
   || [ prwin >= 1.00 ]                  // dead nuts\r\n\
   || [ call  <= 0.00 ]                  // zero call amount (check)\r\n\
   || [ f$evcall > pot / nplayersdealt ] // equal share\r\n\
   || [ br==1 && islistcall ]            // listed opening hand\r\n\
\r\n\
   || [!f0                               // f0 flag button is off\r\n\
        && br==1                         // preflop\r\n\
        && isemptylistcall               // call list is empty\r\n\
        && prwin >= 1/                   // fair chances against\r\n\
             nopponentsdealt             // loose (static)\r\n\
//           nopponents                  // medium (p formula)\r\n\
//           nopponentsplaying           // tight (dynamic)\r\n\
      ]\r\n\
\r\n\
   || [ f0                               // f0 flag button is on\r\n\
        && br==1                         // preflop\r\n\
        && isemptylistcall               // call list is empty\r\n\
        && handrank2652<=2652            // fair chances against\r\n\
           * (1-e**(ln(0.50)/            // 50% chance hand better than all\r\n\
//           nopponentsdealt             // static and tight\r\n\
             nopponents                  // p formula and defense level\r\n\
//           nopponentsplaying           // dynamic and loose\r\n\
             ))\r\n\
      ]\r\n\
";

char * defaultCSprefold = "0  // default is do not pre-fold\r\n";

char * defaultCSevrais = "0                                        // start with zero\r\n\
\r\n\
// DEBITS HERE\r\n\
\r\n\
- call                                   // calling cost\r\n\
- bet                                    // raising cost\r\n\
\r\n\
// CREDITS HERE\r\n\
\r\n\
+  (\r\n\
\r\n\
      pot                                // current pot\r\n\
\r\n\
   + (call+bet*2)                        // my call my bet and a call (minimum)\r\n\
\r\n\
   + (raisshort-call-bet*2)              // additional callers\r\n\
      * 0.00                             //   0% (tight)\r\n\
//    * 0.50                             //  50% (guess)\r\n\
//    * 1.00                             // 100% (loose)\r\n\
\r\n\
   )\r\n\
\r\n\
   * (prwin+prtie/2)                     // adjust for winning chances\r\n\
   * (1+rake)                            // adjust for rake\r\n\
";

char * defaultCSevcall = "0                                        // start with zero\r\n\
\r\n\
// DEBITS HERE\r\n\
\r\n\
- call                                   // calling cost\r\n\
\r\n\
\r\n\
// CREDITS HERE\r\n\
\r\n\
+  (\r\n\
\r\n\
      pot                                // current pot\r\n\
\r\n\
   +  call                               // my call (minimum)\r\n\
\r\n\
   + (callshort-call)                    // additional callers\r\n\
      * 0.00                             //   0% (tight)\r\n\
//    * 0.50                             //  50% (guess)\r\n\
//    * 1.00                             // 100% (loose)\r\n\
\r\n\
   )\r\n\
\r\n\
   * (prwin+prtie/2)                     // adjust for winning chances\r\n\
   * (1+rake)                            // adjust for rake\r\n\
";

char * defaultCSP = "\r\n\
(nopponentsplaying==0) ? 0 :\r\n\
\r\n\
nopponentsplaying + defcon*nopponentsfolded\r\n\
";

char * defaultCSplay = "//------------------------------------------------------------------------\r\n\
//    DISABLED RULE(S)\r\n\
!0                       ? -1 :  // 0=disabled 1=enabled \r\n\
!f9                      ? -1 :  // enabled when f9 is pressed \r\n\
(elapsed%4)              ? -1 :  // disabled 3 out of 4 seconds \r\n\
\r\n\
\r\n\
//------------------------------------------------------------------------\r\n\
//    LEAVE RULE\r\n\
0                                // 0=off 1=on \r\n\
&& issittingout                  // i am sitting out \r\n\
&& elapsedhand >= 600            // 10 minutes since end of last hand \r\n\
&& elapsedauto >= 600            // 10 minutes since autoplayer has acted \r\n\
\r\n\
                         ? -2 :  // leave the table \r\n\
\r\n\
//------------------------------------------------------------------------\r\n\
//    SITIN RULE\r\n\
0                                // 0=off 1=on \r\n\
&& issittingout                  // i am sitting out \r\n\
&& (br==2 || br==3)              // it is the flop or turn \r\n\
&& nopponentsdealt>=6            // 6 or more opponents \r\n\
&& nchairsdealtright==2          // i am about to be big blind \r\n\
\r\n\
                         ?  1 :  // sitin \r\n\
\r\n\
//------------------------------------------------------------------------\r\n\
//    SITOUT RULE\r\n\
0                                // 0=off 1=on \r\n\
&& issittingin                   // i am sitting in \r\n\
&& (br==2 || br==3)              // it is the flop or turn \r\n\
&& nopponentsdealt<=5            // 5 or less opponents \r\n\
\r\n\
                         ?  0 :  // sitout \r\n\
\r\n\
//------------------------------------------------------------------------\r\n\
-1 // disabled\r\n\
";

char * defaultCStest = "";

char * defaultCSdebug = "                 = elapsed\r\n\
\r\n\
                 = sblind\r\n\
                 = bblind\r\n\
                 = bet\r\n\
\r\n\
                 = call\r\n\
                 = currentbet\r\n\
                 = balance\r\n\
\r\n\
                 = f$evcall\r\n\
                 = f$evrais\r\n\
\r\n\
                 = potplayer\r\n\
                 = potcommon\r\n\
                 = pot\r\n\
                 = rake\r\n\
\r\n\
                 = chair\r\n\
                 = handrank2652\r\n\
                 = pokerval\r\n\
                 = prwin\r\n\
                 = prlos\r\n\
                 = prtie\r\n\
                 = nopponents\r\n\
";

double defaultdBankroll = 0.0;

double defaultdDefcon = 0.0;

double defaultdRake = 0.0;

double defaultdNit = 1000.0;

// COpenHoldemDoc

IMPLEMENT_DYNCREATE(COpenHoldemDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenHoldemDoc, CDocument)
END_MESSAGE_MAP()


// COpenHoldemDoc construction/destruction

COpenHoldemDoc::COpenHoldemDoc() {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif
		global.ClearFormula(&global.formula);
		global.formula_name = "";
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemDoc::Constructor : \n"); 
		throw;
	}
#endif
}

COpenHoldemDoc::~COpenHoldemDoc() {
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemDoc::Destructor : \n"); 
		throw;
	}
#endif
}

BOOL COpenHoldemDoc::OnNewDocument() {
#ifdef SEH_ENABLE
	try {
#endif
		SFunction		func;
		CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

		if (!CDocument::OnNewDocument())
			return FALSE;

		// Kill the formula dialog, if it is open
		if(m_formulaScintillaDlg) {
			delete m_formulaScintillaDlg;
			m_formulaScintillaDlg	=	NULL;
			pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);
		}

		// Clear everything
		global.ClearFormula(&global.formula);

		// Default bot
		func.dirty = true;

		func.func = "notes"; func.func_text = defaultCSnotes; global.formula.mFunction.Add(func);
		func.func = "dll"; func.func_text = ""; global.formula.mFunction.Add(func);
		func.func = "f$alli"; func.func_text = defaultCSalli; global.formula.mFunction.Add(func);
		func.func = "f$swag"; func.func_text = defaultCSswag; global.formula.mFunction.Add(func);
		func.func = "f$srai"; func.func_text = defaultCSsrai; global.formula.mFunction.Add(func);
		func.func = "f$rais"; func.func_text = defaultCSrais; global.formula.mFunction.Add(func);
		func.func = "f$call"; func.func_text = defaultCScall; global.formula.mFunction.Add(func);
		func.func = "f$prefold"; func.func_text = defaultCSprefold; global.formula.mFunction.Add(func);
		func.func = "f$evrais"; func.func_text = defaultCSevrais; global.formula.mFunction.Add(func);
		func.func = "f$evcall"; func.func_text = defaultCSevcall; global.formula.mFunction.Add(func);
		func.func = "f$P"; func.func_text = defaultCSP; global.formula.mFunction.Add(func);
		func.func = "f$play"; func.func_text = defaultCSplay; global.formula.mFunction.Add(func);
		func.func = "f$test"; func.func_text = defaultCStest; global.formula.mFunction.Add(func);
		func.func = "f$debug"; func.func_text = defaultCSdebug; global.formula.mFunction.Add(func);

		global.formula.dBankroll = defaultdBankroll;
		global.formula.dDefcon = defaultdDefcon;
		global.formula.dRake = defaultdRake;
		global.formula.dNit = defaultdNit;

		// Unload dll, if one is loaded
		if (cdll.hMod_dll) cdll.unload_dll();

		// Create hand list matrices
		global.create_hand_list_matrices(&global.formula);

		// Create parse trees for default formula
		global.ParseAllFormula(pMyMainWnd->GetSafeHwnd(), &global.formula);

		global.formula_name = "Default";
		SetTitle("Default");
		SetModifiedFlag(true);

		// Load dll, if set in preferences
		if (global.preferences.load_dll_on_startup) {
			cdll.load_dll("");
		}

		return true;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemDoc::OnNewDocument : \n"); 
		throw;
	}
#endif
}

// COpenHoldemDoc serialization
void COpenHoldemDoc::Serialize(CArchive& ar) {
#ifdef SEH_ENABLE
	try {
#endif
		CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

		// Writing a file
		if (ar.IsStoring()) {
			WriteFormula(&global.formula, ar);
		}

		// Reading a file
		else {
			// Kill the formula dialog, if it is open
			if(m_formulaScintillaDlg) {
				delete m_formulaScintillaDlg;
				m_formulaScintillaDlg	=	NULL;
				pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);
			}

			ReadFormula(&global.formula, ar);
			global.formula_name = ar.GetFile()->GetFileName();

			// Unload dll, if one is loaded
			if (cdll.hMod_dll) cdll.unload_dll();

			// Create hand list matrices
			global.create_hand_list_matrices(&global.formula);

			// Create parse trees for newly loaded formula
			global.ParseAllFormula(pMyMainWnd->GetSafeHwnd(), &global.formula);

			// Load dll, if set in preferences
			if (global.preferences.load_dll_on_startup) {
				cdll.load_dll("");
			}

		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemDoc::Serialize :\n"); 
		throw;
	}
#endif
}

COpenHoldemDoc * COpenHoldemDoc::GetDocument() {
#ifdef SEH_ENABLE
	try {
#endif
		CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
		return (COpenHoldemDoc *) pFrame->GetActiveDocument();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemDoc::GetDocument : \n"); 
		throw;
	}
#endif
}


void COpenHoldemDoc::WriteFormula(SFormula *f, CArchive& ar) {
#ifdef SEH_ENABLE
	try {
#endif
		CString s;
		int N, i;
		char nowtime[26];

		CFile *cf_whf = ar.GetFile();
		CFile cf_whx;
		char whxpath[MAX_PATH];
		CString CSpath = cf_whf->GetFilePath();

		strcpy(whxpath, CSpath.GetString());
		whxpath[strlen(whxpath)-1]='x';

		// Write the .whf file
		s.Format("##%s##\r\n\r\n", get_time(nowtime)); ar.WriteString(s);
		N = (int) global.formula.mFunction.GetSize();
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "notes") 
			{ 
				ar.WriteString("##notes##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); 
			}
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "dll") { ar.WriteString("##dll##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		s.Format("##bankroll##\r\n%f\r\n\r\n", f->dBankroll); ar.WriteString(s);
		s.Format("##defcon##\r\n%f\r\n\r\n", f->dDefcon); ar.WriteString(s);
		s.Format("##rake##\r\n%f\r\n\r\n", f->dRake); ar.WriteString(s);
		s.Format("##nit##\r\n%d\r\n\r\n", (int) f->dNit); ar.WriteString(s);
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$alli") { ar.WriteString("##f$alli##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$swag") { ar.WriteString("##f$swag##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$srai") { ar.WriteString("##f$srai##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$rais") { ar.WriteString("##f$rais##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$call") { ar.WriteString("##f$call##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$prefold") { ar.WriteString("##f$prefold##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$evrais") { ar.WriteString("##f$evrais##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$evcall") { ar.WriteString("##f$evcall##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$P") { ar.WriteString("##f$P##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$play") { ar.WriteString("##f$play##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$test") { ar.WriteString("##f$test##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func == "f$debug") { ar.WriteString("##f$debug##\r\n" + f->mFunction[i].func_text + "\r\n\r\n"); }
		}

		// handlists
		N = (int) f->mHandList.GetSize();
		for (i=0; i<N; i++) {
			ar.WriteString("##" + f->mHandList[i].list + "##\r\n" + f->mHandList[i].list_text + "\r\n\r\n");
		}

		cf_whx.Open(whxpath, CFile::modeCreate | CFile::modeWrite);
		CArchive ar_whx(&cf_whx, CArchive::store);

		// Write the .whx file
		N = (int) global.formula.mFunction.GetSize();
		for (i=0; i<N; i++) {
			if (f->mFunction[i].func != "notes" &&
				f->mFunction[i].func != "dll" &&
				f->mFunction[i].func != "f$alli" &&
				f->mFunction[i].func != "f$swag" &&
				f->mFunction[i].func != "f$srai" &&
				f->mFunction[i].func != "f$rais" &&
				f->mFunction[i].func != "f$call" &&
				f->mFunction[i].func != "f$prefold" &&
				f->mFunction[i].func != "f$evrais" &&
				f->mFunction[i].func != "f$evcall" &&
				f->mFunction[i].func != "f$P" &&
				f->mFunction[i].func != "f$play" &&
				f->mFunction[i].func != "f$test" &&
				f->mFunction[i].func != "f$debug" ) {
				ar_whx.WriteString("##" + f->mFunction[i].func + "##\r\n" + f->mFunction[i].func_text + "\r\n\r\n");
			}
		}
		ar_whx.Close();
		cf_whx.Close();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemDoc::WriteFormula :\n"); 
		throw;
	}
#endif
}

void COpenHoldemDoc::ReadFormula(SFormula *f, CArchive& ar) {
#ifdef SEH_ENABLE
	try {
#endif
		CString strOneLine; 
		int content;
		char funcname[256];
		int start, end;
		int N, i;
		bool addit;
		SFunction func;	
		SHandList list;

		CFile *cf_whf = ar.GetFile();
		CFile cf_whx;
		char whxpath[MAX_PATH];
		CString CSpath = cf_whf->GetFilePath();

		strcpy(whxpath, CSpath.GetString());
		whxpath[strlen(whxpath)-1]='x';

		// Clear everything
		global.ClearFormula(&global.formula);
		SetModifiedFlag(false);

		// Ignore first line (date/time)
		ar.ReadString(strOneLine);

		// read data in, one line at a time
		strOneLine = "";
		content = FTnone;
		func.func = "";
		while(ar.ReadString(strOneLine)) { 

			// If this line marks the beginning of a function, then save the previously
			// collected function, and start a new one
			if (strOneLine.Mid(0,2)=="##") {

				// Save the previously collected function
				if (content == FTlist) {
					// Strip the LFCR off the last line (we need to add CRLF for all but the last line)
					list.list_text.TrimRight("\r\n");
					f->mHandList.Add(list); 
				}
				else if (content == FTfunc) {
					func.func_text.TrimRight("\r\n");
					f->mFunction.Add(func);
				}

				// Get the function name
				start = strOneLine.Find("##",0);
				end = strOneLine.Find("##", start+2);
				strcpy(funcname, strOneLine.GetString()+start+2);
				funcname[end-2]='\0';

				if (strcmp(funcname, "bankroll") == 0) { f->dBankroll = 0.0; content = FTbankroll; }
				else if (strcmp(funcname, "defcon") == 0) { f->dDefcon = 0.0; content = FTdefcon; }
				else if (strcmp(funcname, "rake") == 0) { f->dRake = 0.0; content = FTrake; }
				else if (strcmp(funcname, "nit") == 0) { f->dNit = 0.0; content = FTnit; }
				else if (memcmp(funcname, "list", 4) == 0) { 
					content = FTlist;
					list.list = funcname;
					list.list_text = "";
				}
				else {
					content = FTfunc;
					func.func = funcname;
					func.func_text = "";
					func.dirty = true;
				}
			}

			// Get the function content
			else {
				switch (content) {
					 case FTbankroll:
						 if (strOneLine.GetLength()) { f->dBankroll = atof(strOneLine.GetString()); }
						 break;
					 case FTdefcon:
						 if (strOneLine.GetLength()) { f->dDefcon = atof(strOneLine.GetString()); }
						 break;
					 case FTrake:
						 if (strOneLine.GetLength()) { f->dRake = atof(strOneLine.GetString()); }
						 break;
					 case FTnit:
						 if (strOneLine.GetLength()) { f->dNit = atof(strOneLine.GetString()); }
						 break;
					 case FTlist:
						 list.list_text.Append(strOneLine); list.list_text.Append("\r\n");
						 break;
					 case FTfunc:
						 func.func_text.Append(strOneLine); func.func_text.Append("\r\n");
						 break;
				}
			}	
		}  //while(ar.ReadString(strOneLine))

		// Add the last list/function to the CArray on EOF, if it was a list being processed
		if (content == FTlist) {
			list.list_text.TrimRight("\r\n");
			f->mHandList.Add(list); 
		}
		else if (content == FTfunc) {
			func.func_text.TrimRight("\r\n");
			f->mFunction.Add(func);
		}

		//
		// Now load the associated .whx, if it exists
		//
		content = FTnone;
		if (cf_whx.Open(whxpath, CFile::modeNoTruncate | CFile::modeRead)) {
			CArchive ar_whx(&cf_whx, CArchive::load);

			// read data in, one line at a time 
			strOneLine = "";
			while(ar_whx.ReadString(strOneLine)) { 

				// If this line marks the beginning of a function, then save the previously
				// collected function, and start a new one
				if (strOneLine.Mid(0,2)=="##") {

					// Save the previously collected function
					if (content == FTfunc) {
						// Strip the LFCR off the last line (we need to add CRLF for all but the last line)
						func.func_text.TrimRight("\r\n"); // Note: This will clear existing files that have had extra CRLF appended but will also strip intentional space added by users
						f->mFunction.Add(func); 
						content = FTnone;
					}

					// Get the function name
					start = strOneLine.Find("##",0);
					end = strOneLine.Find("##", start+2);
					strcpy(funcname, strOneLine.GetString()+start+2);
					funcname[end-2]='\0';

					content = FTfunc;
					func.func = funcname;
					func.func_text = "";
					func.dirty = true;
				}

				// Get the function content
				else if (content == FTfunc) {
					func.func_text.Append(strOneLine.GetString());  func.func_text.Append("\r\n");
				}	
			}  //while(ar.ReadString(strOneLine))

			// Add the last list to the CArray on EOF, if it was a list being processed
			if (content == FTfunc) {
				// Strip the LFCR off the last line (we need to add CRLF for all but the last line)
				func.func_text.TrimRight("\r\n");
				f->mFunction.Add(func);
			}

			ar_whx.Close();
			cf_whx.Close();
		}

		//////////////////////
		// See if we have all of the default entries, if not, then add them
		N = (int) f->mFunction.GetSize();

		// notes
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="notes") addit = false;
		if (addit==true)  { func.func = "notes"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// dll
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="dll") addit = false;
		if (addit==true)  { func.func = "dll"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$alli
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$alli") addit = false;
		if (addit==true)  { func.func = "f$alli"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$swag
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$swag") addit = false;
		if (addit==true)  { func.func = "f$swag"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$srai
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$srai") addit = false;
		if (addit==true)  { func.func = "f$srai"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$rais
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$rais") addit = false;
		if (addit==true)  { func.func = "f$rais"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$call
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$call") addit = false;
		if (addit==true)  { func.func = "f$call"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$prefold
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$prefold") addit = false;
		if (addit==true)  { func.func = "f$prefold"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$evrais
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$evrais") addit = false;
		if (addit==true)  { func.func = "f$evrais"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$evcall
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$evcall") addit = false;
		if (addit==true)  { func.func = "f$evcall"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$P
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$P") addit = false;
		if (addit==true)  { func.func = "f$P"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$play
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$play") addit = false;
		if (addit==true)  { func.func = "f$play"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$test
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$test") addit = false;
		if (addit==true)  { func.func = "f$test"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

		// f$debug
		addit = true;
		for (i=0; i<N; i++)  if (f->mFunction[i].func=="f$debug") addit = false;
		if (addit==true)  { func.func = "f$debug"; func.func_text = ""; func.dirty = true; f->mFunction.Add(func); }

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemDoc::ReadFormula :\n"); 
		throw;
	}
#endif
}

