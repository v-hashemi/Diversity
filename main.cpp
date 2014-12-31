#include "Def.h"
#include "Prf.h"
#include "Profile.h"
#include "InOut.h"

#include <ctime>


// p.exe [m n N [dataFile]]
int main(int argc, char* argv[]) {
	double t = clock();
	ofstream f;

	vector<int> fPDI = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },//fillSeq(16),  // List of PDI functions to be used.
				vRule = fillSeq(3);                                                             // List of voting methods to be used.
	vector<int> cmpL = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };  // List of (index of) indices that are going to be compared in the comp2p function.

// N: number of samples, m: alternatives, n: voters
	int N = 1000, m = 5, n = 49;
	if (argc >= 4) { m = atoi(argv[1]); n = atoi(argv[2]); N = atoi(argv[3]);}
	string folderName, dataFile = string("a153.txt");
	if (argc == 5) dataFile = string(argv[4]);

	// rnd: 0 to read from the datafile for sampling from that, 1 for impartial culture [IC] and 2 for random walk from unanimous profile [RW].
	int rnd = 1;
	if (argc == 5) rnd = 0;

	folderName = FolderName(m, n, N, rnd, dataFile, "Results");
	f.open((folderName + "_report.txt").c_str());

	vector<string> s(fPDI.size());
	transform(fPDI.begin(), fPDI.end(), s.begin(), [](int i){return fAll[i].second; });

	stringstream ss;
	ss << folderName << endl;
	Out(s, ss);  ss << endl;
	fCout(f, ss);

//---------------------------------------------------------------------
	Profile P(fPDI, vRule, n, m);
	Comp2 cmp(cmpL, fPDI);
	vector<ProfRep> rep(N);
		
	vector<List> ppl;
	if (!rnd) ReadFile(ppl, dataFile, m);

	if (rnd == 2) P.unanimous(n, m);
	for (int i = 0; i < N; i++) {
		if (rnd == 0)
			P.sampleGen(ppl, n);
		else if (rnd == 1)
			P.rndGen(n, m);

		rep[i] = P.Report();

		if (rnd == 2)
			P.rndSwp();
	}

	OutRep(rep, fPDI, folderName, f);
//---------------------------------------------------------------------
	ss << endl << endl << (clock() - t) / 1000.0 << " seconds" << endl;  fCout(f, ss);

	t = clock();
	cmp.OutComp2s(rep, folderName + "_in.txt", N);

//=====================================================================
	ss << endl << endl << (clock() - t) / 1000.0 << " seconds" << endl;  fCout(f, ss);

	f.close();
	return 0;
}
