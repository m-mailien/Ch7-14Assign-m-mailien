// File name: c++code.cpp
// Purpose: implementing mathematical expression evaluations in C++
// Author: Madeline Krehely
// Date Modified: 11/24/24

#include <iostream>
#include <stack>
#include <cmath>
using namespace std;

// declare enum that defines element type
enum ElementType {NUM, OP};
class Element 
{
private:
    string v;
    ElementType type;
public:
    string getString() {return v;}
    ElementType getType() {return type;}
    Element(string s, ElementType t)
    {
        v = s;
        type = t;
    }
    Element()
    {
        v = "";
        type = NUM;
    }
};

stack<Element> stk1, stk2;

// precedence func
int getPrecedence(string op)
{
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

// recursive function for debugging purposes
void printStack(stack<Element> s)
{
    if (s.empty()) return;

    Element t = s.top();
    s.pop();

    printStack(s);

    cout << t.getString() << " ";
    s.push(t);
}

// boolean to identify if string is operator
bool isOperator(char c) 
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

void postfix(string s) // convert infix to postfix
{
    char oper;
    string temp;
    Element t("$", OP);
    stk2.push(t);

    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ') continue; // ignores spaces

        else if (s[i] == '(') // if left parenthesis, push stk2
        {
            stk2.push(Element("(", OP));
            continue;
        }

        else if (s[i] == ')') // right parenthesis; move elements from stk2 to stk1 until left parenthesis found
        {
            while (!stk2.empty()) 
            {
                Element t = stk2.top();
                if (t.getString() == "(") // stop when left found
                {
                    stk2.pop();
                    break;
                } // if
                if (t.getString() == "$") // if reach bottom, error
                {
                    throw string("Error... mismatched parenthesis");
                } // if
                // move elements from stk2 to stk1
                Element k = stk2.top();
                stk2.pop();
                stk1.push(k);
            } // closes while loop
        } // closes right parenthesis else if

        else if (s[i] >= '0' && s[i] <= '9') // collect digits
        {
            string t = "";
            while (s[i] >= '0' && s[i] <= '9')
            {
                t += s[i];
                i++;
            } // while
            i--;
            stk1.push(Element(t, NUM)); // push string t to stk1
        } // else if

        else if (isOperator(s[i])) // if operator encountered, push into stk2 if greater than top operator
        {
            string oper(1, s[i]); // convert incoming to string
            int k = getPrecedence(oper); // get precedence
            while (!stk2.empty())
            {
                Element t = stk2.top(); // get top element
                int j = getPrecedence(t.getString()); // get precedence
                if (k <= j) // if incoming precedence <= top precedence, move operator
                {
                    stk1.push(t);
                    stk2.pop();
                } // if
                else // otherwise, incoming > top
                {
                    Element t(oper, OP);
                    stk2.push(t);
                    break;
                } // else
            } // while
        } // if isOperator

        else throw s[i] + string("Invalid character encountered in expression.");

        while (!stk2.empty()) // move leftovers into stk1
        {
            Element t = stk2.top();
            if (t.getString() == "$") break;
            stk2.pop();
            stk1.push(t);
        }
    } // big for loop
} // end postfix func

int evaluate() // get postfix notation of infix typed by user
{
    int size = stk1.size();
    Element s[size];
    int index = size-1;
    // empty stk1 into string s
    while (!stk1.empty()) 
    {
        Element t = stk1.top();
        stk1.pop();
        s[index] = t;
        index--;
    }

    // s contains postfix; it loops from 0 to end of the array
    for (int i = 0; i < size; i++)
    {
        if (s[i].getType() == OP) 
        {
            if (i < 2) throw string("Invalid expression");
            if (s[i-1].getType() != NUM) throw string("Invalid expression");
            if (s[i-2].getType() != NUM) throw string("Invalid expression");

            int a = stoi(s[i-1].getString()); // get prev index #
            int b = stoi(s[i-2].getString()); // get prev prev index #
            int r; // store result
            
            string op = s[i].getString(); // get operator
            if (op == "+") r = (a + b);
            else if (op == "-") r = (b - a);
            else if (op == "*") r = (b * a);
            else if (op == "/")
            {
                if (a == 0) throw string("Division by zero");
                r = (b/a);
            } // / operator
            else if (op == "^") r = pow(a, b);

            s[i-2] = Element(to_string(r), NUM); // shift array twice from i-2
            for (int j = (i + 1); j < size; j++)
            {
                s[j - 2] = s[j];
            }
            size -= 2; // adjust size
            i -= 2; // adjust i
        } // if getType == OP
    } // for loop

    return stoi(s[0].getString()); // return final
} // evaluate func

int main() // main
{
    string s;

    cout << "Enter your expression: ";
    getline(cin, s);

    int v = 0;
    try
    {
        {
            postfix(s); // call postfix
            v = evaluate();
            cout << "v = " << v << endl;
        }
    }
    catch (string s)
    {
        cout << s << endl;
    }
    
    return 0;
}

// test cases aren't working correctly??? not sure the best way to approach to fix the program

// test case 1 is returning invalid instead of 13
// test case 2 returns invalid parenthesis instead of 27
// test case 3 returns correctly (invalid)
// test case 4 also returns mismatched parenthesis instead of division by 0
// test case 5 returns correctly (invalid character found)