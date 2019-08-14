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
