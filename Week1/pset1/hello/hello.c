#include <stdio.h>
#include <cs50.h>
int main(void)
{
    string s = get_string("What is your name?\n");
    // Gets name

    printf("hello, %s\n", s);
    //says name
}