#ifndef _PROFILE_
#define _PROFILE_

#include "Def.h"
#include "myLib.h"
#include "Prf.h"

struct Profile;
typedef double(Profile::*fFunction)();
typedef List(Profile::*vFunction)();

// struct to store all the calculated values for a profile.
struct ProfRep {
	vector<double> x; // Values for all PDI's (in the order of fPDI)
	vector<List> vs;  // List of winners (for each voting method)
	vector<idType> v; // The winner (for each voting method)
	vector<double> rg;// Regret values of voting methods (it is normalized: [0, 1]).
	bool isTie, isCW, isCC, isCCW;  // There is: tie (for at least one voting method), Condorcet winner, Condorcet cycle, CC but no CW.
};

struct regType {
	double v, min, max;
	regType() {	v = 0; min = INF; max = 0;}
};

// struct for the overall report of each index value for any PDI.
struct indxRep {   // 3: rnd, set, noTie !!
	double indx; // Value of the index
	int n;       // Number of profiles with this index value.
	vector<int> tie; // Number of tie cases, for each voting rule and the last one is the number of cases with at least one tie.
	int cw, cc, ccw; // Number of cases of: Condorcet winner, Condorcet cycle, CC but no CW.
	vector<vector<double> > eq;   // Number of cases with equal results: (for each pairs of methods and all together)
	                              // eq[0]: equal winner (maybe randomly), eq[1]:sum of degree of agreement, eq[2]: perfectly equal  //!!??
	vector< vector<regType> > rg; // min, max and sum of the regret value for each voting method
	indxRep(int nV = 0, int nEq = 0) {
		n = 0;
		cw = cc = ccw = 0;
		tie.resize(nV+1, 0);
		eq.resize(3, vector<double>(nEq, 0));
		rg.resize(3, vector<regType>(nV));
	}
};
//:Total :  1 + (nV+1) + (3*nEq) + (3*nV*3)
//:Calc. :  1 + (nV+1) + (3*nEq) + (1*nV*3)
//:Out   :  1 + (nV+1) + (3*nEq) + (1*nV*2)     = 23   //range for (min, max)
//:Chart :  1 + (nV+1) + (3*nEq) + (1*nV*1)     = 20   //no range


struct Profile {
	vector<Prf> p; // Preferences
	int n, m;
	vector<vector<int> > pr; // Pairwise comparison tbale (m * m). pr[i][j]: number of voters prefer i to j.
	vector<int> copV;        // Copeland value of (m) alternatives. copV[i]: difference of pairwise wins (+) and loss (-) of i against others.
	setList sMG;             // Set of Majority Graph relations. {i, j} in sMG <==> i beats j in pairwise comparison.
	vector<int> plV;         // Plurality value of (m) alternatives. plV[i]: number of preferences with i at the top.

	Profile(vector<int>& f, vector<int>& v, int n0 = 0, int m0 = 0) { n = n0; m = m0;  Reset(f, v); Prf::setHW(fillSeq<double>(m, m, -1)); }

	void reset(int n0 = 0, int m0 = 0) { n = n0; m = m0; p.clear(); p.resize(n, List(m, 0)); pr.clear(); copV.clear(); sMG.clear(); plV.clear(); }
	void reset(vector<Prf>& p0) { reset(p0.size(), p0[0].m);  p = p0;}

	// Randomly generates a profile of n voters over m alternatives [IC]. Will use the current value of n and m as default.
	void rndGen(int n0 = 0, int m0 = 0);
	// Randomly chooses a sample of size n from the preferences of the population ppl. Will use the current value of n as default.
	void sampleGen(vector<List>& ppl, int n0 = 0);
	// Randomly swaps two adjacent alternatives of one of the preferences [RW].
	void rndSwp();
	// Produces the unanimous profile of n voters over m alternatives (the unanimous profile is: 0 > 1 > ... > m-1).
	void unanimous(int n, int m);

	// Calculates the sum of pairwise distances of the profile, the argument is the distance function. 
	double df(double (*f)(Prf&, Prf&));
	// Calculates the sum of compromise-based distances of the profile from the preference c, the argument is the distance function. 
	double cb(Prf& c, double(*f)(Prf&, Prf&));
	// Calculates the sum of compromise-based (Kendall tau) distances of the profile from the relation r. (Other distances are not applicable).
	double cr(setList& r);

	// Functions for different PDI's (not normalized, currently all the returned values are integer):

	// Distance-based PDI's:
  	  // Aggregation operator: Sum
	double dD()  {return df(Prf::dD);}  // Discrete
	double dK()  {return df(Prf::dK);}  // Kendall tau
	double dK3() {return df(Prf::dK3);} // 3_Kendall tau
	double dS()  {return df(Prf::dS);}  // Spearman footrule
	double dM()  {return df(Prf::dM);}  // Maximum displacement

