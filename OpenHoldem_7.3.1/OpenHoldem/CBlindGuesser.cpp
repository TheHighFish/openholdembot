//****************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//****************************************************************************** 
//
// Purpose: first guess of blind values for CTableLimits,
//   which might be overridden bz blind-locking.
//
//****************************************************************************** 

#include "stdafx.h"
#include "CBlindGuesser.h"

#include "CBlindLevels.h"
#include "CScraper.h"
#include "CSymbolEngineHistory.h"
#include "CTableState.h"

CBlindGuesser::CBlindGuesser() {
}

CBlindGuesser::~CBlindGuesser() {
}

// All parameters are out-parameters only
void CBlindGuesser::Guess(double *sblind, double *bblind, double *bbet) {
  GetFirstBlindDataFromScraper(sblind, bblind, bbet);
  // Take what we have and check the blinds on the list of known levels
  // (either complete or partial match)
  if (_blind_levels.BestMatchingBlindLevel(sblind, bblind, bbet)) {
    return;
  }
  if (CompletePartiallyKnownBlinds(sblind, bblind, bbet)) {
    return;
  }
  // Missing input.
  // We have to guess everything from the table
  // And then again try to verify/complete the data.
  GetFirstBlindDataFromBetsAtTheTable(sblind, bblind, bbet);
  if (_blind_levels.BestMatchingBlindLevel(sblind, bblind, bbet)) {
    return;
  }
  if (CompletePartiallyKnownBlinds(sblind, bblind, bbet)) {
    return;
  }
  // Complete failure
  // Assume complete beginner and lowest blinds.
  *sblind = 0.01;
  *bblind = 0.02;
  *bbet   = 0.04;
}

bool CBlindGuesser::CompletePartiallyKnownBlinds(double *sblind, 
                                                 double *bblind, 
                                                 double *bbet) {
  // Not on the list, potentially still ok.
  // But we have to do some guess-work
  if (SBlindBBlindCombinationReasonable(*sblind, *bblind)
      && SBlindBBetCombinationReasonable(*sblind, *bbet)
      && BBlindBBetCombinationReasonable(*bblind, *bbet)) {
    // This looks VERY reasonable
    // keep everything as is.
  } // else if...
  // Otherwise: take just 2 values to guess the 3rd one,
  // starting with sblind, bblind, which we mostly should know.
  else if (SBlindBBlindCombinationReasonable(*sblind, *bblind)) {
    *bbet =  2 * *bblind;
  } else if (SBlindBBetCombinationReasonable(*sblind, *bbet)) {
    *bblind =  ReasonableLookingHalfBlindValue(*bbet);
  } else if (BBlindBBetCombinationReasonable(*bblind, *bbet)) {
    *sblind = ReasonableLookingHalfBlindValue(*bblind);
  } // else if...
  // Finally: Take 1 value to guess all the rest,
  // starting with bblind, which we usually know
  else if (*bblind >= 0.02) {
    *sblind = ReasonableLookingHalfBlindValue(*bblind);
    *bbet   = 2 * *bblind;
  } else if (*bbet >= 0.04) {
    *bblind = ReasonableLookingHalfBlindValue(*bbet);
    *sblind = ReasonableLookingHalfBlindValue(*bblind);
  } else if (*sblind >= 0.01) {
    *bblind = 2 * *sblind;
    *bbet   = 2 * *bblind;
  } else {
    return false;
  }
  // !!!!! logging
  return true;
}

bool CBlindGuesser::SBlindBBlindCombinationReasonable(double sblind, double bblind) {
  if (sblind <= 0.00) return false;
  if (bblind <= 0.00) return false;
  if (sblind < 0.40 * bblind) return false;
  if (sblind > 0.50 * bblind) return false;
  return true;
}

bool CBlindGuesser::SBlindBBetCombinationReasonable(double sblind, double bbet) {
  if (sblind <= 0.00) return false;
  if (bbet   <= 0.00) return false;
  if (sblind < 0.20 * bbet) return false;
  if (sblind > 0.25 * bbet) return false;
  return true;
}

bool CBlindGuesser::BBlindBBetCombinationReasonable(double bblind, double bbet) {
  if (bblind <= 0.00) return false;
  if (bbet   <= 0.00) return false;
  if (bblind < 0.40 * bbet) return false;
  if (bblind > 0.50 * bbet) return false;
  return true;
}

