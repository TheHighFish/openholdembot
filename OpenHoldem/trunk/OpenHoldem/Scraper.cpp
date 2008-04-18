#include "StdAfx.h"

#include "Scraper.h"
#include "debug.h"
#include "../../CTransform/hash/lookup3.h"
#include "global.h"
#include "threads.h"
#include "symbols.h"
#include "../../CTransform/Transform.h"

// Global construction of scraper class
class CScraper	scraper;

CScraper::CScraper(void)
{
    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);
    __SEH_HEADER

    clear_scrape_areas();
    blinds_are_locked = false;
    __SEH_LOGFATAL("CScraper::Constructor : \n");

}

void CScraper::clear_scrape_areas(void)
{
    __SEH_HEADER
    int i;

    EnterCriticalSection(&cs_prwin);

    for (i=0; i<5; i++)
        card_common[i] = CARD_NOCARD;

    for (i=0; i<=9; i++)
    {
        card_player[i][0] = card_player[i][1] = CARD_NOCARD;
        seated[i] = active[i] = "false";
        dealer[i] = false;
        playerbet[i] = 0;
        playername[i] = "";
        name_good_scrape[i] = false;
        playerbalance[i] = 0;
        balance_good_scrape[i] = false;
        i86X_buttonstate[i] = "false";
        buttonstate[i] = "false";
        buttonlabel[i] = "";
        pot[i] = 0;
    }
    i86_buttonstate = "false";

    LeaveCriticalSection(&cs_prwin);

    buttonlabel[0] = "fold";
    buttonlabel[1] = "call";
    buttonlabel[2] = "raise";
    buttonlabel[3] = "allin";

    s_limit_info.istournament = false;

    s_limit_info.handnumber = s_limit_info.sblind = s_limit_info.bblind = s_limit_info.bbet = 0;
    s_limit_info.ante = s_limit_info.sb_bb = s_limit_info.bb_BB = 0;

    s_limit_info.found_handnumber = s_limit_info.found_sblind = s_limit_info.found_bblind = false;
    s_limit_info.found_bbet = s_limit_info.found_ante = s_limit_info.found_limit = false;
    s_limit_info.found_sb_bb = s_limit_info.found_bb_BB = false;

    s_limit_info.limit = -1;

    card_player_for_display[0] = CARD_NOCARD;
    card_player_for_display[1] = CARD_NOCARD;

    // change detection
    ucf_last=false;
    for (i=0; i<=4; i++)
        card_common_last[i]=CARD_NOCARD;

    for (i=0; i<=9; i++)
    {
        card_player_last[i][0]=CARD_NOCARD;
        card_player_last[i][1]=CARD_NOCARD;
        seated_last[i] = active_last[i] = "false";
        dealer_last[i] = false;
        name_last[i]="";
        balance_last[i]=0;
        playerbet_last[i]=0;
        buttonstate_last[i]="";
        i86X_buttonstate_last[i]="";
        buttonlabel_last[i]="";
        pot_last[i]=0;
    }

    i86_buttonstate_last = "";
    istournament_last = false;
    limit_last=0;
    sblind_last=0;
    bblind_last=0;
    sb_bb_last=0;
    bb_BB_last=0;
    bbet_last=0;
    ante_last=0;
    handnumber_last=0;
    strcpy(title_last, "");

    __SEH_LOGFATAL("CScraper::clear_scrape_areas :\n");

}

// returns true if common cards are in the middle of an animation
bool CScraper::is_common_animation(void)
{
    __SEH_HEADER
    bool			animation;
    int				flop_card_count;

    animation		= false; // By default there is no animation going on
    flop_card_count	= 0;

    // Count all the flop cards
    for (int i=0;i<3;i++)
    {
        if (card_common[i] != CARD_NOCARD)
        {
            flop_card_count++;
        }
    }

    // If there are some flop cards present,
    // but not all 3 then there is an animation going on
    if (flop_card_count > 0 && flop_card_count < 3)
    {
        animation = true;
    }
    // If the turn card is present,
    // but not all 3 flop cards are present then there is an animation going on
    else if (card_common[3] != CARD_NOCARD && flop_card_count != 3)
    {
        animation = true;
    }
    // If the river card is present,
    // but the turn card isn't
    // OR not all 3 flop cards are present then there is an animation going on
    else if (card_common[4] != CARD_NOCARD && (card_common[3] == CARD_NOCARD || flop_card_count != 3))
    {
        animation = true;
    }

    return animation;

    __SEH_LOGFATAL("CScraper::is_common_animation :\n");

}

// returns true if window has changed and we processed the changes, false otherwise
int CScraper::DoScrape(void)
{
    __SEH_HEADER
    int				i;
    CString			text;

    // DC
    HDC			hdc = GetDC(global.attached_hwnd);
    HDC			hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
    HDC			hdcCompatible = CreateCompatibleDC(hdcScreen);
    RECT		cr;
    HBITMAP		old_bitmap;

    // Get bitmap of whole window
    GetClientRect(global.attached_hwnd, &cr);
    old_bitmap = (HBITMAP) SelectObject(hdcCompatible, entire_window_Cur);
    BitBlt(hdcCompatible, 0, 0, cr.right-cr.left, cr.bottom-cr.top, hdc, cr.left, cr.top, SRCCOPY);
    SelectObject(hdcCompatible, old_bitmap);

    // get window title
    strcpy(title, "");
    if (!global.ppro_is_connected)
        GetWindowText(global.attached_hwnd, title, 512);

    // If the bitmaps are the same, then return now
    if (bitmaps_same(entire_window_Last, entire_window_Cur) &&
            ucf_last==symbols.user_chair_confirmed &&
            strcmp(title, title_last)==0)
    {
        DeleteDC(hdcCompatible);
        DeleteDC(hdcScreen);
        ReleaseDC(global.attached_hwnd, hdc);
        return NOTHING_CHANGED;
    }

    // Copy into "last" title
    if (strcmp(title, title_last)!=0)
    {
        strcpy(title_last, title);
        s_limit_info.found_handnumber = s_limit_info.found_sblind = s_limit_info.found_bblind = false;
        s_limit_info.found_bbet = s_limit_info.found_ante = s_limit_info.found_sb_bb = false;
        s_limit_info.found_bb_BB = s_limit_info.found_limit = false;

        symbols.reset_stakes = true;
    }

    // Copy into "last" bitmap
    old_bitmap = (HBITMAP) SelectObject(hdcCompatible, entire_window_Last);
    BitBlt(hdcCompatible, 0, 0, cr.right-cr.left, cr.bottom-cr.top, hdc, cr.left, cr.top, SRCCOPY);
    SelectObject(hdc, old_bitmap);

    if (ucf_last != symbols.user_chair_confirmed)
        ucf_last = symbols.user_chair_confirmed;

    // flag for detecting if anything useful has changed - used downstream to decide whether or not to
    // update symbols, etc.
    scrape_something_changed = NOTHING_CHANGED;

    // Common cards
    scrape_common_cards(hdcCompatible, hdc);

    // Player information
    for (i=0; i<=9; i++)
    {
        scrape_player_cards(i, hdcCompatible, hdc);
        scrape_seated(i, hdcCompatible, hdc);
        scrape_active(i, hdcCompatible, hdc);
        scrape_dealer(i, hdcCompatible, hdc);
        scrape_name(i, hdcCompatible, hdc);
        scrape_balance(i, hdcCompatible, hdc);
        scrape_bet(i, hdcCompatible, hdc);

        // Clear some things if no one is at this chair
        if (!is_string_seated(seated[i]) && !is_string_active(active[i]))
        {
            playername[i] = "";
            name_good_scrape[i] = false;
            playerbalance[i] = 0;
            balance_good_scrape[i] = false;
        }
    }

    scrape_buttons(hdcCompatible, hdc);		// Buttons
    scrape_pots(hdcCompatible, hdc);		// Pots
    scrape_limits(hdcCompatible, hdc);		// limits

    DeleteDC(hdcCompatible);
    DeleteDC(hdcScreen);
    ReleaseDC(global.attached_hwnd, hdc);
    return scrape_something_changed;

    __SEH_LOGFATAL("CScraper::DoScrape :\n");

}

