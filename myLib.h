#ifndef _MYLIB_
#define _MYLIB_

// This file contains some general functions

#include "Def.h"

// Returns a sequence of n values, starts from b with s step.
template<typename T = int>
vector<T> fillSeq(int n, T b = 0, T s = 1) {
	vector<T> v(n);
	for (int i = 0; i < n; i++, b += s)
		v[i] = b;
	return v;
}


// Prints c (works for std containers) into fout, sp is separator and an endl if el
template<class C>
void Out(C& c, ostream& fout = cout, string sp = " , ", bool el = 1) {
	if (c.size() == 0) {if (el) fout << endl; return;}
	C::const_iterator i, e = c.end(); e--;
//	for (C::iterator i = c.begin(); i != c.end() - 1; i++)

	for (i = c.begin(); i != e; i++)
		fout << *i << sp;
	fout << *i;
	if (el) fout << endl;
}

// Prints 2-dimensional c (works for combination of std containers) into fout, sp is separator and el is row separator. An endl at the end.
template<class C>
void Out2(C& c, ostream& fout = cout, string sp = ", ", string el = "\t") {
	if (c.size() == 0) return;
	C::const_iterator i, e = c.end(); e--;
//	for (C::iterator i = c.begin(); i != c.end() - 1; i++)
	for (i = c.begin(); i != e; i++) {
		Out(*i, fout, sp, 0);
		fout << el;
	}
	Out(*i, fout, sp);
	fout << endl;
}


// Changes c to its next combination (of lenght k from 0, ..., n-1).
bool next_combination(List& c, int k, int n);

// Returns indices of the elements with max value
List maxSet(const List& p);

// Gets an element of the list, randomly.
inline idType getRand(const List& s) { return s[rand() % s.size()]; }

// Returns (x/y) on the scale of [0 - 100] (assuming x <= y).
inline int per(double x, double y = 1) { return int(round(100 * x / (EPS + y))); }

// x is set of indices, subx is its subset. y will be indices of subx members in x, i.e., subx[i] = x[y[i]].
// The members of subx are ignored if they are not member of x; function will returns 1 in that case.
// Ex: x = {0, 1, 3, 5}, subx = {0, 3, 4, 5} ==> y = {0, 2, 3} and returns 1.
bool SubsetIndx(const vector<int>& x, const vector<int>& subx, vector<int>& y);

// Returns number of common elements of two lists
template<class T>
int numCom(T& s1, T& s2) {
	vector<idType> t(s1.size());
	return set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), t.begin()) - t.begin();
}

// Returns number of common elements of three lists
template<class T>
int numCom(T& s1, T& s2, T& s3) {
	T s;
	vector<idType> t(s1.size());
	set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(s, s.begin()));
	return set_intersection(s.begin(), s.end(), s3.begin(), s3.end(), t.begin()) - t.begin();
}

// Returns number of common elements of a list of lists
template<class T>
int numCom(vector<T>& s) {
	vector<T> t(2);

	t[0] = s[0];
	int i0, i1;
	for (int i = 1; i < s.size(); i++) {
		i0 = (i+1) % 2; i1 = i % 2;
		t[i1].clear();
		t[i1].resize(s[0].size());
		t[i1].resize(set_intersection(t[i0].begin(), t[i0].end(), s[i].begin(), s[i].end(), t[i1].begin()) -t[i1].begin());
	}
	return t[i1].size();
}
#endif