// Some beautiful number, usually in the range of 40..50%
double CBlindGuesser::ReasonableLookingHalfBlindValue(double known_value) {
  assert(known_value >= 0.02);
  // Int for implicit truncation, i.e. "rounding"
  int half_amount_in_cents = 50 * known_value;
  double rounded_amount_in_dollars = 0.01 * double(half_amount_in_cents);
  // Turn it into something reasonable
  // and use small=or equal, because sometimes "half" is just 40%
  return _blind_levels.GetNextSmallerOrEqualBlindOnList(rounded_amount_in_dollars);
}

// All parameters are out-parameters only
// Guessing according to Want2Learns method
// http://www.maxinmontreal.com/forums/viewtopic.php?f=117&t=17380&start=60
void CBlindGuesser::GetFirstBlindDataFromBetsAtTheTable(double *sblind, 
                                                        double *bblind, 
                                                        double *bbet) {
  // Everything is unknown, init to zero
  *sblind = k_undefined_zero;
  *bblind = k_undefined_zero;
  *bbet   = k_undefined_zero;
  // Search first two bets...
  double first_bet_after_dealer  = 0.0;
  double second_bet_after_dealer = 0.0;
  bool   first_chair_immediatelly_after_dealer_betting = false;
  int    second_betting_chair_after_dealer = k_undefined;

  if (first_chair_immediatelly_after_dealer_betting) {
    // Can't be a missing small-blind.
    // Therefore this is the small blind,
    // except maybe for headsup
    assert(first_bet_after_dealer > 0) ;
    *sblind = first_bet_after_dealer;
  } else if (second_bet_after_dealer == 0.0) {
    // Only one blind, must be big-blind, small-blind missing
    *bblind = first_bet_after_dealer;
  } else if (second_bet_after_dealer > 2.5 * first_bet_after_dealer) {
    // Missing small blind and UTG raising to more than 2 big blinds.
    *bblind = first_bet_after_dealer;
  } else if (second_bet_after_dealer == first_bet_after_dealer) {
    // Either completing small-blind
    // or missing small-blind and limping UTG
    *bblind = first_bet_after_dealer;
  } else if (second_bet_after_dealer == 2 * first_bet_after_dealer) {
    // Could be either small-blind/bblind
    // or missing small-blind and min-raising UTG
    // We don't know exactly
    *sblind = first_bet_after_dealer;
  } else {
    // Assume the first bet is "normal" and therefore small-blind
    *sblind = first_bet_after_dealer;
  }
  // Still to do (by the caller):
  // verify and complete these values
}

void CBlindGuesser::GetFirstBlindDataFromScraper(double *sblind, 
                                                 double *bblind, 
                                                 double *bbet) {
  // Get values from the scraper (ttlimits / c0limits)
  assert(p_scraper != NULL);
  assert(p_scraper->s_limit_info() != NULL);
  *sblind = p_scraper->s_limit_info()->sblind;
  *bblind = p_scraper->s_limit_info()->bblind;
  *bbet   = p_scraper->s_limit_info()->bbet;
}

