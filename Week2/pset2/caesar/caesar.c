#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc == 2) // checks if one argument
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (isdigit(argv[1][i])) // checks if a number
            {
                ;
            }
            else
            {
                printf("Usage ./caesar key\n"); // cancels program if i is not a number
                return 1;
            }
        }
        // stores key in int
        int key = atoi(argv[1]);

        // prompts user for string
        string s = get_string("plaintext: ");

        // prints beginning of cipher
        printf("ciphertext: ");

        // converts the plaintext to cipher
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            // starts cipher if the char is a letter
            if (isalpha(s[i]))
            {
                // creates int of ciphered letter
                int c = s[i] + (key % 26);

                // wraps around int if higher the Z or z
                if ((isupper(s[i]) && c > 'Z') || (islower(s[i]) && c > 'z'))
                {
                    // prints int as char
                    printf("%c", c - 26);
                }

                // prints int as a char
                else
                {
                    printf("%c", c);
                }
            }

            // prints char if its punctuation
            else
            {
                printf("%c", s[i]);
            }
        }

        // prints new line after ciphered text
        printf("\n");
    }
    else
    {
        // if more or less than 2 args then prints error
        printf("Usage ./caesar key\n");
        return 1;
    }
}