bool CScraper::process_region(HDC hdcCompatible, HDC hdc, int r$index)
{
    __SEH_HEADER
    HBITMAP			bitmap_last = global.tablemap.r$[r$index].last_bmp;
    HBITMAP			bitmap_cur = global.tablemap.r$[r$index].cur_bmp;
    int				left = global.tablemap.r$[r$index].left;
    int				top = global.tablemap.r$[r$index].top;
    int				right = global.tablemap.r$[r$index].right;
    int				bottom = global.tablemap.r$[r$index].bottom;
    HBITMAP			old_bitmap;

    // Get "current" bitmap
    old_bitmap = (HBITMAP) SelectObject(hdcCompatible, bitmap_cur);
    BitBlt(hdcCompatible, 0, 0, right-left, bottom-top, hdc, left, top, SRCCOPY);
    SelectObject(hdcCompatible, old_bitmap);

    // If the bitmaps are different, then continue on
    if (!bitmaps_same(bitmap_last, bitmap_cur))
    {

        // Copy into "last" bitmap
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, bitmap_last);
        BitBlt(hdcCompatible, 0, 0, right-left, bottom-top, hdc, left, top, SRCCOPY);
        SelectObject(hdcCompatible, old_bitmap);

        return true;
    }
    return false;

    __SEH_LOGFATAL("CScraper::process_region:\n");

}

void CScraper::scrape_common_cards(HDC hdcCompatible, HDC hdc)
{
    __SEH_HEADER
    int					i, r$index;
    HBITMAP				old_bitmap;
    unsigned int		card;
    CString				cardstr, rankstr, suitstr;
    CTransform			trans;

    for (i=0; i<=4; i++)
    {
        card = CARD_NOCARD;

        // try r$c0cardfaceX region first
        r$index = global.tablemap.r$c0cardfaceX_index[i];
        if (r$index != -1)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &cardstr);
            SelectObject(hdcCompatible, old_bitmap);

            if (cardstr!="")
                StdDeck_stringToCard((char *) cardstr.GetString(), (int *) &card);
            else
                card = CARD_NOCARD;

            if (card_common_last[i] != card)
            {
                card_common_last[i] = card;
                scrape_something_changed |= COM_CARD_CHANGED;
            }

            EnterCriticalSection(&cs_prwin);
            card_common[i] = card;
            LeaveCriticalSection(&cs_prwin);
        }

        // try r$c0cardfaceXrank/r$c0cardfaceXsuit region next
        if (global.tablemap.r$c0cardfaceXrank_index[i] != -1 &&
                global.tablemap.r$c0cardfaceXsuit_index[i] != -1 &&
                card==CARD_NOCARD)
        {

            // Get rank
            r$index = global.tablemap.r$c0cardfaceXrank_index[i];
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &rankstr);
            SelectObject(hdcCompatible, old_bitmap);

            // Get suit
            r$index = global.tablemap.r$c0cardfaceXsuit_index[i];
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &suitstr);
            SelectObject(hdcCompatible, old_bitmap);

            if (rankstr=="10") rankstr="T";
            if (rankstr!="" && suitstr!="")
            {
                cardstr = rankstr + suitstr;
                StdDeck_stringToCard((char *) cardstr.GetString(), (int *) &card);
            }
            if (card_common_last[i] != card)
            {
                card_common_last[i] = card;
                scrape_something_changed |= COM_CARD_CHANGED;
            }

            EnterCriticalSection(&cs_prwin);
            card_common[i] = card;
            LeaveCriticalSection(&cs_prwin);
        }

    }

    __SEH_LOGFATAL("CScraper::scrape_common_cards:\n");

}

void CScraper::scrape_player_cards(int chair, HDC hdcCompatible, HDC hdc)
{
    __SEH_HEADER
    int					j, r$index;
    HBITMAP				old_bitmap;
    unsigned int		card;
    bool				got_new_scrape;
    CString				cardstr, rankstr, suitstr;
    CTransform			trans;

    // Player cards
    got_new_scrape = false;
    for (j=0; j<=1; j++)
    {
        card = CARD_NOCARD;

        // try r$uXcardfaceY region first
        r$index = global.tablemap.r$uXcardfaceY_index[chair][j];
        if (r$index!=-1)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &cardstr);
            SelectObject(hdcCompatible, old_bitmap);
            if (cardstr!="")
            {
                StdDeck_stringToCard((char *) cardstr.GetString(), (int *) &card);
                if (card_player_last[chair][j] != card)
                {
                    card_player_last[chair][j] = card;
                    scrape_something_changed |= PL_CARD_CHANGED;
                }
                EnterCriticalSection(&cs_prwin);
                card_player[chair][j] = card;
                LeaveCriticalSection(&cs_prwin);
                if (chair==symbols.sym.userchair)
                {
                    card_player_for_display[j] = card;
                }
            }
        }

        // try r$pXcardfaceY region next
        r$index = global.tablemap.r$pXcardfaceY_index[chair][j];
        if (r$index!=-1 && card==CARD_NOCARD)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            int res = trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &cardstr);
            SelectObject(hdcCompatible, old_bitmap);

            if (cardstr!="")
            {
                StdDeck_stringToCard((char *) cardstr.GetString(), (int *) &card);
                if (card_player_last[chair][j] != card)
                {
                    card_player_last[chair][j] = card;
                    scrape_something_changed |= PL_CARD_CHANGED;
                }
                EnterCriticalSection(&cs_prwin);
                card_player[chair][j] = card;
                LeaveCriticalSection(&cs_prwin);
                if (chair==symbols.sym.userchair)
                {
                    card_player_for_display[j] = card;
                }
            }
        }

        // try r$pXcardfaceYrank/r$pXcardfaceYsuit regions next
        if (global.tablemap.r$pXcardfaceYrank_index[chair][j] != -1 &&
                global.tablemap.r$pXcardfaceYsuit_index[chair][j] != -1 &&
                card==CARD_NOCARD)
        {

            // Get rank
            r$index = global.tablemap.r$pXcardfaceYrank_index[chair][j];
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &rankstr);
            SelectObject(hdcCompatible, old_bitmap);

            // Get suit
            r$index = global.tablemap.r$pXcardfaceYsuit_index[chair][j];
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &suitstr);
            SelectObject(hdcCompatible, old_bitmap);

            if (rankstr=="10") rankstr="T";
            if (rankstr!="" && suitstr!="")
            {
                cardstr = rankstr + suitstr;
                StdDeck_stringToCard((char *) cardstr.GetString(), (int *) &card);

                if (card_player_last[chair][j] != card)
                {
                    card_player_last[chair][j] = card;
                    scrape_something_changed |= PL_CARD_CHANGED;
                }
                EnterCriticalSection(&cs_prwin);
                card_player[chair][j] = card;
                LeaveCriticalSection(&cs_prwin);
                if (chair==symbols.sym.userchair)
                {
                    card_player_for_display[j] = card;
                }
            }
        }

    }

    // Player card backs
    r$index = global.tablemap.r$pXcardback_index[chair];
    if (r$index!=-1 && card==CARD_NOCARD)
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &cardstr);
        SelectObject(hdcCompatible, old_bitmap);
        if (is_string_cardback(cardstr))
        {
            EnterCriticalSection(&cs_prwin);
            card_player[chair][0] = CARD_BACK;
            card_player[chair][1] = CARD_BACK;
            LeaveCriticalSection(&cs_prwin);
        }
        else
        {
            EnterCriticalSection(&cs_prwin);
            card_player[chair][0] = CARD_NOCARD;
            card_player[chair][1] = CARD_NOCARD;
            LeaveCriticalSection(&cs_prwin);
        }
        if (card_player_last[chair][0] != card_player[chair][0] ||
                card_player_last[chair][1] != card_player[chair][1])
        {
            card_player_last[chair][0] = card_player[chair][0];
            card_player_last[chair][1] = card_player[chair][1];
            scrape_something_changed |= PL_CARD_CHANGED;
        }
    }

    __SEH_LOGFATAL("CScraper::scrape_player_cards:\n");

}

