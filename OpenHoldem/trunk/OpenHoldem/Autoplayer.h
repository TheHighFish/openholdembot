#pragma once

class Autoplayer {
public:
	Autoplayer(void);
	~Autoplayer(void);

	void do_autoplayer(void);

private:
	int		alli_but, rais_but, call_but, chec_but, fold_but;
	int		autopost_but, sitin_but, sitout_but, leave_but, prefold_but;
	bool	autopost_state, sitin_state, sitout_state;
	int		i86_but, i86X_but[10];
	bool	i86_state, i86X_state[10];

	int count_same_scrapes(void);
	void do_swag(void);
	void do_arccf(void);
	void do_prefold(void);
	void get_r$_button_indices(void);
	void check_bring_keyboard(void);
	void do_f$play(void);
	void do_i86(void);
	POINT randomize_click_location(int left, int top, int right, int bottom);
	void get_click_point(int x, int y, int rx, int ry, POINT *p);
	double randomNormalScaled(double scale, double m, double s);
	double randomNormal(double m, double s);
	//  2008.02.27 by THF
	void do_Chat(void);
};
