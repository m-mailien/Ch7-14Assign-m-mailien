# File name: pythoncode.py
# Purpose: implementing mathematical expression evaluations in python
# Author: Madeline Krehely
# Date Modified: 11/22/24

# define stacks
stk1 = []
stk2 = []

# define operator precedence as dictionary (array)
p = {'$': 0, '(': 1, '+': 2, '-': 2, '*': 3, '/': 3, '^': 4}

# get input from user
s = input("Enter your expression: ")

# check if char is operation
def isOperator(m):
    return m in ["+", "-", "*", "/", "^"]

# postfix func; loops stuff
def postfix(s):
    global stk1, stk2
    stk1 = []
    stk2 = ['$']
    s = list(s)
    k = 0

    while k < len(s): # cases; standard while loop
        if s[k] == " ": 
            k += 1 
            continue # spaces encountered
        if s[k] == "(": # right parenthesis encountered
            stk2.append(s[k]) 
        elif s[k] == ")":# left parenthesis encountered
            while True:
                if stk2[-1] == "$":
                    raise ValueError("Invalid expression - mismatched parenthesis")
                c = stk2.pop()
                if c == "(": 
                    break
                stk1.append(c)
        elif s[k] >= "0" and s[k] <= "9": # digit encountered
            t = ""
            while k < len(s) and s[k] >= "0" and s[k] <= "9":
                t += s[k]
                k += 1
            k -= 1
            stk1.append(int(t))
        elif isOperator(s[k]): # operator encountered
            while p[s[k]] <= p.get(stk2[-1], 0):
                stk1.append(stk2.pop())
            stk2.append(s[k])
        else: 
            raise ValueError(s[k] + " Invalid character found") # invalid character entered

        k += 1 # increment k; end first while loop
    
    while stk2: # loop through stk2 to move elements to stk1
        c = stk2.pop()
        if c == '$': 
            break
        if c == "(": 
            raise ValueError("Mismatched parenthesis")
        stk1.append(c)

    print("postfix: ", stk1)

# evaluate operation function
def evalOp(stk1, i):
    if i < 2: raise ValueError("Invalid expression - can't compute")
    k = stk1[i-1]
    j = stk1[i-2]
    if type(k) != int or type(j) != int: raise ValueError("Invalid expression...")
    if stk1[i] == "+": r = k+j
    elif stk1[i] == "-": r = k-j
    elif stk1[i] == "*": r = k*j
    elif stk1[i] == "/":
        if k == 0: raise ValueError("Division by zero")
        else: r = j/k
    elif stk1[i] == "^": r = j**k
    return r

    #if ans == "+": 
    #    return a + b
    #elif ans == "-": 
     #   return a - b
   # elif ans == "*": 
   #     return a * b
   # elif ans == "/":
    #    if b == 0: raise ValueError("Division by zero")
    #    else: return a / b
    #elif ans == "^":
    #    return a ** b

# evaluate function
def evaluate():
    stack = []
    for token in stk1:
        if isinstance(token, int):
            stack.append(token)
        elif isOperator(token):
            if len(stack) < 2:
                raise ValueError("insufficient")
            b = stack.pop()
            a = stack.pop()
            stack.append(evalOp(a, b, token))
        else:
            raise ValueError("Invalid token")
    if len(stack) != 1:
        raise ValueError("unresolved token")
    return stack[0]
    #i = 0
    #size = len(stk1)
    #while i < size:
     #   if isOperator(stk1[i]):
      #      r = evalOp(stk1, i)
       #     stk1[i-2] = r
        #    for k in range(i+1, len(stk1)):
         #       stk1[k-2] = stk1[k]
          #  i -= 2
           # size -= 2
        #i += 1
   # return stk1[0]

# main
try:
    postfix(s)
    print("Result=", evaluate())
except ValueError as v:
    print(v)

# test cases now work but i had to change the given code significantly? 