/*
   If $Didchecround[1] = 0 And $Didcallround[1] = 0 And $Didswaground[1] = 0 Then
      $Sblind_seat = 10
      $Bblind_seat = 10
      
      $Mess = "+++++++++++++++++++++++++++++"
      state_logger($Mess)   
      $Mess = "++++++++++++ NEW HAND++++++++"
      state_logger($Mess)      
      $Mess = "+++++++++++++++++++++++++++++"
      state_logger($Mess)   
      
      ;we got the SB value from the title
      If $Sblind > 0 Then
         For $seat = 0 to 9
           If $Currentbet[$seat]=$Sblind Then
             $Sblind_seat = $seat
             ExitLoop
           EndIf         
         Next
         ;we found SB so now it is easy to find the BB
         If $Sblind_seat<10 Then      
           ;because there is always a BB the player, the next player with chips is the BB
           For $seat = $Sblind_seat to 9
             If $seat<> $Sblind_seat And $Currentbet[$seat]>0 Then
               $Bblind_seat = $seat
               ExitLoop
             EndIf
           Next   
            If $Bblind_seat = 10 Then
              For $seat = 0 to $Dealerchair
                If $seat<> $Sblind_seat And $seat<> $Dealerchair And $Currentbet[$seat]> 0 Then
                  $Bblind_seat = $seat
                  ExitLoop
                EndIf
              Next         
            EndIf
         EndIf
         
         ;we found no SB but we have chips in front of us
         If ($Sblind_seat = 10 or $Bblind_seat=10) And $Currentbet[$Userchair] > $Sblind Then
            If $Dealerchair<$Userchair Then
              For $seat = $Dealerchair to $Userchair
                If $PlayerPlaying[$seat]>0 And $seat<>$Dealerchair And $seat<>$Userchair Then
                  $Sblind_seat =    $seat
                  ExitLoop
               EndIf
             Next
            EndIf
          
            If $Dealerchair>$Userchair Then
              For $seat = $Dealerchair to 0
                If $PlayerPlaying[$seat]>0 And $seat<>$Dealerchair And $seat<>$Userchair Then
                  $Sblind_seat = $seat
                  ExitLoop
               EndIf
             Next
             If $Sblind_seat= 10 Then
               For $seat = 0 to $Userchair
                  If $PlayerPlaying[$seat]>0 And $seat<>$Userchair Then
                     $Sblind_seat = $seat
                     ExitLoop
                  EndIf
                Next
             EndIf
           EndIf
           $Bblind_seat = $Userchair
           $Bblind = $Currentbet[$Userchair]
           $Sblind = $Bblind/2
         EndIf
               
         $Mess = " We found by ordinary method $Sblind_seat: "&$Sblind_seat&" and $Bblind_seat"&$Bblind_seat
         state_logger($Mess, 5)   
      EndIf
      
      ;we were unable to find the SB by regular methods
      If $Sblind_seat = 10 or $Bblind_seat =  10 Then
         
         ;if the player next to the Dealer button has chips in front of him, he will always be the SB
         Local $next_seat = $Dealerchair + 1
         If $Dealerchair = 8 And $Currentbet[9]= 0 Then $next_seat = 0
         If $Dealerchair = 9 Then $next_seat = 0
         
         If $Currentbet[$next_seat]> 0 Then
           $Sblind_seat = $next_seat
           $Mess = " We found no empty seat"
            state_logger($Mess, 5)   
           ;if he is not allin then the chips in front of his is the SB
           If $Balance[$next_seat]> 0 Then
             $Sblind = $Currentbet[$next_seat]
           EndIf
          
           ;the next player with chips will always be the BB
           For $seat = $Sblind_seat to 9
             If $seat<> $Sblind_seat And $Currentbet[$seat]> 0 Then
               $Bblind_seat = $seat
               $Bblind = $Currentbet[$seat]
               ExitLoop
             EndIf          
           Next
           If $Bblind_seat = 10 Then
             For $seat = 0 to $Dealerchair
                If $seat<> $Sblind_seat And $seat<> $Dealerchair And $Currentbet[$seat]> 0 Then
                  $Bblind_seat = $seat
                  $Bblind = $Currentbet[$seat]
                  ExitLoop
               EndIf
              Next   
           EndIf
          
            If $Bblind_seat = 10 Then
              If $Currentbet[$Userchair]> $Sblind Then
                $Bblind_seat = $Userchair
                $Bblind = $Currentbet[$Userchair]
              EndIf
              If $Currentbet[$Userchair] = 0 Then
                $Bblind_seat = $Sblind_seat
                $Bblind = $Currentbet[$Sblind_seat]
                $Sblind_seat = 10
                $Sblind = $Bblind/2
              EndIf
            EndIf
            
            $Mess = " We found $Sblind_seat: "&$Sblind_seat&" and $Bblind_seat "&$Bblind_seat&" because no empty seat"
            state_logger($Mess, 5)   
                     
         EndIf
         
         ;if the player next to the button DOES NOT HAVE CHIPS, then we find the first player with chips in front of him         
         If $Currentbet[$next_seat] = 0 Then
           ;we will assume as a default that the first player with chips in front of him after the BTN is the BB
           $Mess = " We found empty seat"
            state_logger($Mess, 5)   
          
           ;just easy to assume we are the BB when then things go wrong
           If $Currentbet[$Userchair] > 10 Then
              $Bblind_seat = $Userchair
              $Bblind = $Currentbet[$Userchair]
              $Sblind = $Bblind/2
           EndIf
          
           If $Bblind_seat = 10 Then            
              For $seat = $Dealerchair to 9
                If $seat<> $Dealerchair And $Currentbet[$seat]> 0 Then
                  $Bblind_seat = $seat
                  $Bblind = $Currentbet[$seat]
                  $Sblind = $Bblind/2
                  ExitLoop
                EndIf
              Next
            
              If $Bblind_seat = 10 Then
                For $seat = 0 to $Dealerchair
                  If $seat<> $Dealerchair And $Currentbet[$seat]> 0 Then
                     $Bblind_seat = $seat
                     $Bblind = $Currentbet[$seat]
                     $Sblind = $Bblind/2
                     ExitLoop
                  EndIf
                Next
              EndIf
            EndIf
                        
            $Mess = " We found possible BB "&$Bblind_seat
            state_logger($Mess, 5)   
            
            If $Bblind_seat <> 10 Then
               ;We now look at the next player with cards because we know there can be no players between the SB and the BB
               Local $next_to_poster = 10
               For $seat = $Bblind_seat to 9
                  If $PlayerSeated[$seat]>0 And $seat<>$Bblind_seat Then
                     $next_to_poster = $seat
                     ExitLoop
                  EndIf            
               Next
               If $next_to_poster = 10 Then
                  For $seat = 0 to $Dealerchair
                     If $PlayerSeated[$seat]>0 And $seat<>$Dealerchair And $seat<>$Bblind_seat Then
                        $next_to_poster = $seat
                        ExitLoop
                     EndIf            
                  Next            
               EndIf
               If $next_to_poster < 10 Then
                  ;if both seats have exactly the same amount of chips or the first has more And we the next player then we BB and SB completed or raised         
                  If $next_to_poster= $Userchair And $Currentbet[$Userchair] > 0 Then
                     $Sblind_seat = $Bblind_seat
                     $Bblind_seat = $Userchair
                     $Bblind = $Currentbet[$Userchair]
                     $Sblind = $Bblind/2
                  EndIf
                  ;NOW WE ARE LEFT WITH THE DIFFICULT SITUATION WHERE:
                  ;1) THERE IS AT LEAST ONE EMPTY SEAT BETWEEN THE BTN AND THE FIRST POSTER
                  ;2) THE SECOND POSTER HAS EXACTLY 2*THE AMOUNT OF THE FIRST POSTER
                  ;THIS SITUATION IS EITHER SB/BB OR BB AND UTG MIN RAISE
                  If $next_to_poster<> $Userchair And $Currentbet[$next_to_poster] = $Bblind Then
                     $Sblind_seat = $Bblind_seat
                     $Bblind_seat = $next_to_poster
                     $Sblind = $Bblind/2
                  EndIf   
               EndIf         
            EndIf
            */




