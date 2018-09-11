#include <bits/stdc++.h>
using namespace std;

const int initial_size_of_dynamic_array = 100;

int input_num_int (int &num) {
	cin >> num;
	return num;
}

struct Node {
	int value;
	int row_sognificant;
	int col_significat;
	Node* next;
};

struct Sparse {
	int non_zero;
	int rows;
	int cols;
	Node* head;
};

void add_node(int row, int col, int value, Sparse* sparse_representation) {
	Node *tmp = new Node;
	tmp -> value = value;
	tmp -> row_sognificant = row;
	tmp -> col_significat = col;
	tmp -> next = nullptr;
	Node* cur = sparse_representation -> head;
	if (cur == nullptr) {
		sparse_representation -> head = tmp;
		cout << "FIRST NODE JUST ADDED: ";
		cout << sparse_representation -> head -> value << " " << sparse_representation -> head -> row_sognificant << ' ' << sparse_representation -> head -> col_significat << endl;
		cout << "-----" << endl;
	} else {
		while (cur -> next) {
			cur = cur -> next;
		}
		cur -> next = tmp;
		cout << "CURRENT VALUE: ";
		cout << "val " << cur -> next -> value << endl;
		cout << "row " << cur -> next -> row_sognificant << endl;
		cout << "col " << cur -> next -> col_significat << endl;
		cout << "---------------" << endl;
	}
}

void initialize_sparse_repr (Sparse *structure) {
	structure -> non_zero = 0;
	structure -> rows = 0;
	structure -> cols = 0;
	structure -> head = nullptr;
}

void initialize_node (Node* node, int row, int col, int val, Node* next) {
	node -> value = val;
	node -> col_significat = row;
	node -> row_sognificant = col;
	node -> next = next;
}

Sparse *enter () {
	int rows, cols;
	int helping_var;
	cout << endl << "gimme your" << endl;
	input_num_int(rows); input_num_int(cols);
	Sparse* sparse_representation = new Sparse;
	initialize_sparse_repr(sparse_representation);
	sparse_representation -> rows = rows;
	sparse_representation -> cols = cols;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			input_num_int(helping_var);
			cout << endl;
			if (helping_var) {
				sparse_representation -> non_zero++;
				add_node(i, j, helping_var, sparse_representation);
			} else {
				cout << "IM IGNORING YOU LOL, PLEASE DO NOT ENTER ZEROS" << endl;
			}
		}
	}
	return sparse_representation;
}

Node* find (Node* head, int row, int col) {
	Node* result = head;
	if (result == nullptr) {
		return nullptr;
	}
	while (result -> row_sognificant != row || result -> col_significat != col) {
		if (result -> next) {
			result = result -> next;
		} else {break;}
	}
	if (result -> row_sognificant == row && result -> col_significat == col) {
		return result;
	}
	return nullptr;
}

void show_compact_respresentation (Sparse* compact) {
	if (compact == nullptr) {
		cout << ":))0)0)))0, good bye";
		return;
	}
	int rows = compact -> rows;
	int cols = compact -> cols;
	Node* tmp = compact -> head;
	Node* res;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if ((res = find(tmp, i, j)) != nullptr) {
				cout << res -> value << ' ';
			}
			else {
				cout << 0 << ' ';
			}
		}
		cout << endl;
	}
}
 

Sparse* multiply (Sparse *a, Sparse *b) {
	if (a -> cols != b -> rows) {
		cout << "error;" << endl;
		return nullptr;
	}
	Node *point_one = a -> head;
	Node *point_two = b -> head;
	Sparse *result = new Sparse;
	initialize_sparse_repr(result);
	result -> rows = a -> rows;
	result -> cols = b -> cols;

	for (int i = 0; i < result -> rows; i++) {
		for (int j = 0; j < result -> cols; j++) {
			for (int k = 0; k < a -> cols; k++) {
				// c[i][j] = a[i][k] * b[k][j]
				if (find(point_one, i, k) != nullptr && find(point_two, k, j) != nullptr) {
					result -> non_zero++;
					int multiple = point_one -> value * point_two -> value;
					add_node(i, j, multiple, result);
				}
			}
		}
	}
	//cout << "oerall: " << result -> non_zero << endl;
	if (result -> non_zero == 0) {
		return nullptr;
	}
	return result;
}

Sparse* add (Sparse* a, Sparse* b) {
	if (a -> rows == b -> rows && b -> cols == a -> cols) {
		Sparse* result = new Sparse;
		initialize_sparse_repr(result);
		result -> rows = a -> rows;
		result -> cols = a -> cols;
		Node* h, *m;
		for (int i = 0; i < a -> rows; i++) {
			for (int j = 0; j < a -> cols; j++) {
				int cur = 0;
				if ((h = find(a -> head, i, j)) != nullptr) {
					cur += h -> value;
				}
				if ((m = find(b -> head, i, j)) != nullptr) {
					cur += m -> value;
				}
				if (cur != 0) {
					result -> non_zero++;
					add_node(i, j, cur, result);
				}
			}
		}
		return result;
	} else {
		return nullptr;
	}
}

void show_sparse(Sparse* m) {
	cout << "Sparse Respr-tion\n";
	cout << "The number of non zero elems is: " << m -> non_zero << endl;
	Node* head = m -> head;
	while (head) {
		cout << "Row: " << head -> row_sognificant << endl;
		cout << "column: " << head -> col_significat << endl;
		cout << "value: " << head -> value << endl;
		head = head -> next;
	}
}

int main() {
	cout << "This program will calculate matrixes...";
	cout << "(size of matrix n by m)";
	//Sparse* sparse_representation = enter();
	Sparse* a = enter();
	show_sparse(a);
	//Sparse* b = enter();
	//Sparse* res = multiply(a, b);
	//show_compact_respresentation(res);
}