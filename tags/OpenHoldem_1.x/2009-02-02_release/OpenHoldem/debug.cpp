#include "stdafx.h"

#include "..\..\dbghelp\dbghelp.h"

#include "OpenHoldem.h"

#include "CSymbols.h"
#include "CScraper.h"
#include "CIteratorThread.h"
#include "CPreferences.h"
#include "..\CTablemap\CTablemap.h"

#include "inlines/eval.h"

//#include <vld.h>			// visual leak detector



FILE *log_fp = NULL;

char * get_time(char * timebuf) 
{
    // returns current system time in WH format
    time_t	ltime;
    char tmptime[30];

    time( &ltime );
    ctime_s(tmptime, 26, &ltime);
    tmptime[24]='\0';

    memcpy(timebuf, tmptime+20, 4); //yyyy
    *(timebuf+4) = '-';

    // mm
    if (memcmp(tmptime+4, "Jan", 3)==0)  {
        *(timebuf+5) = '0';
        *(timebuf+6) = '1';
    }
    else if (memcmp(tmptime+4, "Feb", 3)==0)  {
        *(timebuf+5) = '0';
        *(timebuf+6) = '2';
    }
    else if (memcmp(tmptime+4, "Mar", 3)==0)  {
        *(timebuf+5) = '0';
        *(timebuf+6) = '3';
    }
    else if (memcmp(tmptime+4, "Apr", 3)==0)  {
        *(timebuf+5) = '0';
        *(timebuf+6) = '4';
    }
    else if (memcmp(tmptime+4, "May", 3)==0)  {
        *(timebuf+5) = '0';
        *(timebuf+6) = '5';
    }
    else if (memcmp(tmptime+4, "Jun", 3)==0)  {
        *(timebuf+5) = '0';
        *(timebuf+6) = '6';
    }
    else if (memcmp(tmptime+4, "Jul", 3)==0)  {
        *(timebuf+5) = '0';
        *(timebuf+6) = '7';
    }
    else if (memcmp(tmptime+4, "Aug", 3)==0)  {
        *(timebuf+5) = '0';
        *(timebuf+6) = '8';
    }
    else if (memcmp(tmptime+4, "Sep", 3)==0)  {
        *(timebuf+5) = '0';
        *(timebuf+6) = '9';
    }
    else if (memcmp(tmptime+4, "Oct", 3)==0)  {
        *(timebuf+5) = '1';
        *(timebuf+6) = '0';
    }
    else if (memcmp(tmptime+4, "Nov", 3)==0)  {
        *(timebuf+5) = '1';
        *(timebuf+6) = '1';
    }
    else if (memcmp(tmptime+4, "Dec", 3)==0)  {
        *(timebuf+5) = '1';
        *(timebuf+6) = '2';
    }

    *(timebuf+7) = '-';
    memcpy(timebuf+8, tmptime+8, 2); //dd
    *(timebuf+10) = ' ';
    memcpy(timebuf+11, tmptime+11, 8); //HH:mm:ss
    *(timebuf+19) = '\0';

    return timebuf;
}

char * get_now_time(char * timebuf) 
{
    // returns current system time as a UNIX style string
    time_t	ltime;

    time( &ltime );
    ctime_s(timebuf, 26, &ltime);
    timebuf[24]='\0';

    return timebuf;
}

void logfatal (char* fmt, ...) 
{
    char		buff[10000] ;
    va_list		ap;
    char		fatallogpath[MAX_PATH];
    FILE		*fatallog;
    char		nowtime[26];

    sprintf_s(fatallogpath, MAX_PATH, "%s\\fatal error.log", _startup_path);
    if (fopen_s(&fatallog, fatallogpath, "a")==0)
	{
		va_start(ap, fmt);
		vsprintf_s(buff, 10000, fmt, ap);
		get_now_time(nowtime);
		fprintf(fatallog, "%s> %s", nowtime, buff);

		va_end(ap);
		fclose(fatallog);
	}
}

