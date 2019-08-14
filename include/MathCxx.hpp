#ifndef Math_Cxx
#define Math_Cxx

#define CXX_BEGIN namespace Cxx {
#define CXX_END }

#include <cstdio>
#include <string>

#include <utility>
#include <iomanip>

CXX_BEGIN

std::string expr, number;
unsigned index;

enum token_e {
	EXP, MULT, DIV, PLUS, MINUS, NUMBER, LPARENTH, RPARENTH, END, UNDEFINED,
	EQUAL_TO, NOT_EQUAL_TO, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL, LOGICAL_AND, LOGICAL_OR
} cur_token;

inline bool is_whitespace(char const c) {
	return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\v');
}

inline bool is_digit(char const c) { return ('0' <= c) && (c <= '9'); }

const char* to_str(token_e token)
{
	switch (token) {
		case EXP       : return "^"      ;
		case MULT      : return "*"      ;
		case DIV       : return "/"      ;
		case PLUS      : return "+"      ;
		case MINUS     : return "-"      ;
		case NUMBER    : return "NUMBER" ;
		case LPARENTH  : return "("      ;
		case RPARENTH  : return ")"      ;
		case END       : return ";"      ;
		case UNDEFINED : return "?"      ;

		case EQUAL_TO      : return "==" ;
		case NOT_EQUAL_TO  : return "!=" ;
		case GREATER       : return ">"  ;
		case GREATER_EQUAL : return ">=" ;
		case LESS          : return "<"  ;
		case LESS_EQUAL    : return "<=" ;

		case LOGICAL_AND   : return "&&" ;
		case LOGICAL_OR    : return "||" ;
	}

	return "";
}

token_e get_token() {
	while (index != expr.size() && is_whitespace(expr[index])) ++index;

	if (index == expr.size()) return END;

	switch (expr[index++])
	{
		case '^' : return EXP   ;
		case '*' : return MULT  ;
		case '/' : return DIV   ;
		case '+' : return PLUS  ;
		case '-' : return MINUS ;

		case '(' : return LPARENTH ;
		case ')' : return RPARENTH ;

		case '=' : if (expr[index] == '=') { ++index; return EQUAL_TO      ; }
		case '!' : if (expr[index] == '=') { ++index; return NOT_EQUAL_TO  ; }
		case '>' : if (expr[index] == '=') { ++index; return GREATER_EQUAL ; }
				  else { return GREATER; }

		case '<' : if (expr[index] == '=') { ++index; return LESS_EQUAL    ; }
				  else { return LESS; }

		case '&' : if (expr[index] == '&') { ++index; return LOGICAL_AND   ; }
		case '|' : if (expr[index] == '|') { ++index; return LOGICAL_OR    ; }
	}

	index -= 1;

	if (is_digit(expr[index])) {
		do
		{
			number += expr[index]; ++index;
		}

		while (index != expr.size() && is_digit(expr[index]));
		return NUMBER;
	}

	++index;
	return UNDEFINED;
}

struct Base {
	virtual void   print() const = 0;
	virtual double compute() const = 0;

	virtual ~Base() = 0;
};

inline Base::~Base() {}

struct Calc : Base
{
	token_e op;

	Base* left, * right;
	Calc(token_e op, Base* left, Base* right) : op(op), left(left), right(right) {}

	void print() const override {

		left->print();
		right->print();

		std::printf("%s", to_str(op));
	}

	double compute() const override {
		switch (op) {
		case EXP           : return (int)std::pow(left->compute(), right->compute());
		case MULT          : return left->compute() *  right->compute();
		case DIV           : return left->compute() /  right->compute();
		case PLUS          : return left->compute() +  right->compute();
		case MINUS         : return left->compute() -  right->compute();

		case EQUAL_TO      : return left->compute() == right->compute();
		case NOT_EQUAL_TO  : return left->compute() != right->compute();
		case GREATER       : return left->compute() >  right->compute();
		case LESS          : return left->compute() <  right->compute();
		case GREATER_EQUAL : return left->compute() >= right->compute();
		case LESS_EQUAL    : return left->compute() <= right->compute();
		case LOGICAL_AND   : return left->compute() && right->compute();
		case LOGICAL_OR    : return left->compute() || right->compute();
		}

		return 0;
	}

