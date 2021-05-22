#include "Tokenizer.hpp"

Token::Token(String value, Type type)
{
    this->value = value;
    this->type = type;
}
// Return the precedence value of the current type
int Token::getPrecedence()
{
    // TODO!
    int pre;
    switch (this->type)
    {
    case ADD:
        return 6;
    case SUBT:
        return 6;
    case MULT:
        return 5;
    case DIV:
        return 5;
    case LPAREN:
        return 2;
    case RPAREN:
        return 2;
    default:
        return 0;
    }
    return pre;
}
char Token::getTypeChar()
{
    switch (this->getType())
    {
    case ADD:
        return '+';
    case SUBT:
        return '-';
    case MULT:
        return '*';
    case DIV:
        return '/';
    case LPAREN:
        return '(';
    case RPAREN:
        return ')';
    case VAR:
        return 'V';
    case LITERAL:
        return 'L';
    default:
        return '\0';
    }
}
bool Token::isBinaryOperation()
{
    switch (this->getType())
    {
    case ADD:
        return true;
    case SUBT:
        return true;
    case MULT:
        return true;
    case DIV:
        return true;
    case LPAREN:
        return false;
    case RPAREN:
        return false;
    case VAR:
        return false;
    case LITERAL:
        return false;
    default:
        return '\0';
    }
}
void Token::setValue(String string)
{
    this->value = string;
}
// TODO
bool Token::isUnaryOperation()
{
    return false;
}
bool Token::isHigherPrecedenceThan(Token tok)
{
    return this->getPrecedence() >= tok.getPrecedence();
}
std::vector<Token> tokenization(String expression)
{
    std::vector<Token> tokens{};
    int len = expression.length();
    for (int i = 0; i < len; i++)
    {
        switch (expression.at(i))
        {
        case '*':
            tokens.push_back(consume('*', Type::MULT));
            break;
        case '/':
            tokens.push_back(consume('/', Type::DIV));
            break;
        case '+':
            tokens.push_back(consume('+', Type::ADD));
            break;
        case '-':
            tokens.push_back(consume('-', Type::SUBT));
            break;
        case '(':
        case '[':
        case '{':
            tokens.push_back(consume('(', Type::LPAREN));
            break;
        case ')':
        case '}':
        case ']':
            tokens.push_back(consume(')', Type::RPAREN));
            break;
        case ' ':
        case '\r':
        case '\n':
            // Just skip this.
            break;
        case '_':
            // Can be the start of a variable:
            i = consumeVar(expression, i, tokens);
            break;
        case '.':
            // Can be the start of a floating point literal.
            // \d*.?\d*e-?\d+ for ieee numbers.
            i = consumeLit(expression, i, tokens);
            break;
        case '0':
            // Might be a normal literal, but if it is followed by an x it is a hex digit, or if it is followed by a normal non-zero digit it is an oktal number.
            // TODO!
            break;
        default:
            if (isLetter(expression.at(i)))
            {
                i = consumeVar(expression, i, tokens);
            }
            else if (isDigit(expression.at(i)))
            {
                i = consumeLit(expression, i, tokens);
            }
            else
            {
                std::cout << "There is no such token!" << std::endl;
            }
            break;
        }
    }
    return tokens;
}
std::vector<Token> infixtopostfix(std::vector<Token> tokens)
{
    std::vector<Token> posfix;
    std::deque<Token> operators;
    Token temp = Token("\0", Type::ADD);

    if (tokens.size() == 0)
        return std::vector<Token>();

    for (Token &tok : tokens)
    {
        if (tok.isBinaryOperation())
        {
            // operator:
            while (!operators.empty() && (!operators.front().isParen() && (tok.isHigherPrecedenceThan(operators.front()))))
            {
                Token temp = operators.front();
                operators.pop_front();
                posfix.push_back(temp);
            }
            operators.push_front(tok);
        }
        else if (tok.isUnaryOperation())
        {
            operators.push_front(tok);
        }
        else if (tok.isLeftParen())
        {
            // Left parenthesis:
            operators.push_front(tok);
        }
        else if (tok.isRightParen())
        {
            // Right parenthesis:
            while (operators.front().getType() != Type::LPAREN)
            {
                posfix.push_back(operators.front());
                operators.pop_front();
            }
            operators.pop_front();
        }
        else
        {
            // Constant/Variable:
            posfix.push_back(tok);
        }
    }
    while (!operators.empty())
    {
        posfix.push_back(operators.front());
        operators.pop_front();
    }

    return posfix;
}
double eval(std::vector<Token> rpn)
{
    // http://www-stone.ch.cam.ac.uk/documentation/rrf/rpn.html
    std::deque<double> stack;
    std::reverse(rpn.begin(), rpn.end());
    double operand1, operand2;
    while (!rpn.empty())
    {
        if (rpn.back().isBinaryOperation())
        {
            // The rightmost operand in the binary operation:
            operand2 = stack.front();
            stack.pop_front();
            // The leftmost operand in the binary operation:
            operand1 = stack.front();
            stack.pop_front();

            // The item on the stack is an operation:
            switch (rpn.back().getType())
            {
            case ADD:
                stack.push_front(operand1 + operand2);
                break;
            case SUBT:
                stack.push_front(operand1 - operand2);
                break;
            case MULT:
                stack.push_front(operand1 * operand2);
                break;
            case DIV:
                stack.push_front(operand1 / operand2);
                break;
            default:
                std::cout << "This operation does not exist!" << std::endl;
                return 0.0;
            }
            rpn.pop_back();
        }
        else
        {
            // It is an operand:
            stack.push_front(stod(rpn.back().getValue()));
            rpn.pop_back();
        }
    }
    return stack.front();
}
double evaluate(String expr)
{
    std::vector<Token> tokens = tokenization(expr);
    tokens = infixtopostfix(tokens);
    return eval(tokens);
}
int consumeVar(String expression, unsigned pos, std::vector<Token> &tokens)
{
    unsigned long long skip = pos;
    for (unsigned itr = pos; (expression.length() != itr) && (isLetter(expression.at(itr)) || (expression.at(itr) == '_') || (isDigit(expression.at(itr)))); itr++)
    {
        skip++;
    }
    tokens.push_back(Token{expression.substr(pos, skip - pos), Type::VAR});
    return skip - 1;
}
int consumeLit(String expression, unsigned pos, std::vector<Token> &tokens)
{
    unsigned long long skip = pos;
    for (unsigned itr = pos; (expression.length() != itr) && (isDigit(expression.at(itr)) || (expression.at(itr) == '.')); itr++)
    {
        skip++;
    }
    tokens.push_back(Token{expression.substr(pos, skip - pos), Type::LITERAL});
    return skip - 1;
}
int print(std::vector<Token> tokens)
{
    for (Token &tok : tokens)
    {
        std::cout << tok.toString() << std::endl;
    }
    return 1;
}