LONG WINAPI MyUnHandledExceptionFilter(EXCEPTION_POINTERS *pExceptionPointers) 
{
	char flpath[MAX_PATH];
    char msg[1000];

    /*
    logfatal("########################################################################\n");
    logfatal("FATAL ERROR  (See above for call stack)\n");
    logfatal("########################################################################\n");
    logfatal("Address: %x\n", pExceptionPointers->ExceptionRecord->ExceptionAddress);
    logfatal("Code: %x\n", pExceptionPointers->ExceptionRecord->ExceptionCode);
    logfatal("Flags: %x\n", pExceptionPointers->ExceptionRecord->ExceptionFlags);
    logfatal("Information: %x\n", pExceptionPointers->ExceptionRecord->ExceptionInformation);
    logfatal("Record: %x\n", pExceptionPointers->ExceptionRecord->ExceptionRecord);
    logfatal("Num Params: %x\n", pExceptionPointers->ExceptionRecord->NumberParameters);
    switch (pExceptionPointers->ExceptionRecord->ExceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
        logfatal("Desc: EXCEPTION_ACCESS_VIOLATION The thread tried to read from or write to a virtual address\n");
        logfatal("      for which it does not have the appropriate access.\n");
        break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        logfatal("Desc: EXCEPTION_ARRAY_BOUNDS_EXCEEDED The thread tried to access an array element that is out\n");
        logfatal("      of bounds and the underlying hardware supports bounds checking.\n");
        break;
    case EXCEPTION_BREAKPOINT:
        logfatal("Desc: EXCEPTION_BREAKPOINT A breakpoint was encountered.\n");
        break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        logfatal("Desc: EXCEPTION_DATATYPE_MISALIGNMENT The thread tried to read or write data that is misaligned on\n");
        logfatal("      hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte\n");
        logfatal("      boundaries; 32-bit values on 4-byte boundaries, and so on.\n");
        break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        logfatal("Desc: EXCEPTION_FLT_DENORMAL_OPERAND One of the operands in a floating-point operation is denormal. A\n");
        logfatal("      denormal value is one that is too small to represent as a standard floating-point value.\n");
        break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        logfatal("Desc: EXCEPTION_FLT_DIVIDE_BY_ZERO The thread tried to divide a floating-point value by a floating-point\n");
        logfatal("      divisor of zero.\n");
        break;
    case EXCEPTION_FLT_INEXACT_RESULT:
        logfatal("Desc: EXCEPTION_FLT_INEXACT_RESULT The result of a floating-point operation cannot be represented exactly\n");
        logfatal("      as a decimal fraction.\n");
        break;
    case EXCEPTION_FLT_INVALID_OPERATION:
        logfatal("Desc: EXCEPTION_FLT_INVALID_OPERATION This exception represents any floating-point exception not included\n");
        logfatal("      in this list.\n");
        break;
    case EXCEPTION_FLT_OVERFLOW:
        logfatal("Desc: EXCEPTION_FLT_OVERFLOW The exponent of a floating-point operation is greater than the magnitude\n");
        logfatal("      allowed by the corresponding type.\n");
        break;
    case EXCEPTION_FLT_STACK_CHECK:
        logfatal("Desc: EXCEPTION_FLT_STACK_CHECK The stack overflowed or underflowed as the result of a floating-point\n");
        logfatal("      operation.\n");
        break;
    case EXCEPTION_FLT_UNDERFLOW:
        logfatal("Desc: EXCEPTION_FLT_UNDERFLOW The exponent of a floating-point operation is less than the magnitude\n");
        logfatal("      allowed by the corresponding type.\n");
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        logfatal("Desc: EXCEPTION_ILLEGAL_INSTRUCTION The thread tried to execute an invalid instruction.\n");
        break;
    case EXCEPTION_IN_PAGE_ERROR:
        logfatal("Desc: EXCEPTION_IN_PAGE_ERROR The thread tried to access a page that was not present, and the system\n");
        logfatal("      was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.\n");
        break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        logfatal("Desc: EXCEPTION_INT_DIVIDE_BY_ZERO The thread tried to divide an integer value by an integer divisor\n");
        logfatal("      of zero.\n");
        break;
    case EXCEPTION_INT_OVERFLOW:
        logfatal("Desc: EXCEPTION_INT_OVERFLOW The result of an integer operation caused a carry out of the most significant\n");
        logfatal("      bit of the result.\n");
        break;
    case EXCEPTION_INVALID_DISPOSITION:
        logfatal("Desc: EXCEPTION_INVALID_DISPOSITION An exception handler returned an invalid disposition to the exception\n");
        logfatal("      dispatcher. Programmers using a high-level language such as C should never encounter this exception.\n");
        break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        logfatal("Desc: EXCEPTION_NONCONTINUABLE_EXCEPTION The thread tried to continue execution after a noncontinuable\n");
        logfatal("      exception occurred.\n");
        break;
    case EXCEPTION_PRIV_INSTRUCTION:
        logfatal("Desc: EXCEPTION_PRIV_INSTRUCTION The thread tried to execute an instruction whose operation is not allowed\n");
        logfatal("      in the current machine mode.\n");
        break;
    case EXCEPTION_SINGLE_STEP:
        logfatal("Desc: EXCEPTION_SINGLE_STEP A trace trap or other single-instruction mechanism signaled that one\n");
        logfatal("      instruction has been executed.\n");
        break;
    case EXCEPTION_STACK_OVERFLOW:
        logfatal("Desc: EXCEPTION_STACK_OVERFLOW The thread used up its stack.\n");
        break;
    default:
        logfatal("Desc: Unknown.\n");
        break;
    }
    logfatal("########################################################################\n");
    logfatal("########################################################################\n\n\n");
	*/

	// Create a minidump
	GenerateDump(pExceptionPointers);

    sprintf_s(flpath, MAX_PATH, "%s\\fatal error.log", _startup_path);
    strcpy_s(msg, 1000, "OpenHoldem is about to crash.\n");
    strcat_s(msg, 1000, "A minidump has been created in your\n");
	strcat_s(msg, 1000, "OpenHoldem startup directory.\n");
    strcat_s(msg, 1000, "\n\nOpenHoldem will shut down when you click OK.");
    MessageBox(NULL, msg, "FATAL ERROR", MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);

    return EXCEPTION_EXECUTE_HANDLER;
}

