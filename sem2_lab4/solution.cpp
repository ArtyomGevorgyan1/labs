#include <bits/stdc++.h>
using namespace std;

// ----------------------------------- helping structs and functions

bool isnum(char c) {
	if (c <= '9' && c >= '0') {
		return 1;
	}
	return 0;
}

bool isalpha(char c) {
	if (c >= 'a' && c <= 'z') {
		return 1;
	}
	return 0;
}

enum node_type {
	node_type_variable, 
	node_type_number, 
	node_type_operation
};

enum token {
	operation_plus,
	operation_minus, 
	operation_mult,
	operation_div,
	operation_open_paren,
	operation_close_paren,
	operation_exp,
	operation_log,
	token_variable,
	token_number,
	comma,
	other

};

union content {
	int content_number;
	char* content_variable;
	int content_operation;
};

struct node {
	node_type node_type;
	content content;
	node* left, *right;
};

node* create_operation_node(int operation, node* left = nullptr, node* right = nullptr) {
	node* n = new node;
	n -> left = left;
	n -> right = right;
	n -> node_type = node_type_operation;
	n -> content.content_operation = operation;
	return n;
}

node* create_variable_node(char* var, node* left = nullptr, node* right = nullptr) {
	node* n = new node;
	n -> left = left;
	n -> right = right;
	n -> node_type = node_type_variable;
	n -> content.content_variable = var;
	return n;
}

node* create_number_node(int number, node* left = nullptr, node* right = nullptr) {
	node* n = new node;
	n -> left = left;
	n -> right = right;
	n -> node_type = node_type_number;
	n -> content.content_number = number;
	return n;
}

int cur_parsed_type;
char cur_char;

int parse(bool flag) {
	if (!flag) {
		return cur_parsed_type;
	}
	cin >> cur_char;
	if (cur_char == '*') {
		cur_parsed_type = operation_mult;
		return operation_mult;
	}
	if (cur_char == '/') {
		cur_parsed_type = operation_div;
		return operation_div;
	}
	if (cur_char == '+') {
		cur_parsed_type = operation_plus;
		return operation_plus;
	}
	if (cur_char == '-') {
		cur_parsed_type = operation_minus;
		return operation_minus;
	}
	if (cur_char == '(') {
		cur_parsed_type = operation_open_paren;
		return operation_open_paren;
	}
	if (cur_char == ')') {
		cur_parsed_type = operation_close_paren;
		return operation_close_paren;
	}
	if (cur_char == ',') {
		cur_parsed_type = comma;
		return comma;
	}
	else if (cur_char == 'E') {
		cur_parsed_type = operation_exp;
		return operation_exp;
	}
	else if (cur_char == 'L') {
		cur_parsed_type = operation_log;
		return operation_log;
	}
	else if (isnum(cur_char)) {
		cur_parsed_type = token_number;
		return token_number;
	}
	else if (isalpha(cur_char)) {
		cur_parsed_type = token_variable;
		return token_variable;
	}
	cur_parsed_type = other;
	return other;
}

int get_number() {
	if (cur_parsed_type == token_number) {
		char* str = new char;
		int cur = 0;
		while (cur_parsed_type == token_number) {
			str[cur] = cur_char;
			cur++;
			parse(1);
		}
		str[cur] = '\0';
		int tens = 1;
		int num = 0;
		for (int i = strlen(str) - 1; i >= 0; i--) {
			num = num + tens * (str[i] - '0');
			tens *= 10;
		}
		return num;
	}
}

char* get_var() {
	if (cur_parsed_type == token_variable) {
		char* str = new char;
		int cur = 0;
		while (cur_parsed_type == token_variable) {
			str[cur] = cur_char;
			cur++;
			parse(1);
		}
		str[cur] = '\0';
		return str;
	}
}

/* operation_plus,
	operation_minus, 
	operation_mult,
	operation_div,
	operation_open_paren,
	operation_close_paren,
	operation_exp,
	operation_log,
	token_variable,
	token_number,
	comma,
	other
*/

char decode(int op) {
	switch(op) {
		case 0:
			return '+'; 
		case 1: 
			return '-';
		case 2: 
			return '*';
		case 3: 
			return '/';
		case 4: 
			return '(';
		case 5: 
			return ')';
		case 6: 
			return 'E';
		case 7: 
			return 'L';
		case 10: 
			return ',';
	}
}

void show(node* n, int l) {
	if (n -> left) {
		show(n -> left, l+2);
	}
	for (int i = 0; i < l; i++) {
		cout << " ";
	}
	if (n -> node_type == node_type_number) {
		cout << "num " << n -> content.content_number << endl;
	}
	if (n -> node_type == node_type_variable) {
		cout << "var " << n -> content.content_variable << endl;
	}
	if (n -> node_type == node_type_operation) {
		cout << "oper " << decode(n -> content.content_operation) << endl;
	}
	if (n -> right) {
		show(n -> right, l+2);
	}
}

