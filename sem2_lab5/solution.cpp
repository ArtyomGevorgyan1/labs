// РАБОТАЕТ
#include <bits/stdc++.h>
using namespace std;

struct node {
	int val;
	char* eng;
	char* rus;
	node* left;
	node* right;
	node* parent;
};

void show(node* cur, int offset) {
	if (!cur) {
		cout << "current node is null\n";
		return;
	}
	if (cur -> left) {
		show(cur -> left, offset + 2);
	}
	for (int i = 0; i < offset; i++) {
		cout << ' ';
	}
	cout << "Value: " << cur -> val << endl;
	for (int i = 0; i < offset; i++) {
		cout << " ";
	}
	cout << "English word: " << cur -> eng << endl;
	for (int i = 0; i < offset; i++) {
		cout << " ";
	}
	cout << "Russian word: " << cur -> rus << endl;
	cout << "------- ------- -------\n";
	if (cur -> right) {
		show(cur -> right, offset + 2);
	}
}

node* create_node(int val, char* eng, char* rus, node* left = nullptr, node* right = nullptr, node* parent = nullptr) {
	node* n = new node;
	n -> val = val;
	char* neng = new char;
	neng = eng;
	char* nrus = new char;
	nrus = rus;
	n -> eng = neng;
	n -> rus = nrus;
	n -> left = left;
	n -> right = right;
	n -> parent = parent;
	return n;
}

int compare(char* a, char* b) {
	if (strlen(a) > strlen(b)) {
		return 1;
	}
	if (strlen(a) < strlen(b)) {
		return -1;
	}
	else {
		for (int i = 0; i < strlen(a); i++) {
			if (a[i] > b[i]) {
				return 1;
			}
			if (a[i] < b[i]) {
				return -1;
			}
		}
	}
	return 0;
}

void add_node_to_tree(node* &root, node* n) {
	if (root)  {
		if (compare(n -> eng, root -> eng) < 0) {
			if (!(root -> left)) {
				root -> left = n;
				n -> parent = root;
				n -> left = n -> right = nullptr;
			} else {
				add_node_to_tree(root -> left, n);
			}
		}
		else if (compare(n -> eng, root -> eng) >= 0) {
			if (!(root -> right)) {
				root -> right = n;
				n -> parent = root;
				n -> left = n -> right = nullptr;
			}
			else {
				add_node_to_tree(root -> right, n);
			}
		}
	} else {
		cout << "You are going to create a new tree" << endl;
		root = n;
	}
}

// эта штука просто вернет указатель, если надо обрезать ноду, это делается вне ф-и
node* find_node_with_max_val(node* root) {
	if (!root) {
		return nullptr;
	}
	node* lmax = nullptr;
	node* rmax = nullptr;
	if (root -> left) {
		lmax = find_node_with_max_val(root -> left);
	}
	if (root -> right) {
		rmax = find_node_with_max_val(root -> right);
	}
	node* submax;
	if (lmax && rmax) {
		if (lmax -> val >= rmax -> val) {
			submax = lmax;
		} else {
			submax = rmax;
		}
		if (submax -> val >= root -> val) {
			return submax;
		} else if (submax -> val < root -> val) {
			return root;
		}
	}
	else if (lmax && !rmax) {
		submax = lmax;
	} else if (!lmax && rmax) {
		submax = rmax;
	} else if (!lmax && !rmax) {
		return root;
	}
}


node* copy_node(node* n) {
	char* neng = new char;
	char* nrus = new char;
	for (int i = 0; i < strlen(n -> eng); i++) {
		neng[i] = n -> eng[i];
		cout << neng[i];
	}
	cout << "\n";
	neng[strlen(n -> eng)] = '\0';
	for (int i = 0; i < strlen(n -> rus); i++) {
		nrus[i] = n -> rus[i];
	}
	nrus[strlen(n -> rus)] = '\0';
	node* res = create_node(n -> val, neng, nrus);
	return res;
}

