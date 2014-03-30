#pragma once

class CRunRon {
public:
	CRunRon(void);
	~CRunRon(void);
	void get_counts(void);

	double ron$royfl;
	double ron$strfl;
	double ron$4kind;
	double ron$fullh;
	double ron$flush;
	double ron$strai;
	double ron$3kind;
	double ron$2pair;
	double ron$1pair;
	double ron$hcard;
	double ron$total;
	double ron$pokervalmax;
	double ron$prnuts;
	double ron$prbest;
	double ron$clocks;
	double ron_pokervalmax_count;

	double run$royfl;
	double run$strfl;
	double run$4kind;
	double run$fullh;
	double run$flush;
	double run$strai;
	double run$3kind;
	double run$2pair;
	double run$1pair;
	double run$hcard;
	double run$total;
	double run$pokervalmax;
	double run$prnuts;
	double run$prbest;
	double run$clocks;
	double run_pokervalmax_count;

private:
	double run_count[13][13][10];
	double run_max_pokval[13][13];
	double run_max_pokval_n[13][13];
	double ron_count[13][13][10];
	double ron_max_pokval[13][13];
	double ron_max_pokval_n[13][13];

};
