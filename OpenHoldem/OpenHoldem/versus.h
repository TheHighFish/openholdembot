#include "poker_defs.h"

extern class CVersus {
public:
	CVersus();
	bool get_counts(void);
	double get_symbol(const char *a, int *e);

private:
	int nhands, nwin, ntie, nlos, nhandshi, nhandsti, nhandslo;
	double vsprwin, vsprtie, vsprlos;
	double vsprwinhi, vsprtiehi, vsprloshi;
	double vsprwinti, vsprtieti, vsprlosti;
	double vsprwinlo, vsprtielo, vsprloslo;
	int	nlistwin[100], nlisttie[100], nlistlos[100];
	void do_calc(CardMask plCards, CardMask oppCards, CardMask comCards, 
				 unsigned int *wintemp, unsigned int *tietemp, unsigned int *lostemp);

} versus;