/*




void CSymbolEngineTableLimits::CalcTableLimits_NL_PL()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] CalcTableLimits_NL_PL()\n");
	if (tablelimit_unreliable_input.sblind==0)
	{
		if (p_scraper->s_limit_info()->sb_bb > 0)
			SetSmallBlind(p_scraper->s_limit_info()->sb_bb);
	}
	if (tablelimit_unreliable_input.bblind==0)
	{
		if (p_scraper->s_limit_info()->bb_BB > 0)
			SetBigBlind(p_scraper->s_limit_info()->bb_BB);
	}
	if (tablelimit_unreliable_input.bbet==0)
	{
		if (p_scraper->s_limit_info()->bb_BB > 0)
			SetBigBet(p_scraper->s_limit_info()->bb_BB);
		else if (p_scraper->s_limit_info()->sb_bb > 0)
			SetBigBet(p_scraper->s_limit_info()->sb_bb*2);
		else if (tablelimit_unreliable_input.bblind > 0)
			SetBigBet(tablelimit_unreliable_input.bblind);
		else if (tablelimit_unreliable_input.sblind > 0)
			SetBigBet(tablelimit_unreliable_input.sblind*2);
	}
}


bool CSymbolEngineTableLimits::IsCalculationNeccessary()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] IsCalculationNeccessary()\n");
	if (tablelimit_unreliable_input.sblind <= 0)
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] sblind=0, calculation neccessary.\n");
		return true;
	}
	if (tablelimit_unreliable_input.bblind <= 0)
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] bblind=0, calculation neccessary.\n");
		return true;
	}
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] no calculation neccessary.\n");
	return false;
}

void CSymbolEngineTableLimits::CalcTableLimits_FL_AndUnknownGametype()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] CalcTableLimits_FL_AndUnknownGametype()\n");
	if (tablelimit_unreliable_input.sblind==0)
	{
		if (p_scraper->s_limit_info()->sb_bb > 0)
			SetSmallBlind(p_scraper->s_limit_info()->sb_bb);
	}
	if (tablelimit_unreliable_input.bblind==0)
	{
		if (p_scraper->s_limit_info()->bb_BB > 0)
			SetBigBlind(p_scraper->s_limit_info()->bb_BB);
	}
	if (tablelimit_unreliable_input.bbet==0)
	{
		if (p_scraper->s_limit_info()->bb_BB > 0)
			SetBigBet(p_scraper->s_limit_info()->bb_BB*2);
		else if (p_scraper->s_limit_info()->sb_bb > 0)
			SetBigBet(p_scraper->s_limit_info()->sb_bb*4);
		else if (tablelimit_unreliable_input.bblind >0)
			SetBigBet(tablelimit_unreliable_input.bblind*2);
		else if (tablelimit_unreliable_input.sblind > 0)
			SetBigBet(tablelimit_unreliable_input.sblind*4);
	}
}

void CSymbolEngineTableLimits::SwapBlindsIfSbGreaterThanBBAndNotZero()
{
	// If SB is greater than BB that is usually name mistake and we should swap.
	// But we don't swap, if BB is zero, as that does mean, 
	// that we just got the amount wrong (BB got cards, i.e. dealt, but no bet).
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SwapBlindsIfSbGreaterThanBBAndNotZero()\n");
	if ((tablelimit_unreliable_input.sblind > tablelimit_unreliable_input.bblind)
		&& (tablelimit_unreliable_input.bblind >= 0.01))
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SwapBlindsIfSbGreaterThanBBAndNotZero swap neccessary\n");
		SwapDoubles(&tablelimit_unreliable_input.sblind, &tablelimit_unreliable_input.bblind);
	}
}

void CSymbolEngineTableLimits::SearchTableForSbAndBbValue()
{
	bool	found_inferred_sb = false, found_inferred_bb = false;

	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue()\n");
	for (int i=1; i<=p_tablemap->nchairs(); i++)
	{
		int next_chair = (p_symbol_engine_dealerchair->dealerchair() + i) % p_tablemap->nchairs();
		// We do no longer require cards for the players,
		// as this mixed things up, when e.g. SB folds.
		// We try to look directly at the bets.
		//
		// But not looking for cards is bad either, 
		// as sometimes we don't get the bet correctly 
		// and then end up with the bet of another user.
		// If we find cards without name necessary blind
		// then we will try it as unknown (zero).
		if (p_scraper->player_bet(next_chair) >= 0.01)
		{
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue found bet for chair %d\n", next_chair);
			// Bet found
			if (!found_inferred_sb)
			{
				// Not yet SB known
				if (tablelimit_unreliable_input.sblind==0)
				{
					SetSmallBlind(p_scraper->player_bet(next_chair));
					write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] found inferred SB at chair %d: %f\n", next_chair, p_scraper->player_bet(next_chair));
					found_inferred_sb = true;
				}
			}
			else if (!found_inferred_bb)
			{
				// Not yet BB known
				if (tablelimit_unreliable_input.bblind==0)
				{
					// Not heads up - normal blinds
					if (next_chair != p_symbol_engine_dealerchair->dealerchair())
					{
						SetBigBlind(p_scraper->player_bet(next_chair));
						write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] found inferred BB at chair %d: %f\n", next_chair, p_scraper->player_bet(next_chair));
					}
					// heads up - reversed blinds
					else
					{
						SetBigBlind(tablelimit_unreliable_input.sblind);
						SetSmallBlind(p_scraper->player_bet(next_chair));
						write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] found inferred BB headsup at chair %d: %f\n", next_chair, p_scraper->player_bet(next_chair));
						write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] Swapping blinds.\n");
					}
					found_inferred_bb = true;
				}
				break;
			}
		}
    else if (p_table_state->_players[next_chair].HasAnyCards())
		{
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue found no bet for chair %d\n", next_chair);
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue but found cards for chair %d\n", next_chair);
			// Awful. Found cards, but not the expected blind.
			// Either name misread, or name blind is missing.
			// Read it as zero and try to guess it later correctly.
			if (!found_inferred_sb)
			{
				found_inferred_sb = true;
				write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] expected SB, but not found\n");
				// Don't set any value.
				// Who knows, we might even have correct data 
				// from name previous round!? 
			}
			else if (!found_inferred_bb)
			{
				found_inferred_bb = true;
				write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] expected BB, but not found\n");
				// Don't set any value.
				// Who knows, we might even have correct data 
				// from name previous round!? 
				break;
			}
		}
		else
		{
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] SearchTableForSbAndBbValue found neither bet nor cards for chair %d\n", next_chair);
		}
	}
	SwapBlindsIfSbGreaterThanBBAndNotZero();
}

double CSymbolEngineTableLimits::GuessSmallBlindFromBigBlind()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessSmallBlindFromBigBlind()\n");
	// Special case: 0.02/0.05/0.10
	if (IsEqual(tablelimit_unreliable_input.bblind, 0.05))
	{
		return 0.02;
	}
	// Special case: 0.05/0.15/0.30
	if (IsEqual(tablelimit_unreliable_input.bblind, 0.15))
	{
		return 0.05;
	}
	// Special case: 0.10/0.25/0.50
	if (IsEqual(tablelimit_unreliable_input.bblind, 0.25))
	{
		return 0.10;
	}
	// Special case: 0.02/0.05
	if (IsEqual(tablelimit_unreliable_input.bblind, 0.05))
	{
		return 0.02;
	}
	return (tablelimit_unreliable_input.bblind / 2);
}

double CSymbolEngineTableLimits::GuessBigBlindFromSmallBlind()
{	// Special cases: NL 0.10/0.25.
	// Some casinos do also provide NL 0.10/0.20,
	// but that is rare, we do assume 0.10/0.25 here.
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessBigBlindFromSmallBlind()\n");
	if (gametype() == k_gametype_NL && IsEqual(tablelimit_unreliable_input.sblind, 0.10))
	{
		return 0.25;
	}
	return (tablelimit_unreliable_input.sblind * 2);
}

double CSymbolEngineTableLimits::GuessBigBetFromBigBlind()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessBigBetFromBigBlind()\n");
	if ((gametype() == k_gametype_NL) || (gametype() == k_gametype_PL))
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessBigBetFromBigBlind() BB = bb\n");
		return (tablelimit_unreliable_input.bblind);
	}
	else
	{
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] GuessBigBetFromBigBlind() BB = 2*bb\n");
		return (tablelimit_unreliable_input.bblind*2);
	}
}

void CSymbolEngineTableLimits::AdjustForReasonableness()
{
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] CheckForReasonableness()\n");
	// SB unknown?
	if (IsSmallerOrEqual(tablelimit_unreliable_input.sblind, 0))
	{
		SetSmallBlind(GuessSmallBlindFromBigBlind());
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] adjusting SB\n");
	}
	// BB unknown?
	if (IsSmallerOrEqual(tablelimit_unreliable_input.bblind, 0))
	{
		SetBigBlind(GuessBigBlindFromSmallBlind());
			write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] adjusting BB\n");
	}
	// BB out of range?
	if (IsGreater(tablelimit_unreliable_input.bblind, (tablelimit_unreliable_input.sblind*3)))
	{
		if (IsGreater(tablelimit_unreliable_input.sblind, 0))
		{
			SetBigBlind(GuessBigBlindFromSmallBlind());
		}
	}
	// SB out of range?
	if (IsGreaterOrEqual(tablelimit_unreliable_input.sblind, tablelimit_unreliable_input.bblind))
	{
		// Formerly this code was checking for SB > BB,
		// but up to now we never saw an online-game with identical blinds.
		// SB >= BB does usually mean, that SB did already act and complete.
		SetSmallBlind(GuessSmallBlindFromBigBlind());
		write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] : adjusting SB\n");
	}
	// Always force big-bet = 2 * big-blind
	SetBigBet(GuessBigBetFromBigBlind());
	write_log(preferences.debug_table_limits(), "[CSymbolEngineTableLimits] adjusting big bet\n");
}*/