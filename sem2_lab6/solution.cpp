#include <bits/stdc++.h>
using namespace std;

const int inf = 1e9;

struct G {
	int** adj_mtrx;
	int** paths;
	int n;
};

G* enter_and_count() {
	G* g = new G;
	//init adj matrix
	cout << "enter the number of elements in the graph\n";
	cin >> (g -> n);
	g -> adj_mtrx = new int*[g -> n];
	for (int i = 0; i < g -> n; i++) {
		g -> adj_mtrx[i] = new int;
	}
	// enter adj matrix
	cout << "Enter the adjacency matrix of the graph now\n";
	for (int i = 0; i < g -> n; i++) {
		for (int j = 0; j < g -> n; j++) {
			if (i == j) {
				cout << "a[i][i] is zero! that is chosen for you\n";
			} else {
				char* cur = new char;
				cin >> cur;
				if (cur[0] == 'n') {
					g -> adj_mtrx[i][j] = inf;
				} else {
					g -> adj_mtrx[i][j] = atoi(cur);
				}
			}
		}
	}
	cout << "success\n";
	//init path matrix
	g -> paths = new int*[g -> n];
	for (int i = 0; i < g -> n; i++) {
		g -> paths[i] = new int;
		for (int j = 0; j < g -> n; j++) {
			g -> paths[i][j] = g -> adj_mtrx[i][j];
		}
	}
	/// count
	cout << "fff\n";
	for (int k = 0; k < g -> n; k++) {
		for (int i = 0; i < g -> n; i++) {
			for (int j = 0; j < g -> n; j++) {
				if (g -> paths[i][k] < inf && g -> paths[k][j] < inf) {
					g -> paths[i][j] = min(g -> paths[i][j], g -> paths[i][k] + g -> paths[k][j]);
				}
			}
		}
	}
	return g;
}

int find_diam(G* g) {
	int diam = -inf;
	for (int i = 0; i < g -> n; i++) {
		for (int j = 0; j < g -> n; j++) {
			diam = max (diam, g -> paths[i][j]);
		}
	}
	return diam;
}

struct Pair {
	int sum;
	int index;
};

Pair** sort(Pair** pr, int len);

void sort_by_len_of_adjacent_edges(G* g) {
	Pair** par = new Pair*[g -> n];
	for (int i = 0; i < g -> n; i++) {
		par[i] = new Pair;
	}
	for (int i = 0; i < g -> n; i++) {
		// посчитать значение сумма длин adj ребер и занести в массив
		int sum = 0;
		for (int j = 0; j < g -> n; j++) {
			if (g -> adj_mtrx[i][j] != inf) {
				sum += g -> adj_mtrx[i][j];
			}
		}
		par[i]-> sum = sum;
		par[i] -> index = i;
	}
	par = sort(par, g -> n);
	for (int i = 0; i < g -> n; i++) {
		cout << par[i] -> index;
	}
}

void delpair(Pair** pp, int len) {
	for (int i = 0; i < len; i++) {
		delete pp[i];
	}
}

void delgr(G* g) {
	for (int i = 0; i < g -> n; i++) {
		delete g -> adj_mtrx[i];
		delete g -> paths[i];
	}
	delete g;
}

Pair** sort(Pair** start, int len) {
	for (int i = 0; i < len; i++) {
		for (int j = i; j < len; j++) {
			if (start[i] -> sum < start[j] -> sum) {
				swap(start[i], start[j]);
			}
		}
	}
	return start;
}

void swap(Pair* &a, Pair* &b) {
	Pair* tmp = a;
	a = b;
	b = tmp;
}

int main() {
	G* g = enter_and_count();
	cout << "end\n";
	sort_by_len_of_adjacent_edges(g);
}