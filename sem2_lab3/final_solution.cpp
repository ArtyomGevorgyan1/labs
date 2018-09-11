#include <bits/stdc++.h>
using namespace std;
 
struct Sparse {
	int n;
	int m;
	int not_nulls;
	int **body;
	Sparse(int n_Sparse, int m_Sparse, int not_nulls_Sparse)
	{
		n = n_Sparse;
		m = m_Sparse;
		not_nulls = not_nulls_Sparse;
		
		body = new int*[n];
		for (int i = 0; i < n; i++) {
			body[i] = new int[m];
		}
	}
};

Sparse* input_file () {
	FILE* f = fopen("input.txt", "r+");
	if (f) {
		int c; bool found_cols = 0; bool comment = 0;
		int rows = 0; int cols = 0; 
		while ((c = getc(f)) != EOF) {
			if (comment) {
				if (c == (int)'#') {
					comment = 0;
					continue;
				} else {
					continue;
				}
			} 
			else if (!comment) { 
				if (c == (int)'#') {
					comment = 1; 
					continue;
				}
				else if (c != (int)'\n') {
					if (c != (int)' ' && !found_cols) {
						cols++;
					}
				} else {
					rows++;
					found_cols = 1;
				}
			}
		}
		cout << "n * m: " << rows << ' ' << cols;
		rewind(f); comment = 0;
		int arr[rows][cols]; int cur_row = 0; int cur_col = 0;
		while ((c = getc(f)) != EOF) {
			if (comment) {
				if (c == (int)'#') {
					comment = 0;
					continue;
				} else {
					continue;
				}
			} 
			else if (!comment) { 
				if (c == (int)'#') {
					comment = 1; 
					continue;
				}
				if (c - (int)'0' <= 9 && c - (int)'0' >= 0) {
					if (cur_col <= cols) {
						arr[cur_row][cur_col] = c - '0';
						cur_col++;
					}
					if (cur_col > cols) {
						cur_row++;
						cur_col = 0;
						arr[cur_row][cur_col] = c - '0';
						cur_col++;
					}
				}
			}
		}
		cout << "Your matrix has been interpreted. Here it is..." << endl;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				cout << arr[i][j] << ' ';
			}
			cout << endl;
		}
		cout << "Now converting it to the special format..." << endl;

		Sparse* mtrx = new Sparse(rows, cols, 0);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				mtrx -> body[i][j] = arr[i][j];
				if (mtrx -> body[i][j] != 0) mtrx -> not_nulls++;
			}
			cout << i + 1 << "th row completed" << endl;
		}
		cout << "Ok, got your matrix" << endl;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				cout << mtrx -> body[i][j] << ' ';
			}
			cout << endl;
		}
		return mtrx;
	}
}

struct Compact {
	int n;
	int m;
	int size;
	int *r;
	int *c;
	int *v;
	Compact(int n_Compact, int m_Compact, int size_Compact)
	{
		n = n_Compact;
		m = m_Compact;
		size = size_Compact;
		int length = n * m;
		r = new int[length];
		c = new int[length];
		v = new int[length];
	}
};

int find(Compact* mtrx, int i, int j) {
	for (int k = 0; k < mtrx -> size; k++) {
		if (mtrx -> r[k] == i && mtrx -> c[k] == j) {
			return k;
		}
	}
	return -1;
}

Sparse* input() {
	int n, m, cnt = 0;
	cout << "Enter the size (n by m)" << endl;
	cin >> n >> m;
	Sparse* mtrx = new Sparse(n, m, 0);
	for (int i = 0; i < n; i++) {
		//cout << "cur i: " << i << endl;
		for (int j = 0; j < m; j++) {
			//cout << "cur j: " << j << endl;
			cin >> mtrx -> body[i][j];
			if (mtrx -> body[i][j] != 0) mtrx -> not_nulls++;
		}
		cout << "first row completed" << endl;
	}
	cout << "Ok, got your matrix" << endl;
	return mtrx;
}

Compact* build_compact(Sparse* mtrx) {
	Compact* compact_mtrx = new Compact(mtrx -> n, mtrx -> m, mtrx -> not_nulls);
	int cnt = 0;
	cout << "building compact representation..." << endl;
	for (int i = 0; i < mtrx -> n; i++) {
		for (int j = 0; j < mtrx -> m; j++) {
			if (mtrx -> body[i][j] != 0) {
				compact_mtrx -> r[cnt] = i;
				compact_mtrx -> c[cnt] = j;
				compact_mtrx -> v[cnt] = mtrx -> body[i][j];
				cnt++;
			}
		}
	}
	// delete[] mtrx;
	/*
	for (int i = 0; i < mtrx -> n; i++) {
		delete[] mtrx -> body[i];
	}
	*/
	cout << "build finished" << endl;
	return compact_mtrx;
}

Compact* add(Compact* a, Compact* b) {
	if (a -> n != b -> n || a -> m != b -> m) {
		cout << "Error. The sizes of two matrixes do not match. Cannot do addition. ";
		exit(-1);
	}
	Compact* res = new Compact(a -> n, a -> m, 0);
	int n = a -> n; int m = a -> m; 
	int cur_filled = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int first_pos = find(a, i, j);
			int second_pos = find(b, i, j);
			if (first_pos != -1 && second_pos != -1) {
				res -> v[cur_filled] = a -> v[first_pos] + b -> v[second_pos];
			}
			if (first_pos != -1 && second_pos == -1) {
				res -> v[cur_filled] = a -> v[first_pos];
			}
			if (first_pos == -1 && second_pos != -1) {
				res -> v[cur_filled] = b -> v[second_pos];
			}
			res -> r[cur_filled] = i;
			res -> c[cur_filled] = j;
			++cur_filled;
		}
	}
	res -> size = cur_filled;
	return res;
}

Compact* multiply (Compact* a, Compact* b) {
	if (a -> m != b -> n) {
		cout << "Cannot do multiplication.";
		exit(-1);
	}
	Compact* res = new Compact(a -> n, b -> m, 0);
	int n = a -> n; int m = b -> m;
	int cur_filled = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int temp = 0;
			for (int k = 0; k < a -> m; ++k)
			{
				int first_pos = find(a, i, k);
				int second_pos = find(b, k, j);
				if (first_pos != -1 && second_pos != -1) {
					temp += a -> v[first_pos] * b -> v[second_pos];
				}
				
			}
			res -> r[cur_filled] = i;
			res -> c[cur_filled] = j;
			res -> v[cur_filled] = temp;
			++cur_filled;
		}
	}
	res -> size = cur_filled;
	return res;
}

void show (Compact* mtrx) {
	for (int i = 0; i < mtrx -> n; i++) {
		for (int j = 0; j < mtrx -> m; j++) {
			int pos = find (mtrx, i, j);
			if (pos != -1) {
				cout << mtrx -> v[pos];
			} else {
				cout << 0; 
			}
			cout << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}

int main() {
	/*
	Sparse* mtrx1 = input();
	Sparse* mtrx2 = input();
	Compact* m = build_compact(mtrx1);
	Compact* M = build_compact(mtrx2);
	//add(m, M);
	//show(m);
	Compact *temp = multiply(m, M);
	show(temp);
	*/
	
	input_file();
	return 0;
}