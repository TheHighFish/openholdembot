#include "StdAfx.h"
#include "RunRon.h"
#include "global.h"
#include "scraper.h"
#include "symbols.h"
#include "inlines/eval.h"
#include "debug.h"

CRunRon::CRunRon(void) {
#ifdef SEH_ENABLE
	try {
#endif	

	//POCKET PAIRS
	//============
	run_count[12][12][9]=94; run_count[12][12][8]=122; run_count[12][12][7]=17848; run_count[12][12][6]=181104; run_count[12][12][5]=41562; run_count[12][12][4]=25816; run_count[12][12][3]=249458; run_count[12][12][2]=840456; run_count[12][12][1]=762300; run_count[12][12][0]=0; 
	run_count[11][11][9]=94; run_count[11][11][8]=122; run_count[11][11][7]=17848; run_count[11][11][6]=181104; run_count[11][11][5]=41562; run_count[11][11][4]=25816; run_count[11][11][3]=249458; run_count[11][11][2]=840456; run_count[11][11][1]=762300; run_count[11][11][0]=0; 
	run_count[10][10][9]=94; run_count[10][10][8]=210; run_count[10][10][7]=17848; run_count[10][10][6]=181104; run_count[10][10][5]=41474; run_count[10][10][4]=33774; run_count[10][10][3]=248952; run_count[10][10][2]=838944; run_count[10][10][1]=756360; run_count[10][10][0]=0; 
	run_count[9][9][9]=94; run_count[9][9][8]=298; run_count[9][9][7]=17848; run_count[9][9][6]=181104; run_count[9][9][5]=41386; run_count[9][9][4]=41732; run_count[9][9][3]=248446; run_count[9][9][2]=837432; run_count[9][9][1]=750420; run_count[9][9][0]=0; 
	run_count[8][8][9]=94; run_count[8][8][8]=386; run_count[8][8][7]=17848; run_count[8][8][6]=181104; run_count[8][8][5]=41298; run_count[8][8][4]=49690; run_count[8][8][3]=247940; run_count[8][8][2]=835920; run_count[8][8][1]=744480; run_count[8][8][0]=0; 
	run_count[7][7][9]=4; run_count[7][7][8]=474; run_count[7][7][7]=17848; run_count[7][7][6]=181104; run_count[7][7][5]=41300; run_count[7][7][4]=48700; run_count[7][7][3]=247940; run_count[7][7][2]=835920; run_count[7][7][1]=745470; run_count[7][7][0]=0; 
	run_count[6][6][9]=4; run_count[6][6][8]=474; run_count[6][6][7]=17848; run_count[6][6][6]=181104; run_count[6][6][5]=41300; run_count[6][6][4]=48700; run_count[6][6][3]=247940; run_count[6][6][2]=835920; run_count[6][6][1]=745470; run_count[6][6][0]=0; 
	run_count[5][5][9]=4; run_count[5][5][8]=474; run_count[5][5][7]=17848; run_count[5][5][6]=181104; run_count[5][5][5]=41300; run_count[5][5][4]=48700; run_count[5][5][3]=247940; run_count[5][5][2]=835920; run_count[5][5][1]=745470; run_count[5][5][0]=0; 
	run_count[4][4][9]=4; run_count[4][4][8]=474; run_count[4][4][7]=17848; run_count[4][4][6]=181104; run_count[4][4][5]=41300; run_count[4][4][4]=48700; run_count[4][4][3]=247940; run_count[4][4][2]=835920; run_count[4][4][1]=745470; run_count[4][4][0]=0; 
	run_count[3][3][9]=4; run_count[3][3][8]=476; run_count[3][3][7]=17848; run_count[3][3][6]=181104; run_count[3][3][5]=41298; run_count[3][3][4]=49690; run_count[3][3][3]=247940; run_count[3][3][2]=835920; run_count[3][3][1]=744480; run_count[3][3][0]=0; 
	run_count[2][2][9]=4; run_count[2][2][8]=388; run_count[2][2][7]=17848; run_count[2][2][6]=181104; run_count[2][2][5]=41386; run_count[2][2][4]=41732; run_count[2][2][3]=248446; run_count[2][2][2]=837432; run_count[2][2][1]=750420; run_count[2][2][0]=0; 
	run_count[1][1][9]=4; run_count[1][1][8]=300; run_count[1][1][7]=17848; run_count[1][1][6]=181104; run_count[1][1][5]=41474; run_count[1][1][4]=33774; run_count[1][1][3]=248952; run_count[1][1][2]=838944; run_count[1][1][1]=756360; run_count[1][1][0]=0; 
	run_count[0][0][9]=4; run_count[0][0][8]=212; run_count[0][0][7]=17848; run_count[0][0][6]=181104; run_count[0][0][5]=41562; run_count[0][0][4]=25816; run_count[0][0][3]=249458; run_count[0][0][2]=840456; run_count[0][0][1]=762300; run_count[0][0][0]=0; 
	run_max_pokval[12][12]=2148457658; run_max_pokval[11][11]=2148457658; run_max_pokval[10][10]=2148457658; run_max_pokval[9][9]=2148457658; run_max_pokval[8][8]=2148457658; run_max_pokval[7][7]=2148457658; run_max_pokval[6][6]=2148457658; run_max_pokval[5][5]=2148457658; run_max_pokval[4][4]=2148457658; run_max_pokval[3][3]=2148457658; run_max_pokval[2][2]=2148457658; run_max_pokval[1][1]=2148457658; run_max_pokval[0][0]=2148457658; 
	run_max_pokval_n[12][12]=94; run_max_pokval_n[11][11]=94; run_max_pokval_n[10][10]=94; run_max_pokval_n[9][9]=94; run_max_pokval_n[8][8]=94; run_max_pokval_n[7][7]=4; run_max_pokval_n[6][6]=4; run_max_pokval_n[5][5]=4; run_max_pokval_n[4][4]=4; run_max_pokval_n[3][3]=4; run_max_pokval_n[2][2]=4; run_max_pokval_n[1][1]=4; run_max_pokval_n[0][0]=4; 

	//SUITED HANDS
	//============
	run_count[12][11][9]=1084; run_count[12][11][8]=78; run_count[12][11][7]=2668; run_count[12][11][6]=47124; run_count[12][11][5]=138296; run_count[12][11][4]=65508; run_count[12][11][3]=92004; run_count[12][11][2]=469092; run_count[12][11][1]=916776; run_count[12][11][0]=386130; 
	run_count[12][10][9]=1084; run_count[12][10][8]=122; run_count[12][10][7]=2668; run_count[12][10][6]=47124; run_count[12][10][5]=138252; run_count[12][10][4]=72939; run_count[12][10][3]=92004; run_count[12][10][2]=469092; run_count[12][10][1]=913920; run_count[12][10][0]=381555; 
	run_count[12][9][9]=1084; run_count[12][9][8]=166; run_count[12][9][7]=2668; run_count[12][9][6]=47124; run_count[12][9][5]=138208; run_count[12][9][4]=80370; run_count[12][9][3]=92004; run_count[12][9][2]=469092; run_count[12][9][1]=911064; run_count[12][9][0]=376980; 
	run_count[12][8][9]=1084; run_count[12][8][8]=210; run_count[12][8][7]=2668; run_count[12][8][6]=47124; run_count[12][8][5]=138164; run_count[12][8][4]=87801; run_count[12][8][3]=92004; run_count[12][8][2]=469092; run_count[12][8][1]=908208; run_count[12][8][0]=372405; 
	run_count[12][7][9]=49; run_count[12][7][8]=254; run_count[12][7][7]=2668; run_count[12][7][6]=47124; run_count[12][7][5]=139155; run_count[12][7][4]=52821; run_count[12][7][3]=92565; run_count[12][7][2]=471735; run_count[12][7][1]=925344; run_count[12][7][0]=387045; 
	run_count[12][6][9]=49; run_count[12][6][8]=298; run_count[12][6][7]=2668; run_count[12][6][6]=47124; run_count[12][6][5]=139111; run_count[12][6][4]=60252; run_count[12][6][3]=92565; run_count[12][6][2]=471735; run_count[12][6][1]=922488; run_count[12][6][0]=382470; 
	run_count[12][5][9]=49; run_count[12][5][8]=298; run_count[12][5][7]=2668; run_count[12][5][6]=47124; run_count[12][5][5]=139111; run_count[12][5][4]=60252; run_count[12][5][3]=92565; run_count[12][5][2]=471735; run_count[12][5][1]=922488; run_count[12][5][0]=382470; 
	run_count[12][4][9]=49; run_count[12][4][8]=254; run_count[12][4][7]=2668; run_count[12][4][6]=47124; run_count[12][4][5]=139155; run_count[12][4][4]=52821; run_count[12][4][3]=92565; run_count[12][4][2]=471735; run_count[12][4][1]=925344; run_count[12][4][0]=387045; 
	run_count[12][3][9]=49; run_count[12][3][8]=1245; run_count[12][3][7]=2668; run_count[12][3][6]=47124; run_count[12][3][5]=138164; run_count[12][3][4]=87801; run_count[12][3][3]=92004; run_count[12][3][2]=469092; run_count[12][3][1]=908208; run_count[12][3][0]=372405; 
	run_count[12][2][9]=49; run_count[12][2][8]=1201; run_count[12][2][7]=2668; run_count[12][2][6]=47124; run_count[12][2][5]=138208; run_count[12][2][4]=80370; run_count[12][2][3]=92004; run_count[12][2][2]=469092; run_count[12][2][1]=911064; run_count[12][2][0]=376980; 
	run_count[12][1][9]=49; run_count[12][1][8]=1157; run_count[12][1][7]=2668; run_count[12][1][6]=47124; run_count[12][1][5]=138252; run_count[12][1][4]=72939; run_count[12][1][3]=92004; run_count[12][1][2]=469092; run_count[12][1][1]=913920; run_count[12][1][0]=381555; 
	run_count[12][0][9]=49; run_count[12][0][8]=1113; run_count[12][0][7]=2668; run_count[12][0][6]=47124; run_count[12][0][5]=138296; run_count[12][0][4]=65508; run_count[12][0][3]=92004; run_count[12][0][2]=469092; run_count[12][0][1]=916776; run_count[12][0][0]=386130; 
	run_count[11][10][9]=1084; run_count[11][10][8]=1068; run_count[11][10][7]=2668; run_count[11][10][6]=47124; run_count[11][10][5]=137306; run_count[11][10][4]=99573; run_count[11][10][3]=91443; run_count[11][10][2]=466449; run_count[11][10][1]=899640; run_count[11][10][0]=372405; 
	run_count[11][9][9]=1084; run_count[11][9][8]=1112; run_count[11][9][7]=2668; run_count[11][9][6]=47124; run_count[11][9][5]=137262; run_count[11][9][4]=107004; run_count[11][9][3]=91443; run_count[11][9][2]=466449; run_count[11][9][1]=896784; run_count[11][9][0]=367830; 
	run_count[11][8][9]=1084; run_count[11][8][8]=1156; run_count[11][8][7]=2668; run_count[11][8][6]=47124; run_count[11][8][5]=137218; run_count[11][8][4]=114435; run_count[11][8][3]=91443; run_count[11][8][2]=466449; run_count[11][8][1]=893928; run_count[11][8][0]=363255; 
	run_count[11][7][9]=49; run_count[11][7][8]=1200; run_count[11][7][7]=2668; run_count[11][7][6]=47124; run_count[11][7][5]=138209; run_count[11][7][4]=79455; run_count[11][7][3]=92004; run_count[11][7][2]=469092; run_count[11][7][1]=911064; run_count[11][7][0]=377895; 
	run_count[11][6][9]=49; run_count[11][6][8]=254; run_count[11][6][7]=2668; run_count[11][6][6]=47124; run_count[11][6][5]=139155; run_count[11][6][4]=52821; run_count[11][6][3]=92565; run_count[11][6][2]=471735; run_count[11][6][1]=925344; run_count[11][6][0]=387045; 
	run_count[11][5][9]=49; run_count[11][5][8]=298; run_count[11][5][7]=2668; run_count[11][5][6]=47124; run_count[11][5][5]=139111; run_count[11][5][4]=60252; run_count[11][5][3]=92565; run_count[11][5][2]=471735; run_count[11][5][1]=922488; run_count[11][5][0]=382470; 
	run_count[11][4][9]=49; run_count[11][4][8]=298; run_count[11][4][7]=2668; run_count[11][4][6]=47124; run_count[11][4][5]=139111; run_count[11][4][4]=60252; run_count[11][4][3]=92565; run_count[11][4][2]=471735; run_count[11][4][1]=922488; run_count[11][4][0]=382470; 
	run_count[11][3][9]=49; run_count[11][3][8]=299; run_count[11][3][7]=2668; run_count[11][3][6]=47124; run_count[11][3][5]=139110; run_count[11][3][4]=61167; run_count[11][3][3]=92565; run_count[11][3][2]=471735; run_count[11][3][1]=922488; run_count[11][3][0]=381555; 
	run_count[11][2][9]=49; run_count[11][2][8]=255; run_count[11][2][7]=2668; run_count[11][2][6]=47124; run_count[11][2][5]=139154; run_count[11][2][4]=53736; run_count[11][2][3]=92565; run_count[11][2][2]=471735; run_count[11][2][1]=925344; run_count[11][2][0]=386130; 
	run_count[11][1][9]=49; run_count[11][1][8]=211; run_count[11][1][7]=2668; run_count[11][1][6]=47124; run_count[11][1][5]=139198; run_count[11][1][4]=46305; run_count[11][1][3]=92565; run_count[11][1][2]=471735; run_count[11][1][1]=928200; run_count[11][1][0]=390705; 
	run_count[11][0][9]=49; run_count[11][0][8]=167; run_count[11][0][7]=2668; run_count[11][0][6]=47124; run_count[11][0][5]=139242; run_count[11][0][4]=38874; run_count[11][0][3]=92565; run_count[11][0][2]=471735; run_count[11][0][1]=931056; run_count[11][0][0]=395280; 
	run_count[10][9][9]=1084; run_count[10][9][8]=2102; run_count[10][9][7]=2668; run_count[10][9][6]=47124; run_count[10][9][5]=136272; run_count[10][9][4]=141069; run_count[10][9][3]=90882; run_count[10][9][2]=463806; run_count[10][9][1]=879648; run_count[10][9][0]=354105; 
	run_count[10][8][9]=1084; run_count[10][8][8]=2146; run_count[10][8][7]=2668; run_count[10][8][6]=47124; run_count[10][8][5]=136228; run_count[10][8][4]=148500; run_count[10][8][3]=90882; run_count[10][8][2]=463806; run_count[10][8][1]=876792; run_count[10][8][0]=349530; 
	run_count[10][7][9]=49; run_count[10][7][8]=2190; run_count[10][7][7]=2668; run_count[10][7][6]=47124; run_count[10][7][5]=137219; run_count[10][7][4]=113520; run_count[10][7][3]=91443; run_count[10][7][2]=466449; run_count[10][7][1]=893928; run_count[10][7][0]=364170; 
	run_count[10][6][9]=49; run_count[10][6][8]=1244; run_count[10][6][7]=2668; run_count[10][6][6]=47124; run_count[10][6][5]=138165; run_count[10][6][4]=86886; run_count[10][6][3]=92004; run_count[10][6][2]=469092; run_count[10][6][1]=908208; run_count[10][6][0]=373320; 
	run_count[10][5][9]=49; run_count[10][5][8]=298; run_count[10][5][7]=2668; run_count[10][5][6]=47124; run_count[10][5][5]=139111; run_count[10][5][4]=60252; run_count[10][5][3]=92565; run_count[10][5][2]=471735; run_count[10][5][1]=922488; run_count[10][5][0]=382470; 
	run_count[10][4][9]=49; run_count[10][4][8]=342; run_count[10][4][7]=2668; run_count[10][4][6]=47124; run_count[10][4][5]=139067; run_count[10][4][4]=67683; run_count[10][4][3]=92565; run_count[10][4][2]=471735; run_count[10][4][1]=919632; run_count[10][4][0]=377895; 
	run_count[10][3][9]=49; run_count[10][3][8]=343; run_count[10][3][7]=2668; run_count[10][3][6]=47124; run_count[10][3][5]=139066; run_count[10][3][4]=68598; run_count[10][3][3]=92565; run_count[10][3][2]=471735; run_count[10][3][1]=919632; run_count[10][3][0]=376980; 
	run_count[10][2][9]=49; run_count[10][2][8]=299; run_count[10][2][7]=2668; run_count[10][2][6]=47124; run_count[10][2][5]=139110; run_count[10][2][4]=61167; run_count[10][2][3]=92565; run_count[10][2][2]=471735; run_count[10][2][1]=922488; run_count[10][2][0]=381555; 
	run_count[10][1][9]=49; run_count[10][1][8]=255; run_count[10][1][7]=2668; run_count[10][1][6]=47124; run_count[10][1][5]=139154; run_count[10][1][4]=53736; run_count[10][1][3]=92565; run_count[10][1][2]=471735; run_count[10][1][1]=925344; run_count[10][1][0]=386130; 
	run_count[10][0][9]=49; run_count[10][0][8]=211; run_count[10][0][7]=2668; run_count[10][0][6]=47124; run_count[10][0][5]=139198; run_count[10][0][4]=46305; run_count[10][0][3]=92565; run_count[10][0][2]=471735; run_count[10][0][1]=928200; run_count[10][0][0]=390705; 
	run_count[9][8][9]=1084; run_count[9][8][8]=3136; run_count[9][8][7]=2668; run_count[9][8][6]=47124; run_count[9][8][5]=135238; run_count[9][8][4]=182565; run_count[9][8][3]=90321; run_count[9][8][2]=461163; run_count[9][8][1]=859656; run_count[9][8][0]=335805; 
	run_count[9][7][9]=49; run_count[9][7][8]=3180; run_count[9][7][7]=2668; run_count[9][7][6]=47124; run_count[9][7][5]=136229; run_count[9][7][4]=147585; run_count[9][7][3]=90882; run_count[9][7][2]=463806; run_count[9][7][1]=876792; run_count[9][7][0]=350445; 
	run_count[9][6][9]=49; run_count[9][6][8]=2234; run_count[9][6][7]=2668; run_count[9][6][6]=47124; run_count[9][6][5]=137175; run_count[9][6][4]=120951; run_count[9][6][3]=91443; run_count[9][6][2]=466449; run_count[9][6][1]=891072; run_count[9][6][0]=359595; 
	run_count[9][5][9]=49; run_count[9][5][8]=1288; run_count[9][5][7]=2668; run_count[9][5][6]=47124; run_count[9][5][5]=138121; run_count[9][5][4]=94317; run_count[9][5][3]=92004; run_count[9][5][2]=469092; run_count[9][5][1]=905352; run_count[9][5][0]=368745; 
	run_count[9][4][9]=49; run_count[9][4][8]=342; run_count[9][4][7]=2668; run_count[9][4][6]=47124; run_count[9][4][5]=139067; run_count[9][4][4]=67683; run_count[9][4][3]=92565; run_count[9][4][2]=471735; run_count[9][4][1]=919632; run_count[9][4][0]=377895; 
	run_count[9][3][9]=49; run_count[9][3][8]=387; run_count[9][3][7]=2668; run_count[9][3][6]=47124; run_count[9][3][5]=139022; run_count[9][3][4]=76029; run_count[9][3][3]=92565; run_count[9][3][2]=471735; run_count[9][3][1]=916776; run_count[9][3][0]=372405; 
	run_count[9][2][9]=49; run_count[9][2][8]=343; run_count[9][2][7]=2668; run_count[9][2][6]=47124; run_count[9][2][5]=139066; run_count[9][2][4]=68598; run_count[9][2][3]=92565; run_count[9][2][2]=471735; run_count[9][2][1]=919632; run_count[9][2][0]=376980; 
	run_count[9][1][9]=49; run_count[9][1][8]=299; run_count[9][1][7]=2668; run_count[9][1][6]=47124; run_count[9][1][5]=139110; run_count[9][1][4]=61167; run_count[9][1][3]=92565; run_count[9][1][2]=471735; run_count[9][1][1]=922488; run_count[9][1][0]=381555; 
	run_count[9][0][9]=49; run_count[9][0][8]=255; run_count[9][0][7]=2668; run_count[9][0][6]=47124; run_count[9][0][5]=139154; run_count[9][0][4]=53736; run_count[9][0][3]=92565; run_count[9][0][2]=471735; run_count[9][0][1]=925344; run_count[9][0][0]=386130; 
	run_count[8][7][9]=49; run_count[8][7][8]=4170; run_count[8][7][7]=2668; run_count[8][7][6]=47124; run_count[8][7][5]=135239; run_count[8][7][4]=181650; run_count[8][7][3]=90321; run_count[8][7][2]=461163; run_count[8][7][1]=859656; run_count[8][7][0]=336720; 
	run_count[8][6][9]=49; run_count[8][6][8]=3224; run_count[8][6][7]=2668; run_count[8][6][6]=47124; run_count[8][6][5]=136185; run_count[8][6][4]=155016; run_count[8][6][3]=90882; run_count[8][6][2]=463806; run_count[8][6][1]=873936; run_count[8][6][0]=345870; 
	run_count[8][5][9]=49; run_count[8][5][8]=2278; run_count[8][5][7]=2668; run_count[8][5][6]=47124; run_count[8][5][5]=137131; run_count[8][5][4]=128382; run_count[8][5][3]=91443; run_count[8][5][2]=466449; run_count[8][5][1]=888216; run_count[8][5][0]=355020; 
	run_count[8][4][9]=49; run_count[8][4][8]=1332; run_count[8][4][7]=2668; run_count[8][4][6]=47124; run_count[8][4][5]=138077; run_count[8][4][4]=101748; run_count[8][4][3]=92004; run_count[8][4][2]=469092; run_count[8][4][1]=902496; run_count[8][4][0]=364170; 
	run_count[8][3][9]=49; run_count[8][3][8]=387; run_count[8][3][7]=2668; run_count[8][3][6]=47124; run_count[8][3][5]=139022; run_count[8][3][4]=76029; run_count[8][3][3]=92565; run_count[8][3][2]=471735; run_count[8][3][1]=916776; run_count[8][3][0]=372405; 
	run_count[8][2][9]=49; run_count[8][2][8]=387; run_count[8][2][7]=2668; run_count[8][2][6]=47124; run_count[8][2][5]=139022; run_count[8][2][4]=76029; run_count[8][2][3]=92565; run_count[8][2][2]=471735; run_count[8][2][1]=916776; run_count[8][2][0]=372405; 
	run_count[8][1][9]=49; run_count[8][1][8]=343; run_count[8][1][7]=2668; run_count[8][1][6]=47124; run_count[8][1][5]=139066; run_count[8][1][4]=68598; run_count[8][1][3]=92565; run_count[8][1][2]=471735; run_count[8][1][1]=919632; run_count[8][1][0]=376980; 
	run_count[8][0][9]=49; run_count[8][0][8]=299; run_count[8][0][7]=2668; run_count[8][0][6]=47124; run_count[8][0][5]=139110; run_count[8][0][4]=61167; run_count[8][0][3]=92565; run_count[8][0][2]=471735; run_count[8][0][1]=922488; run_count[8][0][0]=381555; 
	run_count[7][6][9]=4; run_count[7][6][8]=4214; run_count[7][6][7]=2668; run_count[7][6][6]=47124; run_count[7][6][5]=135240; run_count[7][6][4]=180735; run_count[7][6][3]=90321; run_count[7][6][2]=461163; run_count[7][6][1]=859656; run_count[7][6][0]=337635; 
	run_count[7][5][9]=4; run_count[7][5][8]=3268; run_count[7][5][7]=2668; run_count[7][5][6]=47124; run_count[7][5][5]=136186; run_count[7][5][4]=154101; run_count[7][5][3]=90882; run_count[7][5][2]=463806; run_count[7][5][1]=873936; run_count[7][5][0]=346785; 
	run_count[7][4][9]=4; run_count[7][4][8]=2322; run_count[7][4][7]=2668; run_count[7][4][6]=47124; run_count[7][4][5]=137132; run_count[7][4][4]=127467; run_count[7][4][3]=91443; run_count[7][4][2]=466449; run_count[7][4][1]=888216; run_count[7][4][0]=355935; 
	run_count[7][3][9]=4; run_count[7][3][8]=1377; run_count[7][3][7]=2668; run_count[7][3][6]=47124; run_count[7][3][5]=138077; run_count[7][3][4]=101748; run_count[7][3][3]=92004; run_count[7][3][2]=469092; run_count[7][3][1]=902496; run_count[7][3][0]=364170; 
	run_count[7][2][9]=4; run_count[7][2][8]=387; run_count[7][2][7]=2668; run_count[7][2][6]=47124; run_count[7][2][5]=139067; run_count[7][2][4]=67683; run_count[7][2][3]=92565; run_count[7][2][2]=471735; run_count[7][2][1]=919632; run_count[7][2][0]=377895; 
	run_count[7][1][9]=4; run_count[7][1][8]=387; run_count[7][1][7]=2668; run_count[7][1][6]=47124; run_count[7][1][5]=139067; run_count[7][1][4]=67683; run_count[7][1][3]=92565; run_count[7][1][2]=471735; run_count[7][1][1]=919632; run_count[7][1][0]=377895; 
	run_count[7][0][9]=4; run_count[7][0][8]=343; run_count[7][0][7]=2668; run_count[7][0][6]=47124; run_count[7][0][5]=139111; run_count[7][0][4]=60252; run_count[7][0][3]=92565; run_count[7][0][2]=471735; run_count[7][0][1]=922488; run_count[7][0][0]=382470; 
	run_count[6][5][9]=4; run_count[6][5][8]=4214; run_count[6][5][7]=2668; run_count[6][5][6]=47124; run_count[6][5][5]=135240; run_count[6][5][4]=180735; run_count[6][5][3]=90321; run_count[6][5][2]=461163; run_count[6][5][1]=859656; run_count[6][5][0]=337635; 
	run_count[6][4][9]=4; run_count[6][4][8]=3268; run_count[6][4][7]=2668; run_count[6][4][6]=47124; run_count[6][4][5]=136186; run_count[6][4][4]=154101; run_count[6][4][3]=90882; run_count[6][4][2]=463806; run_count[6][4][1]=873936; run_count[6][4][0]=346785; 
	run_count[6][3][9]=4; run_count[6][3][8]=2323; run_count[6][3][7]=2668; run_count[6][3][6]=47124; run_count[6][3][5]=137131; run_count[6][3][4]=128382; run_count[6][3][3]=91443; run_count[6][3][2]=466449; run_count[6][3][1]=888216; run_count[6][3][0]=355020; 
	run_count[6][2][9]=4; run_count[6][2][8]=1333; run_count[6][2][7]=2668; run_count[6][2][6]=47124; run_count[6][2][5]=138121; run_count[6][2][4]=94317; run_count[6][2][3]=92004; run_count[6][2][2]=469092; run_count[6][2][1]=905352; run_count[6][2][0]=368745; 
	run_count[6][1][9]=4; run_count[6][1][8]=343; run_count[6][1][7]=2668; run_count[6][1][6]=47124; run_count[6][1][5]=139111; run_count[6][1][4]=60252; run_count[6][1][3]=92565; run_count[6][1][2]=471735; run_count[6][1][1]=922488; run_count[6][1][0]=382470; 
	run_count[6][0][9]=4; run_count[6][0][8]=343; run_count[6][0][7]=2668; run_count[6][0][6]=47124; run_count[6][0][5]=139111; run_count[6][0][4]=60252; run_count[6][0][3]=92565; run_count[6][0][2]=471735; run_count[6][0][1]=922488; run_count[6][0][0]=382470; 
	run_count[5][4][9]=4; run_count[5][4][8]=4214; run_count[5][4][7]=2668; run_count[5][4][6]=47124; run_count[5][4][5]=135240; run_count[5][4][4]=180735; run_count[5][4][3]=90321; run_count[5][4][2]=461163; run_count[5][4][1]=859656; run_count[5][4][0]=337635; 
	run_count[5][3][9]=4; run_count[5][3][8]=3269; run_count[5][3][7]=2668; run_count[5][3][6]=47124; run_count[5][3][5]=136185; run_count[5][3][4]=155016; run_count[5][3][3]=90882; run_count[5][3][2]=463806; run_count[5][3][1]=873936; run_count[5][3][0]=345870; 
	run_count[5][2][9]=4; run_count[5][2][8]=2279; run_count[5][2][7]=2668; run_count[5][2][6]=47124; run_count[5][2][5]=137175; run_count[5][2][4]=120951; run_count[5][2][3]=91443; run_count[5][2][2]=466449; run_count[5][2][1]=891072; run_count[5][2][0]=359595; 
	run_count[5][1][9]=4; run_count[5][1][8]=1289; run_count[5][1][7]=2668; run_count[5][1][6]=47124; run_count[5][1][5]=138165; run_count[5][1][4]=86886; run_count[5][1][3]=92004; run_count[5][1][2]=469092; run_count[5][1][1]=908208; run_count[5][1][0]=373320; 
	run_count[5][0][9]=4; run_count[5][0][8]=299; run_count[5][0][7]=2668; run_count[5][0][6]=47124; run_count[5][0][5]=139155; run_count[5][0][4]=52821; run_count[5][0][3]=92565; run_count[5][0][2]=471735; run_count[5][0][1]=925344; run_count[5][0][0]=387045; 
	run_count[4][3][9]=4; run_count[4][3][8]=4215; run_count[4][3][7]=2668; run_count[4][3][6]=47124; run_count[4][3][5]=135239; run_count[4][3][4]=181650; run_count[4][3][3]=90321; run_count[4][3][2]=461163; run_count[4][3][1]=859656; run_count[4][3][0]=336720; 
	run_count[4][2][9]=4; run_count[4][2][8]=3225; run_count[4][2][7]=2668; run_count[4][2][6]=47124; run_count[4][2][5]=136229; run_count[4][2][4]=147585; run_count[4][2][3]=90882; run_count[4][2][2]=463806; run_count[4][2][1]=876792; run_count[4][2][0]=350445; 
	run_count[4][1][9]=4; run_count[4][1][8]=2235; run_count[4][1][7]=2668; run_count[4][1][6]=47124; run_count[4][1][5]=137219; run_count[4][1][4]=113520; run_count[4][1][3]=91443; run_count[4][1][2]=466449; run_count[4][1][1]=893928; run_count[4][1][0]=364170; 
	run_count[4][0][9]=4; run_count[4][0][8]=1245; run_count[4][0][7]=2668; run_count[4][0][6]=47124; run_count[4][0][5]=138209; run_count[4][0][4]=79455; run_count[4][0][3]=92004; run_count[4][0][2]=469092; run_count[4][0][1]=911064; run_count[4][0][0]=377895; 
	run_count[3][2][9]=4; run_count[3][2][8]=4216; run_count[3][2][7]=2668; run_count[3][2][6]=47124; run_count[3][2][5]=135238; run_count[3][2][4]=182565; run_count[3][2][3]=90321; run_count[3][2][2]=461163; run_count[3][2][1]=859656; run_count[3][2][0]=335805; 
	run_count[3][1][9]=4; run_count[3][1][8]=3226; run_count[3][1][7]=2668; run_count[3][1][6]=47124; run_count[3][1][5]=136228; run_count[3][1][4]=148500; run_count[3][1][3]=90882; run_count[3][1][2]=463806; run_count[3][1][1]=876792; run_count[3][1][0]=349530; 
	run_count[3][0][9]=4; run_count[3][0][8]=2236; run_count[3][0][7]=2668; run_count[3][0][6]=47124; run_count[3][0][5]=137218; run_count[3][0][4]=114435; run_count[3][0][3]=91443; run_count[3][0][2]=466449; run_count[3][0][1]=893928; run_count[3][0][0]=363255; 
	run_count[2][1][9]=4; run_count[2][1][8]=3182; run_count[2][1][7]=2668; run_count[2][1][6]=47124; run_count[2][1][5]=136272; run_count[2][1][4]=141069; run_count[2][1][3]=90882; run_count[2][1][2]=463806; run_count[2][1][1]=879648; run_count[2][1][0]=354105; 
	run_count[2][0][9]=4; run_count[2][0][8]=2192; run_count[2][0][7]=2668; run_count[2][0][6]=47124; run_count[2][0][5]=137262; run_count[2][0][4]=107004; run_count[2][0][3]=91443; run_count[2][0][2]=466449; run_count[2][0][1]=896784; run_count[2][0][0]=367830; 
	run_count[1][0][9]=4; run_count[1][0][8]=2148; run_count[1][0][7]=2668; run_count[1][0][6]=47124; run_count[1][0][5]=137306; run_count[1][0][4]=99573; run_count[1][0][3]=91443; run_count[1][0][2]=466449; run_count[1][0][1]=899640; run_count[1][0][0]=372405; 
	run_max_pokval[12][11]=2148457658; run_max_pokval[12][10]=2148457658; run_max_pokval[12][9]=2148457658; run_max_pokval[12][8]=2148457658; run_max_pokval[12][7]=2148457658; run_max_pokval[12][6]=2148457658; run_max_pokval[12][5]=2148457658; run_max_pokval[12][4]=2148457658; run_max_pokval[12][3]=2148457658; run_max_pokval[12][2]=2148457658; run_max_pokval[12][1]=2148457658; run_max_pokval[12][0]=2148457658; run_max_pokval[11][10]=2148457658; run_max_pokval[11][9]=2148457658; run_max_pokval[11][8]=2148457658; run_max_pokval[11][7]=2148457658; run_max_pokval[11][6]=2148457658; run_max_pokval[11][5]=2148457658; run_max_pokval[11][4]=2148457658; run_max_pokval[11][3]=2148457658; run_max_pokval[11][2]=2148457658; run_max_pokval[11][1]=2148457658; run_max_pokval[11][0]=2148457658; run_max_pokval[10][9]=2148457658; run_max_pokval[10][8]=2148457658; run_max_pokval[10][7]=2148457658; run_max_pokval[10][6]=2148457658; run_max_pokval[10][5]=2148457658; run_max_pokval[10][4]=2148457658; run_max_pokval[10][3]=2148457658; run_max_pokval[10][2]=2148457658; run_max_pokval[10][1]=2148457658; run_max_pokval[10][0]=2148457658; run_max_pokval[9][8]=2148457658; run_max_pokval[9][7]=2148457658; run_max_pokval[9][6]=2148457658; run_max_pokval[9][5]=2148457658; run_max_pokval[9][4]=2148457658; run_max_pokval[9][3]=2148457658; run_max_pokval[9][2]=2148457658; run_max_pokval[9][1]=2148457658; run_max_pokval[9][0]=2148457658; run_max_pokval[8][7]=2148457658; run_max_pokval[8][6]=2148457658; run_max_pokval[8][5]=2148457658; run_max_pokval[8][4]=2148457658; run_max_pokval[8][3]=2148457658; run_max_pokval[8][2]=2148457658; run_max_pokval[8][1]=2148457658; run_max_pokval[8][0]=2148457658; run_max_pokval[7][6]=2148457658; run_max_pokval[7][5]=2148457658; run_max_pokval[7][4]=2148457658; run_max_pokval[7][3]=2148457658; run_max_pokval[7][2]=2148457658; run_max_pokval[7][1]=2148457658; run_max_pokval[7][0]=2148457658; run_max_pokval[6][5]=2148457658; run_max_pokval[6][4]=2148457658; run_max_pokval[6][3]=2148457658; run_max_pokval[6][2]=2148457658; run_max_pokval[6][1]=2148457658; run_max_pokval[6][0]=2148457658; run_max_pokval[5][4]=2148457658; run_max_pokval[5][3]=2148457658; run_max_pokval[5][2]=2148457658; run_max_pokval[5][1]=2148457658; run_max_pokval[5][0]=2148457658; run_max_pokval[4][3]=2148457658; run_max_pokval[4][2]=2148457658; run_max_pokval[4][1]=2148457658; run_max_pokval[4][0]=2148457658; run_max_pokval[3][2]=2148457658; run_max_pokval[3][1]=2148457658; run_max_pokval[3][0]=2148457658; run_max_pokval[2][1]=2148457658; run_max_pokval[2][0]=2148457658; run_max_pokval[1][0]=2148457658; 
	run_max_pokval_n[12][11]=1084; run_max_pokval_n[12][10]=1084; run_max_pokval_n[12][9]=1084; run_max_pokval_n[12][8]=1084; run_max_pokval_n[12][7]=49; run_max_pokval_n[12][6]=49; run_max_pokval_n[12][5]=49; run_max_pokval_n[12][4]=49; run_max_pokval_n[12][3]=49; run_max_pokval_n[12][2]=49; run_max_pokval_n[12][1]=49; run_max_pokval_n[12][0]=49; run_max_pokval_n[11][10]=1084; run_max_pokval_n[11][9]=1084; run_max_pokval_n[11][8]=1084; run_max_pokval_n[11][7]=49; run_max_pokval_n[11][6]=49; run_max_pokval_n[11][5]=49; run_max_pokval_n[11][4]=49; run_max_pokval_n[11][3]=49; run_max_pokval_n[11][2]=49; run_max_pokval_n[11][1]=49; run_max_pokval_n[11][0]=49; run_max_pokval_n[10][9]=1084; run_max_pokval_n[10][8]=1084; run_max_pokval_n[10][7]=49; run_max_pokval_n[10][6]=49; run_max_pokval_n[10][5]=49; run_max_pokval_n[10][4]=49; run_max_pokval_n[10][3]=49; run_max_pokval_n[10][2]=49; run_max_pokval_n[10][1]=49; run_max_pokval_n[10][0]=49; run_max_pokval_n[9][8]=1084; run_max_pokval_n[9][7]=49; run_max_pokval_n[9][6]=49; run_max_pokval_n[9][5]=49; run_max_pokval_n[9][4]=49; run_max_pokval_n[9][3]=49; run_max_pokval_n[9][2]=49; run_max_pokval_n[9][1]=49; run_max_pokval_n[9][0]=49; run_max_pokval_n[8][7]=49; run_max_pokval_n[8][6]=49; run_max_pokval_n[8][5]=49; run_max_pokval_n[8][4]=49; run_max_pokval_n[8][3]=49; run_max_pokval_n[8][2]=49; run_max_pokval_n[8][1]=49; run_max_pokval_n[8][0]=49; run_max_pokval_n[7][6]=4; run_max_pokval_n[7][5]=4; run_max_pokval_n[7][4]=4; run_max_pokval_n[7][3]=4; run_max_pokval_n[7][2]=4; run_max_pokval_n[7][1]=4; run_max_pokval_n[7][0]=4; run_max_pokval_n[6][5]=4; run_max_pokval_n[6][4]=4; run_max_pokval_n[6][3]=4; run_max_pokval_n[6][2]=4; run_max_pokval_n[6][1]=4; run_max_pokval_n[6][0]=4; run_max_pokval_n[5][4]=4; run_max_pokval_n[5][3]=4; run_max_pokval_n[5][2]=4; run_max_pokval_n[5][1]=4; run_max_pokval_n[5][0]=4; run_max_pokval_n[4][3]=4; run_max_pokval_n[4][2]=4; run_max_pokval_n[4][1]=4; run_max_pokval_n[4][0]=4; run_max_pokval_n[3][2]=4; run_max_pokval_n[3][1]=4; run_max_pokval_n[3][0]=4; run_max_pokval_n[2][1]=4; run_max_pokval_n[2][0]=4; run_max_pokval_n[1][0]=4; 

	//OFF SUIT HANDS
	//==============
	run_count[11][12][9]=94; run_count[11][12][8]=122; run_count[11][12][7]=2668; run_count[11][12][6]=47124; run_count[11][12][5]=41562; run_count[11][12][4]=69954; run_count[11][12][3]=93808; run_count[11][12][2]=480080; run_count[11][12][1]=965568; run_count[11][12][0]=417780; 
	run_count[10][12][9]=94; run_count[10][12][8]=166; run_count[10][12][7]=2668; run_count[10][12][6]=47124; run_count[10][12][5]=41518; run_count[10][12][4]=77912; run_count[10][12][3]=93808; run_count[10][12][2]=480080; run_count[10][12][1]=962560; run_count[10][12][0]=412830; 
	run_count[9][12][9]=94; run_count[9][12][8]=210; run_count[9][12][7]=2668; run_count[9][12][6]=47124; run_count[9][12][5]=41474; run_count[9][12][4]=85870; run_count[9][12][3]=93808; run_count[9][12][2]=480080; run_count[9][12][1]=959552; run_count[9][12][0]=407880; 
	run_count[8][12][9]=94; run_count[8][12][8]=254; run_count[8][12][7]=2668; run_count[8][12][6]=47124; run_count[8][12][5]=41430; run_count[8][12][4]=93828; run_count[8][12][3]=93808; run_count[8][12][2]=480080; run_count[8][12][1]=956544; run_count[8][12][0]=402930; 
	run_count[7][12][9]=49; run_count[7][12][8]=298; run_count[7][12][7]=2668; run_count[7][12][6]=47124; run_count[7][12][5]=41431; run_count[7][12][4]=56658; run_count[7][12][3]=94380; run_count[7][12][2]=482790; run_count[7][12][1]=974592; run_count[7][12][0]=418770; 
	run_count[6][12][9]=49; run_count[6][12][8]=298; run_count[6][12][7]=2668; run_count[6][12][6]=47124; run_count[6][12][5]=41431; run_count[6][12][4]=64616; run_count[6][12][3]=94380; run_count[6][12][2]=482790; run_count[6][12][1]=971584; run_count[6][12][0]=413820; 
	run_count[5][12][9]=49; run_count[5][12][8]=298; run_count[5][12][7]=2668; run_count[5][12][6]=47124; run_count[5][12][5]=41431; run_count[5][12][4]=64616; run_count[5][12][3]=94380; run_count[5][12][2]=482790; run_count[5][12][1]=971584; run_count[5][12][0]=413820; 
	run_count[4][12][9]=49; run_count[4][12][8]=298; run_count[4][12][7]=2668; run_count[4][12][6]=47124; run_count[4][12][5]=41431; run_count[4][12][4]=56658; run_count[4][12][3]=94380; run_count[4][12][2]=482790; run_count[4][12][1]=974592; run_count[4][12][0]=418770; 
	run_count[3][12][9]=49; run_count[3][12][8]=299; run_count[3][12][7]=2668; run_count[3][12][6]=47124; run_count[3][12][5]=41430; run_count[3][12][4]=93828; run_count[3][12][3]=93808; run_count[3][12][2]=480080; run_count[3][12][1]=956544; run_count[3][12][0]=402930; 
	run_count[2][12][9]=49; run_count[2][12][8]=255; run_count[2][12][7]=2668; run_count[2][12][6]=47124; run_count[2][12][5]=41474; run_count[2][12][4]=85870; run_count[2][12][3]=93808; run_count[2][12][2]=480080; run_count[2][12][1]=959552; run_count[2][12][0]=407880; 
	run_count[1][12][9]=49; run_count[1][12][8]=211; run_count[1][12][7]=2668; run_count[1][12][6]=47124; run_count[1][12][5]=41518; run_count[1][12][4]=77912; run_count[1][12][3]=93808; run_count[1][12][2]=480080; run_count[1][12][1]=962560; run_count[1][12][0]=412830; 
	run_count[0][12][9]=49; run_count[0][12][8]=167; run_count[0][12][7]=2668; run_count[0][12][6]=47124; run_count[0][12][5]=41562; run_count[0][12][4]=69954; run_count[0][12][3]=93808; run_count[0][12][2]=480080; run_count[0][12][1]=965568; run_count[0][12][0]=417780; 
	run_count[10][11][9]=94; run_count[10][11][8]=166; run_count[10][11][7]=2668; run_count[10][11][6]=47124; run_count[10][11][5]=41518; run_count[10][11][4]=106134; run_count[10][11][3]=93236; run_count[10][11][2]=477370; run_count[10][11][1]=947520; run_count[10][11][0]=402930; 
	run_count[9][11][9]=94; run_count[9][11][8]=210; run_count[9][11][7]=2668; run_count[9][11][6]=47124; run_count[9][11][5]=41474; run_count[9][11][4]=114092; run_count[9][11][3]=93236; run_count[9][11][2]=477370; run_count[9][11][1]=944512; run_count[9][11][0]=397980; 
	run_count[8][11][9]=94; run_count[8][11][8]=254; run_count[8][11][7]=2668; run_count[8][11][6]=47124; run_count[8][11][5]=41430; run_count[8][11][4]=122050; run_count[8][11][3]=93236; run_count[8][11][2]=477370; run_count[8][11][1]=941504; run_count[8][11][0]=393030; 
	run_count[7][11][9]=49; run_count[7][11][8]=298; run_count[7][11][7]=2668; run_count[7][11][6]=47124; run_count[7][11][5]=41431; run_count[7][11][4]=84880; run_count[7][11][3]=93808; run_count[7][11][2]=480080; run_count[7][11][1]=959552; run_count[7][11][0]=408870; 
	run_count[6][11][9]=49; run_count[6][11][8]=298; run_count[6][11][7]=2668; run_count[6][11][6]=47124; run_count[6][11][5]=41431; run_count[6][11][4]=56658; run_count[6][11][3]=94380; run_count[6][11][2]=482790; run_count[6][11][1]=974592; run_count[6][11][0]=418770; 
	run_count[5][11][9]=49; run_count[5][11][8]=298; run_count[5][11][7]=2668; run_count[5][11][6]=47124; run_count[5][11][5]=41431; run_count[5][11][4]=64616; run_count[5][11][3]=94380; run_count[5][11][2]=482790; run_count[5][11][1]=971584; run_count[5][11][0]=413820; 
	run_count[4][11][9]=49; run_count[4][11][8]=298; run_count[4][11][7]=2668; run_count[4][11][6]=47124; run_count[4][11][5]=41431; run_count[4][11][4]=64616; run_count[4][11][3]=94380; run_count[4][11][2]=482790; run_count[4][11][1]=971584; run_count[4][11][0]=413820; 
	run_count[3][11][9]=49; run_count[3][11][8]=299; run_count[3][11][7]=2668; run_count[3][11][6]=47124; run_count[3][11][5]=41430; run_count[3][11][4]=65606; run_count[3][11][3]=94380; run_count[3][11][2]=482790; run_count[3][11][1]=971584; run_count[3][11][0]=412830; 
	run_count[2][11][9]=49; run_count[2][11][8]=255; run_count[2][11][7]=2668; run_count[2][11][6]=47124; run_count[2][11][5]=41474; run_count[2][11][4]=57648; run_count[2][11][3]=94380; run_count[2][11][2]=482790; run_count[2][11][1]=974592; run_count[2][11][0]=417780; 
	run_count[1][11][9]=49; run_count[1][11][8]=211; run_count[1][11][7]=2668; run_count[1][11][6]=47124; run_count[1][11][5]=41518; run_count[1][11][4]=49690; run_count[1][11][3]=94380; run_count[1][11][2]=482790; run_count[1][11][1]=977600; run_count[1][11][0]=422730; 
	run_count[0][11][9]=49; run_count[0][11][8]=167; run_count[0][11][7]=2668; run_count[0][11][6]=47124; run_count[0][11][5]=41562; run_count[0][11][4]=41732; run_count[0][11][3]=94380; run_count[0][11][2]=482790; run_count[0][11][1]=980608; run_count[0][11][0]=427680; 
	run_count[9][10][9]=94; run_count[9][10][8]=254; run_count[9][10][7]=2668; run_count[9][10][6]=47124; run_count[9][10][5]=41430; run_count[9][10][4]=150272; run_count[9][10][3]=92664; run_count[9][10][2]=474660; run_count[9][10][1]=926464; run_count[9][10][0]=383130; 
	run_count[8][10][9]=94; run_count[8][10][8]=298; run_count[8][10][7]=2668; run_count[8][10][6]=47124; run_count[8][10][5]=41386; run_count[8][10][4]=158230; run_count[8][10][3]=92664; run_count[8][10][2]=474660; run_count[8][10][1]=923456; run_count[8][10][0]=378180; 
	run_count[7][10][9]=49; run_count[7][10][8]=342; run_count[7][10][7]=2668; run_count[7][10][6]=47124; run_count[7][10][5]=41387; run_count[7][10][4]=121060; run_count[7][10][3]=93236; run_count[7][10][2]=477370; run_count[7][10][1]=941504; run_count[7][10][0]=394020; 
	run_count[6][10][9]=49; run_count[6][10][8]=342; run_count[6][10][7]=2668; run_count[6][10][6]=47124; run_count[6][10][5]=41387; run_count[6][10][4]=92838; run_count[6][10][3]=93808; run_count[6][10][2]=480080; run_count[6][10][1]=956544; run_count[6][10][0]=403920; 
	run_count[5][10][9]=49; run_count[5][10][8]=342; run_count[5][10][7]=2668; run_count[5][10][6]=47124; run_count[5][10][5]=41387; run_count[5][10][4]=64616; run_count[5][10][3]=94380; run_count[5][10][2]=482790; run_count[5][10][1]=971584; run_count[5][10][0]=413820; 
	run_count[4][10][9]=49; run_count[4][10][8]=342; run_count[4][10][7]=2668; run_count[4][10][6]=47124; run_count[4][10][5]=41387; run_count[4][10][4]=72574; run_count[4][10][3]=94380; run_count[4][10][2]=482790; run_count[4][10][1]=968576; run_count[4][10][0]=408870; 
	run_count[3][10][9]=49; run_count[3][10][8]=343; run_count[3][10][7]=2668; run_count[3][10][6]=47124; run_count[3][10][5]=41386; run_count[3][10][4]=73564; run_count[3][10][3]=94380; run_count[3][10][2]=482790; run_count[3][10][1]=968576; run_count[3][10][0]=407880; 
	run_count[2][10][9]=49; run_count[2][10][8]=299; run_count[2][10][7]=2668; run_count[2][10][6]=47124; run_count[2][10][5]=41430; run_count[2][10][4]=65606; run_count[2][10][3]=94380; run_count[2][10][2]=482790; run_count[2][10][1]=971584; run_count[2][10][0]=412830; 
	run_count[1][10][9]=49; run_count[1][10][8]=255; run_count[1][10][7]=2668; run_count[1][10][6]=47124; run_count[1][10][5]=41474; run_count[1][10][4]=57648; run_count[1][10][3]=94380; run_count[1][10][2]=482790; run_count[1][10][1]=974592; run_count[1][10][0]=417780; 
	run_count[0][10][9]=49; run_count[0][10][8]=211; run_count[0][10][7]=2668; run_count[0][10][6]=47124; run_count[0][10][5]=41518; run_count[0][10][4]=49690; run_count[0][10][3]=94380; run_count[0][10][2]=482790; run_count[0][10][1]=977600; run_count[0][10][0]=422730; 
	run_count[8][9][9]=94; run_count[8][9][8]=342; run_count[8][9][7]=2668; run_count[8][9][6]=47124; run_count[8][9][5]=41342; run_count[8][9][4]=194410; run_count[8][9][3]=92092; run_count[8][9][2]=471950; run_count[8][9][1]=905408; run_count[8][9][0]=363330; 
	run_count[7][9][9]=49; run_count[7][9][8]=386; run_count[7][9][7]=2668; run_count[7][9][6]=47124; run_count[7][9][5]=41343; run_count[7][9][4]=157240; run_count[7][9][3]=92664; run_count[7][9][2]=474660; run_count[7][9][1]=923456; run_count[7][9][0]=379170; 
	run_count[6][9][9]=49; run_count[6][9][8]=386; run_count[6][9][7]=2668; run_count[6][9][6]=47124; run_count[6][9][5]=41343; run_count[6][9][4]=129018; run_count[6][9][3]=93236; run_count[6][9][2]=477370; run_count[6][9][1]=938496; run_count[6][9][0]=389070; 
	run_count[5][9][9]=49; run_count[5][9][8]=386; run_count[5][9][7]=2668; run_count[5][9][6]=47124; run_count[5][9][5]=41343; run_count[5][9][4]=100796; run_count[5][9][3]=93808; run_count[5][9][2]=480080; run_count[5][9][1]=953536; run_count[5][9][0]=398970; 
	run_count[4][9][9]=49; run_count[4][9][8]=386; run_count[4][9][7]=2668; run_count[4][9][6]=47124; run_count[4][9][5]=41343; run_count[4][9][4]=72574; run_count[4][9][3]=94380; run_count[4][9][2]=482790; run_count[4][9][1]=968576; run_count[4][9][0]=408870; 
	run_count[3][9][9]=49; run_count[3][9][8]=387; run_count[3][9][7]=2668; run_count[3][9][6]=47124; run_count[3][9][5]=41342; run_count[3][9][4]=81522; run_count[3][9][3]=94380; run_count[3][9][2]=482790; run_count[3][9][1]=965568; run_count[3][9][0]=402930; 
	run_count[2][9][9]=49; run_count[2][9][8]=343; run_count[2][9][7]=2668; run_count[2][9][6]=47124; run_count[2][9][5]=41386; run_count[2][9][4]=73564; run_count[2][9][3]=94380; run_count[2][9][2]=482790; run_count[2][9][1]=968576; run_count[2][9][0]=407880; 
	run_count[1][9][9]=49; run_count[1][9][8]=299; run_count[1][9][7]=2668; run_count[1][9][6]=47124; run_count[1][9][5]=41430; run_count[1][9][4]=65606; run_count[1][9][3]=94380; run_count[1][9][2]=482790; run_count[1][9][1]=971584; run_count[1][9][0]=412830; 
	run_count[0][9][9]=49; run_count[0][9][8]=255; run_count[0][9][7]=2668; run_count[0][9][6]=47124; run_count[0][9][5]=41474; run_count[0][9][4]=57648; run_count[0][9][3]=94380; run_count[0][9][2]=482790; run_count[0][9][1]=974592; run_count[0][9][0]=417780; 
	run_count[7][8][9]=49; run_count[7][8][8]=430; run_count[7][8][7]=2668; run_count[7][8][6]=47124; run_count[7][8][5]=41299; run_count[7][8][4]=193420; run_count[7][8][3]=92092; run_count[7][8][2]=471950; run_count[7][8][1]=905408; run_count[7][8][0]=364320; 
	run_count[6][8][9]=49; run_count[6][8][8]=430; run_count[6][8][7]=2668; run_count[6][8][6]=47124; run_count[6][8][5]=41299; run_count[6][8][4]=165198; run_count[6][8][3]=92664; run_count[6][8][2]=474660; run_count[6][8][1]=920448; run_count[6][8][0]=374220; 
	run_count[5][8][9]=49; run_count[5][8][8]=430; run_count[5][8][7]=2668; run_count[5][8][6]=47124; run_count[5][8][5]=41299; run_count[5][8][4]=136976; run_count[5][8][3]=93236; run_count[5][8][2]=477370; run_count[5][8][1]=935488; run_count[5][8][0]=384120; 
	run_count[4][8][9]=49; run_count[4][8][8]=430; run_count[4][8][7]=2668; run_count[4][8][6]=47124; run_count[4][8][5]=41299; run_count[4][8][4]=108754; run_count[4][8][3]=93808; run_count[4][8][2]=480080; run_count[4][8][1]=950528; run_count[4][8][0]=394020; 
	run_count[3][8][9]=49; run_count[3][8][8]=431; run_count[3][8][7]=2668; run_count[3][8][6]=47124; run_count[3][8][5]=41298; run_count[3][8][4]=81522; run_count[3][8][3]=94380; run_count[3][8][2]=482790; run_count[3][8][1]=965568; run_count[3][8][0]=402930; 
	run_count[2][8][9]=49; run_count[2][8][8]=387; run_count[2][8][7]=2668; run_count[2][8][6]=47124; run_count[2][8][5]=41342; run_count[2][8][4]=81522; run_count[2][8][3]=94380; run_count[2][8][2]=482790; run_count[2][8][1]=965568; run_count[2][8][0]=402930; 
	run_count[1][8][9]=49; run_count[1][8][8]=343; run_count[1][8][7]=2668; run_count[1][8][6]=47124; run_count[1][8][5]=41386; run_count[1][8][4]=73564; run_count[1][8][3]=94380; run_count[1][8][2]=482790; run_count[1][8][1]=968576; run_count[1][8][0]=407880; 
	run_count[0][8][9]=49; run_count[0][8][8]=299; run_count[0][8][7]=2668; run_count[0][8][6]=47124; run_count[0][8][5]=41430; run_count[0][8][4]=65606; run_count[0][8][3]=94380; run_count[0][8][2]=482790; run_count[0][8][1]=971584; run_count[0][8][0]=412830; 
	run_count[6][7][9]=4; run_count[6][7][8]=474; run_count[6][7][7]=2668; run_count[6][7][6]=47124; run_count[6][7][5]=41300; run_count[6][7][4]=192430; run_count[6][7][3]=92092; run_count[6][7][2]=471950; run_count[6][7][1]=905408; run_count[6][7][0]=365310; 
	run_count[5][7][9]=4; run_count[5][7][8]=474; run_count[5][7][7]=2668; run_count[5][7][6]=47124; run_count[5][7][5]=41300; run_count[5][7][4]=164208; run_count[5][7][3]=92664; run_count[5][7][2]=474660; run_count[5][7][1]=920448; run_count[5][7][0]=375210; 
	run_count[4][7][9]=4; run_count[4][7][8]=474; run_count[4][7][7]=2668; run_count[4][7][6]=47124; run_count[4][7][5]=41300; run_count[4][7][4]=135986; run_count[4][7][3]=93236; run_count[4][7][2]=477370; run_count[4][7][1]=935488; run_count[4][7][0]=385110; 
	run_count[3][7][9]=4; run_count[3][7][8]=475; run_count[3][7][7]=2668; run_count[3][7][6]=47124; run_count[3][7][5]=41299; run_count[3][7][4]=108754; run_count[3][7][3]=93808; run_count[3][7][2]=480080; run_count[3][7][1]=950528; run_count[3][7][0]=394020; 
	run_count[2][7][9]=4; run_count[2][7][8]=431; run_count[2][7][7]=2668; run_count[2][7][6]=47124; run_count[2][7][5]=41343; run_count[2][7][4]=72574; run_count[2][7][3]=94380; run_count[2][7][2]=482790; run_count[2][7][1]=968576; run_count[2][7][0]=408870; 
	run_count[1][7][9]=4; run_count[1][7][8]=387; run_count[1][7][7]=2668; run_count[1][7][6]=47124; run_count[1][7][5]=41387; run_count[1][7][4]=72574; run_count[1][7][3]=94380; run_count[1][7][2]=482790; run_count[1][7][1]=968576; run_count[1][7][0]=408870; 
	run_count[0][7][9]=4; run_count[0][7][8]=343; run_count[0][7][7]=2668; run_count[0][7][6]=47124; run_count[0][7][5]=41431; run_count[0][7][4]=64616; run_count[0][7][3]=94380; run_count[0][7][2]=482790; run_count[0][7][1]=971584; run_count[0][7][0]=413820; 
	run_count[5][6][9]=4; run_count[5][6][8]=474; run_count[5][6][7]=2668; run_count[5][6][6]=47124; run_count[5][6][5]=41300; run_count[5][6][4]=192430; run_count[5][6][3]=92092; run_count[5][6][2]=471950; run_count[5][6][1]=905408; run_count[5][6][0]=365310; 
	run_count[4][6][9]=4; run_count[4][6][8]=474; run_count[4][6][7]=2668; run_count[4][6][6]=47124; run_count[4][6][5]=41300; run_count[4][6][4]=164208; run_count[4][6][3]=92664; run_count[4][6][2]=474660; run_count[4][6][1]=920448; run_count[4][6][0]=375210; 
	run_count[3][6][9]=4; run_count[3][6][8]=475; run_count[3][6][7]=2668; run_count[3][6][6]=47124; run_count[3][6][5]=41299; run_count[3][6][4]=136976; run_count[3][6][3]=93236; run_count[3][6][2]=477370; run_count[3][6][1]=935488; run_count[3][6][0]=384120; 
	run_count[2][6][9]=4; run_count[2][6][8]=431; run_count[2][6][7]=2668; run_count[2][6][6]=47124; run_count[2][6][5]=41343; run_count[2][6][4]=100796; run_count[2][6][3]=93808; run_count[2][6][2]=480080; run_count[2][6][1]=953536; run_count[2][6][0]=398970; 
	run_count[1][6][9]=4; run_count[1][6][8]=387; run_count[1][6][7]=2668; run_count[1][6][6]=47124; run_count[1][6][5]=41387; run_count[1][6][4]=64616; run_count[1][6][3]=94380; run_count[1][6][2]=482790; run_count[1][6][1]=971584; run_count[1][6][0]=413820; 
	run_count[0][6][9]=4; run_count[0][6][8]=343; run_count[0][6][7]=2668; run_count[0][6][6]=47124; run_count[0][6][5]=41431; run_count[0][6][4]=64616; run_count[0][6][3]=94380; run_count[0][6][2]=482790; run_count[0][6][1]=971584; run_count[0][6][0]=413820; 
	run_count[4][5][9]=4; run_count[4][5][8]=474; run_count[4][5][7]=2668; run_count[4][5][6]=47124; run_count[4][5][5]=41300; run_count[4][5][4]=192430; run_count[4][5][3]=92092; run_count[4][5][2]=471950; run_count[4][5][1]=905408; run_count[4][5][0]=365310; 
	run_count[3][5][9]=4; run_count[3][5][8]=475; run_count[3][5][7]=2668; run_count[3][5][6]=47124; run_count[3][5][5]=41299; run_count[3][5][4]=165198; run_count[3][5][3]=92664; run_count[3][5][2]=474660; run_count[3][5][1]=920448; run_count[3][5][0]=374220; 
	run_count[2][5][9]=4; run_count[2][5][8]=431; run_count[2][5][7]=2668; run_count[2][5][6]=47124; run_count[2][5][5]=41343; run_count[2][5][4]=129018; run_count[2][5][3]=93236; run_count[2][5][2]=477370; run_count[2][5][1]=938496; run_count[2][5][0]=389070; 
	run_count[1][5][9]=4; run_count[1][5][8]=387; run_count[1][5][7]=2668; run_count[1][5][6]=47124; run_count[1][5][5]=41387; run_count[1][5][4]=92838; run_count[1][5][3]=93808; run_count[1][5][2]=480080; run_count[1][5][1]=956544; run_count[1][5][0]=403920; 
	run_count[0][5][9]=4; run_count[0][5][8]=343; run_count[0][5][7]=2668; run_count[0][5][6]=47124; run_count[0][5][5]=41431; run_count[0][5][4]=56658; run_count[0][5][3]=94380; run_count[0][5][2]=482790; run_count[0][5][1]=974592; run_count[0][5][0]=418770; 
	run_count[3][4][9]=4; run_count[3][4][8]=475; run_count[3][4][7]=2668; run_count[3][4][6]=47124; run_count[3][4][5]=41299; run_count[3][4][4]=193420; run_count[3][4][3]=92092; run_count[3][4][2]=471950; run_count[3][4][1]=905408; run_count[3][4][0]=364320; 
	run_count[2][4][9]=4; run_count[2][4][8]=431; run_count[2][4][7]=2668; run_count[2][4][6]=47124; run_count[2][4][5]=41343; run_count[2][4][4]=157240; run_count[2][4][3]=92664; run_count[2][4][2]=474660; run_count[2][4][1]=923456; run_count[2][4][0]=379170; 
	run_count[1][4][9]=4; run_count[1][4][8]=387; run_count[1][4][7]=2668; run_count[1][4][6]=47124; run_count[1][4][5]=41387; run_count[1][4][4]=121060; run_count[1][4][3]=93236; run_count[1][4][2]=477370; run_count[1][4][1]=941504; run_count[1][4][0]=394020; 
	run_count[0][4][9]=4; run_count[0][4][8]=343; run_count[0][4][7]=2668; run_count[0][4][6]=47124; run_count[0][4][5]=41431; run_count[0][4][4]=84880; run_count[0][4][3]=93808; run_count[0][4][2]=480080; run_count[0][4][1]=959552; run_count[0][4][0]=408870; 
	run_count[2][3][9]=4; run_count[2][3][8]=432; run_count[2][3][7]=2668; run_count[2][3][6]=47124; run_count[2][3][5]=41342; run_count[2][3][4]=194410; run_count[2][3][3]=92092; run_count[2][3][2]=471950; run_count[2][3][1]=905408; run_count[2][3][0]=363330; 
	run_count[1][3][9]=4; run_count[1][3][8]=388; run_count[1][3][7]=2668; run_count[1][3][6]=47124; run_count[1][3][5]=41386; run_count[1][3][4]=158230; run_count[1][3][3]=92664; run_count[1][3][2]=474660; run_count[1][3][1]=923456; run_count[1][3][0]=378180; 
	run_count[0][3][9]=4; run_count[0][3][8]=344; run_count[0][3][7]=2668; run_count[0][3][6]=47124; run_count[0][3][5]=41430; run_count[0][3][4]=122050; run_count[0][3][3]=93236; run_count[0][3][2]=477370; run_count[0][3][1]=941504; run_count[0][3][0]=393030; 
	run_count[1][2][9]=4; run_count[1][2][8]=344; run_count[1][2][7]=2668; run_count[1][2][6]=47124; run_count[1][2][5]=41430; run_count[1][2][4]=150272; run_count[1][2][3]=92664; run_count[1][2][2]=474660; run_count[1][2][1]=926464; run_count[1][2][0]=383130; 
	run_count[0][2][9]=4; run_count[0][2][8]=300; run_count[0][2][7]=2668; run_count[0][2][6]=47124; run_count[0][2][5]=41474; run_count[0][2][4]=114092; run_count[0][2][3]=93236; run_count[0][2][2]=477370; run_count[0][2][1]=944512; run_count[0][2][0]=397980; 
	run_count[0][1][9]=4; run_count[0][1][8]=256; run_count[0][1][7]=2668; run_count[0][1][6]=47124; run_count[0][1][5]=41518; run_count[0][1][4]=106134; run_count[0][1][3]=93236; run_count[0][1][2]=477370; run_count[0][1][1]=947520; run_count[0][1][0]=402930; 
	run_max_pokval[11][12]=2148457658; run_max_pokval[10][12]=2148457658; run_max_pokval[9][12]=2148457658; run_max_pokval[8][12]=2148457658; run_max_pokval[7][12]=2148457658; run_max_pokval[6][12]=2148457658; run_max_pokval[5][12]=2148457658; run_max_pokval[4][12]=2148457658; run_max_pokval[3][12]=2148457658; run_max_pokval[2][12]=2148457658; run_max_pokval[1][12]=2148457658; run_max_pokval[0][12]=2148457658; run_max_pokval[10][11]=2148457658; run_max_pokval[9][11]=2148457658; run_max_pokval[8][11]=2148457658; run_max_pokval[7][11]=2148457658; run_max_pokval[6][11]=2148457658; run_max_pokval[5][11]=2148457658; run_max_pokval[4][11]=2148457658; run_max_pokval[3][11]=2148457658; run_max_pokval[2][11]=2148457658; run_max_pokval[1][11]=2148457658; run_max_pokval[0][11]=2148457658; run_max_pokval[9][10]=2148457658; run_max_pokval[8][10]=2148457658; run_max_pokval[7][10]=2148457658; run_max_pokval[6][10]=2148457658; run_max_pokval[5][10]=2148457658; run_max_pokval[4][10]=2148457658; run_max_pokval[3][10]=2148457658; run_max_pokval[2][10]=2148457658; run_max_pokval[1][10]=2148457658; run_max_pokval[0][10]=2148457658; run_max_pokval[8][9]=2148457658; run_max_pokval[7][9]=2148457658; run_max_pokval[6][9]=2148457658; run_max_pokval[5][9]=2148457658; run_max_pokval[4][9]=2148457658; run_max_pokval[3][9]=2148457658; run_max_pokval[2][9]=2148457658; run_max_pokval[1][9]=2148457658; run_max_pokval[0][9]=2148457658; run_max_pokval[7][8]=2148457658; run_max_pokval[6][8]=2148457658; run_max_pokval[5][8]=2148457658; run_max_pokval[4][8]=2148457658; run_max_pokval[3][8]=2148457658; run_max_pokval[2][8]=2148457658; run_max_pokval[1][8]=2148457658; run_max_pokval[0][8]=2148457658; run_max_pokval[6][7]=2148457658; run_max_pokval[5][7]=2148457658; run_max_pokval[4][7]=2148457658; run_max_pokval[3][7]=2148457658; run_max_pokval[2][7]=2148457658; run_max_pokval[1][7]=2148457658; run_max_pokval[0][7]=2148457658; run_max_pokval[5][6]=2148457658; run_max_pokval[4][6]=2148457658; run_max_pokval[3][6]=2148457658; run_max_pokval[2][6]=2148457658; run_max_pokval[1][6]=2148457658; run_max_pokval[0][6]=2148457658; run_max_pokval[4][5]=2148457658; run_max_pokval[3][5]=2148457658; run_max_pokval[2][5]=2148457658; run_max_pokval[1][5]=2148457658; run_max_pokval[0][5]=2148457658; run_max_pokval[3][4]=2148457658; run_max_pokval[2][4]=2148457658; run_max_pokval[1][4]=2148457658; run_max_pokval[0][4]=2148457658; run_max_pokval[2][3]=2148457658; run_max_pokval[1][3]=2148457658; run_max_pokval[0][3]=2148457658; run_max_pokval[1][2]=2148457658; run_max_pokval[0][2]=2148457658; run_max_pokval[0][1]=2148457658; 
	run_max_pokval_n[11][12]=94; run_max_pokval_n[10][12]=94; run_max_pokval_n[9][12]=94; run_max_pokval_n[8][12]=94; run_max_pokval_n[7][12]=49; run_max_pokval_n[6][12]=49; run_max_pokval_n[5][12]=49; run_max_pokval_n[4][12]=49; run_max_pokval_n[3][12]=49; run_max_pokval_n[2][12]=49; run_max_pokval_n[1][12]=49; run_max_pokval_n[0][12]=49; run_max_pokval_n[10][11]=94; run_max_pokval_n[9][11]=94; run_max_pokval_n[8][11]=94; run_max_pokval_n[7][11]=49; run_max_pokval_n[6][11]=49; run_max_pokval_n[5][11]=49; run_max_pokval_n[4][11]=49; run_max_pokval_n[3][11]=49; run_max_pokval_n[2][11]=49; run_max_pokval_n[1][11]=49; run_max_pokval_n[0][11]=49; run_max_pokval_n[9][10]=94; run_max_pokval_n[8][10]=94; run_max_pokval_n[7][10]=49; run_max_pokval_n[6][10]=49; run_max_pokval_n[5][10]=49; run_max_pokval_n[4][10]=49; run_max_pokval_n[3][10]=49; run_max_pokval_n[2][10]=49; run_max_pokval_n[1][10]=49; run_max_pokval_n[0][10]=49; run_max_pokval_n[8][9]=94; run_max_pokval_n[7][9]=49; run_max_pokval_n[6][9]=49; run_max_pokval_n[5][9]=49; run_max_pokval_n[4][9]=49; run_max_pokval_n[3][9]=49; run_max_pokval_n[2][9]=49; run_max_pokval_n[1][9]=49; run_max_pokval_n[0][9]=49; run_max_pokval_n[7][8]=49; run_max_pokval_n[6][8]=49; run_max_pokval_n[5][8]=49; run_max_pokval_n[4][8]=49; run_max_pokval_n[3][8]=49; run_max_pokval_n[2][8]=49; run_max_pokval_n[1][8]=49; run_max_pokval_n[0][8]=49; run_max_pokval_n[6][7]=4; run_max_pokval_n[5][7]=4; run_max_pokval_n[4][7]=4; run_max_pokval_n[3][7]=4; run_max_pokval_n[2][7]=4; run_max_pokval_n[1][7]=4; run_max_pokval_n[0][7]=4; run_max_pokval_n[5][6]=4; run_max_pokval_n[4][6]=4; run_max_pokval_n[3][6]=4; run_max_pokval_n[2][6]=4; run_max_pokval_n[1][6]=4; run_max_pokval_n[0][6]=4; run_max_pokval_n[4][5]=4; run_max_pokval_n[3][5]=4; run_max_pokval_n[2][5]=4; run_max_pokval_n[1][5]=4; run_max_pokval_n[0][5]=4; run_max_pokval_n[3][4]=4; run_max_pokval_n[2][4]=4; run_max_pokval_n[1][4]=4; run_max_pokval_n[0][4]=4; run_max_pokval_n[2][3]=4; run_max_pokval_n[1][3]=4; run_max_pokval_n[0][3]=4; run_max_pokval_n[1][2]=4; run_max_pokval_n[0][2]=4; run_max_pokval_n[0][1]=4; 

	//POCKET PAIRS
	//============
	ron_count[12][12][9]=41580; ron_count[12][12][8]=677292; ron_count[12][12][7]=3825360; ron_count[12][12][6]=57103200; ron_count[12][12][5]=62912388; ron_count[12][12][4]=105053550; ron_count[12][12][3]=104262312; ron_count[12][12][2]=499415868; ron_count[12][12][1]=911017800; ron_count[12][12][0]=353263050; 
	ron_count[11][11][9]=41580; ron_count[11][11][8]=677292; ron_count[11][11][7]=3825360; ron_count[11][11][6]=57103200; ron_count[11][11][5]=62912388; ron_count[11][11][4]=105053550; ron_count[11][11][3]=104262312; ron_count[11][11][2]=499415868; ron_count[11][11][1]=911017800; ron_count[11][11][0]=353263050; 
	ron_count[10][10][9]=41580; ron_count[10][10][8]=637560; ron_count[10][10][7]=3825360; ron_count[10][10][6]=57103200; ron_count[10][10][5]=62952120; ron_count[10][10][4]=99745800; ron_count[10][10][3]=104326068; ron_count[10][10][2]=499717512; ron_count[10][10][1]=913512600; ron_count[10][10][0]=355710600; 
	ron_count[9][9][9]=41580; ron_count[9][9][8]=597828; ron_count[9][9][7]=3825360; ron_count[9][9][6]=57103200; ron_count[9][9][5]=62991852; ron_count[9][9][4]=94438050; ron_count[9][9][3]=104389824; ron_count[9][9][2]=500019156; ron_count[9][9][1]=916007400; ron_count[9][9][0]=358158150; 
	ron_count[8][8][9]=41580; ron_count[8][8][8]=558096; ron_count[8][8][7]=3825360; ron_count[8][8][6]=57103200; ron_count[8][8][5]=63031584; ron_count[8][8][4]=89130300; ron_count[8][8][3]=104453580; ron_count[8][8][2]=500320800; ron_count[8][8][1]=918502200; ron_count[8][8][0]=360605700; 
	ron_count[7][7][9]=83160; ron_count[7][7][8]=518364; ron_count[7][7][7]=3825360; ron_count[7][7][6]=57103200; ron_count[7][7][5]=63029736; ron_count[7][7][4]=90525120; ron_count[7][7][3]=104453580; ron_count[7][7][2]=500320800; ron_count[7][7][1]=918086400; ron_count[7][7][0]=359626680; 
	ron_count[6][6][9]=83160; ron_count[6][6][8]=518364; ron_count[6][6][7]=3825360; ron_count[6][6][6]=57103200; ron_count[6][6][5]=63029736; ron_count[6][6][4]=90525120; ron_count[6][6][3]=104453580; ron_count[6][6][2]=500320800; ron_count[6][6][1]=918086400; ron_count[6][6][0]=359626680; 
	ron_count[5][5][9]=83160; ron_count[5][5][8]=518364; ron_count[5][5][7]=3825360; ron_count[5][5][6]=57103200; ron_count[5][5][5]=63029736; ron_count[5][5][4]=90525120; ron_count[5][5][3]=104453580; ron_count[5][5][2]=500320800; ron_count[5][5][1]=918086400; ron_count[5][5][0]=359626680; 
	ron_count[4][4][9]=83160; ron_count[4][4][8]=518364; ron_count[4][4][7]=3825360; ron_count[4][4][6]=57103200; ron_count[4][4][5]=63029736; ron_count[4][4][4]=90525120; ron_count[4][4][3]=104453580; ron_count[4][4][2]=500320800; ron_count[4][4][1]=918086400; ron_count[4][4][0]=359626680; 
	ron_count[3][3][9]=83160; ron_count[3][3][8]=516516; ron_count[3][3][7]=3825360; ron_count[3][3][6]=57103200; ron_count[3][3][5]=63031584; ron_count[3][3][4]=89130300; ron_count[3][3][3]=104453580; ron_count[3][3][2]=500320800; ron_count[3][3][1]=918502200; ron_count[3][3][0]=360605700; 
	ron_count[2][2][9]=83160; ron_count[2][2][8]=556248; ron_count[2][2][7]=3825360; ron_count[2][2][6]=57103200; ron_count[2][2][5]=62991852; ron_count[2][2][4]=94438050; ron_count[2][2][3]=104389824; ron_count[2][2][2]=500019156; ron_count[2][2][1]=916007400; ron_count[2][2][0]=358158150; 
	ron_count[1][1][9]=83160; ron_count[1][1][8]=595980; ron_count[1][1][7]=3825360; ron_count[1][1][6]=57103200; ron_count[1][1][5]=62952120; ron_count[1][1][4]=99745800; ron_count[1][1][3]=104326068; ron_count[1][1][2]=499717512; ron_count[1][1][1]=913512600; ron_count[1][1][0]=355710600; 
	ron_count[0][0][9]=83160; ron_count[0][0][8]=635712; ron_count[0][0][7]=3825360; ron_count[0][0][6]=57103200; ron_count[0][0][5]=62912388; ron_count[0][0][4]=105053550; ron_count[0][0][3]=104262312; ron_count[0][0][2]=499415868; ron_count[0][0][1]=911017800; ron_count[0][0][0]=353263050; 
	ron_max_pokval[12][12]=2148457658; ron_max_pokval[11][11]=2148457658; ron_max_pokval[10][10]=2148457658; ron_max_pokval[9][9]=2148457658; ron_max_pokval[8][8]=2148457658; ron_max_pokval[7][7]=2148457658; ron_max_pokval[6][6]=2148457658; ron_max_pokval[5][5]=2148457658; ron_max_pokval[4][4]=2148457658; ron_max_pokval[3][3]=2148457658; ron_max_pokval[2][2]=2148457658; ron_max_pokval[1][1]=2148457658; ron_max_pokval[0][0]=2148457658; 
	ron_max_pokval_n[12][12]=41580; ron_max_pokval_n[11][11]=41580; ron_max_pokval_n[10][10]=41580; ron_max_pokval_n[9][9]=41580; ron_max_pokval_n[8][8]=41580; ron_max_pokval_n[7][7]=83160; ron_max_pokval_n[6][6]=83160; ron_max_pokval_n[5][5]=83160; ron_max_pokval_n[4][4]=83160; ron_max_pokval_n[3][3]=83160; ron_max_pokval_n[2][2]=83160; ron_max_pokval_n[1][1]=83160; ron_max_pokval_n[0][0]=83160; 

	//SUITED HANDS
	//============
	ron_count[12][11][9]=62370; ron_count[12][11][8]=676368; ron_count[12][11][7]=3506580; ron_count[12][11][6]=54289620; ron_count[12][11][5]=64943802; ron_count[12][11][4]=105887082; ron_count[12][11][3]=100955778; ron_count[12][11][2]=491617224; ron_count[12][11][1]=914261796; ron_count[12][11][0]=361371780; 
	ron_count[12][10][9]=62370; ron_count[12][10][8]=656502; ron_count[12][10][7]=3506580; ron_count[12][10][6]=54289620; ron_count[12][10][5]=64963668; ron_count[12][10][4]=103305699; ron_count[12][10][3]=100992969; ron_count[12][10][2]=491783922; ron_count[12][10][1]=915511590; ron_count[12][10][0]=362499480; 
	ron_count[12][9][9]=62370; ron_count[12][9][8]=636636; ron_count[12][9][7]=3506580; ron_count[12][9][6]=54289620; ron_count[12][9][5]=64983534; ron_count[12][9][4]=100724316; ron_count[12][9][3]=101030160; ron_count[12][9][2]=491950620; ron_count[12][9][1]=916761384; ron_count[12][9][0]=363627180; 
	ron_count[12][8][9]=62370; ron_count[12][8][8]=616770; ron_count[12][8][7]=3506580; ron_count[12][8][6]=54289620; ron_count[12][8][5]=65003400; ron_count[12][8][4]=98142933; ron_count[12][8][3]=101067351; ron_count[12][8][2]=492117318; ron_count[12][8][1]=918011178; ron_count[12][8][0]=364754880; 
	ron_count[12][7][9]=62370; ron_count[12][7][8]=596904; ron_count[12][7][7]=3506580; ron_count[12][7][6]=54289620; ron_count[12][7][5]=65023266; ron_count[12][7][4]=98116158; ron_count[12][7][3]=101079132; ron_count[12][7][2]=492172821; ron_count[12][7][1]=918152739; ron_count[12][7][0]=364572810; 
	ron_count[12][6][9]=62370; ron_count[12][6][8]=597828; ron_count[12][6][7]=3506580; ron_count[12][6][6]=54289620; ron_count[12][6][5]=65022342; ron_count[12][6][4]=98272209; ron_count[12][6][3]=101079132; ron_count[12][6][2]=492172821; ron_count[12][6][1]=918092763; ron_count[12][6][0]=364476735; 
	ron_count[12][5][9]=62370; ron_count[12][5][8]=597828; ron_count[12][5][7]=3506580; ron_count[12][5][6]=54289620; ron_count[12][5][5]=65022342; ron_count[12][5][4]=98272209; ron_count[12][5][3]=101079132; ron_count[12][5][2]=492172821; ron_count[12][5][1]=918092763; ron_count[12][5][0]=364476735; 
	ron_count[12][4][9]=62370; ron_count[12][4][8]=596904; ron_count[12][4][7]=3506580; ron_count[12][4][6]=54289620; ron_count[12][4][5]=65023266; ron_count[12][4][4]=98116158; ron_count[12][4][3]=101079132; ron_count[12][4][2]=492172821; ron_count[12][4][1]=918152739; ron_count[12][4][0]=364572810; 
	ron_count[12][3][9]=62370; ron_count[12][3][8]=616770; ron_count[12][3][7]=3506580; ron_count[12][3][6]=54289620; ron_count[12][3][5]=65003400; ron_count[12][3][4]=98142933; ron_count[12][3][3]=101067351; ron_count[12][3][2]=492117318; ron_count[12][3][1]=918011178; ron_count[12][3][0]=364754880; 
	ron_count[12][2][9]=62370; ron_count[12][2][8]=636636; ron_count[12][2][7]=3506580; ron_count[12][2][6]=54289620; ron_count[12][2][5]=64983534; ron_count[12][2][4]=100724316; ron_count[12][2][3]=101030160; ron_count[12][2][2]=491950620; ron_count[12][2][1]=916761384; ron_count[12][2][0]=363627180; 
	ron_count[12][1][9]=62370; ron_count[12][1][8]=656502; ron_count[12][1][7]=3506580; ron_count[12][1][6]=54289620; ron_count[12][1][5]=64963668; ron_count[12][1][4]=103305699; ron_count[12][1][3]=100992969; ron_count[12][1][2]=491783922; ron_count[12][1][1]=915511590; ron_count[12][1][0]=362499480; 
	ron_count[12][0][9]=62370; ron_count[12][0][8]=676368; ron_count[12][0][7]=3506580; ron_count[12][0][6]=54289620; ron_count[12][0][5]=64943802; ron_count[12][0][4]=105887082; ron_count[12][0][3]=100955778; ron_count[12][0][2]=491617224; ron_count[12][0][1]=914261796; ron_count[12][0][0]=361371780; 
	ron_count[11][10][9]=62370; ron_count[11][10][8]=676368; ron_count[11][10][7]=3506580; ron_count[11][10][6]=54289620; ron_count[11][10][5]=64943802; ron_count[11][10][4]=103865013; ron_count[11][10][3]=100981188; ron_count[11][10][2]=491728419; ron_count[11][10][1]=915211710; ron_count[11][10][0]=362307330; 
	ron_count[11][9][9]=62370; ron_count[11][9][8]=656502; ron_count[11][9][7]=3506580; ron_count[11][9][6]=54289620; ron_count[11][9][5]=64963668; ron_count[11][9][4]=101283630; ron_count[11][9][3]=101018379; ron_count[11][9][2]=491895117; ron_count[11][9][1]=916461504; ron_count[11][9][0]=363435030; 
	ron_count[11][8][9]=62370; ron_count[11][8][8]=636636; ron_count[11][8][7]=3506580; ron_count[11][8][6]=54289620; ron_count[11][8][5]=64983534; ron_count[11][8][4]=98702247; ron_count[11][8][3]=101055570; ron_count[11][8][2]=492061815; ron_count[11][8][1]=917711298; ron_count[11][8][0]=364562730; 
	ron_count[11][7][9]=62370; ron_count[11][7][8]=616770; ron_count[11][7][7]=3506580; ron_count[11][7][6]=54289620; ron_count[11][7][5]=65003400; ron_count[11][7][4]=98675472; ron_count[11][7][3]=101067351; ron_count[11][7][2]=492117318; ron_count[11][7][1]=917852859; ron_count[11][7][0]=364380660; 
	ron_count[11][6][9]=62370; ron_count[11][6][8]=596904; ron_count[11][6][7]=3506580; ron_count[11][6][6]=54289620; ron_count[11][6][5]=65023266; ron_count[11][6][4]=98116158; ron_count[11][6][3]=101079132; ron_count[11][6][2]=492172821; ron_count[11][6][1]=918152739; ron_count[11][6][0]=364572810; 
	ron_count[11][5][9]=62370; ron_count[11][5][8]=597828; ron_count[11][5][7]=3506580; ron_count[11][5][6]=54289620; ron_count[11][5][5]=65022342; ron_count[11][5][4]=98272209; ron_count[11][5][3]=101079132; ron_count[11][5][2]=492172821; ron_count[11][5][1]=918092763; ron_count[11][5][0]=364476735; 
	ron_count[11][4][9]=62370; ron_count[11][4][8]=597828; ron_count[11][4][7]=3506580; ron_count[11][4][6]=54289620; ron_count[11][4][5]=65022342; ron_count[11][4][4]=98272209; ron_count[11][4][3]=101079132; ron_count[11][4][2]=492172821; ron_count[11][4][1]=918092763; ron_count[11][4][0]=364476735; 
	ron_count[11][3][9]=62370; ron_count[11][3][8]=596904; ron_count[11][3][7]=3506580; ron_count[11][3][6]=54289620; ron_count[11][3][5]=65023266; ron_count[11][3][4]=97583619; ron_count[11][3][3]=101079132; ron_count[11][3][2]=492172821; ron_count[11][3][1]=918311058; ron_count[11][3][0]=364947030; 
	ron_count[11][2][9]=62370; ron_count[11][2][8]=616770; ron_count[11][2][7]=3506580; ron_count[11][2][6]=54289620; ron_count[11][2][5]=65003400; ron_count[11][2][4]=100165002; ron_count[11][2][3]=101041941; ron_count[11][2][2]=492006123; ron_count[11][2][1]=917061264; ron_count[11][2][0]=363819330; 
	ron_count[11][1][9]=62370; ron_count[11][1][8]=636636; ron_count[11][1][7]=3506580; ron_count[11][1][6]=54289620; ron_count[11][1][5]=64983534; ron_count[11][1][4]=102746385; ron_count[11][1][3]=101004750; ron_count[11][1][2]=491839425; ron_count[11][1][1]=915811470; ron_count[11][1][0]=362691630; 
	ron_count[11][0][9]=62370; ron_count[11][0][8]=656502; ron_count[11][0][7]=3506580; ron_count[11][0][6]=54289620; ron_count[11][0][5]=64963668; ron_count[11][0][4]=105327768; ron_count[11][0][3]=100967559; ron_count[11][0][2]=491672727; ron_count[11][0][1]=914561676; ron_count[11][0][0]=361563930; 
	ron_count[10][9][9]=62370; ron_count[10][9][8]=656502; ron_count[10][9][7]=3506580; ron_count[10][9][6]=54289620; ron_count[10][9][5]=64963668; ron_count[10][9][4]=99261561; ron_count[10][9][3]=101043789; ron_count[10][9][2]=492006312; ron_count[10][9][1]=917411418; ron_count[10][9][0]=364370580; 
	ron_count[10][8][9]=62370; ron_count[10][8][8]=636636; ron_count[10][8][7]=3506580; ron_count[10][8][6]=54289620; ron_count[10][8][5]=64983534; ron_count[10][8][4]=96680178; ron_count[10][8][3]=101080980; ron_count[10][8][2]=492173010; ron_count[10][8][1]=918661212; ron_count[10][8][0]=365498280; 
	ron_count[10][7][9]=62370; ron_count[10][7][8]=616770; ron_count[10][7][7]=3506580; ron_count[10][7][6]=54289620; ron_count[10][7][5]=65003400; ron_count[10][7][4]=96653403; ron_count[10][7][3]=101092761; ron_count[10][7][2]=492228513; ron_count[10][7][1]=918802773; ron_count[10][7][0]=365316210; 
	ron_count[10][6][9]=62370; ron_count[10][6][8]=596904; ron_count[10][6][7]=3506580; ron_count[10][6][6]=54289620; ron_count[10][6][5]=65023266; ron_count[10][6][4]=96094089; ron_count[10][6][3]=101104542; ron_count[10][6][2]=492284016; ron_count[10][6][1]=919102653; ron_count[10][6][0]=365508360; 
	ron_count[10][5][9]=62370; ron_count[10][5][8]=577038; ron_count[10][5][7]=3506580; ron_count[10][5][6]=54289620; ron_count[10][5][5]=65043132; ron_count[10][5][4]=95534775; ron_count[10][5][3]=101116323; ron_count[10][5][2]=492339519; ron_count[10][5][1]=919402533; ron_count[10][5][0]=365700510; 
	ron_count[10][4][9]=62370; ron_count[10][4][8]=577962; ron_count[10][4][7]=3506580; ron_count[10][4][6]=54289620; ron_count[10][4][5]=65042208; ron_count[10][4][4]=95690826; ron_count[10][4][3]=101116323; ron_count[10][4][2]=492339519; ron_count[10][4][1]=919342557; ron_count[10][4][0]=365604435; 
	ron_count[10][3][9]=62370; ron_count[10][3][8]=577038; ron_count[10][3][7]=3506580; ron_count[10][3][6]=54289620; ron_count[10][3][5]=65043132; ron_count[10][3][4]=95002236; ron_count[10][3][3]=101116323; ron_count[10][3][2]=492339519; ron_count[10][3][1]=919560852; ron_count[10][3][0]=366074730; 
	ron_count[10][2][9]=62370; ron_count[10][2][8]=596904; ron_count[10][2][7]=3506580; ron_count[10][2][6]=54289620; ron_count[10][2][5]=65023266; ron_count[10][2][4]=97583619; ron_count[10][2][3]=101079132; ron_count[10][2][2]=492172821; ron_count[10][2][1]=918311058; ron_count[10][2][0]=364947030; 
	ron_count[10][1][9]=62370; ron_count[10][1][8]=616770; ron_count[10][1][7]=3506580; ron_count[10][1][6]=54289620; ron_count[10][1][5]=65003400; ron_count[10][1][4]=100165002; ron_count[10][1][3]=101041941; ron_count[10][1][2]=492006123; ron_count[10][1][1]=917061264; ron_count[10][1][0]=363819330; 
	ron_count[10][0][9]=62370; ron_count[10][0][8]=636636; ron_count[10][0][7]=3506580; ron_count[10][0][6]=54289620; ron_count[10][0][5]=64983534; ron_count[10][0][4]=102746385; ron_count[10][0][3]=101004750; ron_count[10][0][2]=491839425; ron_count[10][0][1]=915811470; ron_count[10][0][0]=362691630; 
	ron_count[9][8][9]=62370; ron_count[9][8][8]=636636; ron_count[9][8][7]=3506580; ron_count[9][8][6]=54289620; ron_count[9][8][5]=64983534; ron_count[9][8][4]=94658109; ron_count[9][8][3]=101106390; ron_count[9][8][2]=492284205; ron_count[9][8][1]=919611126; ron_count[9][8][0]=366433830; 
	ron_count[9][7][9]=62370; ron_count[9][7][8]=616770; ron_count[9][7][7]=3506580; ron_count[9][7][6]=54289620; ron_count[9][7][5]=65003400; ron_count[9][7][4]=94631334; ron_count[9][7][3]=101118171; ron_count[9][7][2]=492339708; ron_count[9][7][1]=919752687; ron_count[9][7][0]=366251760; 
	ron_count[9][6][9]=62370; ron_count[9][6][8]=596904; ron_count[9][6][7]=3506580; ron_count[9][6][6]=54289620; ron_count[9][6][5]=65023266; ron_count[9][6][4]=94072020; ron_count[9][6][3]=101129952; ron_count[9][6][2]=492395211; ron_count[9][6][1]=920052567; ron_count[9][6][0]=366443910; 
	ron_count[9][5][9]=62370; ron_count[9][5][8]=577038; ron_count[9][5][7]=3506580; ron_count[9][5][6]=54289620; ron_count[9][5][5]=65043132; ron_count[9][5][4]=93512706; ron_count[9][5][3]=101141733; ron_count[9][5][2]=492450714; ron_count[9][5][1]=920352447; ron_count[9][5][0]=366636060; 
	ron_count[9][4][9]=62370; ron_count[9][4][8]=557172; ron_count[9][4][7]=3506580; ron_count[9][4][6]=54289620; ron_count[9][4][5]=65062998; ron_count[9][4][4]=92953392; ron_count[9][4][3]=101153514; ron_count[9][4][2]=492506217; ron_count[9][4][1]=920652327; ron_count[9][4][0]=366828210; 
	ron_count[9][3][9]=62370; ron_count[9][3][8]=557172; ron_count[9][3][7]=3506580; ron_count[9][3][6]=54289620; ron_count[9][3][5]=65062998; ron_count[9][3][4]=92420853; ron_count[9][3][3]=101153514; ron_count[9][3][2]=492506217; ron_count[9][3][1]=920810646; ron_count[9][3][0]=367202430; 
	ron_count[9][2][9]=62370; ron_count[9][2][8]=577038; ron_count[9][2][7]=3506580; ron_count[9][2][6]=54289620; ron_count[9][2][5]=65043132; ron_count[9][2][4]=95002236; ron_count[9][2][3]=101116323; ron_count[9][2][2]=492339519; ron_count[9][2][1]=919560852; ron_count[9][2][0]=366074730; 
	ron_count[9][1][9]=62370; ron_count[9][1][8]=596904; ron_count[9][1][7]=3506580; ron_count[9][1][6]=54289620; ron_count[9][1][5]=65023266; ron_count[9][1][4]=97583619; ron_count[9][1][3]=101079132; ron_count[9][1][2]=492172821; ron_count[9][1][1]=918311058; ron_count[9][1][0]=364947030; 
	ron_count[9][0][9]=62370; ron_count[9][0][8]=616770; ron_count[9][0][7]=3506580; ron_count[9][0][6]=54289620; ron_count[9][0][5]=65003400; ron_count[9][0][4]=100165002; ron_count[9][0][3]=101041941; ron_count[9][0][2]=492006123; ron_count[9][0][1]=917061264; ron_count[9][0][0]=363819330; 
	ron_count[8][7][9]=62370; ron_count[8][7][8]=616770; ron_count[8][7][7]=3506580; ron_count[8][7][6]=54289620; ron_count[8][7][5]=65003400; ron_count[8][7][4]=92609265; ron_count[8][7][3]=101143581; ron_count[8][7][2]=492450903; ron_count[8][7][1]=920702601; ron_count[8][7][0]=367187310; 
	ron_count[8][6][9]=62370; ron_count[8][6][8]=596904; ron_count[8][6][7]=3506580; ron_count[8][6][6]=54289620; ron_count[8][6][5]=65023266; ron_count[8][6][4]=92049951; ron_count[8][6][3]=101155362; ron_count[8][6][2]=492506406; ron_count[8][6][1]=921002481; ron_count[8][6][0]=367379460; 
	ron_count[8][5][9]=62370; ron_count[8][5][8]=577038; ron_count[8][5][7]=3506580; ron_count[8][5][6]=54289620; ron_count[8][5][5]=65043132; ron_count[8][5][4]=91490637; ron_count[8][5][3]=101167143; ron_count[8][5][2]=492561909; ron_count[8][5][1]=921302361; ron_count[8][5][0]=367571610; 
	ron_count[8][4][9]=62370; ron_count[8][4][8]=557172; ron_count[8][4][7]=3506580; ron_count[8][4][6]=54289620; ron_count[8][4][5]=65062998; ron_count[8][4][4]=90931323; ron_count[8][4][3]=101178924; ron_count[8][4][2]=492617412; ron_count[8][4][1]=921602241; ron_count[8][4][0]=367763760; 
	ron_count[8][3][9]=62370; ron_count[8][3][8]=536382; ron_count[8][3][7]=3506580; ron_count[8][3][6]=54289620; ron_count[8][3][5]=65083788; ron_count[8][3][4]=89683419; ron_count[8][3][3]=101190705; ron_count[8][3][2]=492672915; ron_count[8][3][1]=922120416; ron_count[8][3][0]=368426205; 
	ron_count[8][2][9]=62370; ron_count[8][2][8]=557172; ron_count[8][2][7]=3506580; ron_count[8][2][6]=54289620; ron_count[8][2][5]=65062998; ron_count[8][2][4]=92420853; ron_count[8][2][3]=101153514; ron_count[8][2][2]=492506217; ron_count[8][2][1]=920810646; ron_count[8][2][0]=367202430; 
	ron_count[8][1][9]=62370; ron_count[8][1][8]=577038; ron_count[8][1][7]=3506580; ron_count[8][1][6]=54289620; ron_count[8][1][5]=65043132; ron_count[8][1][4]=95002236; ron_count[8][1][3]=101116323; ron_count[8][1][2]=492339519; ron_count[8][1][1]=919560852; ron_count[8][1][0]=366074730; 
	ron_count[8][0][9]=62370; ron_count[8][0][8]=596904; ron_count[8][0][7]=3506580; ron_count[8][0][6]=54289620; ron_count[8][0][5]=65023266; ron_count[8][0][4]=97583619; ron_count[8][0][3]=101079132; ron_count[8][0][2]=492172821; ron_count[8][0][1]=918311058; ron_count[8][0][0]=364947030; 
	ron_count[7][6][9]=83160; ron_count[7][6][8]=596904; ron_count[7][6][7]=3506580; ron_count[7][6][6]=54289620; ron_count[7][6][5]=65002476; ron_count[7][6][4]=93297855; ron_count[7][6][3]=101143581; ron_count[7][6][2]=492450903; ron_count[7][6][1]=920484306; ron_count[7][6][0]=366717015; 
	ron_count[7][5][9]=83160; ron_count[7][5][8]=577038; ron_count[7][5][7]=3506580; ron_count[7][5][6]=54289620; ron_count[7][5][5]=65022342; ron_count[7][5][4]=92738541; ron_count[7][5][3]=101155362; ron_count[7][5][2]=492506406; ron_count[7][5][1]=920784186; ron_count[7][5][0]=366909165; 
	ron_count[7][4][9]=83160; ron_count[7][4][8]=557172; ron_count[7][4][7]=3506580; ron_count[7][4][6]=54289620; ron_count[7][4][5]=65042208; ron_count[7][4][4]=92179227; ron_count[7][4][3]=101167143; ron_count[7][4][2]=492561909; ron_count[7][4][1]=921084066; ron_count[7][4][0]=367101315; 
	ron_count[7][3][9]=83160; ron_count[7][3][8]=536382; ron_count[7][3][7]=3506580; ron_count[7][3][6]=54289620; ron_count[7][3][5]=65062998; ron_count[7][3][4]=90931323; ron_count[7][3][3]=101178924; ron_count[7][3][2]=492617412; ron_count[7][3][1]=921602241; ron_count[7][3][0]=367763760; 
	ron_count[7][2][9]=83160; ron_count[7][2][8]=536382; ron_count[7][2][7]=3506580; ron_count[7][2][6]=54289620; ron_count[7][2][5]=65062998; ron_count[7][2][4]=92953392; ron_count[7][2][3]=101153514; ron_count[7][2][2]=492506217; ron_count[7][2][1]=920652327; ron_count[7][2][0]=366828210; 
	ron_count[7][1][9]=83160; ron_count[7][1][8]=557172; ron_count[7][1][7]=3506580; ron_count[7][1][6]=54289620; ron_count[7][1][5]=65042208; ron_count[7][1][4]=95690826; ron_count[7][1][3]=101116323; ron_count[7][1][2]=492339519; ron_count[7][1][1]=919342557; ron_count[7][1][0]=365604435; 
	ron_count[7][0][9]=83160; ron_count[7][0][8]=577038; ron_count[7][0][7]=3506580; ron_count[7][0][6]=54289620; ron_count[7][0][5]=65022342; ron_count[7][0][4]=98272209; ron_count[7][0][3]=101079132; ron_count[7][0][2]=492172821; ron_count[7][0][1]=918092763; ron_count[7][0][0]=364476735; 
	ron_count[6][5][9]=83160; ron_count[6][5][8]=596904; ron_count[6][5][7]=3506580; ron_count[6][5][6]=54289620; ron_count[6][5][5]=65002476; ron_count[6][5][4]=93297855; ron_count[6][5][3]=101143581; ron_count[6][5][2]=492450903; ron_count[6][5][1]=920484306; ron_count[6][5][0]=366717015; 
	ron_count[6][4][9]=83160; ron_count[6][4][8]=577038; ron_count[6][4][7]=3506580; ron_count[6][4][6]=54289620; ron_count[6][4][5]=65022342; ron_count[6][4][4]=92738541; ron_count[6][4][3]=101155362; ron_count[6][4][2]=492506406; ron_count[6][4][1]=920784186; ron_count[6][4][0]=366909165; 
	ron_count[6][3][9]=83160; ron_count[6][3][8]=556248; ron_count[6][3][7]=3506580; ron_count[6][3][6]=54289620; ron_count[6][3][5]=65043132; ron_count[6][3][4]=91490637; ron_count[6][3][3]=101167143; ron_count[6][3][2]=492561909; ron_count[6][3][1]=921302361; ron_count[6][3][0]=367571610; 
	ron_count[6][2][9]=83160; ron_count[6][2][8]=556248; ron_count[6][2][7]=3506580; ron_count[6][2][6]=54289620; ron_count[6][2][5]=65043132; ron_count[6][2][4]=93512706; ron_count[6][2][3]=101141733; ron_count[6][2][2]=492450714; ron_count[6][2][1]=920352447; ron_count[6][2][0]=366636060; 
	ron_count[6][1][9]=83160; ron_count[6][1][8]=556248; ron_count[6][1][7]=3506580; ron_count[6][1][6]=54289620; ron_count[6][1][5]=65043132; ron_count[6][1][4]=95534775; ron_count[6][1][3]=101116323; ron_count[6][1][2]=492339519; ron_count[6][1][1]=919402533; ron_count[6][1][0]=365700510; 
	ron_count[6][0][9]=83160; ron_count[6][0][8]=577038; ron_count[6][0][7]=3506580; ron_count[6][0][6]=54289620; ron_count[6][0][5]=65022342; ron_count[6][0][4]=98272209; ron_count[6][0][3]=101079132; ron_count[6][0][2]=492172821; ron_count[6][0][1]=918092763; ron_count[6][0][0]=364476735; 
	ron_count[5][4][9]=83160; ron_count[5][4][8]=596904; ron_count[5][4][7]=3506580; ron_count[5][4][6]=54289620; ron_count[5][4][5]=65002476; ron_count[5][4][4]=93297855; ron_count[5][4][3]=101143581; ron_count[5][4][2]=492450903; ron_count[5][4][1]=920484306; ron_count[5][4][0]=366717015; 
	ron_count[5][3][9]=83160; ron_count[5][3][8]=576114; ron_count[5][3][7]=3506580; ron_count[5][3][6]=54289620; ron_count[5][3][5]=65023266; ron_count[5][3][4]=92049951; ron_count[5][3][3]=101155362; ron_count[5][3][2]=492506406; ron_count[5][3][1]=921002481; ron_count[5][3][0]=367379460; 
	ron_count[5][2][9]=83160; ron_count[5][2][8]=576114; ron_count[5][2][7]=3506580; ron_count[5][2][6]=54289620; ron_count[5][2][5]=65023266; ron_count[5][2][4]=94072020; ron_count[5][2][3]=101129952; ron_count[5][2][2]=492395211; ron_count[5][2][1]=920052567; ron_count[5][2][0]=366443910; 
	ron_count[5][1][9]=83160; ron_count[5][1][8]=576114; ron_count[5][1][7]=3506580; ron_count[5][1][6]=54289620; ron_count[5][1][5]=65023266; ron_count[5][1][4]=96094089; ron_count[5][1][3]=101104542; ron_count[5][1][2]=492284016; ron_count[5][1][1]=919102653; ron_count[5][1][0]=365508360; 
	ron_count[5][0][9]=83160; ron_count[5][0][8]=576114; ron_count[5][0][7]=3506580; ron_count[5][0][6]=54289620; ron_count[5][0][5]=65023266; ron_count[5][0][4]=98116158; ron_count[5][0][3]=101079132; ron_count[5][0][2]=492172821; ron_count[5][0][1]=918152739; ron_count[5][0][0]=364572810; 
	ron_count[4][3][9]=83160; ron_count[4][3][8]=595980; ron_count[4][3][7]=3506580; ron_count[4][3][6]=54289620; ron_count[4][3][5]=65003400; ron_count[4][3][4]=92609265; ron_count[4][3][3]=101143581; ron_count[4][3][2]=492450903; ron_count[4][3][1]=920702601; ron_count[4][3][0]=367187310; 
	ron_count[4][2][9]=83160; ron_count[4][2][8]=595980; ron_count[4][2][7]=3506580; ron_count[4][2][6]=54289620; ron_count[4][2][5]=65003400; ron_count[4][2][4]=94631334; ron_count[4][2][3]=101118171; ron_count[4][2][2]=492339708; ron_count[4][2][1]=919752687; ron_count[4][2][0]=366251760; 
	ron_count[4][1][9]=83160; ron_count[4][1][8]=595980; ron_count[4][1][7]=3506580; ron_count[4][1][6]=54289620; ron_count[4][1][5]=65003400; ron_count[4][1][4]=96653403; ron_count[4][1][3]=101092761; ron_count[4][1][2]=492228513; ron_count[4][1][1]=918802773; ron_count[4][1][0]=365316210; 
	ron_count[4][0][9]=83160; ron_count[4][0][8]=595980; ron_count[4][0][7]=3506580; ron_count[4][0][6]=54289620; ron_count[4][0][5]=65003400; ron_count[4][0][4]=98675472; ron_count[4][0][3]=101067351; ron_count[4][0][2]=492117318; ron_count[4][0][1]=917852859; ron_count[4][0][0]=364380660; 
	ron_count[3][2][9]=83160; ron_count[3][2][8]=615846; ron_count[3][2][7]=3506580; ron_count[3][2][6]=54289620; ron_count[3][2][5]=64983534; ron_count[3][2][4]=94658109; ron_count[3][2][3]=101106390; ron_count[3][2][2]=492284205; ron_count[3][2][1]=919611126; ron_count[3][2][0]=366433830; 
	ron_count[3][1][9]=83160; ron_count[3][1][8]=615846; ron_count[3][1][7]=3506580; ron_count[3][1][6]=54289620; ron_count[3][1][5]=64983534; ron_count[3][1][4]=96680178; ron_count[3][1][3]=101080980; ron_count[3][1][2]=492173010; ron_count[3][1][1]=918661212; ron_count[3][1][0]=365498280; 
	ron_count[3][0][9]=83160; ron_count[3][0][8]=615846; ron_count[3][0][7]=3506580; ron_count[3][0][6]=54289620; ron_count[3][0][5]=64983534; ron_count[3][0][4]=98702247; ron_count[3][0][3]=101055570; ron_count[3][0][2]=492061815; ron_count[3][0][1]=917711298; ron_count[3][0][0]=364562730; 
	ron_count[2][1][9]=83160; ron_count[2][1][8]=635712; ron_count[2][1][7]=3506580; ron_count[2][1][6]=54289620; ron_count[2][1][5]=64963668; ron_count[2][1][4]=99261561; ron_count[2][1][3]=101043789; ron_count[2][1][2]=492006312; ron_count[2][1][1]=917411418; ron_count[2][1][0]=364370580; 
	ron_count[2][0][9]=83160; ron_count[2][0][8]=635712; ron_count[2][0][7]=3506580; ron_count[2][0][6]=54289620; ron_count[2][0][5]=64963668; ron_count[2][0][4]=101283630; ron_count[2][0][3]=101018379; ron_count[2][0][2]=491895117; ron_count[2][0][1]=916461504; ron_count[2][0][0]=363435030; 
	ron_count[1][0][9]=83160; ron_count[1][0][8]=655578; ron_count[1][0][7]=3506580; ron_count[1][0][6]=54289620; ron_count[1][0][5]=64943802; ron_count[1][0][4]=103865013; ron_count[1][0][3]=100981188; ron_count[1][0][2]=491728419; ron_count[1][0][1]=915211710; ron_count[1][0][0]=362307330; 
	ron_max_pokval[12][11]=2148457658; ron_max_pokval[12][10]=2148457658; ron_max_pokval[12][9]=2148457658; ron_max_pokval[12][8]=2148457658; ron_max_pokval[12][7]=2148457658; ron_max_pokval[12][6]=2148457658; ron_max_pokval[12][5]=2148457658; ron_max_pokval[12][4]=2148457658; ron_max_pokval[12][3]=2148457658; ron_max_pokval[12][2]=2148457658; ron_max_pokval[12][1]=2148457658; ron_max_pokval[12][0]=2148457658; ron_max_pokval[11][10]=2148457658; ron_max_pokval[11][9]=2148457658; ron_max_pokval[11][8]=2148457658; ron_max_pokval[11][7]=2148457658; ron_max_pokval[11][6]=2148457658; ron_max_pokval[11][5]=2148457658; ron_max_pokval[11][4]=2148457658; ron_max_pokval[11][3]=2148457658; ron_max_pokval[11][2]=2148457658; ron_max_pokval[11][1]=2148457658; ron_max_pokval[11][0]=2148457658; ron_max_pokval[10][9]=2148457658; ron_max_pokval[10][8]=2148457658; ron_max_pokval[10][7]=2148457658; ron_max_pokval[10][6]=2148457658; ron_max_pokval[10][5]=2148457658; ron_max_pokval[10][4]=2148457658; ron_max_pokval[10][3]=2148457658; ron_max_pokval[10][2]=2148457658; ron_max_pokval[10][1]=2148457658; ron_max_pokval[10][0]=2148457658; ron_max_pokval[9][8]=2148457658; ron_max_pokval[9][7]=2148457658; ron_max_pokval[9][6]=2148457658; ron_max_pokval[9][5]=2148457658; ron_max_pokval[9][4]=2148457658; ron_max_pokval[9][3]=2148457658; ron_max_pokval[9][2]=2148457658; ron_max_pokval[9][1]=2148457658; ron_max_pokval[9][0]=2148457658; ron_max_pokval[8][7]=2148457658; ron_max_pokval[8][6]=2148457658; ron_max_pokval[8][5]=2148457658; ron_max_pokval[8][4]=2148457658; ron_max_pokval[8][3]=2148457658; ron_max_pokval[8][2]=2148457658; ron_max_pokval[8][1]=2148457658; ron_max_pokval[8][0]=2148457658; ron_max_pokval[7][6]=2148457658; ron_max_pokval[7][5]=2148457658; ron_max_pokval[7][4]=2148457658; ron_max_pokval[7][3]=2148457658; ron_max_pokval[7][2]=2148457658; ron_max_pokval[7][1]=2148457658; ron_max_pokval[7][0]=2148457658; ron_max_pokval[6][5]=2148457658; ron_max_pokval[6][4]=2148457658; ron_max_pokval[6][3]=2148457658; ron_max_pokval[6][2]=2148457658; ron_max_pokval[6][1]=2148457658; ron_max_pokval[6][0]=2148457658; ron_max_pokval[5][4]=2148457658; ron_max_pokval[5][3]=2148457658; ron_max_pokval[5][2]=2148457658; ron_max_pokval[5][1]=2148457658; ron_max_pokval[5][0]=2148457658; ron_max_pokval[4][3]=2148457658; ron_max_pokval[4][2]=2148457658; ron_max_pokval[4][1]=2148457658; ron_max_pokval[4][0]=2148457658; ron_max_pokval[3][2]=2148457658; ron_max_pokval[3][1]=2148457658; ron_max_pokval[3][0]=2148457658; ron_max_pokval[2][1]=2148457658; ron_max_pokval[2][0]=2148457658; ron_max_pokval[1][0]=2148457658; 
	ron_max_pokval_n[12][11]=62370; ron_max_pokval_n[12][10]=62370; ron_max_pokval_n[12][9]=62370; ron_max_pokval_n[12][8]=62370; ron_max_pokval_n[12][7]=62370; ron_max_pokval_n[12][6]=62370; ron_max_pokval_n[12][5]=62370; ron_max_pokval_n[12][4]=62370; ron_max_pokval_n[12][3]=62370; ron_max_pokval_n[12][2]=62370; ron_max_pokval_n[12][1]=62370; ron_max_pokval_n[12][0]=62370; ron_max_pokval_n[11][10]=62370; ron_max_pokval_n[11][9]=62370; ron_max_pokval_n[11][8]=62370; ron_max_pokval_n[11][7]=62370; ron_max_pokval_n[11][6]=62370; ron_max_pokval_n[11][5]=62370; ron_max_pokval_n[11][4]=62370; ron_max_pokval_n[11][3]=62370; ron_max_pokval_n[11][2]=62370; ron_max_pokval_n[11][1]=62370; ron_max_pokval_n[11][0]=62370; ron_max_pokval_n[10][9]=62370; ron_max_pokval_n[10][8]=62370; ron_max_pokval_n[10][7]=62370; ron_max_pokval_n[10][6]=62370; ron_max_pokval_n[10][5]=62370; ron_max_pokval_n[10][4]=62370; ron_max_pokval_n[10][3]=62370; ron_max_pokval_n[10][2]=62370; ron_max_pokval_n[10][1]=62370; ron_max_pokval_n[10][0]=62370; ron_max_pokval_n[9][8]=62370; ron_max_pokval_n[9][7]=62370; ron_max_pokval_n[9][6]=62370; ron_max_pokval_n[9][5]=62370; ron_max_pokval_n[9][4]=62370; ron_max_pokval_n[9][3]=62370; ron_max_pokval_n[9][2]=62370; ron_max_pokval_n[9][1]=62370; ron_max_pokval_n[9][0]=62370; ron_max_pokval_n[8][7]=62370; ron_max_pokval_n[8][6]=62370; ron_max_pokval_n[8][5]=62370; ron_max_pokval_n[8][4]=62370; ron_max_pokval_n[8][3]=62370; ron_max_pokval_n[8][2]=62370; ron_max_pokval_n[8][1]=62370; ron_max_pokval_n[8][0]=62370; ron_max_pokval_n[7][6]=83160; ron_max_pokval_n[7][5]=83160; ron_max_pokval_n[7][4]=83160; ron_max_pokval_n[7][3]=83160; ron_max_pokval_n[7][2]=83160; ron_max_pokval_n[7][1]=83160; ron_max_pokval_n[7][0]=83160; ron_max_pokval_n[6][5]=83160; ron_max_pokval_n[6][4]=83160; ron_max_pokval_n[6][3]=83160; ron_max_pokval_n[6][2]=83160; ron_max_pokval_n[6][1]=83160; ron_max_pokval_n[6][0]=83160; ron_max_pokval_n[5][4]=83160; ron_max_pokval_n[5][3]=83160; ron_max_pokval_n[5][2]=83160; ron_max_pokval_n[5][1]=83160; ron_max_pokval_n[5][0]=83160; ron_max_pokval_n[4][3]=83160; ron_max_pokval_n[4][2]=83160; ron_max_pokval_n[4][1]=83160; ron_max_pokval_n[4][0]=83160; ron_max_pokval_n[3][2]=83160; ron_max_pokval_n[3][1]=83160; ron_max_pokval_n[3][0]=83160; ron_max_pokval_n[2][1]=83160; ron_max_pokval_n[2][0]=83160; ron_max_pokval_n[1][0]=83160; 

	//OFF SUIT HANDS
	//==============
	ron_count[11][12][9]=41580; ron_count[11][12][8]=677292; ron_count[11][12][7]=3506580; ron_count[11][12][6]=54289620; ron_count[11][12][5]=62912388; ron_count[11][12][4]=105980448; ron_count[11][12][3]=100993662; ron_count[11][12][2]=491847972; ron_count[11][12][1]=915286428; ron_count[11][12][0]=362036430; 
	ron_count[10][12][9]=41580; ron_count[10][12][8]=657426; ron_count[10][12][7]=3506580; ron_count[10][12][6]=54289620; ron_count[10][12][5]=62932254; ron_count[10][12][4]=103410132; ron_count[10][12][3]=101030853; ron_count[10][12][2]=492014670; ron_count[10][12][1]=916533030; ron_count[10][12][0]=363156255; 
	ron_count[9][12][9]=41580; ron_count[9][12][8]=637560; ron_count[9][12][7]=3506580; ron_count[9][12][6]=54289620; ron_count[9][12][5]=62952120; ron_count[9][12][4]=100839816; ron_count[9][12][3]=101068044; ron_count[9][12][2]=492181368; ron_count[9][12][1]=917779632; ron_count[9][12][0]=364276080; 
	ron_count[8][12][9]=41580; ron_count[8][12][8]=617694; ron_count[8][12][7]=3506580; ron_count[8][12][6]=54289620; ron_count[8][12][5]=62971986; ron_count[8][12][4]=98269500; ron_count[8][12][3]=101105235; ron_count[8][12][2]=492348066; ron_count[8][12][1]=919026234; ron_count[8][12][0]=365395905; 
	ron_count[7][12][9]=62370; ron_count[7][12][8]=597828; ron_count[7][12][7]=3506580; ron_count[7][12][6]=54289620; ron_count[7][12][5]=62971062; ron_count[7][12][4]=98196735; ron_count[7][12][3]=101117247; ron_count[7][12][2]=492404976; ron_count[7][12][1]=919186947; ron_count[7][12][0]=365239035; 
	ron_count[6][12][9]=62370; ron_count[6][12][8]=597828; ron_count[6][12][7]=3506580; ron_count[6][12][6]=54289620; ron_count[6][12][5]=62971062; ron_count[6][12][4]=98363853; ron_count[6][12][3]=101117247; ron_count[6][12][2]=492404976; ron_count[6][12][1]=919123779; ron_count[6][12][0]=365135085; 
	ron_count[5][12][9]=62370; ron_count[5][12][8]=597828; ron_count[5][12][7]=3506580; ron_count[5][12][6]=54289620; ron_count[5][12][5]=62971062; ron_count[5][12][4]=98363853; ron_count[5][12][3]=101117247; ron_count[5][12][2]=492404976; ron_count[5][12][1]=919123779; ron_count[5][12][0]=365135085; 
	ron_count[4][12][9]=62370; ron_count[4][12][8]=597828; ron_count[4][12][7]=3506580; ron_count[4][12][6]=54289620; ron_count[4][12][5]=62971062; ron_count[4][12][4]=98196735; ron_count[4][12][3]=101117247; ron_count[4][12][2]=492404976; ron_count[4][12][1]=919186947; ron_count[4][12][0]=365239035; 
	ron_count[3][12][9]=62370; ron_count[3][12][8]=596904; ron_count[3][12][7]=3506580; ron_count[3][12][6]=54289620; ron_count[3][12][5]=62971986; ron_count[3][12][4]=98269500; ron_count[3][12][3]=101105235; ron_count[3][12][2]=492348066; ron_count[3][12][1]=919026234; ron_count[3][12][0]=365395905; 
	ron_count[2][12][9]=62370; ron_count[2][12][8]=616770; ron_count[2][12][7]=3506580; ron_count[2][12][6]=54289620; ron_count[2][12][5]=62952120; ron_count[2][12][4]=100839816; ron_count[2][12][3]=101068044; ron_count[2][12][2]=492181368; ron_count[2][12][1]=917779632; ron_count[2][12][0]=364276080; 
	ron_count[1][12][9]=62370; ron_count[1][12][8]=636636; ron_count[1][12][7]=3506580; ron_count[1][12][6]=54289620; ron_count[1][12][5]=62932254; ron_count[1][12][4]=103410132; ron_count[1][12][3]=101030853; ron_count[1][12][2]=492014670; ron_count[1][12][1]=916533030; ron_count[1][12][0]=363156255; 
	ron_count[0][12][9]=62370; ron_count[0][12][8]=656502; ron_count[0][12][7]=3506580; ron_count[0][12][6]=54289620; ron_count[0][12][5]=62912388; ron_count[0][12][4]=105980448; ron_count[0][12][3]=100993662; ron_count[0][12][2]=491847972; ron_count[0][12][1]=915286428; ron_count[0][12][0]=362036430; 
	ron_count[10][11][9]=41580; ron_count[10][11][8]=657426; ron_count[10][11][7]=3506580; ron_count[10][11][6]=54289620; ron_count[10][11][5]=62932254; ron_count[10][11][4]=104002794; ron_count[10][11][3]=101018841; ron_count[10][11][2]=491957760; ron_count[10][11][1]=916217190; ron_count[10][11][0]=362948355; 
	ron_count[9][11][9]=41580; ron_count[9][11][8]=637560; ron_count[9][11][7]=3506580; ron_count[9][11][6]=54289620; ron_count[9][11][5]=62952120; ron_count[9][11][4]=101432478; ron_count[9][11][3]=101056032; ron_count[9][11][2]=492124458; ron_count[9][11][1]=917463792; ron_count[9][11][0]=364068180; 
	ron_count[8][11][9]=41580; ron_count[8][11][8]=617694; ron_count[8][11][7]=3506580; ron_count[8][11][6]=54289620; ron_count[8][11][5]=62971986; ron_count[8][11][4]=98862162; ron_count[8][11][3]=101093223; ron_count[8][11][2]=492291156; ron_count[8][11][1]=918710394; ron_count[8][11][0]=365188005; 
	ron_count[7][11][9]=62370; ron_count[7][11][8]=597828; ron_count[7][11][7]=3506580; ron_count[7][11][6]=54289620; ron_count[7][11][5]=62971062; ron_count[7][11][4]=98789397; ron_count[7][11][3]=101105235; ron_count[7][11][2]=492348066; ron_count[7][11][1]=918871107; ron_count[7][11][0]=365031135; 
	ron_count[6][11][9]=62370; ron_count[6][11][8]=597828; ron_count[6][11][7]=3506580; ron_count[6][11][6]=54289620; ron_count[6][11][5]=62971062; ron_count[6][11][4]=98196735; ron_count[6][11][3]=101117247; ron_count[6][11][2]=492404976; ron_count[6][11][1]=919186947; ron_count[6][11][0]=365239035; 
	ron_count[5][11][9]=62370; ron_count[5][11][8]=597828; ron_count[5][11][7]=3506580; ron_count[5][11][6]=54289620; ron_count[5][11][5]=62971062; ron_count[5][11][4]=98363853; ron_count[5][11][3]=101117247; ron_count[5][11][2]=492404976; ron_count[5][11][1]=919123779; ron_count[5][11][0]=365135085; 
	ron_count[4][11][9]=62370; ron_count[4][11][8]=597828; ron_count[4][11][7]=3506580; ron_count[4][11][6]=54289620; ron_count[4][11][5]=62971062; ron_count[4][11][4]=98363853; ron_count[4][11][3]=101117247; ron_count[4][11][2]=492404976; ron_count[4][11][1]=919123779; ron_count[4][11][0]=365135085; 
	ron_count[3][11][9]=62370; ron_count[3][11][8]=596904; ron_count[3][11][7]=3506580; ron_count[3][11][6]=54289620; ron_count[3][11][5]=62971986; ron_count[3][11][4]=97676838; ron_count[3][11][3]=101117247; ron_count[3][11][2]=492404976; ron_count[3][11][1]=919342074; ron_count[3][11][0]=365603805; 
	ron_count[2][11][9]=62370; ron_count[2][11][8]=616770; ron_count[2][11][7]=3506580; ron_count[2][11][6]=54289620; ron_count[2][11][5]=62952120; ron_count[2][11][4]=100247154; ron_count[2][11][3]=101080056; ron_count[2][11][2]=492238278; ron_count[2][11][1]=918095472; ron_count[2][11][0]=364483980; 
	ron_count[1][11][9]=62370; ron_count[1][11][8]=636636; ron_count[1][11][7]=3506580; ron_count[1][11][6]=54289620; ron_count[1][11][5]=62932254; ron_count[1][11][4]=102817470; ron_count[1][11][3]=101042865; ron_count[1][11][2]=492071580; ron_count[1][11][1]=916848870; ron_count[1][11][0]=363364155; 
	ron_count[0][11][9]=62370; ron_count[0][11][8]=656502; ron_count[0][11][7]=3506580; ron_count[0][11][6]=54289620; ron_count[0][11][5]=62912388; ron_count[0][11][4]=105387786; ron_count[0][11][3]=101005674; ron_count[0][11][2]=491904882; ron_count[0][11][1]=915602268; ron_count[0][11][0]=362244330; 
	ron_count[9][10][9]=41580; ron_count[9][10][8]=617694; ron_count[9][10][7]=3506580; ron_count[9][10][6]=54289620; ron_count[9][10][5]=62971986; ron_count[9][10][4]=99454824; ron_count[9][10][3]=101081211; ron_count[9][10][2]=492234246; ron_count[9][10][1]=918394554; ron_count[9][10][0]=364980105; 
	ron_count[8][10][9]=41580; ron_count[8][10][8]=597828; ron_count[8][10][7]=3506580; ron_count[8][10][6]=54289620; ron_count[8][10][5]=62991852; ron_count[8][10][4]=96884508; ron_count[8][10][3]=101118402; ron_count[8][10][2]=492400944; ron_count[8][10][1]=919641156; ron_count[8][10][0]=366099930; 
	ron_count[7][10][9]=62370; ron_count[7][10][8]=577962; ron_count[7][10][7]=3506580; ron_count[7][10][6]=54289620; ron_count[7][10][5]=62990928; ron_count[7][10][4]=96811743; ron_count[7][10][3]=101130414; ron_count[7][10][2]=492457854; ron_count[7][10][1]=919801869; ron_count[7][10][0]=365943060; 
	ron_count[6][10][9]=62370; ron_count[6][10][8]=577962; ron_count[6][10][7]=3506580; ron_count[6][10][6]=54289620; ron_count[6][10][5]=62990928; ron_count[6][10][4]=96219081; ron_count[6][10][3]=101142426; ron_count[6][10][2]=492514764; ron_count[6][10][1]=920117709; ron_count[6][10][0]=366150960; 
	ron_count[5][10][9]=62370; ron_count[5][10][8]=577962; ron_count[5][10][7]=3506580; ron_count[5][10][6]=54289620; ron_count[5][10][5]=62990928; ron_count[5][10][4]=95626419; ron_count[5][10][3]=101154438; ron_count[5][10][2]=492571674; ron_count[5][10][1]=920433549; ron_count[5][10][0]=366358860; 
	ron_count[4][10][9]=62370; ron_count[4][10][8]=577962; ron_count[4][10][7]=3506580; ron_count[4][10][6]=54289620; ron_count[4][10][5]=62990928; ron_count[4][10][4]=95793537; ron_count[4][10][3]=101154438; ron_count[4][10][2]=492571674; ron_count[4][10][1]=920370381; ron_count[4][10][0]=366254910; 
	ron_count[3][10][9]=62370; ron_count[3][10][8]=577038; ron_count[3][10][7]=3506580; ron_count[3][10][6]=54289620; ron_count[3][10][5]=62991852; ron_count[3][10][4]=95106522; ron_count[3][10][3]=101154438; ron_count[3][10][2]=492571674; ron_count[3][10][1]=920588676; ron_count[3][10][0]=366723630; 
	ron_count[2][10][9]=62370; ron_count[2][10][8]=596904; ron_count[2][10][7]=3506580; ron_count[2][10][6]=54289620; ron_count[2][10][5]=62971986; ron_count[2][10][4]=97676838; ron_count[2][10][3]=101117247; ron_count[2][10][2]=492404976; ron_count[2][10][1]=919342074; ron_count[2][10][0]=365603805; 
	ron_count[1][10][9]=62370; ron_count[1][10][8]=616770; ron_count[1][10][7]=3506580; ron_count[1][10][6]=54289620; ron_count[1][10][5]=62952120; ron_count[1][10][4]=100247154; ron_count[1][10][3]=101080056; ron_count[1][10][2]=492238278; ron_count[1][10][1]=918095472; ron_count[1][10][0]=364483980; 
	ron_count[0][10][9]=62370; ron_count[0][10][8]=636636; ron_count[0][10][7]=3506580; ron_count[0][10][6]=54289620; ron_count[0][10][5]=62932254; ron_count[0][10][4]=102817470; ron_count[0][10][3]=101042865; ron_count[0][10][2]=492071580; ron_count[0][10][1]=916848870; ron_count[0][10][0]=363364155; 
	ron_count[8][9][9]=41580; ron_count[8][9][8]=577962; ron_count[8][9][7]=3506580; ron_count[8][9][6]=54289620; ron_count[8][9][5]=63011718; ron_count[8][9][4]=94906854; ron_count[8][9][3]=101143581; ron_count[8][9][2]=492510732; ron_count[8][9][1]=920571918; ron_count[8][9][0]=367011855; 
	ron_count[7][9][9]=62370; ron_count[7][9][8]=558096; ron_count[7][9][7]=3506580; ron_count[7][9][6]=54289620; ron_count[7][9][5]=63010794; ron_count[7][9][4]=94834089; ron_count[7][9][3]=101155593; ron_count[7][9][2]=492567642; ron_count[7][9][1]=920732631; ron_count[7][9][0]=366854985; 
	ron_count[6][9][9]=62370; ron_count[6][9][8]=558096; ron_count[6][9][7]=3506580; ron_count[6][9][6]=54289620; ron_count[6][9][5]=63010794; ron_count[6][9][4]=94241427; ron_count[6][9][3]=101167605; ron_count[6][9][2]=492624552; ron_count[6][9][1]=921048471; ron_count[6][9][0]=367062885; 
	ron_count[5][9][9]=62370; ron_count[5][9][8]=558096; ron_count[5][9][7]=3506580; ron_count[5][9][6]=54289620; ron_count[5][9][5]=63010794; ron_count[5][9][4]=93648765; ron_count[5][9][3]=101179617; ron_count[5][9][2]=492681462; ron_count[5][9][1]=921364311; ron_count[5][9][0]=367270785; 
	ron_count[4][9][9]=62370; ron_count[4][9][8]=558096; ron_count[4][9][7]=3506580; ron_count[4][9][6]=54289620; ron_count[4][9][5]=63010794; ron_count[4][9][4]=93056103; ron_count[4][9][3]=101191629; ron_count[4][9][2]=492738372; ron_count[4][9][1]=921680151; ron_count[4][9][0]=367478685; 
	ron_count[3][9][9]=62370; ron_count[3][9][8]=557172; ron_count[3][9][7]=3506580; ron_count[3][9][6]=54289620; ron_count[3][9][5]=63011718; ron_count[3][9][4]=92536206; ron_count[3][9][3]=101191629; ron_count[3][9][2]=492738372; ron_count[3][9][1]=921835278; ron_count[3][9][0]=367843455; 
	ron_count[2][9][9]=62370; ron_count[2][9][8]=577038; ron_count[2][9][7]=3506580; ron_count[2][9][6]=54289620; ron_count[2][9][5]=62991852; ron_count[2][9][4]=95106522; ron_count[2][9][3]=101154438; ron_count[2][9][2]=492571674; ron_count[2][9][1]=920588676; ron_count[2][9][0]=366723630; 
	ron_count[1][9][9]=62370; ron_count[1][9][8]=596904; ron_count[1][9][7]=3506580; ron_count[1][9][6]=54289620; ron_count[1][9][5]=62971986; ron_count[1][9][4]=97676838; ron_count[1][9][3]=101117247; ron_count[1][9][2]=492404976; ron_count[1][9][1]=919342074; ron_count[1][9][0]=365603805; 
	ron_count[0][9][9]=62370; ron_count[0][9][8]=616770; ron_count[0][9][7]=3506580; ron_count[0][9][6]=54289620; ron_count[0][9][5]=62952120; ron_count[0][9][4]=100247154; ron_count[0][9][3]=101080056; ron_count[0][9][2]=492238278; ron_count[0][9][1]=918095472; ron_count[0][9][0]=364483980; 
	ron_count[7][8][9]=62370; ron_count[7][8][8]=538230; ron_count[7][8][7]=3506580; ron_count[7][8][6]=54289620; ron_count[7][8][5]=63030660; ron_count[7][8][4]=92856435; ron_count[7][8][3]=101180772; ron_count[7][8][2]=492677430; ron_count[7][8][1]=921663393; ron_count[7][8][0]=367766910; 
	ron_count[6][8][9]=62370; ron_count[6][8][8]=538230; ron_count[6][8][7]=3506580; ron_count[6][8][6]=54289620; ron_count[6][8][5]=63030660; ron_count[6][8][4]=92263773; ron_count[6][8][3]=101192784; ron_count[6][8][2]=492734340; ron_count[6][8][1]=921979233; ron_count[6][8][0]=367974810; 
	ron_count[5][8][9]=62370; ron_count[5][8][8]=538230; ron_count[5][8][7]=3506580; ron_count[5][8][6]=54289620; ron_count[5][8][5]=63030660; ron_count[5][8][4]=91671111; ron_count[5][8][3]=101204796; ron_count[5][8][2]=492791250; ron_count[5][8][1]=922295073; ron_count[5][8][0]=368182710; 
	ron_count[4][8][9]=62370; ron_count[4][8][8]=538230; ron_count[4][8][7]=3506580; ron_count[4][8][6]=54289620; ron_count[4][8][5]=63030660; ron_count[4][8][4]=91078449; ron_count[4][8][3]=101216808; ron_count[4][8][2]=492848160; ron_count[4][8][1]=922610913; ron_count[4][8][0]=368390610; 
	ron_count[3][8][9]=62370; ron_count[3][8][8]=537306; ron_count[3][8][7]=3506580; ron_count[3][8][6]=54289620; ron_count[3][8][5]=63031584; ron_count[3][8][4]=89798772; ron_count[3][8][3]=101228820; ron_count[3][8][2]=492905070; ron_count[3][8][1]=923145048; ron_count[3][8][0]=369067230; 
	ron_count[2][8][9]=62370; ron_count[2][8][8]=557172; ron_count[2][8][7]=3506580; ron_count[2][8][6]=54289620; ron_count[2][8][5]=63011718; ron_count[2][8][4]=92536206; ron_count[2][8][3]=101191629; ron_count[2][8][2]=492738372; ron_count[2][8][1]=921835278; ron_count[2][8][0]=367843455; 
	ron_count[1][8][9]=62370; ron_count[1][8][8]=577038; ron_count[1][8][7]=3506580; ron_count[1][8][6]=54289620; ron_count[1][8][5]=62991852; ron_count[1][8][4]=95106522; ron_count[1][8][3]=101154438; ron_count[1][8][2]=492571674; ron_count[1][8][1]=920588676; ron_count[1][8][0]=366723630; 
	ron_count[0][8][9]=62370; ron_count[0][8][8]=596904; ron_count[0][8][7]=3506580; ron_count[0][8][6]=54289620; ron_count[0][8][5]=62971986; ron_count[0][8][4]=97676838; ron_count[0][8][3]=101117247; ron_count[0][8][2]=492404976; ron_count[0][8][1]=919342074; ron_count[0][8][0]=365603805; 
	ron_count[6][7][9]=83160; ron_count[6][7][8]=518364; ron_count[6][7][7]=3506580; ron_count[6][7][6]=54289620; ron_count[6][7][5]=63029736; ron_count[6][7][4]=93543450; ron_count[6][7][3]=101180772; ron_count[6][7][2]=492677430; ron_count[6][7][1]=921445098; ron_count[6][7][0]=367298190; 
	ron_count[5][7][9]=83160; ron_count[5][7][8]=518364; ron_count[5][7][7]=3506580; ron_count[5][7][6]=54289620; ron_count[5][7][5]=63029736; ron_count[5][7][4]=92950788; ron_count[5][7][3]=101192784; ron_count[5][7][2]=492734340; ron_count[5][7][1]=921760938; ron_count[5][7][0]=367506090; 
	ron_count[4][7][9]=83160; ron_count[4][7][8]=518364; ron_count[4][7][7]=3506580; ron_count[4][7][6]=54289620; ron_count[4][7][5]=63029736; ron_count[4][7][4]=92358126; ron_count[4][7][3]=101204796; ron_count[4][7][2]=492791250; ron_count[4][7][1]=922076778; ron_count[4][7][0]=367713990; 
	ron_count[3][7][9]=83160; ron_count[3][7][8]=517440; ron_count[3][7][7]=3506580; ron_count[3][7][6]=54289620; ron_count[3][7][5]=63030660; ron_count[3][7][4]=91078449; ron_count[3][7][3]=101216808; ron_count[3][7][2]=492848160; ron_count[3][7][1]=922610913; ron_count[3][7][0]=368390610; 
	ron_count[2][7][9]=83160; ron_count[2][7][8]=537306; ron_count[2][7][7]=3506580; ron_count[2][7][6]=54289620; ron_count[2][7][5]=63010794; ron_count[2][7][4]=93056103; ron_count[2][7][3]=101191629; ron_count[2][7][2]=492738372; ron_count[2][7][1]=921680151; ron_count[2][7][0]=367478685; 
	ron_count[1][7][9]=83160; ron_count[1][7][8]=557172; ron_count[1][7][7]=3506580; ron_count[1][7][6]=54289620; ron_count[1][7][5]=62990928; ron_count[1][7][4]=95793537; ron_count[1][7][3]=101154438; ron_count[1][7][2]=492571674; ron_count[1][7][1]=920370381; ron_count[1][7][0]=366254910; 
	ron_count[0][7][9]=83160; ron_count[0][7][8]=577038; ron_count[0][7][7]=3506580; ron_count[0][7][6]=54289620; ron_count[0][7][5]=62971062; ron_count[0][7][4]=98363853; ron_count[0][7][3]=101117247; ron_count[0][7][2]=492404976; ron_count[0][7][1]=919123779; ron_count[0][7][0]=365135085; 
	ron_count[5][6][9]=83160; ron_count[5][6][8]=518364; ron_count[5][6][7]=3506580; ron_count[5][6][6]=54289620; ron_count[5][6][5]=63029736; ron_count[5][6][4]=93543450; ron_count[5][6][3]=101180772; ron_count[5][6][2]=492677430; ron_count[5][6][1]=921445098; ron_count[5][6][0]=367298190; 
	ron_count[4][6][9]=83160; ron_count[4][6][8]=518364; ron_count[4][6][7]=3506580; ron_count[4][6][6]=54289620; ron_count[4][6][5]=63029736; ron_count[4][6][4]=92950788; ron_count[4][6][3]=101192784; ron_count[4][6][2]=492734340; ron_count[4][6][1]=921760938; ron_count[4][6][0]=367506090; 
	ron_count[3][6][9]=83160; ron_count[3][6][8]=517440; ron_count[3][6][7]=3506580; ron_count[3][6][6]=54289620; ron_count[3][6][5]=63030660; ron_count[3][6][4]=91671111; ron_count[3][6][3]=101204796; ron_count[3][6][2]=492791250; ron_count[3][6][1]=922295073; ron_count[3][6][0]=368182710; 
	ron_count[2][6][9]=83160; ron_count[2][6][8]=537306; ron_count[2][6][7]=3506580; ron_count[2][6][6]=54289620; ron_count[2][6][5]=63010794; ron_count[2][6][4]=93648765; ron_count[2][6][3]=101179617; ron_count[2][6][2]=492681462; ron_count[2][6][1]=921364311; ron_count[2][6][0]=367270785; 
	ron_count[1][6][9]=83160; ron_count[1][6][8]=557172; ron_count[1][6][7]=3506580; ron_count[1][6][6]=54289620; ron_count[1][6][5]=62990928; ron_count[1][6][4]=95626419; ron_count[1][6][3]=101154438; ron_count[1][6][2]=492571674; ron_count[1][6][1]=920433549; ron_count[1][6][0]=366358860; 
	ron_count[0][6][9]=83160; ron_count[0][6][8]=577038; ron_count[0][6][7]=3506580; ron_count[0][6][6]=54289620; ron_count[0][6][5]=62971062; ron_count[0][6][4]=98363853; ron_count[0][6][3]=101117247; ron_count[0][6][2]=492404976; ron_count[0][6][1]=919123779; ron_count[0][6][0]=365135085; 
	ron_count[4][5][9]=83160; ron_count[4][5][8]=518364; ron_count[4][5][7]=3506580; ron_count[4][5][6]=54289620; ron_count[4][5][5]=63029736; ron_count[4][5][4]=93543450; ron_count[4][5][3]=101180772; ron_count[4][5][2]=492677430; ron_count[4][5][1]=921445098; ron_count[4][5][0]=367298190; 
	ron_count[3][5][9]=83160; ron_count[3][5][8]=517440; ron_count[3][5][7]=3506580; ron_count[3][5][6]=54289620; ron_count[3][5][5]=63030660; ron_count[3][5][4]=92263773; ron_count[3][5][3]=101192784; ron_count[3][5][2]=492734340; ron_count[3][5][1]=921979233; ron_count[3][5][0]=367974810; 
	ron_count[2][5][9]=83160; ron_count[2][5][8]=537306; ron_count[2][5][7]=3506580; ron_count[2][5][6]=54289620; ron_count[2][5][5]=63010794; ron_count[2][5][4]=94241427; ron_count[2][5][3]=101167605; ron_count[2][5][2]=492624552; ron_count[2][5][1]=921048471; ron_count[2][5][0]=367062885; 
	ron_count[1][5][9]=83160; ron_count[1][5][8]=557172; ron_count[1][5][7]=3506580; ron_count[1][5][6]=54289620; ron_count[1][5][5]=62990928; ron_count[1][5][4]=96219081; ron_count[1][5][3]=101142426; ron_count[1][5][2]=492514764; ron_count[1][5][1]=920117709; ron_count[1][5][0]=366150960; 
	ron_count[0][5][9]=83160; ron_count[0][5][8]=577038; ron_count[0][5][7]=3506580; ron_count[0][5][6]=54289620; ron_count[0][5][5]=62971062; ron_count[0][5][4]=98196735; ron_count[0][5][3]=101117247; ron_count[0][5][2]=492404976; ron_count[0][5][1]=919186947; ron_count[0][5][0]=365239035; 
	ron_count[3][4][9]=83160; ron_count[3][4][8]=517440; ron_count[3][4][7]=3506580; ron_count[3][4][6]=54289620; ron_count[3][4][5]=63030660; ron_count[3][4][4]=92856435; ron_count[3][4][3]=101180772; ron_count[3][4][2]=492677430; ron_count[3][4][1]=921663393; ron_count[3][4][0]=367766910; 
	ron_count[2][4][9]=83160; ron_count[2][4][8]=537306; ron_count[2][4][7]=3506580; ron_count[2][4][6]=54289620; ron_count[2][4][5]=63010794; ron_count[2][4][4]=94834089; ron_count[2][4][3]=101155593; ron_count[2][4][2]=492567642; ron_count[2][4][1]=920732631; ron_count[2][4][0]=366854985; 
	ron_count[1][4][9]=83160; ron_count[1][4][8]=557172; ron_count[1][4][7]=3506580; ron_count[1][4][6]=54289620; ron_count[1][4][5]=62990928; ron_count[1][4][4]=96811743; ron_count[1][4][3]=101130414; ron_count[1][4][2]=492457854; ron_count[1][4][1]=919801869; ron_count[1][4][0]=365943060; 
	ron_count[0][4][9]=83160; ron_count[0][4][8]=577038; ron_count[0][4][7]=3506580; ron_count[0][4][6]=54289620; ron_count[0][4][5]=62971062; ron_count[0][4][4]=98789397; ron_count[0][4][3]=101105235; ron_count[0][4][2]=492348066; ron_count[0][4][1]=918871107; ron_count[0][4][0]=365031135; 
	ron_count[2][3][9]=83160; ron_count[2][3][8]=536382; ron_count[2][3][7]=3506580; ron_count[2][3][6]=54289620; ron_count[2][3][5]=63011718; ron_count[2][3][4]=94906854; ron_count[2][3][3]=101143581; ron_count[2][3][2]=492510732; ron_count[2][3][1]=920571918; ron_count[2][3][0]=367011855; 
	ron_count[1][3][9]=83160; ron_count[1][3][8]=556248; ron_count[1][3][7]=3506580; ron_count[1][3][6]=54289620; ron_count[1][3][5]=62991852; ron_count[1][3][4]=96884508; ron_count[1][3][3]=101118402; ron_count[1][3][2]=492400944; ron_count[1][3][1]=919641156; ron_count[1][3][0]=366099930; 
	ron_count[0][3][9]=83160; ron_count[0][3][8]=576114; ron_count[0][3][7]=3506580; ron_count[0][3][6]=54289620; ron_count[0][3][5]=62971986; ron_count[0][3][4]=98862162; ron_count[0][3][3]=101093223; ron_count[0][3][2]=492291156; ron_count[0][3][1]=918710394; ron_count[0][3][0]=365188005; 
	ron_count[1][2][9]=83160; ron_count[1][2][8]=576114; ron_count[1][2][7]=3506580; ron_count[1][2][6]=54289620; ron_count[1][2][5]=62971986; ron_count[1][2][4]=99454824; ron_count[1][2][3]=101081211; ron_count[1][2][2]=492234246; ron_count[1][2][1]=918394554; ron_count[1][2][0]=364980105; 
	ron_count[0][2][9]=83160; ron_count[0][2][8]=595980; ron_count[0][2][7]=3506580; ron_count[0][2][6]=54289620; ron_count[0][2][5]=62952120; ron_count[0][2][4]=101432478; ron_count[0][2][3]=101056032; ron_count[0][2][2]=492124458; ron_count[0][2][1]=917463792; ron_count[0][2][0]=364068180; 
	ron_count[0][1][9]=83160; ron_count[0][1][8]=615846; ron_count[0][1][7]=3506580; ron_count[0][1][6]=54289620; ron_count[0][1][5]=62932254; ron_count[0][1][4]=104002794; ron_count[0][1][3]=101018841; ron_count[0][1][2]=491957760; ron_count[0][1][1]=916217190; ron_count[0][1][0]=362948355; 
	ron_max_pokval[11][12]=2148457658; ron_max_pokval[10][12]=2148457658; ron_max_pokval[9][12]=2148457658; ron_max_pokval[8][12]=2148457658; ron_max_pokval[7][12]=2148457658; ron_max_pokval[6][12]=2148457658; ron_max_pokval[5][12]=2148457658; ron_max_pokval[4][12]=2148457658; ron_max_pokval[3][12]=2148457658; ron_max_pokval[2][12]=2148457658; ron_max_pokval[1][12]=2148457658; ron_max_pokval[0][12]=2148457658; ron_max_pokval[10][11]=2148457658; ron_max_pokval[9][11]=2148457658; ron_max_pokval[8][11]=2148457658; ron_max_pokval[7][11]=2148457658; ron_max_pokval[6][11]=2148457658; ron_max_pokval[5][11]=2148457658; ron_max_pokval[4][11]=2148457658; ron_max_pokval[3][11]=2148457658; ron_max_pokval[2][11]=2148457658; ron_max_pokval[1][11]=2148457658; ron_max_pokval[0][11]=2148457658; ron_max_pokval[9][10]=2148457658; ron_max_pokval[8][10]=2148457658; ron_max_pokval[7][10]=2148457658; ron_max_pokval[6][10]=2148457658; ron_max_pokval[5][10]=2148457658; ron_max_pokval[4][10]=2148457658; ron_max_pokval[3][10]=2148457658; ron_max_pokval[2][10]=2148457658; ron_max_pokval[1][10]=2148457658; ron_max_pokval[0][10]=2148457658; ron_max_pokval[8][9]=2148457658; ron_max_pokval[7][9]=2148457658; ron_max_pokval[6][9]=2148457658; ron_max_pokval[5][9]=2148457658; ron_max_pokval[4][9]=2148457658; ron_max_pokval[3][9]=2148457658; ron_max_pokval[2][9]=2148457658; ron_max_pokval[1][9]=2148457658; ron_max_pokval[0][9]=2148457658; ron_max_pokval[7][8]=2148457658; ron_max_pokval[6][8]=2148457658; ron_max_pokval[5][8]=2148457658; ron_max_pokval[4][8]=2148457658; ron_max_pokval[3][8]=2148457658; ron_max_pokval[2][8]=2148457658; ron_max_pokval[1][8]=2148457658; ron_max_pokval[0][8]=2148457658; ron_max_pokval[6][7]=2148457658; ron_max_pokval[5][7]=2148457658; ron_max_pokval[4][7]=2148457658; ron_max_pokval[3][7]=2148457658; ron_max_pokval[2][7]=2148457658; ron_max_pokval[1][7]=2148457658; ron_max_pokval[0][7]=2148457658; ron_max_pokval[5][6]=2148457658; ron_max_pokval[4][6]=2148457658; ron_max_pokval[3][6]=2148457658; ron_max_pokval[2][6]=2148457658; ron_max_pokval[1][6]=2148457658; ron_max_pokval[0][6]=2148457658; ron_max_pokval[4][5]=2148457658; ron_max_pokval[3][5]=2148457658; ron_max_pokval[2][5]=2148457658; ron_max_pokval[1][5]=2148457658; ron_max_pokval[0][5]=2148457658; ron_max_pokval[3][4]=2148457658; ron_max_pokval[2][4]=2148457658; ron_max_pokval[1][4]=2148457658; ron_max_pokval[0][4]=2148457658; ron_max_pokval[2][3]=2148457658; ron_max_pokval[1][3]=2148457658; ron_max_pokval[0][3]=2148457658; ron_max_pokval[1][2]=2148457658; ron_max_pokval[0][2]=2148457658; ron_max_pokval[0][1]=2148457658; 
	ron_max_pokval_n[11][12]=41580; ron_max_pokval_n[10][12]=41580; ron_max_pokval_n[9][12]=41580; ron_max_pokval_n[8][12]=41580; ron_max_pokval_n[7][12]=62370; ron_max_pokval_n[6][12]=62370; ron_max_pokval_n[5][12]=62370; ron_max_pokval_n[4][12]=62370; ron_max_pokval_n[3][12]=62370; ron_max_pokval_n[2][12]=62370; ron_max_pokval_n[1][12]=62370; ron_max_pokval_n[0][12]=62370; ron_max_pokval_n[10][11]=41580; ron_max_pokval_n[9][11]=41580; ron_max_pokval_n[8][11]=41580; ron_max_pokval_n[7][11]=62370; ron_max_pokval_n[6][11]=62370; ron_max_pokval_n[5][11]=62370; ron_max_pokval_n[4][11]=62370; ron_max_pokval_n[3][11]=62370; ron_max_pokval_n[2][11]=62370; ron_max_pokval_n[1][11]=62370; ron_max_pokval_n[0][11]=62370; ron_max_pokval_n[9][10]=41580; ron_max_pokval_n[8][10]=41580; ron_max_pokval_n[7][10]=62370; ron_max_pokval_n[6][10]=62370; ron_max_pokval_n[5][10]=62370; ron_max_pokval_n[4][10]=62370; ron_max_pokval_n[3][10]=62370; ron_max_pokval_n[2][10]=62370; ron_max_pokval_n[1][10]=62370; ron_max_pokval_n[0][10]=62370; ron_max_pokval_n[8][9]=41580; ron_max_pokval_n[7][9]=62370; ron_max_pokval_n[6][9]=62370; ron_max_pokval_n[5][9]=62370; ron_max_pokval_n[4][9]=62370; ron_max_pokval_n[3][9]=62370; ron_max_pokval_n[2][9]=62370; ron_max_pokval_n[1][9]=62370; ron_max_pokval_n[0][9]=62370; ron_max_pokval_n[7][8]=62370; ron_max_pokval_n[6][8]=62370; ron_max_pokval_n[5][8]=62370; ron_max_pokval_n[4][8]=62370; ron_max_pokval_n[3][8]=62370; ron_max_pokval_n[2][8]=62370; ron_max_pokval_n[1][8]=62370; ron_max_pokval_n[0][8]=62370; ron_max_pokval_n[6][7]=83160; ron_max_pokval_n[5][7]=83160; ron_max_pokval_n[4][7]=83160; ron_max_pokval_n[3][7]=83160; ron_max_pokval_n[2][7]=83160; ron_max_pokval_n[1][7]=83160; ron_max_pokval_n[0][7]=83160; ron_max_pokval_n[5][6]=83160; ron_max_pokval_n[4][6]=83160; ron_max_pokval_n[3][6]=83160; ron_max_pokval_n[2][6]=83160; ron_max_pokval_n[1][6]=83160; ron_max_pokval_n[0][6]=83160; ron_max_pokval_n[4][5]=83160; ron_max_pokval_n[3][5]=83160; ron_max_pokval_n[2][5]=83160; ron_max_pokval_n[1][5]=83160; ron_max_pokval_n[0][5]=83160; ron_max_pokval_n[3][4]=83160; ron_max_pokval_n[2][4]=83160; ron_max_pokval_n[1][4]=83160; ron_max_pokval_n[0][4]=83160; ron_max_pokval_n[2][3]=83160; ron_max_pokval_n[1][3]=83160; ron_max_pokval_n[0][3]=83160; ron_max_pokval_n[1][2]=83160; ron_max_pokval_n[0][2]=83160; ron_max_pokval_n[0][1]=83160; 


#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CRunRon::constructor :\n"); 
		throw;
	}
#endif
}

