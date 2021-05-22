#include <iostream>
#include <string>
#include "Tokenizer.hpp"

using namespace std;
using String = string;

int main(void)
{
    String string = String{"(1+2+3+4+5+6+7+8+9+10)"};
    std::cout << evaluate(string);
    return 0;
}