void CScraper::scrape_seated(int chair, HDC hdcCompatible, HDC hdc)
{
    __SEH_HEADER
    int					r$index;
    HBITMAP				old_bitmap;
    CString				result;
    CTransform			trans;

    seated[chair] = "false";

    // try u region first
    r$index = global.tablemap.r$uXseated_index[chair];
    if (r$index!=-1)
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &result);
        SelectObject(hdcCompatible, old_bitmap);

        if (result!="")
            seated[chair] = result;
    }

    // try p region next,
    // but only if we didn't get a positive result from the u region
    r$index = global.tablemap.r$pXseated_index[chair];
    if (r$index!=-1 && !is_string_seated(seated[chair]))
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &result);
        SelectObject(hdcCompatible, old_bitmap);

        if (result!="")
            seated[chair] = result;
    }

    if (seated_last[chair] != seated[chair])
    {
        seated_last[chair] = seated[chair];
        scrape_something_changed |= SEATED_CHANGED;
    }


    __SEH_LOGFATAL("CScraper::scrape_seated:\n");

}

void CScraper::scrape_active(int chair, HDC hdcCompatible, HDC hdc)
{
    __SEH_HEADER
    int					r$index;
    HBITMAP				old_bitmap;
    CString				result;
    CTransform			trans;

    // try u region first
    active[chair] = "false";
    r$index = global.tablemap.r$uXactive_index[chair];
    if (r$index!=-1)
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &result);
        SelectObject(hdcCompatible, old_bitmap);

        if (result!="")
            active[chair] = result;
    }

    // try p region next,
    // but only if we didn't get a positive result from the u region
    r$index = global.tablemap.r$pXactive_index[chair];
    if (r$index!=-1 && !is_string_active(active[chair]))
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &result);
        SelectObject(hdcCompatible, old_bitmap);

        if (result!="")
            active[chair] = result;
    }

    if (active_last[chair] != active[chair])
    {
        active_last[chair] = active[chair];
        scrape_something_changed |= ACTIVE_CHANGED;
    }


    __SEH_LOGFATAL("CScraper::scrape_active:\n");

}

void CScraper::scrape_dealer(int chair, HDC hdcCompatible, HDC hdc)
{
    __SEH_HEADER
    int					r$index;
    HBITMAP				old_bitmap;
    CString				result;
    CTransform			trans;

    // Dealer button
    r$index = global.tablemap.r$pXdealer_index[chair];
    if (r$index!=-1)
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &result);
        SelectObject(hdcCompatible, old_bitmap);

        if (is_string_dealer(result))
            dealer[chair] = true;
        else
            dealer[chair] = false;

        if (dealer_last[chair] != dealer[chair])
        {
            dealer_last[chair] = dealer[chair];
            scrape_something_changed |= DEALER_CHANGED;
        }

    }

    __SEH_LOGFATAL("CScraper::scrape_dealer:\n");

}

void CScraper::scrape_name(int chair, HDC hdcCompatible, HDC hdc) {

    __SEH_HEADER
    int					r$index;
    HBITMAP				old_bitmap;
    bool				got_new_scrape;
    CString				text;
    int					ret;
    CTransform			trans;

    // Player name
    got_new_scrape = false;
    r$index = global.tablemap.r$uname_index;
    if (r$index!=-1 && symbols.user_chair_confirmed && chair==symbols.sym.chair && seated[chair])
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        ret = trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);

        if (ret==ERR_GOOD_SCRAPE_GENERAL && text!="")
            got_new_scrape = true;
    }

    r$index = global.tablemap.r$uXname_index[chair];
    if (r$index!=-1 && !got_new_scrape && symbols.user_chair_confirmed && chair==symbols.sym.chair && seated[chair])
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        ret = trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);

        if (ret==ERR_GOOD_SCRAPE_GENERAL && text!="")
            got_new_scrape = true;
    }

    r$index = global.tablemap.r$pXname_index[chair];
    if (r$index!=-1 && !got_new_scrape && seated[chair])
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        ret = trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);

        if (ret==ERR_GOOD_SCRAPE_GENERAL && text!="")
            got_new_scrape = true;
    }

    if (got_new_scrape)
    {
        name_good_scrape[chair] = true;
        playername[chair] = text;
    }
    else
    {
        name_good_scrape[chair] = false;
    }

    if (name_last[chair] != playername[chair])
    {
        name_last[chair] = playername[chair];
        scrape_something_changed |= NAME_CHANGED;
    }


    __SEH_LOGFATAL("CScraper::scrape_name:\n");

}

