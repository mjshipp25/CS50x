#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string s);
int count_words(string s);
int count_sent(string s);

int main(void)
{
    // prompts for text
    string s = get_string("Text: ");
    int l = count_letters(s);
    int w = count_words(s);
    int e = count_sent(s);
    
    // formula: index = 0.0588 * L - 0.296 * S - 15.8
    // compute index
    float adj_L = (l * 100.0) /  w;
    float adj_S = (e * 100.0) /  w;
    float index = 0.0588 * adj_L - 0.296 * adj_S - 15.8;
    index = round(index);
    
    // prints index
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index < 16)
    {
        printf("Grade %i\n", (int) index);
    }
    else
    {
         printf("Grade 16+\n");
    }
}

// counts letters
int count_letters(string s)
{
    int x = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isalpha(s[i]))
        {
            x++;
        }
    }
    return x;
}

// counts words
int count_words(string s)
{
    int x = 0;
    for (int i = 0,  n = strlen(s); i < n; i++)
    {
        if (isspace(s[i]))
        {
            x++;
        }
    }
    return x + 1;
}

// counts sentences
int count_sent(string s)
{
    int x = 0;
    for (int i = 0,  n = strlen(s); i < n; i++)
    {
        if ((s[i]) == '.' || (s[i]) == '?' || (s[i]) == '!')
        {
            x++;
        }
    }
    return x ;
}