node* simple() {
	node* n = new node;
	n = nullptr;
	if (cur_parsed_type == token_variable) {
		node* n = create_variable_node(get_var());
		//cout << "suka\n";
		return n;
	}
	if (cur_parsed_type == token_number) {
		//cout << "fuckyou\n";
		node* n = create_number_node(get_number());
		return n;

	}
	return n;
}

/// три основные ф-и
node* plusmin();

node* three() {
	if (cur_parsed_type == operation_minus) {
		parse(1);
		node* left = create_operation_node(operation_minus, create_number_node(0), simple());
		return left;
	}
	if (cur_parsed_type == operation_open_paren) {
		parse(1);
		node* expr = plusmin();
		if (cur_parsed_type != operation_close_paren) {
			cout << "error\n";
		}
		parse(1);
		return expr;
	}
	if (cur_parsed_type == operation_exp) {
		parse(1);
		if (cur_parsed_type != operation_open_paren) {
			cout << "err\n";
		}
		parse(1);
		node* left = plusmin();
		if (cur_parsed_type != comma) {
			cout << "err\n";
		}
		parse(1);
		node* right = plusmin();
		if (cur_parsed_type != operation_close_paren) {
			cout << "err\n";
		}
		parse(1);
		node* exp = create_operation_node(operation_exp, left, right);
		return exp;
	}
	if (cur_parsed_type == operation_log) {
		parse(1);
		if (cur_parsed_type != operation_open_paren) {
			cout << "err\n";
		}
		parse(1);
		node* left = plusmin();
		if (cur_parsed_type != comma) {
			cout << "err\n";
		}
		parse(1);
		node* right = plusmin();
		if (cur_parsed_type != operation_close_paren) {
			cout << "err\n";
		}
		parse(1);
		node* log = create_operation_node(operation_log, left, right);
		return log;
	}
	return simple();
}

// когда бинарная операция - распознали токен и двигаем его на 1 впереде
// и не только тогда -- вообще надо не забывать двигать вперед символ

node* muldiv() {
	node* left = three();
	while (cur_parsed_type == operation_mult || cur_parsed_type == operation_div) {
		if (cur_parsed_type == operation_div) {
			parse(1);
			left = create_operation_node(operation_div, left, three());
		}
		if (cur_parsed_type == operation_mult) {
			parse(1);
			left = create_operation_node(operation_mult, left, three());
		}
	}
	return left;
}

node* plusmin() {
	node* left = muldiv();
	while(cur_parsed_type == operation_plus || cur_parsed_type == operation_minus) {
		if (cur_parsed_type == operation_plus) {
			parse(1);
			left = create_operation_node(operation_plus, left, muldiv());
		}
		if (cur_parsed_type == operation_minus) {
			parse(1);
			left = create_operation_node(operation_minus, left, muldiv());
		}
	}
	return left;
}

// --- ф-я рассчета 
const int inf = 1e9;

int log(int a, int b) {
	if (b < a) {
		cout << "we don't know how to calculate this :(\n";
		return inf;
	}
	int pow = 1;
	for (; ; ) {
		if (a == 1) {
			cout << "we don't know how to calculate this\n";
			return inf; 
		}
		if (a < b) {
			a *= a;
			pow++;
		} else {
			break;
		}
	}
	return pow;
}

int exp(int a, int b) {
	if (b == 0) {
		return 1;
	}
	if (b % 2 == 0) {
		return exp(a, b / 2) * exp(a, b / 2);
	}
	if (b % 2 == 1) {
		return a * exp(a, b / 2) * exp(a, b / 2);
	}
}

// ф-и для работы с переменными
const int arr_size = 1000;
int variables[arr_size];


void init_var_arr() {
	for (int i = 0; i < arr_size; i++) {
		variables[i] = inf;
	}
}

int assign(char* c) {
	int v;
	if (variables[(int)*c] != inf) {
		return variables[(int)*c];
	}
	cout << "enter the value of " << c[0] <<  " pls ";
	cin >> v;
	variables[(int)c[0]] = v;
	return v;
}

int calculate(node* n) { 
	if (n -> node_type == node_type_number) {
		return n -> content.content_number;
	}
	if (n -> node_type == node_type_variable) {
		n = create_number_node(assign(n -> content.content_variable));
		return n -> content.content_number;
	}
	if (n -> node_type == node_type_operation) {
		int left = calculate(n -> left);
		int right = calculate(n -> right);
		if (n -> content.content_operation == operation_minus) {
			return left - right;
		}
		if (n -> content.content_operation == operation_plus) {
			return left + right;
		}
		if (n -> content.content_operation == operation_mult) {
			return left * right;
		}
		if (n -> content.content_operation == operation_div) {
			return left / right;
		}
		if (n -> content.content_operation == operation_log) {
			return log(left, right);
		}
		if (n -> content.content_operation == operation_exp) {
			return exp(left, right);
		}
	}
}

int main() {
	parse(1);
	node* n = plusmin();
	show(n, 0);
	init_var_arr();
	cout << calculate(n);
}
