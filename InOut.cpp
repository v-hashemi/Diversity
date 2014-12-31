#include "InOut.h"

string ito3(int x, int l) {
	//	string s(l, '<');
	string s(l, '0');
	int d, i = l;
	while (x > 0) {
		d = x % 3;
		x /= 3;
		i--;
		//		if (d == 2) s[i] = '>'; else if (d == 1) s[i] = '=';
		s[i] = char(d + 48);
	}
	return s;
}


int Comp2::comp2p(vector<double>& p1, vector<double>& p2) {
	int c, s = 0;
	int j, k = cmpL.size();
	double d;
	for (int ii = 0; ii < k; ii++) {
		j = cmpL[ii];
		d = p1[j] - p2[j];
		if (d > 0) c = 2; else if (d < 0) c = 0; else c = 1;
		s = 3 * s + c;
	}
	return s;
}


void Comp2::OutComp2s(vector<ProfRep>& rep, string fileName, int K) {
	ofstream f;
	f.open(fileName.c_str());

	int L = cmpL.size(), LL = int((pow(3, L) - 1) / 2);

	int i, j;
	vector<int> cmpC(int(pow(3, L)), 0);
	for (i = 0; i < K; i++)
	    for (j = i + 1; j < K; j++)
			cmpC[comp2p(rep[i].x, rep[j].x)]++;

	for (i = 0; i < LL; i++) {
		j = cmpC[i] + cmpC[2 * LL - i];
		if (j > 0) f << ito3(i, L) << " " << j << endl;
	}
	if (cmpC[LL] > 0) f << ito3(LL, L) << " " << cmpC[LL] << endl;
	f.close();
}





int ReadFile(vector<List>& ppl, string fileName, int m = 5) {
	ifstream f;
	f.open(fileName.c_str());
	List p(m);
	int i, j, k, t, n, mp, np, nr, ii = 0;
	f >> mp >> n >> np;
	ppl.clear();
	//	ppl.resize(n, List(m, 0));
	for (i = 0; i < np; i++) {
		f >> nr;
		k = 0;
		for (j = 0; j < mp; j++) {
			f >> t;
			if (t < m) p[k++] = t;
		}
		for (j = 0; j < nr; j++)
			ppl.push_back(p);
		//			ppl[ii++] = p;
	}

	n = ppl.size();
	cout << n << endl;
	return np;
}



