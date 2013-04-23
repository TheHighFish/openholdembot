// prwinnow_table.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "poker_defs.h"
#include "inlines/eval.h"
#include <conio.h>
#include <io.h>
#include <fcntl.h>

double calc_pokerval(HandVal hv, int n);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

void do_plcard_enum(CardMask comCards, int ncomCards);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int				i, j, k, nRetCode = 0;
	CardMask		DeadCards, ExtraCards, EvalCards;
	HandVal			hv;
	double			pokval, max_pokval[13][13], max_pokval_count[13][13];
	double			hv_totals[13][13][10], endings_total[13][13];
	int				hv_index1, hv_index2;
	FILE			*fp;
	char			pcard0, pcard1, ocard0, ocard1;
	double			endings;
	char			start=0, end=50;

	// for ron processing
	bool			completed[52][52];
	DWORD			nBytes;
	HANDLE			h;


	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		return 1;
	}

	// arguments for ron tables only (run tables execute quickly)
	// "runron_table.exe start end dump"
	// start is starting card (0 to 50)
	// end is ending card (start to 50)
	// dump - if present, will create ron.txt and skip calculation engine  (use last after all calcs are done)
	// ron calcs use file locking to keep a single file updated with progress.  can ctrl-c anytime and restart anytime
	// delete ron.bin to start from scratch


	printf("RUN TABLES\n\n");

	// setup the counters
	for (i=0; i<=12; i++) { 
		for (j=0; j<=12; j++) { 
			endings_total[i][j] = 0; 
			max_pokval[i][j] = 0; 
			max_pokval_count[i][j] = 0; 
			for (k=0; k<=9; k++) { 
				hv_totals[i][j][k] = 0; 
			}
		}
	}


	for (pcard0=0; pcard0<=50; pcard0++) {
		for (pcard1=pcard0+1; pcard1<=51; pcard1++) {
			printf("%d %d\n", pcard0, pcard1);

			// suited, store in hi/lo
			if (StdDeck_SUIT(pcard0) == StdDeck_SUIT(pcard1)) {
				if (StdDeck_RANK(pcard0) >= StdDeck_RANK(pcard1)) {
					hv_index1 = StdDeck_RANK(pcard0);
					hv_index2 = StdDeck_RANK(pcard1);
				}
				else {
					hv_index1 = StdDeck_RANK(pcard1);
					hv_index2 = StdDeck_RANK(pcard0);
				}
			}
			// unsuited, store in lo/hi
			else {
				if (StdDeck_RANK(pcard0) >= StdDeck_RANK(pcard1)) {
					hv_index1 = StdDeck_RANK(pcard1);
					hv_index2 = StdDeck_RANK(pcard0);
				}
				else {
					hv_index1 = StdDeck_RANK(pcard0);
					hv_index2 = StdDeck_RANK(pcard1);
				}
			}

			// Dead cards
			CardMask_RESET(DeadCards);
			CardMask_SET(DeadCards, pcard0);
			CardMask_SET(DeadCards, pcard1);

			endings = 0;
			ENUMERATE_N_CARDS_D(ExtraCards, 5, DeadCards,
				{
					endings++;
					CardMask_OR(EvalCards, DeadCards, ExtraCards);
					hv = Hand_EVAL_N(EvalCards, 7);
					pokval = calc_pokerval(hv, 7);

					if (pokval>max_pokval[hv_index1][hv_index2]) { 
						max_pokval[hv_index1][hv_index2] = pokval; 
						max_pokval_count[hv_index1][hv_index2]=1; 
					}
					else if (pokval==max_pokval[hv_index1][hv_index2]) { 
						max_pokval_count[hv_index1][hv_index2] = max_pokval_count[hv_index1][hv_index2] + 1; 
					}
					
					if (HandVal_HANDTYPE(hv)==HandType_STFLUSH && StdDeck_RANK(HandVal_TOP_CARD(hv))==Rank_ACE) { 
						++hv_totals[hv_index1][hv_index2][9]; 
					}
					else { 
						++hv_totals[hv_index1][hv_index2][HandVal_HANDTYPE(hv)]; 
					}
				});
			endings_total[hv_index1][hv_index2] = endings_total[hv_index1][hv_index2] + endings;
		}
	}

	fp = fopen("run.txt", "a");
	// pairs
	fprintf(fp, "double run_count[13][13][10];\n");
	fprintf(fp, "double run_max_pokval[13][13];\n");
	fprintf(fp, "double run_max_pokval_n[13][13];\n\n");
	fprintf(fp, "//POCKET PAIRS\n//============\n");
	for (i=12; i>=0; i--) {
		for (j=9; j>=0; j--) {
			fprintf(fp, "run_count[%d][%d][%d]=%.0f; ", i, i, j, hv_totals[i][i][j]/6);
		}
		fprintf(fp, "\n");
	}
	for (i=12; i>=0; i--) {
		fprintf(fp, "run_max_pokval[%d][%d]=%.0f; ", i, i, max_pokval[i][i]);
	}
	fprintf(fp, "\n");
	for (i=12; i>=0; i--) {
		fprintf(fp, "run_max_pokval_n[%d][%d]=%.0f; ", i, i, max_pokval_count[i][i]/6);
	}
	fprintf(fp, "\n\n");

	// suited
	fprintf(fp, "//SUITED HANDS\n//============\n");
	for (i=12; i>=1; i--) {
		for (j=i-1; j>=0; j--) {
			for (k=9; k>=0; k--) {
				fprintf(fp, "run_count[%d][%d][%d]=%.0f; ", i, j, k, hv_totals[i][j][k]/4);
			}
			fprintf(fp, "\n");
		}
	}
	for (i=12; i>=0; i--) {
		for (j=i-1; j>=0; j--) {
			fprintf(fp, "run_max_pokval[%d][%d]=%.0f; ", i, j, max_pokval[i][j]);
		}
	}
	fprintf(fp, "\n");
	for (i=12; i>=0; i--) {
		for (j=i-1; j>=0; j--) {
			fprintf(fp, "run_max_pokval_n[%d][%d]=%.0f; ", i, j, max_pokval_count[i][j]/4);
		}
	}
	fprintf(fp, "\n\n");

	// offsuit
	fprintf(fp, "//OFF SUIT HANDS\n//==============\n");
	for (i=12; i>=1; i--) {
		for (j=i-1; j>=0; j--) {
			for (k=9; k>=0; k--) {
				fprintf(fp, "run_count[%d][%d][%d]=%.0f; ", j, i, k, hv_totals[j][i][k]/12);
			}
			fprintf(fp, "\n");
		}
	}
	for (i=12; i>=0; i--) {
		for (j=i-1; j>=0; j--) {
			fprintf(fp, "run_max_pokval[%d][%d]=%.0f; ", j, i, max_pokval[j][i]);
		}
	}
	fprintf(fp, "\n");
	for (i=12; i>=0; i--) {
		for (j=i-1; j>=0; j--) {
			fprintf(fp, "run_max_pokval_n[%d][%d]=%.0f; ", j, i, max_pokval_count[j][i]/12);
		}
	}
	fprintf(fp, "\n\n");

	fclose(fp);

	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	if (argc==4) {
		printf("\nDumping RON TABLES\nCreating ron.txt\n\n");
			
		h = CreateFile("ron.bin", GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);

		if (h==INVALID_HANDLE_VALUE && GetLastError()==ERROR_FILE_NOT_FOUND) {
			printf("!!! ron.bin does not exist, this should not happen.  Exiting.\n");
			return 1;
		}
		
		ReadFile(h, completed, sizeof(completed), &nBytes, NULL);
		ReadFile(h, max_pokval, sizeof(max_pokval), &nBytes, NULL);
		ReadFile(h, max_pokval_count, sizeof(max_pokval_count), &nBytes, NULL);
		ReadFile(h, hv_totals, sizeof(hv_totals), &nBytes, NULL);
		ReadFile(h, endings_total, sizeof(endings_total), &nBytes, NULL);
		CloseHandle(h);

		for (i=0; i<=50; i++) {
			for (j=i+1; j<51; j++) {
				if (completed[i][j]==true) { printf("X"); }
				else { printf("."); }
			}
			printf("\n");
		}
		printf("\n");

		fp = fopen("ron.txt", "w");
		// pairs
		fprintf(fp, "double ron_count[13][13][10];\n");
		fprintf(fp, "double ron_max_pokval[13][13];\n");
		fprintf(fp, "double ron_max_pokval_n[13][13];\n\n");
		fprintf(fp, "//POCKET PAIRS\n//============\n");
		for (i=12; i>=0; i--) {
			for (j=9; j>=0; j--) {
				fprintf(fp, "ron_count[%d][%d][%d]=%.0f; ", i, i, j, hv_totals[i][i][j]/6);
			}
			fprintf(fp, "\n");
		}
		for (i=12; i>=0; i--) {
			fprintf(fp, "ron_max_pokval[%d][%d]=%.0f; ", i, i, max_pokval[i][i]);
		}
		fprintf(fp, "\n");
		for (i=12; i>=0; i--) {
			fprintf(fp, "ron_max_pokval_n[%d][%d]=%.0f; ", i, i, max_pokval_count[i][i]/6);
		}
		fprintf(fp, "\n\n");

		// suited
		fprintf(fp, "//SUITED HANDS\n//============\n");
		for (i=12; i>=1; i--) {
			for (j=i-1; j>=0; j--) {
				for (k=9; k>=0; k--) {
					fprintf(fp, "ron_count[%d][%d][%d]=%.0f; ", i, j, k, hv_totals[i][j][k]/4);
				}
				fprintf(fp, "\n");
			}
		}
		for (i=12; i>=0; i--) {
			for (j=i-1; j>=0; j--) {
				fprintf(fp, "ron_max_pokval[%d][%d]=%.0f; ", i, j, max_pokval[i][j]);
			}
		}
		fprintf(fp, "\n");
		for (i=12; i>=0; i--) {
			for (j=i-1; j>=0; j--) {
				fprintf(fp, "ron_max_pokval_n[%d][%d]=%.0f; ", i, j, max_pokval_count[i][j]/4);
			}
		}
		fprintf(fp, "\n\n");

		// offsuit
		fprintf(fp, "//OFF SUIT HANDS\n//==============\n");
		for (i=12; i>=1; i--) {
			for (j=i-1; j>=0; j--) {
				for (k=9; k>=0; k--) {
					fprintf(fp, "ron_count[%d][%d][%d]=%.0f; ", j, i, k, hv_totals[j][i][k]/12);
				}
				fprintf(fp, "\n");
			}
		}
		for (i=12; i>=0; i--) {
			for (j=i-1; j>=0; j--) {
				fprintf(fp, "ron_max_pokval[%d][%d]=%.0f; ", j, i, max_pokval[j][i]);
			}
		}
		fprintf(fp, "\n");
		for (i=12; i>=0; i--) {
			for (j=i-1; j>=0; j--) {
				fprintf(fp, "ron_max_pokval_n[%d][%d]=%.0f; ", j, i, max_pokval_count[j][i]/12);
			}
		}
		fprintf(fp, "\n\n");

		fclose(fp);

		return 1;

	}


	printf("\n\nRON TABLES\n\n");

	// setup the counters
	for (i=0; i<=12; i++) { 
		for (j=0; j<=12; j++) { 
			endings_total[i][j] = 0; 
			max_pokval[i][j] = 0; 
			max_pokval_count[i][j] = 0; 
			for (k=0; k<=9; k++) { 
				hv_totals[i][j][k] = 0; 
			}
		}
	}
	for (i=0; i<=51; i++) {
		for (j=0; j<=51; j++) {
			completed[i][j] = 0;
		}
	}


	// see if ron.bin exists, if not, create it
	h = CreateFile("ron.bin", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (h==INVALID_HANDLE_VALUE && GetLastError()==ERROR_FILE_NOT_FOUND) {
		printf("ron.bin does not exist, creating...");
		h = CreateFile("ron.bin", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, NULL, NULL);
		if (h==INVALID_HANDLE_VALUE) {
			printf("failed.\n");
			return 1;
		}
		else {
			WriteFile(h, completed, sizeof(completed), &nBytes, NULL);
			WriteFile(h, max_pokval, sizeof(max_pokval), &nBytes, NULL);
			WriteFile(h, max_pokval_count, sizeof(max_pokval_count), &nBytes, NULL);
			WriteFile(h, hv_totals, sizeof(hv_totals), &nBytes, NULL);
			WriteFile(h, endings_total, sizeof(endings_total), &nBytes, NULL);
			CloseHandle(h);
			printf("success.\n");
		}
	}
	// if it exists, load our progress
	else {
		ReadFile(h, completed, sizeof(completed), &nBytes, NULL);
		ReadFile(h, max_pokval, sizeof(max_pokval), &nBytes, NULL);
		ReadFile(h, max_pokval_count, sizeof(max_pokval_count), &nBytes, NULL);
		ReadFile(h, hv_totals, sizeof(hv_totals), &nBytes, NULL);
		ReadFile(h, endings_total, sizeof(endings_total), &nBytes, NULL);
	}

	// see if we have start/end arguments
	if (argc==3) {
		start = atoi(argv[1]);
		end = atoi(argv[2]);
	}

	// DO IT!
	for (pcard0=start; pcard0<=end; pcard0++) {
		for (pcard1=pcard0+1; pcard1<=51; pcard1++) {

			// If we have already processed this pcard0/pcard1, then skip it, otherwise, go do it!
			if (completed[pcard0][pcard1] == true) {
				printf("%d %d : Already complete.\n", pcard0, pcard1);
				CloseHandle(h);
			}
			else {
				printf("%d %d : Processing...", pcard0, pcard1);

				// suited, store in hi/lo
				if (StdDeck_SUIT(pcard0) == StdDeck_SUIT(pcard1)) {
					if (StdDeck_RANK(pcard0) >= StdDeck_RANK(pcard1)) {
						hv_index1 = StdDeck_RANK(pcard0);
						hv_index2 = StdDeck_RANK(pcard1);
					}
					else {
						hv_index1 = StdDeck_RANK(pcard1);
						hv_index2 = StdDeck_RANK(pcard0);
					}
				}
				// unsuited, store in lo/hi
				else {
					if (StdDeck_RANK(pcard0) >= StdDeck_RANK(pcard1)) {
						hv_index1 = StdDeck_RANK(pcard1);
						hv_index2 = StdDeck_RANK(pcard0);
					}
					else {
						hv_index1 = StdDeck_RANK(pcard0);
						hv_index2 = StdDeck_RANK(pcard1);
					}
				}

				// Enumerate through all possible opponent hands (excludes player cards)
				for (ocard0=0; ocard0<=50; ocard0++) {
					for (ocard1=ocard0+1; ocard1<=51; ocard1++) {
						if (ocard0!=pcard0 && ocard0!=pcard1 && ocard1!=pcard0 && ocard1!=pcard1) {
							//printf("\t%d %d\n", ocard0, ocard1);

							// Dead cards
							CardMask_RESET(DeadCards);
							CardMask_SET(DeadCards, pcard0);
							CardMask_SET(DeadCards, pcard1);
							CardMask_SET(DeadCards, ocard0);
							CardMask_SET(DeadCards, ocard1);

							endings = 0;
							ENUMERATE_N_CARDS_D(ExtraCards, 5, DeadCards,
								{
									endings++;
									EvalCards = ExtraCards;
									CardMask_SET(EvalCards, ocard0);
									CardMask_SET(EvalCards, ocard1);
									hv = Hand_EVAL_N(EvalCards, 7);
									pokval = calc_pokerval(hv, 7);

									if (pokval>max_pokval[hv_index1][hv_index2]) { 
										max_pokval[hv_index1][hv_index2] = pokval; max_pokval_count[hv_index1][hv_index2]=1; 
									}
									else if (pokval==max_pokval[hv_index1][hv_index2]) { 
										max_pokval_count[hv_index1][hv_index2] = max_pokval_count[hv_index1][hv_index2] + 1; 
									}
									
									if (HandVal_HANDTYPE(hv)==HandType_STFLUSH && StdDeck_RANK(HandVal_TOP_CARD(hv))==Rank_ACE) { 
										++hv_totals[hv_index1][hv_index2][9]; 
									}
									else { 
										++hv_totals[hv_index1][hv_index2][HandVal_HANDTYPE(hv)]; 
									}
								});
							endings_total[hv_index1][hv_index2] = endings_total[hv_index1][hv_index2] + endings;
						}
					}
				}

				// Update ron.bin with progress
				printf("complete.  Updating ron.bin...");
				h = CreateFile("ron.bin", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);

				while (h==INVALID_HANDLE_VALUE && GetLastError()==ERROR_SHARING_VIOLATION) {
					printf("\nron.bin being used by someone else.  Trying again in 3 seconds.\n");
					Sleep(3000);
					h = CreateFile("ron.bin", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
				}

				completed[pcard0][pcard1] = true;
				WriteFile(h, completed, sizeof(completed), &nBytes, NULL);
				WriteFile(h, max_pokval, sizeof(max_pokval), &nBytes, NULL);
				WriteFile(h, max_pokval_count, sizeof(max_pokval_count), &nBytes, NULL);
				WriteFile(h, hv_totals, sizeof(hv_totals), &nBytes, NULL);
				WriteFile(h, endings_total, sizeof(endings_total), &nBytes, NULL);

				CloseHandle(h);
				printf("done.\n");


			} // if (completed[pcard0][pcard1]

		} // for (pcard1=pcard0+1; pcard1<=51; pcard1++)

	} // for (pcard0=start; pcard0<=end; pcard0++)

	return 1;
}

double calc_pokerval(HandVal hv, int n) {
	double		pv = 0;

	double	hicard			= 0x00000001,
			onepair			= 0x01000000,
			twopair			= 0x02000000,
			threeofakind	= 0x04000000,
			straight		= 0x08000000,
			flush			= 0x10000000,
			fullhouse		= 0x20000000,
			fourofakind		= 0x40000000,
			straightflush	= 0x80000000,
			royalflush		= 0x800EDCBA,
			fiveofakind		= 0xFF000000;

	switch HandVal_HANDTYPE(hv) {
	case HandType_STFLUSH:
		pv += straightflush;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-0)<<16;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-1)<<12;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-2)<<8;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-3)<<4;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-4)<<0;
		break;
	case HandType_QUADS:
		pv += fourofakind;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<16;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<12;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<8;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<4;
		if (n>=5) { pv += (StdDeck_RANK(HandVal_SECOND_CARD(hv))+2)<<0; }
		break;
	case HandType_FULLHOUSE:
		pv += fullhouse;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<16;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<12;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<8;
		pv += (StdDeck_RANK(HandVal_SECOND_CARD(hv))+2)<<4;
		pv += (StdDeck_RANK(HandVal_SECOND_CARD(hv))+2)<<0;
		break;
	case HandType_FLUSH:
		pv += flush;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<16;
		pv += (StdDeck_RANK(HandVal_SECOND_CARD(hv))+2)<<12;
		pv += (StdDeck_RANK(HandVal_THIRD_CARD(hv))+2)<<8;
		pv += (StdDeck_RANK(HandVal_FOURTH_CARD(hv))+2)<<4;
		pv += (StdDeck_RANK(HandVal_FIFTH_CARD(hv))+2)<<0;
		break;
	case HandType_STRAIGHT:
		pv += straight;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-0)<<16;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-1)<<12;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-2)<<8;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-3)<<4;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2-4)<<0;
		break;
	case HandType_TRIPS:
		pv += threeofakind;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<16;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<12;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<8;
		if (n>=4) { pv += (StdDeck_RANK(HandVal_SECOND_CARD(hv))+2)<<4; }
		if (n>=5) { pv += (StdDeck_RANK(HandVal_THIRD_CARD(hv))+2)<<0; }
		break;
	case HandType_TWOPAIR:
		pv += twopair;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<16;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<12;
		pv += (StdDeck_RANK(HandVal_SECOND_CARD(hv))+2)<<8;
		pv += (StdDeck_RANK(HandVal_SECOND_CARD(hv))+2)<<4;
		if (n>=5) { pv += (StdDeck_RANK(HandVal_THIRD_CARD(hv))+2)<<0; }
		break;
	case HandType_ONEPAIR:
		pv += onepair;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<16;
		pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<12;
		if (n>=3) { pv += (StdDeck_RANK(HandVal_SECOND_CARD(hv))+2)<<8; }
		if (n>=4) { pv += (StdDeck_RANK(HandVal_THIRD_CARD(hv))+2)<<4; }
		if (n>=5) { pv += (StdDeck_RANK(HandVal_FOURTH_CARD(hv))+2)<<0; }
		break;
	case HandType_NOPAIR:
		if (n>=1) { pv += (StdDeck_RANK(HandVal_TOP_CARD(hv))+2)<<16; }
		if (n>=2) { pv += (StdDeck_RANK(HandVal_SECOND_CARD(hv))+2)<<12; }
		if (n>=3) { pv += (StdDeck_RANK(HandVal_THIRD_CARD(hv))+2)<<8; }
		if (n>=4) { pv += (StdDeck_RANK(HandVal_FOURTH_CARD(hv))+2)<<4; }
		if (n>=5) { pv += (StdDeck_RANK(HandVal_FIFTH_CARD(hv))+2)<<0; }
		break;
	}

	return pv;
}
