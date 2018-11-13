import os
import sys
       
def divisibleNumbers(n):
    if(int(n) % 10 == 0): return 0;

    for i in range(int(n), sys.maxsize, int(n)):
        if('0' not in str(i)):
            arr = list(str(i));
            print(arr);
            if(compareAddProd(arr)):
                digitString = str(i);
                return len(digitString);

def compareAddProd(arr):
    addResult = 0;
    prodResult = 1;
    for x in arr:
        addResult += int(x);
        prodResult *= int(x);
    return addResult >= prodResult;

if __name__ == '__main__':
    n = input("Input number ");
    print("The smallest divisble number of " + n + " is...");
    print(divisibleNumbers(n));
