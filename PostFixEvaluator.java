/* Joel Cribby - 100134591
COMP 2113 X1
Assignment 3

Some Assignment contributions owed to stackoverflow.com and Object-Oriented 
Data Structures using Java. 3rd Edition, N. Dale, D.T. Joyce, C. Weems. Jones 
and Barlett Computer Science, ISBN: 1449613543
*/

import java.io.*;
import java.util.*;


public class PostFixEvaluator
{
	
   public static int evaluate(String expression)
   {
      ArrayListStack <Integer> stack = new ArrayListStack <Integer>();
      
      int value;
      String operator;
      
      int operand1;
      int operand2;
      
      int result= 0;
	  
      Scanner tokenizer = new Scanner(expression);
	  
      
      while (tokenizer.hasNext())
      {
         if (tokenizer.hasNextInt())
         {
            value= tokenizer.nextInt();
			
            stack.push(value);
         }
         
         else
         {
            operator= tokenizer.next();
            
            if(stack.isEmpty())
              throw new PostFixException("Not enough operands- stack" +
                                         "underflow");
            
            operand2= stack.top();
            stack.pop();
            
            if (stack.isEmpty())
              throw new PostFixException("Not enough operands - stack"+
                                         "underflow");
            
            operand1= stack.top();
            stack.pop();
            
				if (operator.equals("/")) {
					result= operand1/operand2;
				
				} else if(operator.equals("*")) {
					result= operand1 * operand2;
            
				} else if(operator.equals("+")) {
					result= operand1 + operand2;
            
				} else if(operator.equals("-")) {
					result= operand1-operand2;
				
				} else {
					throw new PostFixException("Illegal symbol: " + operator);
				}
				
				stack.push(result);
			
         }
      }
      
      if(stack.isEmpty())
        throw new PostFixException("Not enough operands - stack underflow");
      result= stack.top();
      stack.pop();
      
      if(!stack.isEmpty())
        throw new PostFixException("Too many operands - operands left over");
	 
      return result;
   }
}

interface UnboundedStackInterface<T> extends StackInterface<T> {
  void push(T value);

}
   
interface StackInterface<Integer> {
	   
	   void pop() throws StackUnderflowException;
	   
	   Integer top() throws StackUnderflowException;
   }
   
class PostFixException extends RuntimeException
{
  public PostFixException()
  {
    super();
  }

  public PostFixException(String message)
  {
    super(message);
  }
}

class StackUnderflowException extends Exception
{
  StackUnderflowException(String msg)
  {
    super(msg);
  }
}


class ArrayListStack<T> implements UnboundedStackInterface<T>
{
  protected ArrayList<T> stack;  

  public ArrayListStack() 
  {
    stack = new ArrayList<T>();      
  }

  public void push(T value)   

  {
    stack.add(value);
  }

  public void pop()               

  {
	try {
		if (!isEmpty())
		{
		stack.remove(stack.size() - 1);
		
		} else {
		throw new StackUnderflowException("Pop was used on an empty stack.");
		}
	} catch(StackUnderflowException ex) {
			
			System.out.println("Exception occurred: " + ex);
			
			System.exit(1);
	}
  }

  public T top()             
  {
    T topOfStack = null;
	try {
		if (!isEmpty()) {
		topOfStack = stack.get(stack.size() - 1);
		} else {
		throw new StackUnderflowException("Top was used on an empty stack."); 
		}
	} catch(StackUnderflowException ex) {
			
			System.out.println("Exception occurred: " + ex);
			
			System.exit(1);
	}
    return topOfStack;
  }

  public boolean isEmpty()         
  {
    if (stack.size() == 0)
      return true;
    else 
      return false;
  }
}

