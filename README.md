# Math-and-Logic-Expression-Evaluator

Usage

```cpp
#include "MathCxx.hpp"

#include <iostream>
#include <memory>

int main() {
	using namespace Cxx;

	std::cout << "10 / 2 : " << Cxx::Compute("10 / 2") << '\n';

	std::unique_ptr <Base> ptr_x{ Cxx::ASTof("2 + 3") };
	std::unique_ptr <Base> ptr_y{ Cxx::ASTof("4 + 5") };

	if (Cxx::Compute("(2 + 3) + (4 + 5) == 14"))
	std::cout << "\n: " << ptr_x->compute() + ptr_y->compute(); // (2 + 3) + (4 + 5)

	else {
		std::cout << "Error";
	}
}
```

```
10 / 2 : 5

: 14
```
\
\
Grammar

```
EXPR_P -> AND_OR

AND_OR             -> LESS_GREATER_EQUAL (("&&" | "||") LESS_GREATER_EQUAL)* ;
LESS_GREATER_EQUAL -> PLUS_MINUS (("==" | "!=" | ">" | "<" | ">=" || "<=") PLUS_MINUS)* ;
PLUS_MINUS         -> MULT_DIV (("+" | "-") MULT_DIV)* ;
MULT_DIV           -> UNARY ((("*" | "/") UNARY)* | (("^") MULT_DIV)*) ;
UNARY              -> ("-") UNARY | PRIMARY
PRIMARY            -> "(" | EXPR_P | NUMBER
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
