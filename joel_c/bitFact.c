//cited:
//http://www.techcrashcourse.com/2015/08/c-program-t-convert-decimal-number-binary.html
//hhtps://stackoverflow.com/questions/2416483/how-to-find-a-factorial
//
//Author: Joel Cribby V00850649
//Date: 2017-09-23
//Course CSC 230
//Professor: LillAnne Jackson
//
//Description: This program takes an inputted integer, calculates the
//factorial of that integer and outputs the result in decimal, hexidecimal
//and binary. 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE_INT 16

//Function to compute and return the factorial of a number
unsigned short factorial(unsigned short num) {
	if (num <= 0)
		return 1;
	return (num * factorial(num - 1));	
}

//Prints the unsigned char array of bits
void printBitArray(unsigned char* theBits) {
    int i;
    for (i = SIZE_INT-1; i >= 0; i--) {
        printf("%d", theBits[i]);
    }    
}

//Converts an unsigned short number into an unsigned char array containing
//the binary equivalent of the inputted number
void toBits(unsigned short value, unsigned char inBits[SIZE_INT]) {
    int i;
    for (i = 0; i < SIZE_INT; i++) {
        inBits[i] = value & (1 << i) ? 1 : 0;
    }

}

//Prints the hexidecimal equivalent of a binary number
void printHex(unsigned char* theBits) {
    int i;
    int num = 0; 
    int count = 0;
    
    for(i = 15; i >= 0; i--) {
        //Equation to calculate binary to hexidecimal conversion
        num = num + (theBits[i] << ((i) % 4));
        count++;
        //Matches calculated number to switch case after each multiple of 4
        //iteration and prints the corresponding hexidecimal character
        if (count == 4) {
            switch (num) {
                case 0:
                    printf("0");
                    break;
                case 1:
                    printf("1");
                    break;
                case 2:
                    printf("2");
                    break;
                case 3:
                    printf("3");
                    break;
                case 4:
                    printf("4");
                    break;
                case 5:
                    printf("5");
                    break;
                case 6:
                    printf("6");
                    break;
                case 7:
                    printf("7");
                    break;
                case 8:
                    printf("8");
                    break;
                case 9:
                    printf("9");
                    break;
                case 10:
                    printf("a");
                    break;
                case 11:
                    printf("b");
                    break;
                case 12:
                    printf("c");
                    break;
                case 13:
                    printf("d");
                    break;
                case 14:
                    printf("e");
                    break;
                case 15:
                    printf("f");
                    break;
                default:
                    printf("\n\nError\n\n");
            }
            count = 0;
            num = 0;
        }
    }
}


int main(int argc, char *argv[]) {
    int num;
    unsigned short fact;
    unsigned char bits[SIZE_INT];
    char choice;

    printf("FACTORIAL & BIT TESTER\n\n");
    //Loops through main function if user inputs "y" when prompted
    do {
        //Loops through main function if incorrect input is provided
        do {
            printf("Input a positive integer ==> ");
            scanf("\n%d", &num);
            if (num <= 0) {
                printf("Incorrect input. Try again.\n\n");
                fflush(stdin);        
            }    
        } while ((num >= 0 && isdigit(num)));
        fact = factorial(num);
        toBits(fact, bits);   
        printf("  %d    Factorial =  %d or 0x", num, fact);
        printHex(bits);
        printf(" or 0b");
        printBitArray(bits);
        printf("\n\n");
        fflush(stdin);
        //(y/n) question loop
        do {
            printf("Do another (y/n)? ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'n') {
                break;
            } else {
                printf("\n\nIncorrect input. Try again.\n\n");
                continue;
            }
        } while ((choice != 'y') && (choice != 'n'));
    } while (choice == 'y');
    return 0;
}
