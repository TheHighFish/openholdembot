#include "stdafx.h"
#include "CAutoplayerFunctions.h"

CAutoplayerFunctions *p_autoplayer_functions = NULL;


CAutoplayerFunctions::CAutoplayerFunctions()
{
	Reset();
}

void CAutoplayerFunctions::Reset()
{
	set_f$alli(0);
	set_f$betsize(0);
	set_f$rais(0);
	set_f$call(0);
	set_f$sitin(0);
	set_f$sitout(0);
	set_f$leave(0);
	set_f$close(0);
	set_f$prefold(0);
	set_f$rebuy(0);
	set_f$delay(0);
	set_f$chat(0);
}

