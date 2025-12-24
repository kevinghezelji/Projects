#include <stdio.h>

typedef struct // Struct to hold count
{
    char text[64];  // Storage for each word
    int count;      // Count of that word in file
}WORD;

int wordCount = 0;  // Global Count set up
WORD storage[1024]; // Global Array set up

int stringCompare(char *stringOne, char *stringTwo) // Function to compare if two strings are identical
{
    for (int i = 0; stringOne[i] == stringTwo[i]; i++) // Loops between two strings entry by entry
        {
        if (stringOne[i] == '\0') // Checks if String One makes it to NULL Operator (end of string)
            {
            return 0; // Returns 0 if Identical
        }
    }
    return 1; // If the strings are not identical, Function returns 1
}

void stringCopy(char *original, char *copy) // Function to copy strings
{
    int i;
    for (i = 0; original[i] != '\0'; i++) // Passover Loop
        {
        copy[i] = original[i]; // Entry from original becomes entry for copy
    }
    copy[i] = '\0'; // Manually close the string
}

void lowercase(char string[]) //Function to Lowercase String Array's passed to it
{
    for (int i = 0; string[i] != '\0'; i++) // Passover loop
        {
        if (string[i] >= 'A' && string[i] <= 'Z') // Checks to see if ASCII value falls between capital range
            {
            string[i] += 32; // If so, then add 32 to convert it to lowercase counterpart
        }
    }
}

void stripChar(char string[]) // Function to strip String Array's of all special characters with spaces between
{
    for (int i = 0; string[i] != '\0'; i++) // Passover Loop
        {
        if (!( (string[i] >= 'a' && string[i] <= 'z') ||
            (string[i] >= 'A' && string[i] <= 'Z') ||
            (string[i] >= '0' && string[i] <= '9') ||
            (string[i] == ' ') || (string[i] == '\n') )) // Checks if entry is a special character
            {
        string[i] = ' '; // If so, replaces it with a space
        }
    }
}

// COMMENT ON FUNCTION
void updateCount(char *word) {
    for (int i = 0; i < wordCount; i++) // wordCount starts at 1, ignores first passover
        {
        if (stringCompare(storage[i].text, word) == 0) // If the return is zero the words are identical
            {
            storage[i].count++; // Add to the words count
            return;
        }
    }
    stringCopy(word, storage[wordCount].text); // If word doesn't match it copies it to storage
    storage[wordCount].count = 1; // Sets the starting count
    wordCount++; // Updates global
}

void stringTokenizer(char *ptr) // Function to tokenize each separate word to set up for count
{
    while (*ptr != 0) // Loop to go until final NULL Operator
        {
        while ((*ptr == ' ' || *ptr == '\n') && *ptr != '\0') // Search for the first entry of a word
            {
            ptr++;                                         // Moves on to next memory cell
        }
        if (*ptr == '\0') break;          // Safety check
        char *start = ptr;                // Marks the start of the word

        while ((*ptr != ' ' && *ptr != '\n') && *ptr != '\0') // Search for the end of the word
            {
            ptr++;                          // Moves on to next memory cell
        }
        char temp = *ptr;   // Save the where the check stopped into Temporary Variable
        *ptr = '\0';        // Overwrite the stop with a NULL Operator

        updateCount(start);

        *ptr = temp;        // Put the entry back

        if (temp == '\0') break;    // Safety check
        ptr++;                      // Moves on to next memory cell
    }
}

void processFile(FILE *fp) // Function to read through file and clean up data
{
    char temp[256]; // String set up
    while (fgets(temp, sizeof(temp), fp) != NULL) // While loop to go through file line by line
        {
        stripChar(temp); // Strip all special and unrequired characters
        lowercase(temp); // Set every character to lowercase
        stringTokenizer(temp); // Tokenize the words
    }
}

void printResults() // Function to display results
{
    printf("WORD\tCOUNT\n"); // Header
    for (int i = 0; i < wordCount; i++) // Passover loop to print Array
    {
        printf("%s\t%d\n", storage[i].text, storage[i].count); // Prints each word and how many times it appears
    }
}

int main()
{
    char filename[256]; // String initialization

    printf("File Analyzer\nPlease enter filename: "); // Print and Scan functions for filename
    scanf("%s", filename);

    FILE *fp = fopen(filename, "r"); // Open file and set to read
    if (fp == NULL) // Make sure file opens
        {
        printf("Error, File could not be opened\n"); // Error statement
        return 1; // Program failed
    }

    processFile(fp); // Pass to functions
    fclose(fp); // Close file
    printResults(); // Print

    return 0;
}