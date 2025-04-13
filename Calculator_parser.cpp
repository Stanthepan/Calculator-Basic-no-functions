#include <iostream>
#include <string>
#include <vector>
/*
 *Works, but not complete, can't find incorrect inputs, negative numbers and a lot of symbols (e.g. % functions e.t.c.).
 *Update: some invalid inputs are solved, negative numbers work, exponentiation fixed, and if functions like sin() cos() tan() are to be added,
 *then the stacks for operators need to be changed to std::string which will mess up the switch statements and any single quotation marks.
 *Also I just realized that floats aren't supported in input so that's something to work on.
 */
[[noreturn]] void terminateProgram(std::string errorMessage = "Invalid input, problem occured when evaluating.\n")
{
    //[_Analysis_noreturn_];
    std::cout << errorMessage;
    terminate();
}
int precedence(const char operator_)
{
    switch (operator_)
    {
    case '(':
        return 5;
    case '^':
        return 4;
    case '*':
    case 'x':
    case '/':
        return 3;
    case '+':
    case '-':
        return 2;
    case '%':
        return 1;
    case '=':
        return 0;
    default:
        return -10;
    }
}
char associativity(char operator_)
{
    switch (operator_)
    {
    case '^':
        return 'r';
    case '#': //peak
        return 'd';
    default:
        return 'l';
    }
}
bool isAsciiNum(char const n)
{
    return n >= '0' && n <= '9';
}
bool isValidChar(char const c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    switch (c)
    {
    case '(':
    case ')':
    case 'x':
    case '/':
    case '*':
    case '+':
    case '-':
    case '%':
    case '^':
    case ' ':
    case '.':
        return true;
    default:
        return false;
    }
}
std::string getReversePolishNotation(std::string input)
{
    std::string finalString;
    int count = 0; 
    std::vector<char> operatorStack;
    bool breakOut = false;
    std::erase(input, ' ');
    while (count < static_cast<int>(input.length()))
    {
        if (input[count] == '(')
        {
            operatorStack.push_back(input[count]);
            count++;
            continue;
        }
        if (count == 0 and input[count] == '-')
        {
            if (!isAsciiNum(input[count+1]))
            {
               return "Exit failure."; //Error case
            }
            finalString += input[count];
            count++;
        }
        while (isAsciiNum(input[count])) //ASCII values of 0-9 range from 49-57
        {
            finalString += input[count];
            count++;
        }
        //for negative numbers, since whitespace is stripped, the '-' should go in front of the number (assuming there is another operator)
        //e.g. 5 + -3 to RPN should be 5 -3 +
        //this means for execution a number can be checked for negativity by just checking if input[count+1] is a number, otherwise the '-' is an operator
        if (count == 0)
        {
            terminateProgram("Invalid input, stating character was not a number nor negative symbol.");
        }
        if (precedence(input[count-1]) > 0 and isAsciiNum(input[count+1])) // precedence > 0 means the character before is an operator
        {
            finalString += input[count];
            count++;
            continue;
        }
        finalString += ' '; //to split the numbers (so 4 5 - doesn't look like 45-)
        if (operatorStack.empty())
        {
            operatorStack.push_back(input[count]);
            count++;
            continue;   
        }
        if (input[count] == ')' )
        {
            while (operatorStack.back() != '(')
            {
                finalString += operatorStack.back();
                operatorStack.pop_back();
            }
            operatorStack.pop_back();
            count++;
            continue;
        }
        if (associativity(input[count]) == 'r' and precedence(input[count]) == precedence(operatorStack.back()))
        {
            operatorStack.push_back(input[count]);
            count++;
            continue;
        }
        if (precedence(input[count]) > precedence(operatorStack.back()) or operatorStack.back() == '(')
        {
            operatorStack.push_back(input[count]);
            count++;
            continue;
        }
        if (count >= static_cast<int>(input.length()))
            break;
        // this is causing problems
        while (!(precedence(input[count]) > precedence(operatorStack.back()) or operatorStack.back() == '(') and !isAsciiNum(input[count]))
        {
            finalString += operatorStack.back();
            operatorStack.pop_back();
            if (operatorStack.empty())
            {
                break;
            }
            if (precedence(operatorStack.back()) <= precedence(input[count]))
                operatorStack.push_back(input[count]);
            else
            {
                finalString += operatorStack.back();
                operatorStack.pop_back();
                operatorStack.push_back(input[count]);
            }
            count++;
            if (operatorStack.empty() or count >= static_cast<int>(input.length()))
            {
                breakOut = true;
                break;
            }
            if (operatorStack.empty())
                break;
        }
        if (breakOut)
            break;
        
    }
    while (!operatorStack.empty())
    {
        finalString += operatorStack.back();
        operatorStack.pop_back();
    }
    return finalString;
}
template <typename T, typename U>
T operation(T op2,T op1, U operator_)
{

    switch(operator_)
    {
    case '+':
        return op1 + op2;
    case '-':
        return op1 - op2;
    case '/':
        return op1 / op2;
    case 'x':
    case '*':
        return op1 * op2;
    case '^':
        return static_cast<float>(pow(op1, op2));
    case '%':
        return static_cast<float>(static_cast<int>(op1) % static_cast<int>(op2));
    default:
        return 3.00;
    }
}
float evaluateRPN(std::string input)
{
    std::cout << input << '\n';
    std::vector<float> stack;
    std::string tempString{};
    int count = 0;
    while (count < static_cast<int>(input.length()))
    {
        int change = 1;
        tempString = "";
        while (input[count] == ' ')
        {
            count++;
        }
        if (input[count] == '-' and isAsciiNum(input[count+1]))
        {
            change = -1;
            count++;
        }
        while (input[count] >= '0' && input[count] <= '9')
        {
            tempString += input[count];
            count++;
        }
        if (!tempString.empty())
        {
            stack.push_back(static_cast<float>(std::stoi(tempString) * change));
            continue;
        }
        if (stack.size() > 1)
        {
            const float x = stack.back();
            stack.pop_back();
            const float y = stack.back();
            stack.pop_back();
            float const b = operation(x,y,input[count]);
            if (static_cast<int>(b) == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
            stack.push_back(static_cast<float>(b));
            count++;
            continue;
        }
        count++;
    }
    if (stack.size() != 1)
    {
        terminateProgram("Invalid input, too many operators or operands given.");
    }
    return stack.back();
}
std::string getInput()
{
    std::string input;
    while (true)
    {
        bool valid = true;
        input = "";
        std::getline(std::cin, input);
        for (int i = 0; i < static_cast<int>(input.length()); i++)
        {
            if (!isValidChar(input[i]))
            {
                std::cout << "Invalid token, re-enter input." << '\n';
                valid = false;
                break;
            }
        }
        if (valid)
            break;
    }
    return input;
}

int main()
{
    while  (true){
        const std::string input = getInput();
        const std::string reversePolishNotation = getReversePolishNotation(input);
        if (reversePolishNotation == "Exit failure.")
        {
            std::cout << "Invalid input." << '\n';
            continue;
        }
        std::cout << evaluateRPN(reversePolishNotation) << '\n';
    }
}