void CScraper::scrape_balance(int chair, HDC hdcCompatible, HDC hdc)
{
    __SEH_HEADER
    int					r$index;
    HBITMAP				old_bitmap;
    bool				got_new_scrape;
    CString				text;
    int					ret;
    CTransform			trans;

    // Player balance
    got_new_scrape = false;
    sittingout[chair] = false;

    r$index = global.tablemap.r$ubalance_index;
    if (r$index!=-1 && symbols.user_chair_confirmed && chair==symbols.sym.chair && seated[chair])
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        ret = trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);

        if (ret == ERR_GOOD_SCRAPE_GENERAL)
        {
            if (is_string_allin(text))
            {
                got_new_scrape = true;
                text = "0";
            }

            else if (text.MakeLower().Find("out")!=-1)
            {
                sittingout[chair]=true;
            }

            else
            {
                text.Remove(',');
                text.Remove('$');

                if (text!="" && is_numeric(text))
                    got_new_scrape = true;
            }
        }
    }

    r$index = global.tablemap.r$uXbalance_index[chair];
    if (r$index!=-1 && !got_new_scrape && symbols.user_chair_confirmed && chair==symbols.sym.chair && seated[chair])
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        ret = trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);

        if (ret == ERR_GOOD_SCRAPE_GENERAL)
        {
            if (is_string_allin(text))
            {
                got_new_scrape = true;
                text = "0";
            }

            else if (text.MakeLower().Find("out")!=-1)
            {
                sittingout[chair]=true;
            }

            else
            {
                text.Remove(',');
                text.Remove('$');

                if (text!="" && is_numeric(text))
                    got_new_scrape = true;
            }
        }
    }

    r$index = global.tablemap.r$pXbalance_index[chair];
    if (r$index!=-1 && !got_new_scrape && seated[chair])
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        ret = trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);

        if (ret == ERR_GOOD_SCRAPE_GENERAL)
        {
            if (is_string_allin(text))
            {
                got_new_scrape = true;
                text = "0";
            }

            else if (text.MakeLower().Find("out")!=-1)
            {
                sittingout[chair]=true;
            }

            else
            {
                text.Remove(',');
                text.Remove('$');

                if (text!="" && is_numeric(text))
                    got_new_scrape = true;
            }
        }
    }

    if (got_new_scrape)
    {
        balance_good_scrape[chair] = true;
        playerbalance[chair] = trans.string_to_money(text);
    }
    else
    {
        balance_good_scrape[chair] = false;
    }

    if (balance_last[chair] != playerbalance[chair])
    {
        balance_last[chair] = playerbalance[chair];
        scrape_something_changed |= BALANCE_CHANGED;
    }


    __SEH_LOGFATAL("CScraper::scrape_balance:\n");

}

void CScraper::scrape_bet(int chair, HDC hdcCompatible, HDC hdc)
{
    __SEH_HEADER
    int					r$index;
    HBITMAP				old_bitmap;
    CString				text;
    CTransform			trans;

    // Player bet
    playerbet[chair] = 0;

    r$index = global.tablemap.r$pXbet_index[chair];
    if (r$index!=-1)
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);
        text.Remove(',');
        text.Remove('$');

        if (text!="")
        {
            playerbet[chair] = trans.string_to_money(text);
            if (playerbet_last[chair] != playerbet[chair])
            {
                playerbet_last[chair] = playerbet[chair];
                scrape_something_changed |= PLAYERBET_CHANGED;
            }
        }
    }

    if (playerbet[chair] == 0)
    {
        r$index = global.tablemap.r$pXchip_index[chair][0][0];
        if (r$index!=-1)
        {
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, entire_window_Cur);
            playerbet[chair] = do_chip_scrape(hdcCompatible, r$index);
            SelectObject(hdcCompatible, old_bitmap);
            if (playerbet_last[chair] != playerbet[chair])
            {
                playerbet_last[chair] = playerbet[chair];
                scrape_something_changed |= PLAYERBET_CHANGED;
            }
        }
    }

    __SEH_LOGFATAL("CScraper::scrape_bet:\n");

}

void CScraper::scrape_buttons(HDC hdcCompatible, HDC hdc) {
    __SEH_HEADER
    int					j, k, r$index;
    HBITMAP				old_bitmap;
    CString				text;
    CTransform			trans;

    buttonlabel[0] = "fold";
    buttonlabel[1] = "call";
    buttonlabel[2] = "raise";
    buttonlabel[3] = "allin";
    for (j=4; j<=9; j++)
        buttonlabel[j] = "";

    for (j=0; j<=9; j++)
    {
        // Button state
        buttonstate[j] = "false";
        r$index = global.tablemap.r$iXstate_index[j];
        if (r$index!=-1)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);

            if (text!="")
                buttonstate[j] = text;

            if (buttonstate_last[j] != buttonstate[j])
            {
                buttonstate_last[j] = buttonstate[j];
                scrape_something_changed |= BUTTONSTATE_CHANGED;
            }
        }

        // i86X button state
        i86X_buttonstate[j] = "false";
        r$index = global.tablemap.r$i86Xstate_index[j];
        if (r$index!=-1)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);

            if (text!="")
                i86X_buttonstate[j] = text;

            if (i86X_buttonstate_last[j] != i86X_buttonstate[j])
            {
                i86X_buttonstate_last[j] = i86X_buttonstate[j];
                scrape_something_changed |= BUTTONSTATE_CHANGED;
            }
        }

        // Button label
        // First check iXlabel
        text = "";
        r$index = global.tablemap.r$iXlabel_index[j];
        if (r$index!=-1)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);

            if (text!="")
                buttonlabel[j] = text;
        }

        // Second check iXlabelY
        for (k=0; k<=9 && text == ""; k++)
        {
            r$index = global.tablemap.r$iXlabelY_index[j][k];
            if (r$index!=-1)
            {
                process_region(hdcCompatible, hdc, r$index);
                old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
                trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
                SelectObject(hdcCompatible, old_bitmap);

                if (text!="")
                    buttonlabel[j] = text;
            }
        }

        if (buttonlabel_last[j] != buttonlabel[j])
        {
            buttonlabel_last[j] = buttonlabel[j];
            scrape_something_changed |= BUTTONLABEL_CHANGED;
        }
    }

    // i86 button state
    r$index = global.tablemap.r$i86state_index;
    if (r$index!=-1)
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);

        if (text!="")
            i86_buttonstate = text;
        else
            i86_buttonstate = "false";

        if (i86_buttonstate_last != i86_buttonstate)
        {
            i86_buttonstate_last = i86_buttonstate;
            scrape_something_changed |= BUTTONSTATE_CHANGED;
        }
    }

    __SEH_LOGFATAL("CScraper::scrape_buttons:\n");

}

void CScraper::scrape_pots(HDC hdcCompatible, HDC hdc)
{
    __SEH_HEADER
    int					j, r$index;
    HBITMAP				old_bitmap;
    CString				text;
    CTransform			trans;

    for (j=0; j<=4; j++)
    {
        pot[j] = 0;

        // r$c0potX
        r$index = global.tablemap.r$c0potX_index[j];
        if (r$index!=-1)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);
            text.Remove(',');
            text.Remove('$');

            if (text!="")
            {
                pot[j] = trans.string_to_money(text);
                if (pot_last[j] != pot[j])
                {
                    pot_last[j] = pot[j];
                    scrape_something_changed |= POT_CHANGED;
                }
            }
        }
        // r$c0potXchipYZ_index
        if (pot[j] == 0)
        {
            r$index = global.tablemap.r$c0potXchipYZ_index[j][0][0];
            if (r$index!=-1)
            {
                process_region(hdcCompatible, hdc, r$index);	//update bitmap for scraper output window
                old_bitmap = (HBITMAP) SelectObject(hdcCompatible, entire_window_Cur);
                pot[j] = do_chip_scrape(hdcCompatible, r$index);
                SelectObject(hdcCompatible, old_bitmap);

                if (pot_last[j] != pot[j])
                {
                    pot_last[j] = pot[j];
                    scrape_something_changed |= POT_CHANGED;
                }
            }

            // update the bitmap for second chip position in the first stack
            r$index = global.tablemap.r$c0potXchipYZ_index[j][0][1];
            if (r$index!=-1)
                process_region(hdcCompatible, hdc, r$index);

            // update the bitmap for first chip position in the second stack
            r$index = global.tablemap.r$c0potXchipYZ_index[j][1][0];
            if (r$index!=-1)
                process_region(hdcCompatible, hdc, r$index);
        }
    }


    __SEH_LOGFATAL("CScraper::scrape_pots:\n");

}