	double dHam() { return df(Prf::dHam); }   // Hamming
	double dWHam() { return df(Prf::dWHam); } // Weighted Hamming

	  // Aggregation operator: max
	double dKmax(); // Kendall tau

	// Compromise-based PDI's:
	double cbPlK() {return cb(Prf(Rank(Plurality())), Prf::dK); } // Plurality
	double cbBrK() {return cb(Prf(Rank(Borda())), Prf::dK); }     // Borda
	double cbCpK() {return cb(Prf(Rank(Copeland())), Prf::dK); }  // Copeland

	double majK() { return cr(mgR()); }  // Majority graph

	// Support-based PDI's:
	double Sup2() {return Sup(2);}
	double Sup3() {return Sup(3);}
	double Supm() {return Sup(m);}  // = Sups  // Is this really faster than Sups? why? !!??
	double Sups() {setList s; for (int i = 0; i < n; i++) s.insert(p[i].p); return s.size();} // Simple support-based

	// A random dummy PDI!
	double Dummy() { return rand() % (100 * abs(p[0].p[0]-p[1].p[0]) + 1); }

	// Produces the majority graph (sMG). Also, calculates the pairwise comparison tbale (pr) and the Copeland values (copV).
	void prMaj();
	// Returns the majority graph (sMG).
	setList& mgR();

	// General function for support-based PDI's
	double Sup(int k);

	// Returns the (index of) member of v with highest value. If there are more than one, if noTie is 1 returns -1 otherwise returns one of them randomly.
	static idType Winner(const List& v, bool noTie = 0);
	// Returns the set of (indices of) members of v with highest value.
	static List Winners(const List& v);
	// Returns the sorted list of the alternatives based on their points (v).
	static List Rank(const List& v);

	// Voting methods:
	// Each function returns the points of alternatives based on the corresponding method.
	List Plurality();
	List Borda();
	List Copeland();
	List Runoff();

	// Checks the existence of Condorcet cycle.
	bool isCC();

	// Calculates the overall regret of the voters if w is the winner.
	double regret(idType w);
	void Out(ostream& fout = cout);

	// Reports all the calculated parameters for the profile.
	// noTie is not used!
	ProfRep Report(bool noTie = 0);

	// Sets the lists of PDI's to be calculated and voting methods to be used for agreement between the methods and voters' regret.
	void Reset(vector<int>& f, vector<int>& v);

	vector<fFunction> fPDI;  // PDI's to be calculated.
	vector<vFunction> vRule; // Voting rules to be used.

//	static void OutR(indxRep& r, ostream& f = cout);
//	static void OutRep(vector<ProfRep>& rep, string folderName = "", ostream& ff = cout);
};


// list of all PDIs and voting methods
static vector<pair<fFunction, string> > fAll = { { &Profile::dD, "dD" }, { &Profile::dK, "dK" }, { &Profile::dKmax, "dKmax" }, { &Profile::dK3, "dK3" },
                                                  { &Profile::dS, "dS" }, { &Profile::dM, "dM" },
												  { &Profile::Sup2, "Sup2" }, { &Profile::Sup3, "Sup3" }, { &Profile::Sups, "SupS" }, 
												  { &Profile::dHam, "dHam" }, { &Profile::dWHam, "dWHam" },
												  { &Profile::cbPlK, "cbPlK" }, { &Profile::cbBrK, "cbBrK" }, { &Profile::cbCpK, "cbCpK" }, { &Profile::majK, "majK" }, { &Profile::Dummy, "Dummy" }
};

static vector<pair<vFunction, string> > vAll = { { &Profile::Plurality, "Plurality" }, { &Profile::Borda, "Borda" }, { &Profile::Copeland, "Copeland" } };

/*
const vector<string> sfAll = { "dD", "dK", "dKmax", "dK3", "dS", "dM", "Sup2", "Sup3", "Sups", "dHam", "dWHam", "cbPlK", "cbBrK", "cbCpK", "majK", "Dummy" };
static fFunction fAll[] = {&Profile::dD, &Profile::dK, &Profile::dKmax, &Profile::dK3, &Profile::dS, &Profile::dM,
                           &Profile::Sup2, &Profile::Sup3, &Profile::Sups, &Profile::dHam, &Profile::dWHam, &Profile::cbPlK, &Profile::cbBrK, &Profile::cbCpK, &Profile::majK, &Profile::Dummy};

const vector<string> svAll = { "Plurality", "Borda", "Copeland" };
static vFunction vAll[] = {&Profile::Plurality, &Profile::Borda, &Profile::Copeland};
*/
#endif
