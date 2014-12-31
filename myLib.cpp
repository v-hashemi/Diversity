#include "myLib.h"

bool next_combination(List& c, int k, int n) {
	for (int i = k - 1; i >= 0; i--) {
		if (c[i] < n - k + i) {
			c[i]++;
			for (int j = i + 1; j < k; j++)
				c[j] = c[j-1] + 1;
			return 1;
		}
	}
	return 0;
}

List maxSet(const List& v) { //first max, then find the set: faster? ??  reserve or not reserve? 
	List s;
	s.reserve(v.size());
	s.push_back(0);
	for (int i = 1; i < v.size(); i++)
		if (v[i] > v[s[0]]) {s.clear(); s.push_back(i);}
		else if (v[i] == v[s[0]]) s.push_back(i);
	return s;
}

bool SubsetIndx(const vector<int>& x, const vector<int>& subx, vector<int>& y) {
	bool flg = 0;
	vector<int>::const_iterator it;
	y.clear();
	for (int i = 0; i < subx.size(); i++) {
		it = find(x.begin(), x.end(), subx[i]);
		if (it == x.end())
			flg = 1;
		else
			y.push_back(it - x.begin());
	}
	return flg;
}
