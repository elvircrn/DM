#include <bits/stdc++.h>
using namespace std;
struct clan {
    int a, p;
    clan() {}
    clan(int _a, int _p) : a(_a), p(_p) {}
};
struct poli {
    vector <clan> P;
    poli() {}
    poli(vector <pair<int,int>> v) {
        for(auto par : v) {
            P.push_back(clan(par.first, par.second));
        }
    }
    void output() {
        cout << "P = ";
        for(int i = P.size() - 1; i > -1; i--) {
            cout << P[P.size()-1-i].a << "*" << "x^" << P[P.size()-1-i].p;
            if(i < P.size()-1) cout << '\n';
        }
        cout << endl;
    }
};
poli operator* (const poli& A, const poli& B) {
    vector <pair<int,int>> vc;
    map <int, int> a;
    set <int> found;
    for(int i = 0; i < A.P.size(); ++i) {
        for(int j = 0; j < B.P.size(); ++j) {
            found.insert(A.P[i].p + B.P[j].p);
            a[A.P[i].p + B.P[j].p] += A.P[i].a * B.P[j].a;
        }
    }
    for(auto st : found) {
        vc.push_back(make_pair(a[st],st));
    }
    return poli(vc);
}
int main() {
	vector <pair <int,int>> v[100];
	for(int i = 0; i < 7; ++i) {
		for(int j = 0; j <= 20; ++j) {
			v[i].push_back(make_pair((0 <= j && j <= 4),j));
		}
	}
	poli P(v[0]);
	for(int i = 1; i < 7; ++i) P = P * poli(v[i]);
	P.output();

	return 0;
    return 0;
}