CRunRon::~CRunRon(void) {
#ifdef SEH_ENABLE
	try {
#endif	
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CRunRon::destructor :\n"); 
		throw;
	}
#endif
}


void CRunRon::get_counts(void) {
#ifdef SEH_ENABLE
	try {
#endif	

		unsigned int	pcard[2], ocard[2];
		int				index1, index2;

		int				i;
		CardMask		DeadCards, ExtraCards, EvalCards, ComCards, AllDeadCards;
		int				nDeadCards, nComCards;
		HandVal			hv;
		int				hv_totals[10];
		double			dummy;
		double			max_pokval, max_pokval_count;
		unsigned int	pokval;
		LARGE_INTEGER	bcount, ecount;

		// Preflop is pre-calculated
		if (symbols.sym.br==1) {

			if (!symbols.user_chair_confirmed) {
				return;
			}

			//////////////////////////////////////////////////////////////
			// run symbols

			// time it
			QueryPerformanceCounter(&bcount);
		
			run$royfl = run$strfl = run$4kind = run$fullh = run$flush = run$strai = run$3kind = 0.0;
			run$2pair = run$1pair = run$hcard = run$total = run$pokervalmax = run$prnuts = run$prbest = run$clocks = 0.0;

			pcard[0] = scraper.card_player[(int) symbols.sym.userchair][0];
			pcard[1] = scraper.card_player[(int) symbols.sym.userchair][1];

			// suited, store in hi/lo
			if (StdDeck_SUIT(pcard[0]) == StdDeck_SUIT(pcard[1])) {
				if (StdDeck_RANK(pcard[0]) >= StdDeck_RANK(pcard[1])) {
					index1 = StdDeck_RANK(pcard[0]);
					index2 = StdDeck_RANK(pcard[1]);
				}
				else {
					index1 = StdDeck_RANK(pcard[1]);
					index2 = StdDeck_RANK(pcard[0]);
				}
			}
			// unsuited, store in lo/hi
			else {
				if (StdDeck_RANK(pcard[0]) >= StdDeck_RANK(pcard[1])) {
					index1 = StdDeck_RANK(pcard[1]);
					index2 = StdDeck_RANK(pcard[0]);
				}
				else {
					index1 = StdDeck_RANK(pcard[0]);
					index2 = StdDeck_RANK(pcard[1]);
				}
			}
			
			// time it
			QueryPerformanceCounter(&ecount);

			// populate symbols
			run$royfl = run_count[index1][index2][9];
			run$strfl = run_count[index1][index2][8];
			run$4kind = run_count[index1][index2][7];
			run$fullh = run_count[index1][index2][6];
			run$flush = run_count[index1][index2][5];
			run$strai = run_count[index1][index2][4];
			run$3kind = run_count[index1][index2][3];
			run$2pair = run_count[index1][index2][2];
			run$1pair = run_count[index1][index2][1];
			run$hcard = run_count[index1][index2][0];
			run$total = run$royfl+run$strfl+run$4kind+run$fullh+run$flush+
						run$strai+run$3kind+run$2pair+run$1pair+run$hcard;
			run$prbest = run_max_pokval_n[index1][index2]/run$total;
			run$pokervalmax = run_max_pokval[index1][index2];
			run_pokervalmax_count = run_max_pokval_n[index1][index2];
			run$clocks = ecount.LowPart - bcount.LowPart;


			//////////////////////////////////////////////////////////////
			// ron symbols
			// time it
			QueryPerformanceCounter(&bcount);
		
			ron$royfl = ron$strfl = ron$4kind = ron$fullh = ron$flush = ron$strai = ron$3kind = 0.0;
			ron$2pair = ron$1pair = ron$hcard = ron$total = ron$pokervalmax = ron$prnuts = ron$prbest = ron$clocks = 0.0;

			pcard[0] = scraper.card_player[(int) symbols.sym.userchair][0];
			pcard[1] = scraper.card_player[(int) symbols.sym.userchair][1];

			// suited, store in hi/lo
			if (StdDeck_SUIT(pcard[0]) == StdDeck_SUIT(pcard[1])) {
				if (StdDeck_RANK(pcard[0]) >= StdDeck_RANK(pcard[1])) {
					index1 = StdDeck_RANK(pcard[0]);
					index2 = StdDeck_RANK(pcard[1]);
				}
				else {
					index1 = StdDeck_RANK(pcard[1]);
					index2 = StdDeck_RANK(pcard[0]);
				}
			}
			// unsuited, store in lo/hi
			else {
				if (StdDeck_RANK(pcard[0]) >= StdDeck_RANK(pcard[1])) {
					index1 = StdDeck_RANK(pcard[1]);
					index2 = StdDeck_RANK(pcard[0]);
				}
				else {
					index1 = StdDeck_RANK(pcard[0]);
					index2 = StdDeck_RANK(pcard[1]);
				}
			}
			
			// time it
			QueryPerformanceCounter(&ecount);

			// populate symbols
			ron$royfl = ron_count[index1][index2][9];
			ron$strfl = ron_count[index1][index2][8];
			ron$4kind = ron_count[index1][index2][7];
			ron$fullh = ron_count[index1][index2][6];
			ron$flush = ron_count[index1][index2][5];
			ron$strai = ron_count[index1][index2][4];
			ron$3kind = ron_count[index1][index2][3];
			ron$2pair = ron_count[index1][index2][2];
			ron$1pair = ron_count[index1][index2][1];
			ron$hcard = ron_count[index1][index2][0];
			ron$total = ron$royfl+ron$strfl+ron$4kind+ron$fullh+ron$flush+
						ron$strai+ron$3kind+ron$2pair+ron$1pair+ron$hcard;
			ron$prbest = ron_max_pokval_n[index1][index2]/ron$total;
			ron$pokervalmax = ron_max_pokval[index1][index2];
			ron_pokervalmax_count = ron_max_pokval_n[index1][index2];

			ron$clocks = ecount.LowPart - bcount.LowPart;
		
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Post flop is calculated on the fly
		else if (symbols.sym.br>=2) {
			//////////////////////////////////////////////////////////////
			// run symbols

			// time it
			QueryPerformanceCounter(&bcount);

			// player cards
			nDeadCards = nComCards = 0;
			CardMask_RESET(DeadCards);
			for (i=0; i<=1; i++) {
				if (scraper.card_player[(int) symbols.sym.userchair][i] != CARD_BACK &&
					scraper.card_player[(int) symbols.sym.userchair][i] != CARD_NOCARD) {
					CardMask_SET(DeadCards, scraper.card_player[(int) symbols.sym.userchair][i]);
					nDeadCards++;
				}
			}

			// common cards
			if (symbols.sym.br>=2) {
				for (i=0; i<=2; i++) {
					if (scraper.card_common[i] != CARD_BACK && scraper.card_common[i] != CARD_NOCARD) {
						CardMask_SET(DeadCards, scraper.card_common[i]);
						nDeadCards++;
						nComCards++;
					}
				}
			}
			if (symbols.sym.br>=3) {
				if (scraper.card_common[3] != CARD_BACK && scraper.card_common[3] != CARD_NOCARD) {
					CardMask_SET(DeadCards, scraper.card_common[3]);
					nDeadCards++;
					nComCards++;
				}
			}
			if (symbols.sym.br>=4) {
				if (scraper.card_common[4] != CARD_BACK && scraper.card_common[4] != CARD_NOCARD) {
					CardMask_SET(DeadCards, scraper.card_common[4]);
					nDeadCards++;
					nComCards++;
				}
			}

			// setup the counters
			for (i=0; i<=9; i++) { hv_totals[i] = 0; }
			max_pokval = max_pokval_count = 0;

			// Do the enumeration
			ENUMERATE_N_CARDS_D(ExtraCards, 5-nComCards, DeadCards,
				{
					CardMask_OR(EvalCards, DeadCards, ExtraCards);
					hv = Hand_EVAL_N(EvalCards, 7);
					pokval = symbols.calc_pokerval(hv, 7, &dummy, CARD_NOCARD, CARD_NOCARD);

					if (pokval>max_pokval) { max_pokval = pokval; max_pokval_count=1; }
					else if (pokval==max_pokval) { max_pokval_count++; }
					
					if (HandVal_HANDTYPE(hv)==HandType_STFLUSH && StdDeck_RANK(HandVal_TOP_CARD(hv))==Rank_ACE) { ++hv_totals[9]; }
					else { ++hv_totals[HandVal_HANDTYPE(hv)]; }
				});

			// time it
			QueryPerformanceCounter(&ecount);

			// populate symbols
			run$royfl = hv_totals[9];
			run$strfl = hv_totals[8];
			run$4kind = hv_totals[7];
			run$fullh = hv_totals[6];
			run$flush = hv_totals[5];
			run$strai = hv_totals[4];
			run$3kind = hv_totals[3];
			run$2pair = hv_totals[2];
			run$1pair = hv_totals[1];
			run$hcard = hv_totals[0];
			run$total = run$royfl+run$strfl+run$4kind+run$fullh+run$flush+
						run$strai+run$3kind+run$2pair+run$1pair+run$hcard;
			run$prbest = max_pokval_count/run$total;
			run$pokervalmax = max_pokval;
			run_pokervalmax_count = max_pokval_count;
			run$clocks = ecount.LowPart - bcount.LowPart;

			//////////////////////////////////////////////////////////////
			// ron symbols

			// time it
			QueryPerformanceCounter(&bcount);

			// player cards
			nDeadCards = nComCards = 0;
			CardMask_RESET(DeadCards);
			for (i=0; i<=1; i++) {
				if (scraper.card_player[(int) symbols.sym.userchair][i] != CARD_BACK &&
					scraper.card_player[(int) symbols.sym.userchair][i] != CARD_NOCARD) {
					CardMask_SET(DeadCards, scraper.card_player[(int) symbols.sym.userchair][i]);
					nDeadCards++;
				}
			}

			// common cards
			CardMask_RESET(ComCards);
			if (symbols.sym.br>=2) {
				for (i=0; i<=2; i++) {
					if (scraper.card_common[i] != CARD_BACK && scraper.card_common[i] != CARD_NOCARD) {
						CardMask_SET(DeadCards, scraper.card_common[i]);
						CardMask_SET(ComCards, scraper.card_common[i]);
						nDeadCards++;
						nComCards++;
					}
				}
			}
			if (symbols.sym.br>=3) {
				if (scraper.card_common[3] != CARD_BACK && scraper.card_common[3] != CARD_NOCARD) {
					CardMask_SET(DeadCards, scraper.card_common[3]);
					CardMask_SET(ComCards, scraper.card_common[3]);
					nDeadCards++;
					nComCards++;
				}
			}
			if (symbols.sym.br>=4) {
				if (scraper.card_common[4] != CARD_BACK && scraper.card_common[4] != CARD_NOCARD) {
					CardMask_SET(DeadCards, scraper.card_common[4]);
					CardMask_SET(ComCards, scraper.card_common[4]);
					nDeadCards++;
					nComCards++;
				}
			}

			// setup the counters
			for (i=0; i<=9; i++) { hv_totals[i] = 0; }
			max_pokval = max_pokval_count = 0;

			// Enumerate through all possible opponent hands (excludes player and common cards)
			for (ocard[0]=0; ocard[0]<=50; ocard[0]++) {
				for (ocard[1]=ocard[0]+1; ocard[1]<=51; ocard[1]++) {
					if (!CardMask_CARD_IS_SET(DeadCards, ocard[0]) &&
						!CardMask_CARD_IS_SET(DeadCards, ocard[1])) {

						// All dead cards (includes opp cards too)
						AllDeadCards = DeadCards;
						CardMask_SET(AllDeadCards, ocard[0]);
						CardMask_SET(AllDeadCards, ocard[1]);

						ENUMERATE_N_CARDS_D(ExtraCards, 5-nComCards, AllDeadCards,
							{
								CardMask_OR(EvalCards, ComCards, ExtraCards);
								CardMask_SET(EvalCards, ocard[0]);
								CardMask_SET(EvalCards, ocard[1]);
								hv = Hand_EVAL_N(EvalCards, 7);
								pokval = symbols.calc_pokerval(hv, 7, &dummy, CARD_NOCARD, CARD_NOCARD);

								if (pokval>max_pokval) { max_pokval = pokval; max_pokval_count=1; }
								else if (pokval==max_pokval) { max_pokval_count++; }
								
								if (HandVal_HANDTYPE(hv)==HandType_STFLUSH && StdDeck_RANK(HandVal_TOP_CARD(hv))==Rank_ACE) { ++hv_totals[9]; }
								else { ++hv_totals[HandVal_HANDTYPE(hv)]; }
							});
					}
				}
			}

			// time it
			QueryPerformanceCounter(&ecount);

			// populate symbols
			ron$royfl = hv_totals[9];
			ron$strfl = hv_totals[8];
			ron$4kind = hv_totals[7];
			ron$fullh = hv_totals[6];
			ron$flush = hv_totals[5];
			ron$strai = hv_totals[4];
			ron$3kind = hv_totals[3];
			ron$2pair = hv_totals[2];
			ron$1pair = hv_totals[1];
			ron$hcard = hv_totals[0];
			ron$total = ron$royfl+ron$strfl+ron$4kind+ron$fullh+ron$flush+
						ron$strai+ron$3kind+ron$2pair+ron$1pair+ron$hcard;
			ron$prbest = max_pokval_count/ron$total;
			ron$pokervalmax = max_pokval;
			ron_pokervalmax_count = max_pokval_count;
			ron$clocks = ecount.LowPart - bcount.LowPart;

		}

		// NUTS probabilities
		if (run$royfl>ron$royfl) { run$prnuts+=(run$royfl-ron$royfl); }
		else if (run$strfl>ron$strfl) { run$prnuts+=(run$strfl-ron$strfl); }
		else if (run$4kind>ron$4kind) { run$prnuts+=(run$4kind-ron$4kind); }
		else if (run$fullh>ron$fullh) { run$prnuts+=(run$fullh-ron$fullh); }
		else if (run$flush>ron$flush) { run$prnuts+=(run$flush-ron$flush); }
		else if (run$strai>ron$strai) { run$prnuts+=(run$strai-ron$strai); }
		else if (run$3kind>ron$3kind) { run$prnuts+=(run$3kind-ron$3kind); }
		else if (run$2pair>ron$2pair) { run$prnuts+=(run$2pair-ron$2pair); }
		else if (run$1pair>ron$1pair) { run$prnuts+=(run$1pair-ron$1pair); }
		else if (run$hcard>ron$hcard) { run$prnuts+=(run$hcard-ron$hcard); }
		run$prnuts = run$prnuts / run$total;

		if (ron$royfl>run$royfl) { ron$prnuts+=(ron$royfl-run$royfl); }
		else if (ron$strfl>run$strfl) { ron$prnuts+=(ron$strfl-run$strfl); }
		else if (ron$4kind>run$4kind) { ron$prnuts+=(ron$4kind-run$4kind); }
		else if (ron$fullh>run$fullh) { ron$prnuts+=(ron$fullh-run$fullh); }
		else if (ron$flush>run$flush) { ron$prnuts+=(ron$flush-run$flush); }
		else if (ron$strai>run$strai) { ron$prnuts+=(ron$strai-run$strai); }
		else if (ron$3kind>run$3kind) { ron$prnuts+=(ron$3kind-run$3kind); }
		else if (ron$2pair>run$2pair) { ron$prnuts+=(ron$2pair-run$2pair); }
		else if (ron$1pair>run$1pair) { ron$prnuts+=(ron$1pair-run$1pair); }
		else if (ron$hcard>run$hcard) { ron$prnuts+=(ron$hcard-run$hcard); }
		ron$prnuts = ron$prnuts / ron$total;


#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CRunRon::get_counts :\n"); 
		throw;
	}
#endif
}