	~Calc() {
		if (left)
			delete left;

		if (right)
			delete right;
	}
};

struct Unary : Base
{
	token_e op;

	Base* value;
	Unary(token_e op, Base* value) : op(op), value(value) {}

	void print() const override {
		std::printf("%s", to_str(op));
		value->print();
	}

	double compute() const override {
		if (op == MINUS) return (value->compute() * -1);
		return 0;
	}

	~Unary() {
		if (value) delete value;
	}
};

struct Value : Base
{
	double value;

	Value(double value) : value(value) {}

	void print() const override {
		std::printf("%f", value);
	}

	double compute() const override {
		return value;
	}

	~Value() = default;
};

template <class enum_x, class... enums>
inline bool match(enum_x arg_x, enums... args) {
	return (arg_x == cur_token) || ((cur_token == args) || ...);
}

template <class... types>
void advance(token_e& cur_token, token_e(*ptr_f)(), types&& ... args) {
	cur_token = ptr_f(std::forward <types>(args)...);
}

Base* plus_minus();
Base* mult_div();
Base* unary();
Base* primary();
Base* and_or();
Base* less_greater_equal();
Base* expr_p();

Base* expr_p() {
	cur_token = get_token();

	return and_or();
}

Base* and_or()
{
	Base* left = less_greater_equal();

	if (match(LOGICAL_OR, LOGICAL_AND)) {
		do {
			token_e op = cur_token;
			advance(cur_token, get_token);

			Base* right = less_greater_equal();
			left = new Calc(op, left, right);
		}

		while (cur_token != END && match(LOGICAL_AND, LOGICAL_OR));
	}

	return left;
}

Base* less_greater_equal()
{
	Base* left = plus_minus();

	if (match(EQUAL_TO, NOT_EQUAL_TO, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL)) {
		do {
			token_e op = cur_token;
			advance(cur_token, get_token);

			Base* right = plus_minus();
			left = new Calc(op, left, right);
		}

		while (cur_token != END && match(EQUAL_TO, NOT_EQUAL_TO, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL));
	}

	return left;
}


Base* plus_minus()
{
	Base* left = mult_div();

	if (match(PLUS, MINUS)) {
		do {
			token_e op = cur_token;
			advance(cur_token, get_token);

			Base* right = mult_div();
			left = new Calc(op, left, right);
		}

		while (cur_token != END, match(PLUS, MINUS));
	}

	return left;
}

Base* mult_div()
{
	Base* left = unary();

	if (match(MULT, DIV)) {
		do {
			token_e op = cur_token;
			advance(cur_token, get_token);

			Base* right = unary();

			left = new Calc(op, left, right);
		}

		while (cur_token != END, match(MULT, DIV));
	}

	else if (match(EXP)) {
		token_e op = cur_token;
		advance(cur_token, get_token);

		Base* right = mult_div();
		return new Calc(op, left, right);
	}

	return left;
}

Base* unary()
{
	if (match(MINUS)) {
		token_e op = cur_token;
		advance(cur_token, get_token);

		Base* value = unary();

		return new Unary(op, value);
	}

	return primary();
}

Base* primary()
{
	if (match(NUMBER)) {
		advance(cur_token, get_token);

		double value = std::stod(number);
		number.clear();

		return new Value(value);
	}

	if (match(LPARENTH)) {
		Base* left = expr_p();
		advance(cur_token, get_token);

		return left;
	}

	return nullptr;
}

Base* AST = nullptr;

double Compute(std::string expr) {
	cxx::expr = std::move(expr);
	index = 0;

	AST = expr_p();
	return AST->compute();
}

CXX_END

#endif
