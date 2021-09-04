#include <stdio.h>
#include <cs50.h>

int get_height_int(void);

// Prints pyramid
int main(void)
{
    int height = get_height_int(); // Sets the height obtained from the user prompt
    int vert = 0; // columns
    int horz = 0; // rows
    int rowNum = 1; // Tracks the row number
    int spCounter = 0; // Counts the number of spaces on a row
    while (vert < height) // Checks to see if the height is at the desired 
    {
        horz++; // sets rows to 1
        while (spCounter < (height - rowNum)) // starts space loop as long as the space counter is less than height-1
        {
            printf(" "); // prints n spaces
            spCounter++; // adds to space counter
        }
        printf("#"); // prints n #s
        while (vert < horz)  // starts loop for a new line
        {
            printf("\n"); // prints the new line
            vert++; // adds to column tracker
            rowNum++; // adds to row number
            horz = 0; // sets row to 0
            spCounter = 0; // sets space counter to 0
        }
    }
}
 
// Prompt User for Height
int get_height_int(void)
{
    int h;
    do 
    {
        h = get_int("Height: ");
    }
    while (h > 8 || h < 1);
    return h;
}