void CScraper::scrape_limits(HDC hdcCompatible, HDC hdc)
{
    __SEH_HEADER
    int					j, r$index, s$index;
    HBITMAP				old_bitmap;
    CString				text;
    CString				titletext;
    char				c_titletext[128];
    bool				got_new_scrape, log_blind_change;
    CTransform			trans;

    s_limit_info.found_handnumber = s_limit_info.found_sblind = s_limit_info.found_bblind = false;
    s_limit_info.found_bbet = s_limit_info.found_ante = s_limit_info.found_limit = false;
    s_limit_info.found_sb_bb = s_limit_info.found_bb_BB = false;

    // istournament
    r$index = global.tablemap.r$c0istournament_index;
    if (r$index!=-1)
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);

        if (text!="")
            s_limit_info.istournament = true;
        else
            s_limit_info.istournament = false;

        if (istournament_last != s_limit_info.istournament)
        {
            istournament_last = s_limit_info.istournament;
            scrape_something_changed |= LIMITS_CHANGED;
        }
    }

    // r$c0handnumber
    got_new_scrape = false;
    r$index = global.tablemap.r$c0handnumber_index;
    if (r$index!=-1)
    {
        process_region(hdcCompatible, hdc, r$index);
        old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
        trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
        SelectObject(hdcCompatible, old_bitmap);
        text.Remove(',');
        if (text!="")
        {
            s_limit_info.handnumber = get_handnum_from_string(text);
            got_new_scrape = true;
        }
    }
    for (j=0; j<=9; j++)
    {
        // r$c0handnumberX
        r$index = global.tablemap.r$c0handnumberX_index[j];
        if (r$index!=-1 && !got_new_scrape)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);
            if (text!="")
            {
                s_limit_info.handnumber = get_handnum_from_string(text);
                got_new_scrape = true;
            }
        }
    }

    if (!blinds_are_locked)
    {
        // s$ttlimits - Scrape blinds/stakes/limit info from title text
        s$index = global.tablemap.s$ttlimits_index;
        if (s$index!=-1)
        {
            GetWindowText(global.attached_hwnd, c_titletext, 128);
            titletext = c_titletext;
            trans.parse_string_bsl(titletext, global.tablemap.s$[s$index].text, NULL,
                                   &s_limit_info.handnumber, &s_limit_info.sblind, &s_limit_info.bblind, &s_limit_info.bbet,
                                   &s_limit_info.ante, &s_limit_info.limit, &s_limit_info.sb_bb, &s_limit_info.bb_BB,
                                   &s_limit_info.istournament,
                                   &s_limit_info.found_handnumber, &s_limit_info.found_sblind, &s_limit_info.found_bblind,
                                   &s_limit_info.found_bbet, &s_limit_info.found_ante, &s_limit_info.found_limit,
                                   &s_limit_info.found_sb_bb, &s_limit_info.found_bb_BB);
        }

        // s$ttlimitsX - Scrape blinds/stakes/limit info from title text
        for (j=0; j<=4; j++)
        {
            s$index = global.tablemap.s$ttlimitsX_index[j];
            if (s$index!=-1)
            {
                GetWindowText(global.attached_hwnd, c_titletext, 128);
                titletext = c_titletext;
                trans.parse_string_bsl(titletext, global.tablemap.s$[s$index].text, NULL,
                                       &s_limit_info.handnumber, &s_limit_info.sblind, &s_limit_info.bblind, &s_limit_info.bbet,
                                       &s_limit_info.ante, &s_limit_info.limit, &s_limit_info.sb_bb, &s_limit_info.bb_BB,
                                       &s_limit_info.istournament,
                                       &s_limit_info.found_handnumber, &s_limit_info.found_sblind, &s_limit_info.found_bblind,
                                       &s_limit_info.found_bbet, &s_limit_info.found_ante, &s_limit_info.found_limit,
                                       &s_limit_info.found_sb_bb, &s_limit_info.found_bb_BB);
            }
        }

        // r$c0limits, s$c0limits
        r$index = global.tablemap.r$c0limits_index;
        s$index = global.tablemap.s$c0limits_index;
        if (r$index!=-1 && s$index!=-1)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);
            if (text!="")
                trans.parse_string_bsl(text, global.tablemap.s$[s$index].text, NULL,
                                       &s_limit_info.handnumber, &s_limit_info.sblind, &s_limit_info.bblind, &s_limit_info.bbet,
                                       &s_limit_info.ante, &s_limit_info.limit, &s_limit_info.sb_bb, &s_limit_info.bb_BB,
                                       &s_limit_info.istournament,
                                       &s_limit_info.found_handnumber, &s_limit_info.found_sblind, &s_limit_info.found_bblind,
                                       &s_limit_info.found_bbet, &s_limit_info.found_ante, &s_limit_info.found_limit,
                                       &s_limit_info.found_sb_bb, &s_limit_info.found_bb_BB);
        }

        for (j=0; j<=9; j++)
        {
            // r$c0limitsX, s$c0limitsX
            r$index = global.tablemap.r$c0limitsX_index[j];
            s$index = global.tablemap.s$c0limitsX_index[j];
            if (r$index!=-1 && s$index!=-1)
            {
                process_region(hdcCompatible, hdc, r$index);
                old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
                trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
                SelectObject(hdcCompatible, old_bitmap);

                if (text!="")
                    trans.parse_string_bsl(text, global.tablemap.s$[s$index].text, NULL,
                                           &s_limit_info.handnumber, &s_limit_info.sblind, &s_limit_info.bblind, &s_limit_info.bbet,
                                           &s_limit_info.ante, &s_limit_info.limit, &s_limit_info.sb_bb, &s_limit_info.bb_BB,
                                           &s_limit_info.istournament,
                                           &s_limit_info.found_handnumber, &s_limit_info.found_sblind, &s_limit_info.found_bblind,
                                           &s_limit_info.found_bbet, &s_limit_info.found_ante, &s_limit_info.found_limit,
                                           &s_limit_info.found_sb_bb, &s_limit_info.found_bb_BB);
            }
        }

        // r$c0sblind
        r$index = global.tablemap.r$c0sblind_index;
        if (r$index!=-1 && !s_limit_info.found_sblind)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);
            text.Remove(',');
            text.Remove('$');

            if (text!="")
                s_limit_info.sblind = trans.string_to_money(text);
        }

        // r$c0bblind
        r$index = global.tablemap.r$c0bblind_index;
        if (r$index!=-1 && !s_limit_info.found_bblind)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);
            text.Remove(',');
            text.Remove('$');

            if (text!="")
                s_limit_info.bblind = trans.string_to_money(text);
        }

        // r$c0bigbet
        r$index = global.tablemap.r$c0bigbet_index;
        if (r$index!=-1 && !s_limit_info.found_bbet)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);
            text.Remove(',');
            text.Remove('$');

            if (text!="")
                s_limit_info.bbet = trans.string_to_money(text);
        }

        // r$c0ante
        r$index = global.tablemap.r$c0ante_index;
        if (r$index!=-1 && !s_limit_info.found_ante)
        {
            process_region(hdcCompatible, hdc, r$index);
            old_bitmap = (HBITMAP) SelectObject(hdcCompatible, global.tablemap.r$[r$index].cur_bmp);
            trans.do_transform(&global.tablemap, &global.tablemap.r$[r$index], hdcCompatible, &text);
            SelectObject(hdcCompatible, old_bitmap);
            text.Remove(',');
            text.Remove('$');

            if (text!="")
                s_limit_info.ante = trans.string_to_money(text);
        }

    }
    else
    {
        s_limit_info.sblind = locked_sblind;
        s_limit_info.found_sblind = true;
        s_limit_info.bblind = locked_bblind;
        s_limit_info.found_bblind = true;
        s_limit_info.bbet = locked_bbet;
        s_limit_info.found_bbet = true;
        s_limit_info.ante = locked_ante;
        s_limit_info.found_ante = true;
        s_limit_info.limit = locked_gametype;
        s_limit_info.found_limit = true;
    }


    // see if anything changed
    log_blind_change = false;
    if (limit_last != s_limit_info.limit)
    {
        limit_last = s_limit_info.limit;
        scrape_something_changed |= LIMITS_CHANGED;
    }
    if (sblind_last != s_limit_info.sblind)
    {
        sblind_last = s_limit_info.sblind;
        scrape_something_changed |= LIMITS_CHANGED;
        log_blind_change = true;
    }
    if (bblind_last != s_limit_info.bblind)
    {
        bblind_last = s_limit_info.bblind;
        scrape_something_changed |= LIMITS_CHANGED;
        log_blind_change = true;
    }
    if (sb_bb_last != s_limit_info.sb_bb)
    {
        sb_bb_last = s_limit_info.sb_bb;
        scrape_something_changed |= LIMITS_CHANGED;
        log_blind_change = true;
    }
    if (bb_BB_last != s_limit_info.bb_BB)
    {
        bb_BB_last = s_limit_info.bb_BB;
        scrape_something_changed |= LIMITS_CHANGED;
        log_blind_change = true;
    }
    if (bbet_last != s_limit_info.bbet)
    {
        bbet_last = s_limit_info.bbet;
        scrape_something_changed |= LIMITS_CHANGED;
        log_blind_change = true;
    }
    if (ante_last != s_limit_info.ante)
    {
        ante_last = s_limit_info.ante;
        scrape_something_changed |= LIMITS_CHANGED;
        log_blind_change = true;
    }
    if (handnumber_last != s_limit_info.handnumber)
    {
        handnumber_last = s_limit_info.handnumber;
        scrape_something_changed |= LIMITS_CHANGED;
    }

    // log the stakes change
    if (log_blind_change)
        write_log("\n*************************************************************\nNEW STAKES sb(%.2f) bb(%.2f) BB(%.2f) ante(%.2f)\n*************************************************************\n",
                  s_limit_info.sblind, s_limit_info.bblind, s_limit_info.bbet, s_limit_info.ante);

    __SEH_LOGFATAL("CScraper::scrape_limits:\n");

}

