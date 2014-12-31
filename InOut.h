#ifndef _INOUT_
#define _INOUT_

#include <direct.h>

#include "Def.h"
#include "Profile.h"

// To print s on cout and file f, simultaneously.
#define fCout(f, s) {cout << s.str(); f << s.str(); s.str(string()); s.clear(); }

inline string iStr(int x) { return static_cast<ostringstream*>( &(ostringstream() << x) )->str();}  // int to_sting!
inline string dig3(string s) { if (s.size() > 3) return dig3(s.substr(0, s.size() - 3)) + "," + s.substr(s.size() - 3, 3); return s; } // Returns comma separated s.


string ito3(int x, int l);  // Returns a string of length l, which is x in base 3.

// Reads a list of preferences from fileName to ppl.
// It is assumed that the alternative names are 0, 1, ... and it ignores the ones beyond the first m (0, ..., m-1).   !!!!!!!!!!!!!!!!!: fix this
int ReadFile(vector<List>& ppl, string fileName, int m);// = 5);


void OutR(indxRep& r, ostream& f = cout);
void OutRep(vector<ProfRep>& rep, vector<int>& sf, string folderName = "", ostream& ff = cout);

// Returns folder name for the results of the corresponding scenario, e.g., "Results\[IC]m5n50(10,000)\".
// Based on typ, [s] is [IC] or [RW] (for typ = 1 or 2) and if typ = 0, s is the name of dataFile.
// It makes the folder r if it does not exist.
string FolderName(int m, int n, int N, int typ, string dataFile, string r);// = "");

// Class for comparing pairs of profiles based on different PDIs.
class Comp2 {
	vector<int> cmpL;
	bool flg;
public:
	// Sets cmpL based on cl (compare list) and fl (function list).
	Comp2(const vector<int>& cl, const vector<int>& fl) { flg = SubsetIndx(fl, cl, cmpL); }

	// Compares the indices of the two preferences. Inputs are list of values of all indices.
	// Returns an integer (of base 3), for each pair (2, 1 or 0) represent (>, =, <) respectively.
	// The indices are compared in the order of cmpL.
	int comp2p(vector<double>& p1, vector<double>& p2);

	// Compares all pairs of (first K) preferences in rep by their indices.
	// Prints the overall result to fileName, i.e., for each siuation (written as base 3 number) the number of its occurrences.
	void OutComp2s(vector<ProfRep>& rep, string fileName, int K);
};

#endif
