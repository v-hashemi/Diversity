#include "Prf.h"


void Prf::rndGen(int m0) {
	if (m0 > 0) m = m0;
	p.resize(m);
	List v = fillSeq(m);

	for (int i = 0; i < m; i++) {
		int r = rand() % v.size();
		p[i] = v[r];
		v.erase(v.begin()+r);
	}
	reset(p);
}

void Prf::PosList() {
	pos.resize(p.size());
	for (int i = 0; i < p.size(); i++)
		pos[p[i]] = i;
}


void Prf::Support(const List& p, setList& s, int k) {
	List c(k), q(k);
	c = fillSeq(k);
	s.clear();

	do {
		for (int i = 0; i < k; i++) q[i] = p[c[i]];
		s.insert(q);
	} while(next_combination(c, k, p.size()));
}


double Prf::dK(setList& r1, setList& r2) {
	vector<List> ds(r1.size());
	return (r1.size() + r2.size())/2 -	(set_intersection(r1.begin(), r1.end(), r2.begin(), r2.end(), ds.begin()) - ds.begin());
}


double Prf::dK(Prf& p, Prf& q) {
	setList *ps = &p.Support(2), *qs = &q.Support(2);//, ds;
	vector<List> ds(ps->size());
	return set_difference(ps->begin(), ps->end(), qs->begin(), qs->end(), ds.begin()) - ds.begin();
}

double Prf::dK3(Prf& p, Prf& q) {
	setList *ps = &p.Support(3), *qs = &q.Support(3);
	vector<List> ds(ps->size());
	return set_difference(ps->begin(), ps->end(), qs->begin(), qs->end(), ds.begin()) - ds.begin();
}


double Prf::dS(Prf& p, Prf& q) {
	int t = 0;
	for (int i = 0; i < p.m; i++)
		t += abs(p.pos[i] - q.pos[i]);

	return t;
}

double Prf::dM(Prf& p, Prf& q) {
	int t = 0;
	for (int i = 0; i < p.m; i++)
		t = max(t, abs(p.pos[i] - q.pos[i]));

	return t;
}

double Prf::dHam(Prf& p, Prf& q) {
	int t = 0;
	for (int i = 0; i < p.p.size(); i++)
		if (p.p[i] != q.p[i]) t++;
	return t;
}

double Prf::dWHam(Prf& p, Prf& q, const vector<double>& w) {
	double t = 0;
	for (int i = 0; i < p.p.size(); i++)
		if (p.p[i] != q.p[i]) t+= w[i];

	return t;
}

vector<double> Prf::hw = vector<double>();
