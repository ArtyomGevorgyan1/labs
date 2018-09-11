#include <bits/stdc++.h>
using namespace std;

struct Node {
	Node* next;
	int coef;
	int pow;
};

struct Rnode {
	Rnode *next;
	int val;
};

long long mypow (long long base, long long pow) {
	if (pow == 0) {
		return 1;
	}
	if (pow % 2 == 1) { 
		return base * mypow(base, pow / 2) * mypow(base, pow / 2);
	}
	if (pow % 2 == 0) {
		return mypow(base, pow / 2) * mypow(base, pow / 2);
	}
}

long long pol_eval (Node* head, int x) {
	long long ans = 0;
	while (head) {
		ans += mypow(x, head -> pow) * (1LL * head -> coef);
		head = head -> next;
	}
	return ans;
}

Node* builder (int power) {
	Node* head = nullptr;
	int cur;
	for (int i = 0; i <= power; i++) {
		Node* tmp = new Node;
		cin >> cur;
		tmp -> coef = cur;
		tmp -> pow = i;
		tmp -> next = head;
		head = tmp;
	}
	return head;
}

Rnode* calc_rems (Node* head, int i) {
	Rnode* rhead = nullptr;
	for (int x = 1; x <= i; x++) {
		Rnode* tmp = new Rnode;
		tmp -> val = pol_eval(head, x) % i; 
		tmp -> next = rhead;
		rhead = tmp;
	}
	return rhead;
}

Rnode* del_repeats (Rnode* head) {
	Rnode* outer = head;
	while (outer) {
		Rnode* inner = outer -> next;
		Rnode* pinner = outer;
		while (inner) {
			if (inner -> val == outer -> val && inner != outer) {
				pinner -> next = inner -> next;
				delete(inner);
				inner = pinner;
			}
			pinner = inner;
			inner = inner -> next;
		}
		if (outer) {
			outer = outer -> next;
		}
	}
	return head;
}

void show (Rnode* head) {
	while (head) {
		cout << "Rem: " << head -> val << endl;
		head = head -> next;
	}
}

void solve() {
	int power;
	cout << "What is the power of the polinomial? ";
	cin >> power;
	cout << "Ok, let me see..." << endl;
	Node* head = builder(power);
	cout << "We got the polinomial, what is the integer you want it to be divided by? ";
	int i;
	cin >> i;
	cout << "Ok, wait for a sec...";
	Rnode* rhead = calc_rems(head, i);
	cout << "Yout rems have been already calculated, let me now delete the repeating ones... Please, hold on." << endl;
	rhead = del_repeats(rhead);
	cout << "We are ready to go!... Here your remnants come: " << endl;
	show(rhead);
	cout << "Hope you are satisfied :)";
	exit(1);
}

int main() {
	solve();
}
