// File name: javaCode.java
// Purpose: implementing mathematical expression evaluations in java
// Author: Madeline Krehely
// Date Modified: 11/25/24

import java.util.Stack;
import java.util.Scanner;

enum ElementType {NUM, OP}

class Element {
    private String v;
    private ElementType type;

    public Element(String v, ElementType type) {
        this.v = v;
        this.type = type;
    }

    public String getString() {
        return v;
    }

    public ElementType getType() {
        return type;
    }
}

public class javaCode {
    public Stack<Element> stack1 = new Stack<>();
    public Stack<Element> stack2 = new Stack<>();

    private int getPrecedence(String op) {
        switch(op) {
            case "^": return 3;
            case "*":
            case "/": return 2;
            case "+":
            case "-": return 1;
            default: return 0;
        }
    }

    private void printStack(Stack<Element> s) {
        if (s.empty()) return;

        Element t = s.pop();
        printStack(s);
        System.out.print(t.getString() + " ");
        s.push(t);
    }

    private boolean isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }

    private void postfix(String s) throws Exception {
        this.stack2.push(new Element("$", ElementType.OP));

        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);

            if (c == ' ') {
                continue;
            } else if (c == '(') {
                this.stack2.push(new Element("(", ElementType.OP));
            } else if (c == ')') {
                while(!this.stack2.isEmpty()) {
                    Element t = this.stack2.pop();
                    if (t.getString().equals("(")) {
                        break;
                    } else if (t.getString().equals("$")) {
                        throw new Exception("Error... mismatched parenthesis");
                    }
                    this.stack1.push(t);
                } // right parenthesis
            } else if (Character.isDigit(c)) {
                StringBuilder number = new StringBuilder();
                while (i < s.length() && Character.isDigit(s.charAt(i))) {
                    number.append(s.charAt(i));
                    i++;
                } // while
                i--;
                this.stack1.push(new Element(number.toString(), ElementType.NUM));
            } else if (isOperator(c)) {
                String oper = String.valueOf(c);
                int precedence = getPrecedence(oper);

                while (!this.stack2.isEmpty()) {
                    Element t = this.stack2.peek();
                    int topPrec = getPrecedence(t.getString());

                    if (precedence <= topPrec) {
                        this.stack1.push(stack2.pop());
                    } else {
                        break;
                    }
                }
                this.stack2.push(new Element(oper, ElementType.OP));
            } else {
                throw new Exception("Invalid expression: " + c);
            }
        }
        while (!this.stack2.isEmpty()) {
            Element t = this.stack2.pop();
            if (!t.getString().equals("$")) {
                this.stack1.push(t);
            }
        }
    }

    private int evaluate() throws Exception {
        int size = this.stack1.size();
        Element[] s = new Element[size];
        int index = size-1;

        while(!this.stack1.isEmpty()) {
            s[index--] = this.stack1.pop();
        }

        for (int i = 0; i < size; i++) {
            if (s[i].getType() == ElementType.OP) {
                if (i < 2) throw new Exception("Invalid expression");

                int b = Integer.parseInt(s[i-1].getString());
                int a = Integer.parseInt(s[i-2].getString());
                int r; // result

                switch (s[i].getString()) {
                    case "+":
                        r = a + b;
                        break;
                    case "-":
                        r = a - b;
                        break;
                    case "*":
                        r = a * b;
                        break;
                    case "/":
                        if (b == 0) throw new Exception("Division by 0");
                        r = a/b;
                        break;
                    case "^":
                        r = (int) Math.pow(a, b);
                        break;
                    default:
                        throw new Exception("Invalid");
                }

                s[i-2] = new Element(String.valueOf(r), ElementType.NUM);

                System.arraycopy(s, i + 1, s, i - 1, size - i - 1);
                size -= 2;
                i -= 2;
            }
        }
        return Integer.parseInt(s[0].getString());
    }
}

public static void main(String[] args) {
    Scanner scanner = new Scanner(System.in);
    javaCode evaluator = new javaCode();

    System.out.print("Enter your expression: ");
    String s = scanner.nextLine();

    try {
        evaluator.postfix(s);
        int r = evaluator.evaluate();
        System.out.println("Result = " + r);
    } catch (Exception e) {
        System.out.println(e.getMessage());
    }
}