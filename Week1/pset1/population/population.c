#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int n;
    do
    {
        n = get_int("Start size: \n");
    }
    while (n < 9);
    
    // TODO: Prompt for end size
    int end;
    do
    {
        end = get_int("End size: \n");
    }
    while (end < n);
    
    //Keep track of years
    int year = 0; 
    
    // TODO: Calculate number of years until we reach threshold
    int born = n / 3;
    int die = n / 4;

    while (n < end)
    {
        n = n + (n / 3) - (n / 4);
        year++;
    }
    
    // TODO: Print number of years
    printf("Years: %i\n", year);
}