double CScraper::get_handnum_from_string(CString t)
{
    __SEH_HEADER
    char		newstr[256] = {0};
    int			i, newpos = 0;

    for (i=0; i<t.GetLength(); i++) {
        if (t.Mid(i,1) >= "0" && t.Mid(i,1) <= "9")
        {
            newstr[newpos++] = t.Mid(i,1)[0];
        }
    }

    newstr[newpos] = 0;

    return atof(newstr);


    __SEH_LOGFATAL("CScraper::get_handnum_from_string:\n");

}


bool CScraper::bitmaps_same(HBITMAP HBitmapLeft, HBITMAP HBitmapRight) {
    __SEH_HEADER
    if (HBitmapLeft == HBitmapRight)
        return true;

    if (NULL == HBitmapLeft || NULL == HBitmapRight)
        return false;


    bool bSame = false;

    HDC hdc = GetDC(NULL);
    BITMAPINFO BitmapInfoLeft = {0};
    BITMAPINFO BitmapInfoRight = {0};

    BitmapInfoLeft.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitmapInfoRight.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

    if (0 != GetDIBits(hdc, HBitmapLeft, 0, 0, NULL, &BitmapInfoLeft, DIB_RGB_COLORS) &&
            0 != GetDIBits(hdc, HBitmapRight, 0, 0, NULL, &BitmapInfoRight, DIB_RGB_COLORS))
    {
        // Compare the BITMAPINFOHEADERs of the two bitmaps
        if (0 == memcmp(&BitmapInfoLeft.bmiHeader, &BitmapInfoRight.bmiHeader, sizeof(BITMAPINFOHEADER)))
        {
            // The BITMAPINFOHEADERs are the same so now compare the actual bitmap bits
            BYTE *pLeftBits = new BYTE[BitmapInfoLeft.bmiHeader.biSizeImage];
            BYTE *pRightBits = new BYTE[BitmapInfoRight.bmiHeader.biSizeImage];
            BYTE *pByteLeft = NULL;
            BYTE *pByteRight = NULL;

            PBITMAPINFO pBitmapInfoLeft = &BitmapInfoLeft;
            PBITMAPINFO pBitmapInfoRight = &BitmapInfoRight;

            // calculate the size in BYTEs of the additional
            // memory needed for the bmiColor table
            int AdditionalMemory = 0;
            switch (BitmapInfoLeft.bmiHeader.biBitCount)
            {
            case 1:
                AdditionalMemory = 1 * sizeof(RGBQUAD);
                break;
            case 4:
                AdditionalMemory = 15 * sizeof(RGBQUAD);
                break;
            case 8:
                AdditionalMemory = 255 * sizeof(RGBQUAD);
                break;
            case 16:
            case 32:
                AdditionalMemory = 2 * sizeof(RGBQUAD);
            }

            if (AdditionalMemory)
            {
                // we have to allocate room for the bmiColor table that will be
                // attached to our BITMAPINFO variables
                pByteLeft = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
                if (pByteLeft)
                {
                    memset(pByteLeft, 0, sizeof(BITMAPINFO) + AdditionalMemory);
                    memcpy(pByteLeft, pBitmapInfoLeft, sizeof(BITMAPINFO));
                    pBitmapInfoLeft = (PBITMAPINFO)pByteLeft;
                }

                pByteRight = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
                if (pByteRight)
                {
                    memset(pByteRight, 0, sizeof(BITMAPINFO) + AdditionalMemory);
                    memcpy(pByteRight, pBitmapInfoRight, sizeof(BITMAPINFO));
                    pBitmapInfoRight = (PBITMAPINFO)pByteRight;
                }
            }

            if (pLeftBits && pRightBits && pBitmapInfoLeft && pBitmapInfoRight)
            {
                // zero out the bitmap bit buffers
                memset(pLeftBits, 0, BitmapInfoLeft.bmiHeader.biSizeImage);
                memset(pRightBits, 0, BitmapInfoRight.bmiHeader.biSizeImage);

                // fill the bit buffers with the actual bitmap bits
                if (0 != GetDIBits(hdc, HBitmapLeft, 0, pBitmapInfoLeft->bmiHeader.biHeight, pLeftBits, pBitmapInfoLeft, DIB_RGB_COLORS) &&
                        0 != GetDIBits(hdc, HBitmapRight, 0, pBitmapInfoRight->bmiHeader.biHeight, pRightBits, pBitmapInfoRight, DIB_RGB_COLORS))
                {
                    // compare the actual bitmap bits of the two bitmaps
                    bSame = 0 == memcmp(pLeftBits, pRightBits, pBitmapInfoLeft->bmiHeader.biSizeImage);
                }
            }

            // clean up
            delete[] pLeftBits;
            delete[] pRightBits;
            delete[] pByteLeft;
            delete[] pByteRight;
        }
    }

    ReleaseDC(NULL, hdc);

    return bSame;

    __SEH_LOGFATAL("CScraper::bitmaps_same :\n");

}

