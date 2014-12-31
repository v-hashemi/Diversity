#ifndef _PRF_
#define _PRF_

#include "Def.h"
#include "myLib.h"

//Preference class
struct Prf {
	List p;    // Order of alternatives (0, ..., m-1) in the preference
	List pos;  // Positions of alternatives (dual of p)

	vector<setList> sup;  // Support sets of the preference, for different k's. sup[0] is sup2, sup[1] is sup3, ....
	int m;

	Prf() {m = 0;}
	Prf(const List& p0) {reset(p0);}

	// Sets the preference and correspondingly pos. Does not set any support-set list here, it is only set to be filled for the (sup2 and sup3) later.
	void reset(const List& p0) {p = p0; m = p.size(); sup.clear(); sup.resize(2); PosList();}

	// Generates a random preference over (0, ..., m-1).
	void rndGen(int m0 = 0);

	// Sets pos (the positions of alternatives in the preference).
	void PosList();

	// Calculate the k_support-set of p, returns it by s.
	static void Support(const List& p, setList& s, int k = 2);
	// Returns k_support-set of p.
	static setList Support(const List& p, int k = 2) { setList s;  Support(p, s, k);  return s;	}
	// Returns k_support-set of the preference. It is originally stored in the sup vector of the preference.
	setList& Support(int k) {
		if (k-1 > sup.size()) sup.resize(k-1);
		if (sup[k-2].empty()) Support(p, sup[k-2], k);
		return sup[k-2];
	}


	static double dK(setList& r1, setList& r2); // Kendall tau distance of two sets of relations
	// Distances of two profiles:
	static double dD(Prf& p, Prf& q) {if (p.p != q.p) return 1; return 0;} // Discrete
	static double dK(Prf& p, Prf& q);  // Kendall tau
	static double dK3(Prf& p, Prf& q); // 3_Kendall tau
	static double dS(Prf& p, Prf& q);  // Spearman footrule
	static double dM(Prf& p, Prf& q);  // Maximum displacement

	static double dHam(Prf& p, Prf& q); // Hamming
	static double dWHam(Prf& p, Prf& q, const vector<double>& w); // Weighted Hamming (w is the vector of the weights)
	static double dWHam(Prf& p, Prf& q) { return dWHam(p, q, hw); } // Weighted Hamming (based on hw)
	static vector<double> hw;
	static void setHW(const vector<double>& w) {hw = w;}


	void Out(ostream& fout = cout) {::Out(p, fout, " ", 1);}
};

#endif
