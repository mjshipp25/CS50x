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
        
        
        int key = atoi(argv[1]); // stores correct key in int value
        string s = get_string("plaintext: "); // prompts user for a text to cipher
        printf("ciphertext: "); // prints the beginning of answer
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            if (isalpha(s[i]))
            {
                int c = s[i] + (key % 26); // cipher-ed
                if ((isupper(s[i]) && c > 'Z') || (islower(s[i]) && c > 'z')) // wrap arounds for z
                {
                    printf("%c", c - 26);
                }
                else
                {
                    printf("%c", c);
                }
            }

            else
            {
                printf("%c", s[i]);
            }
        }
        printf("\n");

    }
    else
    {
    // if more or less than 2 args then prints error
        printf("Usage ./caesar key\n");
        return 1;
    }
}