void CScraper::create_bitmaps(void)
{
    __SEH_HEADER
    int			i, w, h;
    HDC			hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
    RECT		cr;

    // Whole window
    GetClientRect(global.attached_hwnd, &cr);
    w = cr.right - cr.left;
    h = cr.bottom - cr.top;
    entire_window_Last = CreateCompatibleBitmap(hdcScreen, w, h);
    entire_window_Cur = CreateCompatibleBitmap(hdcScreen, w, h);

    // r$regions
    for (i=0; i<(int) global.tablemap.r$.GetSize(); i++)
    {
        w = global.tablemap.r$[i].right - global.tablemap.r$[i].left;
        h = global.tablemap.r$[i].bottom - global.tablemap.r$[i].top;
        global.tablemap.r$[i].last_bmp = CreateCompatibleBitmap(hdcScreen, w, h);
        global.tablemap.r$[i].cur_bmp = CreateCompatibleBitmap(hdcScreen, w, h);
    }

    DeleteDC(hdcScreen);

    __SEH_LOGFATAL("CScraper::create_bitmaps :\n");

}

void CScraper::delete_bitmaps(void)
{
    __SEH_HEADER
    int			i;

    // Whole window
    DeleteObject(entire_window_Last);
    DeleteObject(entire_window_Cur);

    // Common cards
    for (i=0; i<(int) global.tablemap.r$.GetSize(); i++)
    {
        DeleteObject(global.tablemap.r$[i].last_bmp);
        DeleteObject(global.tablemap.r$[i].cur_bmp);
    }


    __SEH_LOGFATAL("CScraper::delete_bitmaps :\n");

}

// This is the chip scrape routine
double CScraper::do_chip_scrape(HDC hdc, int i)
{
    __SEH_HEADER
    int				j, stackindex, chipindex, hashindex;
    CString			type, cstemp;
    int				istart, ivert[10] = { -1 }, ihoriz[10] = { -1 }, index, vertcount=0, horizcount=0;
    int				hash_type, num_precs, pixcount, chipwidth, chipheight;
    int				top, bottom, left, right;
    bool			stop_loop;
    uint32_t		*uresult, hash, pix[MAX_HASH_WIDTH*MAX_HASH_HEIGHT];
    double			result=0;
    CString			resstring;

    // figure out if we are dealing with a pot or playerbet here
    if (global.tablemap.r$[i].name.Mid(0,5)=="c0pot" && global.tablemap.r$[i].name.Mid(6,4)=="chip")
        type = global.tablemap.r$[i].name.Mid(0,10);

    else if (global.tablemap.r$[i].name.Mid(0,1)=="p" && global.tablemap.r$[i].name.Mid(2,4)=="chip")
        type = global.tablemap.r$[i].name.Mid(0,6);

    else
        return 0;

    // find index of start, vert stride, and horiz stride regions
    for (j=0; j<(int) global.tablemap.r$.GetSize(); j++)
    {
        if (global.tablemap.r$[j].name.Find(type + "00")!=-1)
        {
            istart = j;
        }
        else if (global.tablemap.r$[j].name.Find(type + "0")!=-1)
        {
            cstemp = type + "0";
            index = global.tablemap.r$[j].name.Mid(cstemp.GetLength(), 1).GetString()[0] - '0';
            ivert[index] = j;
            vertcount++;
        }
        else if (global.tablemap.r$[j].name.Find(type)!=-1)
        {
            index = global.tablemap.r$[j].name.Mid(type.GetLength(), 1).GetString()[0] - '0';
            ihoriz[index] = j;
            horizcount++;
        }
    }

    hash_type = global.tablemap.r$[istart].transform.GetString()[1] - '0';

    stop_loop = false;
    // loop through horizontal stacks
    for (stackindex=0; stackindex<MAX_CHIP_STACKS && !stop_loop; stackindex++)
    {
        // loop through vertical chips
        for (chipindex=0; chipindex<MAX_CHIPS_PER_STACK && !stop_loop; chipindex++)
        {

            // figure out left, right, top, bottom of next chip to be scraped
            if (vertcount==1)
            {
                top = global.tablemap.r$[istart].top +
                      chipindex*(global.tablemap.r$[ivert[1]].top - global.tablemap.r$[istart].top);
                bottom = global.tablemap.r$[istart].bottom +
                         chipindex*(global.tablemap.r$[ivert[1]].bottom - global.tablemap.r$[istart].bottom);
            }
            else
            {
                if (ivert[chipindex]==-1)
                {
                    stop_loop = true;
                }
                else
                {
                    top = global.tablemap.r$[ivert[chipindex]].top;
                    bottom = global.tablemap.r$[ivert[chipindex]].bottom;
                }
            }
            if (horizcount==1)
            {
                left = global.tablemap.r$[istart].left +
                       stackindex*(global.tablemap.r$[ihoriz[1]].left - global.tablemap.r$[istart].left);
                right = global.tablemap.r$[istart].right +
                        stackindex*(global.tablemap.r$[ihoriz[1]].right - global.tablemap.r$[istart].right);
            }
            else
            {
                if (ihoriz[stackindex]==-1)
                {
                    stop_loop = true;
                }
                else
                {
                    left = global.tablemap.r$[ihoriz[stackindex]].left;
                    right = global.tablemap.r$[ihoriz[stackindex]].right;
                }
            }

            if (!stop_loop)
            {
                chipwidth = right - left;
                chipheight = bottom - top;

                // calculate hash
                if (hash_type>=1 && hash_type<=3)
                {
                    num_precs = (int) global.tablemap.p$.GetSize();
                    pixcount = 0;
                    for (j=0; j<num_precs; j++)
                    {
                        if (global.tablemap.p$[j].number == hash_type)
                        {
                            pix[pixcount++] = GetPixel(hdc, left + global.tablemap.p$[j].x, top + global.tablemap.p$[j].y);
                        }
                    }

                    if (hash_type==1) hash = hashword(&pix[0], pixcount, HASH_SEED_1);
                    else if (hash_type==2) hash = hashword(&pix[0], pixcount, HASH_SEED_2);
                    else if (hash_type==3) hash = hashword(&pix[0], pixcount, HASH_SEED_3);
                }

                // bsearch h$ for hash
                uresult = (uint32_t *) bsearch(&hash, &global.tablemap.hashes, global.tablemap.h$.GetSize(),
                                               sizeof(uint32_t),
                                               (int (*)(const void*, const void*)) compare_hash);

                // no hash match
                if (uresult == NULL)
                {
                    // stop vertical scrape loop on a non-match
                    chipindex = MAX_CHIPS_PER_STACK+1;
                }
                // hash match found
                else
                {
                    hashindex = ((long) uresult - (long) global.tablemap.hashes)/sizeof(uint32_t);
                    resstring = global.tablemap.h$[hashindex].name;
                    resstring.Remove(',');
                    resstring.Remove('$');
                    result += atof(resstring.GetString());
                }
            }
        }
    }

    return result;

    __SEH_LOGFATAL("CScraper::do_chip_scrape :\n");

}

