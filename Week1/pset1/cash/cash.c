#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    int c = 0;
    float cash;
    // ask for cash owed
    do
    {
        cash = get_float("Change owed: ");
    }
    while (cash < 0);
    // convert cash to change
    int change = round(cash * 100);
    // take out quarters
    while (change  >= 25)
    {
        change = change - 25;
        c++;
    }
    // take out dimes
    while (change >= 10)
    {
        change = change - 10;
        c++;
    }
    // take out nickles
    while (change >= 5)
    {
        change = change - 5;
        c++;
    }
    // take out pennies
    c = c + change;
    // prints coins
    printf("%i\n", c);
}