void OutRep(vector<ProfRep>& rep, vector<int>& sf, string folderName, ostream& ff) {
	ofstream f;
	stringstream ss;
	int i, j, k, l, ie, feq, ii, indx, N = rep.size(), nV = rep[0].v.size(), nEq = nV * (nV - 1) / 2 + 1;
	map<int, indxRep> rr;
	indxRep r, *rp;
	vector<map<double, int> > rnk(sf.size());

	ff << "PDI" << "\t" << "Range" << " \t: " << "min" << " \t " << "max" << "\t\t" << "Levels" << endl;

	for (ii = 0; ii < sf.size(); ii++) {
		f.open((folderName + fAll[sf[ii]].second + ".txt").c_str());

		rr.clear();
		for (i = 0; i < N; i++) {
			indx = int(rep[i].x[ii]);

			rp = &rr[indx];
			if (rp->n == 0) *rp = indxRep(nV, nEq);
			r = *rp;

			r.n++;
			r.tie[nV] += rep[i].isTie;
			r.cw += rep[i].isCW;
			r.cc += rep[i].isCC;
			r.ccw += rep[i].isCCW;

			ie = 0;
			feq = 0;
			double teqs = 1;
			for (j = 0; j < nV; j++) {
				if (rep[i].vs[j].size() > 1) r.tie[j]++;
				r.rg[0][j].v += rep[i].rg[j];
				r.rg[0][j].min = min(r.rg[0][j].min, rep[i].rg[j]);
				r.rg[0][j].max = max(r.rg[0][j].max, rep[i].rg[j]);
				for (k = j + 1; k < nV; k++, ie++) {
					if (rep[i].v[j] == rep[i].v[k]) {
						r.eq[0][ie]++;
						if (!rep[i].isTie) r.eq[2][ie]++;
					}
					else feq++;
					l = numCom(rep[i].vs[j], rep[i].vs[k]);
					r.eq[1][ie] += double(l) / (rep[i].vs[j].size() * rep[i].vs[k].size());
					//					r.eq[1][ie] += double(l) / sqrt(rep[i].vs[j].size() * rep[i].vs[k].size());
				}

				teqs *= rep[i].vs[j].size();
			}
			if (!feq) {
				r.eq[0][ie]++;
				if (!rep[i].isTie) r.eq[2][ie]++;
			}
			r.eq[1][ie] += numCom(rep[i].vs[0], rep[i].vs[1], rep[i].vs[2]) / teqs;   /// ???????	
			//			r.eq[1][ie] += (numCom(rep[i].vs[0], rep[i].vs[1], rep[i].vs[2]) / pow(teqs, 1.0 / (nEq - 1)));   /// ???????	
			r.indx = indx;
			*rp = r;
		}


		int minId = rr.begin()->first, maxId = rr.rbegin()->first;
		ss << fAll[sf[ii]].second << "\t" << 1 + maxId - minId << " \t: " << minId << " \t " << maxId << "\t\t" << rr.size() << endl;
		fCout(ff, ss);

		int t = 1;
		for (map<int, indxRep>::iterator it = rr.begin(); it != rr.end(); it++) {
			OutR(it->second, f);
			rnk[ii][it->second.indx] = t;
			t += it->second.n;
		}

		f.close();
	}

	vector<double> tx(sf.size());
	for (i = 0; i < N; i++) {
		transform(rep[i].x.begin(), rep[i].x.end(), rnk.begin(), tx.begin(), [N](double x, map<double, int> r) {return ceil(100 * r[x] / N); });
//		transform(rep[i].x.begin(), rep[i].x.end(), rnk.begin(), tx.begin(), [](double x, map<double, int> r) {return int(x / r[x]); });
		//		::Out(tx, cout, "\t");
		transform(rep[i].x.begin(), rep[i].x.end(), rnk.begin(), tx.begin(), [N](double x, map<double, int> r) {
			double mn = r.begin()->first, mx = r.rbegin()->first, d = mx - mn;
			return int(100 * (d > 0 ? (x - mn) / d : 1)); });
			//		::Out(tx, cout, "\t");
			//		cout << endl;
	}
}

void OutR(indxRep& r, ostream& f) {
	int i, nV = r.rg[0].size(), nEq = r.eq[0].size();

	f << r.indx << "\t" << r.n << "\t";
	for (i = 0; i < nV + 1; i++)
		f << 100 - per(r.tie[i], r.n) << "\t";
	f << per(r.cw, r.n) << '\t' << per(r.cc, r.n) << '\t' << per(r.ccw, r.n) << '\t';
	for (i = 0; i < nEq; i++)
		f << per(r.eq[0][i], r.n) << "\t" << per(r.eq[1][i], r.n) << "\t" << per(r.eq[2][i], r.n - r.tie[nV]) << "\t";
	for (i = 0; i < nV; i++)
		f << 100 - per(r.rg[0][i].v, r.n) << "\t" << int(r.rg[0][i].max - r.rg[0][i].min) << "\t";
	f << endl;
}

string FolderName(int m, int n, int N, int typ, string dataFile, string r = "") {
	string s = "IC";
	if (typ == 2) s = "RW"; else if (!typ) s = dataFile.substr(0, dataFile.find('.'));
	if (r != "") {
		_mkdir(r.c_str());
		r += "\\";
	}
	string folderName = r + "[" + s + "]m" + iStr(m) + "n" + iStr(n) + "(" + dig3(iStr(N)) + ")\\";
	_mkdir(folderName.c_str());
	return folderName;
}