BOOL CreateBMPFile(const char *szFile, HBITMAP hBMP) 
{
    // Saves the hBitmap as a bitmap.
    HDC					hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
    HDC					hdcCompatible = CreateCompatibleDC(hdcScreen);
    PBITMAPINFO			pbmi=NULL;
    BOOL				bret=FALSE;
    HANDLE				hf=NULL; // file handle
    BITMAPFILEHEADER	hdr; // bitmap file-header
    PBITMAPINFOHEADER	pbih=NULL; // bitmap info-header
    LPBYTE				lpBits=NULL; // memory pointer
    DWORD				dwTotal=0; // total count of bytes
    DWORD				cb=0; // incremental count of bytes
    BYTE				*hp=NULL; // byte pointer
    DWORD				dwTmp=0;
    BITMAP				bmp;

    memset(&bmp,0,sizeof(BITMAP));
    GetObject(hBMP,sizeof(BITMAP),&bmp);
    memset(&hdr,0,sizeof(hdr));
    {
        int cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
        if (cClrBits>8) {
            // No Palette (normally)
            pbmi = (PBITMAPINFO) calloc(1, sizeof(BITMAPINFOHEADER));
        }
        else {
            pbmi = (PBITMAPINFO) calloc(1, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<<(min(8,cClrBits))));
            pbmi->bmiHeader.biClrUsed = (1<<cClrBits);
        }

        // Initialize the fields in the BITMAPINFO structure.
        pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

        // Retrieve the color table (RGBQUAD array) and the bits
        // (array of palette indices) from the DIB.
        if (!GetDIBits(hdcCompatible, hBMP, 0, bmp.bmHeight, NULL, pbmi, DIB_RGB_COLORS)) {
            goto to_return;
        }
    }
    pbih = &(pbmi->bmiHeader);
    pbmi->bmiHeader.biCompression=BI_RGB;
    lpBits = (LPBYTE) calloc(1, pbih->biSizeImage);

    if (!lpBits) {
        goto to_return;
    }

    if (!GetDIBits(hdcCompatible, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) {
        goto to_return;
    }

    // Create the .BMP file.
    hf = CreateFile(szFile, GENERIC_READ | GENERIC_WRITE, (DWORD) 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL);
    if (hf == INVALID_HANDLE_VALUE) {
        goto to_return;
    }
    hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M"
    // Compute the size of the entire file.
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed*sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    // Compute the offset to the array of color indices.
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed*sizeof (RGBQUAD);

    // Copy the BITMAPFILEHEADER into the .BMP file.
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),	(LPDWORD) &dwTmp, NULL)) {
        goto to_return;
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, ( NULL))) {
        goto to_return;
    }

    // Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) {
        goto to_return;
    }

    // Close the .BMP file.
    if (!CloseHandle(hf)) {
        goto to_return;
    }
    bret=TRUE;