bool CScraper::get_button_state(int button_index)
{
    __SEH_HEADER
    if (button_index<=9)
    {
        if (scraper.buttonstate[button_index] == "true" ||
                scraper.buttonstate[button_index] == "on" ||
                scraper.buttonstate[button_index] == "yes" ||
                scraper.buttonstate[button_index] == "checked" ||
                scraper.buttonstate[button_index] == "lit")
        {
            return true;
        }
    }
    else if (button_index==86)
    {
        if (scraper.i86_buttonstate == "true" ||
                scraper.i86_buttonstate == "on" ||
                scraper.i86_buttonstate == "yes" ||
                scraper.i86_buttonstate == "checked" ||
                scraper.i86_buttonstate == "lit")
        {
            return true;
        }
    }
    else if (button_index>=860)
    {
        if (scraper.i86X_buttonstate[button_index-860] == "true" ||
                scraper.i86X_buttonstate[button_index-860] == "on" ||
                scraper.i86X_buttonstate[button_index-860] == "yes" ||
                scraper.i86X_buttonstate[button_index-860] == "checked" ||
                scraper.i86X_buttonstate[button_index-860] == "lit")
        {
            return true;
        }
    }

    return false;

    __SEH_LOGFATAL("CScraper::get_button_state :\n");

}

bool CScraper::is_numeric(CString t)
{
    __SEH_HEADER
    int i;
    for (i=0; i<t.GetLength(); i++)
    {
        if (t.Mid(i,1).FindOneOf("$0123456789,.�ckm") == -1)
            return false;
    }

    return true;


    __SEH_LOGFATAL("CScraper::is_numeric :\n");

}

bool CScraper::is_string_allin(CString s)
{
    if (s=="")
        return false;

    s.Remove(' ');
    s.Remove('-');

    if (s.MakeLower().Find("allin")!=-1 ||
            s.MakeLower().Find("a11in")!=-1 ||
            s.MakeLower().Find("allln")!=-1 ||
            s.MakeLower().Find("a111n")!=-1 ||
            s.MakeLower().Find("aiiin")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_raise(CString s)
{
    if (s=="")
        return false;

    if (s.MakeLower().Find("raise")!=-1 ||
            s.MakeLower().Find("ra1se")!=-1 ||
            s.MakeLower().Find("ralse")!=-1 ||
            s.MakeLower().Find("bet")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_call(CString s)
{
    if (s=="")
        return false;

    if (s.MakeLower().Find("call")!=-1 ||
            s.MakeLower().Find("caii")!=-1 ||
            s.MakeLower().Find("ca11")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_check(CString s)
{
    if (s=="")
        return false;

    if (s.MakeLower().Find("check")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_fold(CString s)
{
    if (s=="")
        return false;

    if ((s.MakeLower().Find("fold")!=-1 ||
            s.MakeLower().Find("fo1d")!=-1 ||
            s.MakeLower().Find("foid")!=-1) &&
            s.MakeLower().Find("prefold")==-1 )
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_autopost(CString s)
{
    if (s=="")
        return false;

    s.Remove(' ');
    s.Remove('-');

    if (s.MakeLower().Find("autopost")!=-1 ||
            s.MakeLower().Find("aut0p0st")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_sitin(CString s)
{
    if (s=="")
        return false;

    s.Remove(' ');
    s.Remove('-');

    if (s.MakeLower().Find("sitin")!=-1 ||
            s.MakeLower().Find("s1t1n")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_sitout(CString s)
{
    if (s=="")
        return false;

    s.Remove(' ');
    s.Remove('-');

    if (s.MakeLower().Find("sitout")!=-1 ||
            s.MakeLower().Find("s1tout")!=-1 ||
            s.MakeLower().Find("sit0ut")!=-1 ||
            s.MakeLower().Find("s1t0ut")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_leave(CString s)
{
    if (s=="")
        return false;

    if (s.MakeLower().Find("leave")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_prefold(CString s)
{
    if (s=="")
        return false;

    if (s.MakeLower().Find("prefold")!=-1)
    {
        return true;
    }

    return false;
}


bool CScraper::is_string_seated(CString s)
{
    if (s=="")
        return false;

    if (s.MakeLower().Find("false")!=-1 ||
            s.MakeLower().Find("unseated")!=-1)
    {
        return false;
    }

    else if (s.MakeLower().Find("true")!=-1 ||
             s.MakeLower().Find("seated")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_active(CString s)
{
    // new method: active unless pXactive returns false/inactive/out/away
    if (global.tablemap.activemethod == 2)
    {
        if (s.MakeLower().Find("false")!=-1 ||
                s.MakeLower().Find("inactive")!=-1 ||
                s.MakeLower().Find("out")!=-1 ||
                s.MakeLower().Find("away")!=-1)
        {
            return false;
        }

        return true;
    }
    // old method: inactive unless pXactive returns true/active
    else
    {
        if (s=="")
            return false;

        if (s.MakeLower().Find("inactive")!=-1)
        {
            return false;
        }

        if (s.MakeLower().Find("true")!=-1 ||
                s.MakeLower().Find("active")!=-1)
        {
            return true;
        }

        return false;
    }
}

bool CScraper::is_string_cardback(CString s)
{
    if (s=="")
        return false;

    if (s.MakeLower().Find("true")!=-1 ||
            s.MakeLower().Find("cardback")!=-1)
    {
        return true;
    }

    return false;
}

bool CScraper::is_string_dealer(CString s)
{
    if (s=="")
        return false;

    if (s.MakeLower().Find("true")!=-1 ||
            s.MakeLower().Find("dealer")!=-1)
    {
        return true;
    }

    return false;
}