void dele(node* &d) {
	if (d -> parent == nullptr) {
		//cout << "null\n";

		node* lt = d -> left;
		node* rt = d -> right;
		if (lt && rt) {
			node* new_root = lt;
			new_root -> parent = nullptr;
			while (lt -> right) {
				lt = lt -> right;
			}
			lt -> right = rt;
			rt -> parent = lt;
			delete d;
			d = new_root;
		}
		else if (lt && !rt) {
			//cout << "left\n";
			node* new_root = lt;
			new_root -> parent = nullptr;
			delete d;
			d = new_root;
		}
		else if (!lt && rt) {
			//cout << "right\n";
			node* new_root = rt;
			new_root -> parent = nullptr;
			delete d;
			d = new_root;
		}
		else if (!lt && !rt) {
			cout << "alarm\n";
			delete d;
			d = nullptr;
		}
	}
	else {
		node* p = d -> parent;
		node* lt = d -> left;
		node* rt = d -> right;
		if (lt && rt) {
			if (p -> left == d) {
				p -> left = lt;
				lt -> parent = p;
			}
			else if (p -> right == d) {
				p -> right = lt;
				lt -> parent = p;
			}
			while (lt -> right) {
				lt = lt -> right;
			}
			lt -> right = rt;
			rt -> parent = lt;
			delete d;
			d = nullptr;
		}
		if (lt && !rt) {
			if (p -> left == d) {
				p -> left = lt;
				lt -> parent = p;
			}
			if (p -> right == d) {
				p -> right = lt;
				lt -> parent = p;
			}
			delete d;
			d = nullptr;
		}
		if (!lt && rt) {
			if (p -> left == d) {
				p -> left = rt;
				rt -> parent = p;
			}
			if (p -> right == d) {
				p -> right = rt;
				rt -> parent = p;
			}
			delete d;
			d = nullptr;
		}
		if (!lt && !rt) {
			cout << "alert\n";
			if (p -> left == d) {
				p -> left = nullptr;
				delete d;
				d = nullptr;
			}
			if (p -> right == d) {
				p -> right = nullptr;
				delete d;
				d = nullptr;
			}
		}
	}
}

void menu() {
	cout << "You want to enter the dictionary, do it.\n";
	cout << "How many words are there in your dictionary?\n";
	int n;
	cin >> n;
	cout << "Ok, " << n << " entries. Let's start fillin in the forms\n";
	int f;
	char* eng = new char;
	char* rus = new char;
	cout << "Enter the first element's frqency value: ";
	cin >> f;
	cout << "\nEnter the first element's english word: ";
	cin >> eng;
	cout << "\nEnter the firs element's russian equivalent: ";
	cin >> rus;
	node* root = create_node(f, eng, rus);
	for (int i = 0; i < n - 1; i++) {
		int f;
		eng = new char;
		rus = new char;
		cout << "Enter the " << i <<  "'st element's frqency value: ";
		cin >> f;
		cout << "Enter the " << i <<  "'st element's english word: ";
		cin >> eng;
		cout << "Enter the " << i <<  "'st element's russian equivalent: ";
		cin >> rus;
		node* node = create_node(f, eng, rus);
		add_node_to_tree(root, node);
	}
	cout << "The tree has been built. Here it is.." << endl;
	show(root, 0);
	cout << "Ok. Now let's build a good tree ;)" << endl;
	cout << "Building......\n";
	if (root) {
		node* new_root = copy_node(find_node_with_max_val(root));
		cout << "Copied the root of the new tree\n";
		node* l = find_node_with_max_val(root);
		dele(l);
		while(root) {
			
			add_node_to_tree(new_root, copy_node(find_node_with_max_val(root)));
			node* tmp = find_node_with_max_val(root);
			if (tmp == root) {
				dele(tmp);
				root = nullptr;
			} else {
				dele(tmp);
			}

		}
		cout << "The new root is ready for you to see it\n";
		show(new_root, 0);
	} else {
		cout << "The tree is empty 0_0\n";
		return;
	}
}

int main() {
	/*
	int n;
	cin >> n;
	char* eng = new char;
	char* rus = new char;
	cin >> eng >> rus;
	node* r = create_node(1, eng, rus);
	for (int i = 0; i < n-1; i++) {
		eng = new char;
		rus = new char;
		cin >> eng >> rus;
		node* tmp = create_node(i, eng, rus);
		add_node_to_tree(r, tmp);
	}
	show(r, 0);
	node* nr = copy_node(find_node_with_max_val(r));
	node* l = find_node_with_max_val(r);
	dele(l);
	while (r) {
		add_node_to_tree(nr, copy_node(find_node_with_max_val(r)));
		node* tmp = find_node_with_max_val(r);
		if (tmp == r) {
			dele(tmp);
			r = nullptr;
		} else {
			dele(tmp);
		}

	}
	cout << "llllllllllll\n";
	show(nr, 0);
	*/
	menu();
}
