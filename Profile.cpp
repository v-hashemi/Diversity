#include "Profile.h"


void Profile::rndGen(int n0, int m0) {
	if (n0 > 0) n = n0;
	if (m0 > 0) m = m0;
	reset(n, m);
//	p.resize(n);
	for (int i = 0; i < n; i++)
		p[i].rndGen(m);
//	reset(p);
}

void Profile::sampleGen(vector<List>& ppl, int n0) {
	if (n0 > 0) n = n0;
	m = ppl[0].size();
	reset(n, m);
	int np = ppl.size();
	for (int i = 0; i < n; i++)
		p[i].reset(ppl[rand() % np]);
}

void Profile::rndSwp() {
	pr.clear(); copV.clear(); sMG.clear(); plV.clear();
	int i = rand() % n;
	int j = rand() % (m - 1);
	swap(p[i].p[j], p[i].p[j + 1]);
	p[i].reset(p[i].p);
}

void Profile::unanimous(int n, int m) {
	reset(n, m);
	List v = fillSeq(m);
	for (int i = 0; i < n; i++)
		p[i].reset(v);
}

void Profile::Out(ostream& fout) {
	for (int i = 0; i < n; i++)
		p[i].Out(fout);
	fout << endl;
}


double Profile::df(double (*f)(Prf&, Prf&)) {
	double t = 0;
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			t += f(p[i], p[j]);
	return t;
}

double Profile::cb(Prf& c, double(*f)(Prf&, Prf&)) {
	double t = 0;
	for (int i = 0; i < n; i++)
		t += f(c, p[i]);
	return t;
}

double Profile::cr(setList& r) {
	double t = 0;
	for (int i = 0; i < n; i++)
		t += Prf::dK(r, p[i].Support(2));
	return t;
}

double Profile::dKmax() {
	double t = 0;
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			t = max(t, Prf::dK(p[i], p[j]));

	return t;
}


double Profile::Sup(int k) {
	setList s, *sk;
	for (int i = 0; i < n; i++) {
		sk = &p[i].Support(k);
		s.insert(sk->begin(), sk->end());
	}

	return s.size();
}


List Profile::Plurality() {
//	List v(m, 0)
	if (plV.size() > 0) return plV;
	plV.resize(m, 0);
	for (int i = 0; i < n; i++)
		plV[p[i].p[0]]++;

	return plV;
}

List Profile::Borda() {
	List v(m, 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			v[p[i].p[j]]+= m - j - 1;
	return v;
}

List Profile::Copeland() {
	prMaj();
	return copV;
}

List Profile::Runoff() {
	cout << "**Runoff***";
	if (plV.size() == 0) Plurality();
	if (pr.size() == 0) prMaj();
	List v(m, 0), t;
	t = maxSet(plV);
//	if (t.size() == 1 && plV[t[0]] > n / 2) v[t[0]] = 1;     //plV *= 100 ??
	
	return v;
}

bool Profile::isCC() {
	int i, j, k;
	for (i = 0; i < m; i++)
		for (j = i + 1; j < m; j++)
			for (k = j + 1; k < m; k++)
			if ((pr[i][j] >= pr[j][i] && pr[j][k] >= pr[k][j] && pr[k][i] >= pr[i][k]) ||
				(pr[i][j] <= pr[j][i] && pr[j][k] <= pr[k][j] && pr[k][i] <= pr[i][k]))
				return 1;

	return 0;
}

idType Profile::Winner(const List& v, bool noTie) {
	List s = maxSet(v);
	if (s.size() == 1) return s[0];
	if (noTie) return -1;
	return getRand(s);
}

List Profile::Winners(const List& v) {
	return maxSet(v);
}

List Profile::Rank(const List& v) { // sort :  std::greater<int> ??
	vector<pair<idType, idType> > a(v.size());
	List r(v.size());
	for (int i = 0; i < v.size(); i++)
		a[i] = pair<idType, idType>(v[i], i);
	sort(a.begin(), a.end());
	for (int i = 0; i < v.size(); i++)
		r[i] = a[v.size() - i - 1].second;
//	r[i] = a[i].second;

	return r;
}


double Profile::regret(idType w) {
	double r = 0;
	for (int i = 0; i < n; i++)
		r += p[i].pos[w];
	return r;
}


void Profile::prMaj() {
	if (pr.size() > 0) return;
	pr.resize(m, vector<int>(m, 0));
	copV.resize(m, 0);
	sMG.clear();
	List t(2);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int k = j + 1; k < m; k++)
				pr[p[i].p[j]][p[i].p[k]]++;

	for (int j = 0; j < m; j++)
		for (int k = j + 1; k < m; k++) {
			if (pr[j][k] > pr[k][j]) { copV[j]++; copV[k]--; t[0] = j;  t[1] = k; sMG.insert(t); }
			else
			if (pr[j][k] < pr[k][j]) { copV[j]--; copV[k]++; t[0] = k;  t[1] = j; sMG.insert(t); }
		}

}

setList& Profile::mgR() {
	prMaj();
	return sMG;
}


ProfRep Profile::Report(bool noTie) {
	int i, nf = fPDI.size(), nv = vRule.size();
	vector<double> x(nf);
	vector<List> vs(nv);
	vector<idType> v(nv);
	vector<double> rg(nv);
	ProfRep r;
	double rgf = 1.0 / (n * (m - 1));

	for (i = 0; i < nf; i++)
		x[i] = (this->*fPDI[i]) ();


	r.isTie = 0;
	for (i = 0; i < nv; i++) {
		vs[i] = Winners((this->*vRule[i]) ());
		if (vs[i].size() == 1)
			v[i] = *vs[i].begin();
		else {
			v[i] = getRand(vs[i]);
			r.isTie = 1;
		}
		rg[i] = rgf * regret(v[i]);
	}

	if (copV[vs[2][0]] == m - 1) r.isCW = 1; else r.isCW = 0;
	r.isCC = isCC();
	r.isCCW = !r.isCW && r.isCC;

	r.x = x;
	r.vs = vs;
	r.v = v;
	r.rg= rg;
	return r;
}


void Profile::Reset(vector<int>& f, vector<int>& v) {
	fPDI.resize(f.size()); vRule.resize(v.size());
	transform(f.begin(), f.end(), fPDI.begin(), [](int i){return fAll[i].first; });
	transform(v.begin(), v.end(), vRule.begin(), [](int i){return vAll[i].first; });
}
