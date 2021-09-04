#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Gets numbers from users
    float x = get_float("x: ");
    float y = get_float("y: ");
    
    // Divides x by y
    float z = x / y;
    printf("%f\n", z);
}