to_return:
    ;
    // Free memory.
    if (pbmi)free(pbmi);
    if (lpBits)free(lpBits);
    DeleteDC(hdcCompatible);
    DeleteDC(hdcScreen);

    return bret;
}

void start_log(void) 
{
    if (log_fp==NULL) 
	{
        CString fn;
        fn.Format("%s\\oh_%lu.log", _startup_path, theApp._session_id);

		if ((log_fp = _fsopen(fn.GetString(), "a", _SH_DENYWR)) != 0)
		{
			write_log("! log file open\n");
			fprintf(log_fp, "yyyy.mm.dd hh:mm:ss -  # hand commoncard rank poker  win  los  tie  P      nit bestaction - play*      call       bet       pot   balance - FCRA FCRA swag\n");
			fprintf(log_fp, "----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			fflush(log_fp);
		}
    }
}

void write_log(char* fmt, ...) 
{
    char		buff[10000] ;
    va_list		ap;
    char		nowtime[26];

    if (log_fp != NULL) 
	{

        va_start(ap, fmt);
        vsprintf_s(buff, 10000, fmt, ap);
		get_time(nowtime);
        fprintf(log_fp, "%s - %s", nowtime, buff);

        va_end(ap);

        fflush(log_fp);
    }
}

void write_log_nostamp(char* fmt, ...) 
{
	char		buff[10000] ;
    va_list		ap;

    if (log_fp != NULL) 
	{

        va_start(ap, fmt);
        vsprintf_s(buff, 10000, fmt, ap);
        fprintf(log_fp, "%s", buff);

        va_end(ap);

        fflush(log_fp);
    }
}

void write_logautoplay(const char * action) 
{
    char		nowtime[26];
    CString		pcards, comcards, temp, rank, pokerhand, bestaction, fcra_seen;
    char		*card;
    CardMask	Cards;
    int			i, nCards;
    HandVal		hv;
    CString		fcra_formula_status;

	int			sym_userchair = (int) p_symbols->sym()->userchair;
	int			sym_br = (int) p_symbols->sym()->br;


	if (log_fp != NULL) 
	{
		// log$ writing
		if (prefs.log_symbol_enabled())
		{
			int max_log = p_symbols->logsymbols_collection()->GetCount();

			if (max_log > 0)
			{
				if (max_log > prefs.log_symbol_max_log())
				{
					max_log = prefs.log_symbol_max_log();
				}

				write_log("*** log$ (Total: %d | Showing: %d)\n", p_symbols->logsymbols_collection()->GetCount(), max_log);

				for (int i=0; i<max_log; i++)
				{
					write_log("***     %s\n", p_symbols->logsymbols_collection()->GetAt(i));
				}
			}
		}
		
		CardMask_RESET(Cards);
		nCards=0;

		// player cards
		if (p_symbols->user_chair_confirmed()) 
		{
			for (i=0; i<=1; i++) 
			{
				card = StdDeck_cardString(p_scraper->card_player(sym_userchair, i));
				temp.Format("%s", card);
				pcards.Append(temp);
				CardMask_SET(Cards, p_scraper->card_player(sym_userchair, i));
				nCards++;
			}
		}
		else 
		{
			pcards = "....";
		}

		// common cards
		comcards = "";
		if (sym_br >= 2) 
		{
			for (i=0; i<=2; i++) 
			{
				if (p_scraper->card_common(i) != CARD_BACK && 
					p_scraper->card_common(i) != CARD_NOCARD) 
				{
					card = StdDeck_cardString(p_scraper->card_common(i));
					temp.Format("%s", card);
					comcards.Append(temp);
					CardMask_SET(Cards, p_scraper->card_common(i));
					nCards++;
				}
			}
		}

		if (sym_br >= 3) 
		{
			card = StdDeck_cardString(p_scraper->card_common(3));
			temp.Format("%s", card);
			comcards.Append(temp);
			CardMask_SET(Cards, p_scraper->card_common(3));
			nCards++;
		}

		if (sym_br >= 4) 
		{
			card = StdDeck_cardString(p_scraper->card_common(4));
			temp.Format("%s", card);
			comcards.Append(temp);
			CardMask_SET(Cards, p_scraper->card_common(4));
			nCards++;
		}

        comcards.Append("..........");
        comcards = comcards.Left(10);

        // handrank
        if (prefs.sym_handrank_value() == "169")
			rank.Format("%.0f", p_symbols->sym()->handrank169);

        else if (prefs.sym_handrank_value() == "1000")
            rank.Format("%.0f", p_symbols->sym()->handrank1000);

        else if (prefs.sym_handrank_value() == "1326")
            rank.Format("%.0f", p_symbols->sym()->handrank1326);

        else if (prefs.sym_handrank_value() == "2652")
            rank.Format("%.0f", p_symbols->sym()->handrank2652);

        else if (prefs.sym_handrank_value() == "p")
            rank.Format("%.2f", p_symbols->sym()->handrankp);


        // poker hand
        hv = Hand_EVAL_N(Cards, nCards);
        if (HandVal_HANDTYPE(hv)==HandType_STFLUSH && StdDeck_RANK(HandVal_TOP_CARD(hv))==Rank_ACE)
            pokerhand="royfl";

        else if (HandVal_HANDTYPE(hv)==HandType_STFLUSH)
            pokerhand="strfl";

        else if (HandVal_HANDTYPE(hv)==HandType_QUADS)
            pokerhand="4kind";

        else if (HandVal_HANDTYPE(hv)==HandType_FULLHOUSE)
            pokerhand="fullh";

        else if (HandVal_HANDTYPE(hv)==HandType_FLUSH)
            pokerhand="flush";

        else if (HandVal_HANDTYPE(hv)==HandType_STRAIGHT)
            pokerhand="strai";

        else if (HandVal_HANDTYPE(hv)==HandType_TRIPS)
            pokerhand="3kind";

        else if (HandVal_HANDTYPE(hv)==HandType_TWOPAIR)
            pokerhand="2pair";

        else if (HandVal_HANDTYPE(hv)==HandType_ONEPAIR)
            pokerhand="1pair";

        else if (HandVal_HANDTYPE(hv)==HandType_NOPAIR)
            pokerhand="hcard";


        // best action
        if (strcmp(action, "SWAG")==0) 
		{
            bestaction.Format("$%.2f", p_symbols->f$swag());
        }
        else 
		{
            if (p_symbols->f$alli())
                bestaction = "Allin";

            else if (p_symbols->f$swag())
                bestaction = "SWAG";

            else if (p_symbols->f$rais())
                bestaction = "Bet/Raise";

            else if (p_symbols->f$call())
                bestaction = "Call/Check";

            else if (p_symbols->f$prefold())
                bestaction = "Pre-fold";

            else
                bestaction = "Fold/Check";

        }

        // fcra_seen
		int sym_myturnbits = (int) p_symbols->sym()->myturnbits;
        fcra_seen.Format("%s%s%s%s",
                         sym_myturnbits&0x1 ? "F" : ".",
                         sym_myturnbits&0x2 ? "C" : ".",
                         sym_myturnbits&0x4 ? "R" : ".",
                         sym_myturnbits&0x8 ? "A" : ".");

        // fcra formula status
        fcra_formula_status.Format("%s%s%s%s",
                                   !p_symbols->f$alli() && !p_symbols->f$rais() && !p_symbols->f$call() && !p_symbols->f$swag() ? "F" : ".",
                                   p_symbols->f$call() ? "C" : ".",
                                   p_symbols->f$rais() ? "R" : ".",
                                   p_symbols->f$alli() ? "A" : ".");

        fprintf(log_fp, "%s - %1d ", 
			get_time(nowtime), 
			p_tablemap->s$items()->num_chairs);
        fprintf(log_fp, "%4s %10s %4s %5s ", 
			pcards.GetString(), 
			comcards.GetString(), 
			rank.GetString(), 
			pokerhand.GetString());
        fprintf(log_fp, "%4d %4d %4d ", 
			(int) (p_symbols->sym()->prwin*1000),
			(int) (p_symbols->sym()->prlos*1000), 
			(int) (p_symbols->sym()->prtie*1000));
        fprintf(log_fp, "%2d %8d %-10s - ", 
			(int) p_symbols->sym()->nopponents,
			(int) p_symbols->sym()->nit,
			bestaction.GetString());
        fprintf(log_fp, "%-5s %9.2f %9.2f %9.2f ", 
			action, 
			p_symbols->sym()->call, 
			p_symbols->sym()->bet[4], 
			p_symbols->sym()->pot);
		fprintf(log_fp, "%9.2f - %s %s %.2f\n", 
			p_symbols->sym()->balance[10], 
			fcra_seen.GetString(), 
			fcra_formula_status.GetString(), 
			p_symbols->f$swag() );

		if (prefs.trace_enabled() && p_symbols->symboltrace_collection()->GetSize() > 0)
		{
			write_log_nostamp("***** Autoplayer Trace ****\n");
			for (int i=0; i<p_symbols->symboltrace_collection()->GetSize(); i++)
			{
				write_log_nostamp("%s\n", p_symbols->symboltrace_collection()->GetAt(i));
			}
			write_log_nostamp("***********************\n");
		}

		fflush(log_fp);
    }
}

void stop_log(void) 
{
    if (log_fp != NULL) 
	{
        write_log("! log file closed\n");
        fclose(log_fp);
        log_fp = NULL;
    }
}

int GenerateDump(EXCEPTION_POINTERS *pExceptionPointers)
{
    bool		bMiniDumpSuccessful;
    char		szFileName[MAX_PATH]; 
    DWORD		dwBufferSize = MAX_PATH;
    HANDLE		hDumpFile;
    SYSTEMTIME	stLocalTime;
    MINIDUMP_EXCEPTION_INFORMATION	ExpParam;

    GetLocalTime(&stLocalTime);

    sprintf_s(szFileName, MAX_PATH, "%s\\%s-%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", _startup_path, 
			"OpenHoldem", VERSION_TEXT, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
			stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, GetCurrentProcessId(), 
			GetCurrentThreadId());

    hDumpFile = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ, 
						   0, CREATE_ALWAYS, 0, 0);
   
    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = pExceptionPointers;
    ExpParam.ClientPointers = TRUE;

	MINIDUMP_TYPE	mdt = (MINIDUMP_TYPE) (MiniDumpWithPrivateReadWriteMemory | 
										   MiniDumpWithDataSegs | 
										   MiniDumpWithHandleData |
										   MiniDumpWithFullMemoryInfo | 
										   MiniDumpWithThreadInfo | 
										   MiniDumpWithUnloadedModules);
   
    bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
											mdt, &ExpParam, NULL, NULL);

    return EXCEPTION_EXECUTE_HANDLER;
}

