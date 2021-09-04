#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0) // checks if name string is in the candidates array
        {
            candidates[i].votes++; // adds vote to matching candidate
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    candidate winners[candidate_count];
    int winner_length = 0; // Counts the length of the winner array 
    for (int i = 0; i < candidate_count; i++)
    {
        // Adds Candidate to Winner array if their votes are greater than the 1st value in winner array
        if (candidates[i].votes > winners[0].votes)
        {
            winners[0] = candidates[i];
           
        }
        // Adds Candidate to Winner array if their votes are the same as 1st value in winner array
        else if (candidates[i].votes == winners[0].votes)
        {
            winner_length++;
            winners[winner_length].name = candidates[i].name;
        }

    }
    // prints the names in winner array
    for (int i = 0; i <= winner_length; i++)
    {
        printf("%s\n", winners[i].name);
    }
    return;
}

