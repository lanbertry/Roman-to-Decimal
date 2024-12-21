#include <stdio.h>  // For reading and writing to files, and printing messages
#include <string.h> // For handling and manipulating strings
#include <stdlib.h> // For using standard library functions like memory allocation
#include <ctype.h>  // For functions that deal with character types (e.g., checking letters)

// Function declarations
int romanToDecimal(const char *roman);           // Converts Roman numeral to decimal
void convertToWords(int number, char *words);    // Converts a number to its word representation
int applyOperation(int num1, int num2, char op); // Applies an arithmetic operation (+, -, *)
int isValidRoman(const char *roman);             // Checks if a Roman numeral is valid
void processInputLine(char *line, FILE *output); // Processes each line of input from the file

// Main function where the program starts running
int main()
{
    FILE *input, *output; // Declare file pointers for input and output files
    char line[256];       // Array to hold each line read from the input file

    // Try to open the input and output files
    input = fopen("input.txt", "r");   // Open the input file in "read" mode
    output = fopen("output.txt", "w"); // Open the output file in "write" mode

    // If either file fails to open, print an error and exit
    if (!input || !output)
    {
        printf("Error: Couldn't open input/output files.\n"); // Error message if files can't be opened
        return 1;                                             // Exit the program with an error code
    }

    // Read each line from the input file and process it
    while (fgets(line, sizeof(line), input)) // Read a line from the input file
    {
        processInputLine(line, output); // Process the line and write the result to the output file
    }

    // After processing all lines, close the files
    fclose(input);  // Close the input file
    fclose(output); // Close the output file

    printf("Processing complete. Results are in output.txt.\n"); // Let the user know the process is finished
    return 0;                                                    // Return success
}

// Function to convert Roman numeral to decimal number
int romanToDecimal(const char *roman)
{
    int values[256] = {0}; // Array to store the values of Roman numeral characters, initialized to 0
    values['I'] = 1;       // Value of 'I'
    values['V'] = 5;       // Value of 'V'
    values['X'] = 10;      // Value of 'X'
    values['L'] = 50;      // Value of 'L'
    values['C'] = 100;     // Value of 'C'
    values['D'] = 500;     // Value of 'D'
    values['M'] = 1000;    // Value of 'M'

    int total = 0;              // To keep track of the total decimal value
    int length = strlen(roman); // Get the length of the Roman numeral string

    // Loop through each character of the Roman numeral string
    for (int i = 0; i < length; i++)
    {
        int current = values[roman[i]];                         // Get the value of the current Roman numeral character
        int next = (i + 1 < length) ? values[roman[i + 1]] : 0; // Look ahead to the next character's value

        // If the current numeral is smaller than the next, subtract it (e.g., IV = 4)
        if (current < next)
        {
            total -= current; // Subtract the current numeral's value
        }
        else
        {
            total += current; // Otherwise, add it (e.g., VI = 6)
        }
    }

    return total; // Return the total decimal value
}

// Function to check if a Roman numeral string is valid
int isValidRoman(const char *roman)
{
    while (*roman) // Loop through each character in the Roman numeral string
    {
        // If a character is not a valid Roman numeral, return 0 (false)
        if (!strchr("IVXLCDM", *roman))
        {
            return 0; // Invalid character found
        }
        roman++; // Move to the next character
    }
    return 1; // All characters are valid
}

// Function to convert a number to words
void convertToWords(int number, char *words)
{
    // Words for numbers 1-19
    const char *belowTwenty[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten",
                                 "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen",
                                 "Eighteen", "Nineteen"};
    // Words for tens (20, 30, 40, etc.)
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    // Words for larger numbers (thousand, million, billion)
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    // Special case for zero
    if (number == 0)
    {
        strcpy(words, "Zero"); // If the number is 0, simply return "Zero"
        return;
    }

    // Handle negative numbers
    if (number < 0)
    {
        strcpy(words, "Negative "); // Add "Negative" as a prefix
        number = -number;           // Make the number positive for further processing
    }
    else
    {
        words[0] = '\0'; // Initialize with an empty string for positive numbers
    }

    char temp[512] = ""; // Temporary string to build the word representation
    int index = 0;       // Index to track large number scales (e.g., Thousand, Million)

    // Process the number in chunks of 1000 (thousands, millions, etc.)
    while (number > 0)
    {
        int chunk = number % 1000; // Get the last 3 digits (chunk)
        if (chunk > 0)             // If the chunk is non-zero, convert it to words
        {
            char chunkWords[256] = ""; // Temporary string to store the words for the current chunk

            if (chunk / 100 > 0) // If there's a hundreds place
            {
                strcat(chunkWords, belowTwenty[chunk / 100]); // Add the hundreds word
                strcat(chunkWords, " Hundred ");              // Add "Hundred"
            }

            int remainder = chunk % 100; // Get the remainder (last two digits)
            if (remainder < 20)          // If the remainder is less than 20
            {
                strcat(chunkWords, belowTwenty[remainder]); // Add the word for the remainder
            }
            else // If the remainder is 20 or more
            {
                strcat(chunkWords, tens[remainder / 10]); // Add the tens place word
                if (remainder % 10 > 0)                   // If there's a ones place
                {
                    strcat(chunkWords, " ");
                    strcat(chunkWords, belowTwenty[remainder % 10]); // Add the ones place word
                }
            }

            strcat(chunkWords, " ");
            strcat(chunkWords, thousands[index]); // Add the scale (Thousand, Million, etc.)
            strcat(chunkWords, " ");

            // Prepend the chunk words to the result (to form the number in correct order)
            char tempCopy[512];
            strcpy(tempCopy, temp);
            strcpy(temp, chunkWords);
            strcat(temp, tempCopy);
        }

        number /= 1000; // Move to the next chunk (divide by 1000)
        index++;        // Move to the next scale (Thousand -> Million -> Billion)
    }

    // Remove any trailing spaces
    int len = strlen(temp);
    if (temp[len - 1] == ' ')
    {
        temp[len - 1] = '\0'; // Remove the last space
    }

    strcat(words, temp); // Concatenate the final result into the words string
}

// Function to perform basic arithmetic operations (+, -, *)
int applyOperation(int num1, int num2, char op)
{
    switch (op) // Check which operator was used
    {
    case '+':
        return num1 + num2; // Add the two numbers
    case '-':
        return num1 - num2; // Subtract the second number from the first
    case '*':
        return num1 * num2; // Multiply the two numbers
    default:
        return 0; // If the operator is invalid, return 0
    }
}

// Function to process a single line of input
void processInputLine(char *line, FILE *output)
{
    char roman1[50], roman2[50], operator; // Variables to store the Roman numerals and operator
    int num1, num2, result;                // Variables to store the decimal values of the Roman numerals and the result
    char resultWords[512];                 // Array to store the result in words

    // Try to parse the line into two Roman numerals and an operator
    if (sscanf(line, "%s %c %s", roman1, &operator, roman2) == 3)
    {
        // If either Roman numeral is invalid, print an error and stop processing this line
        if (!isValidRoman(roman1) || !isValidRoman(roman2))
        {
            fprintf(output, "Invalid Roman numeral in input.\n"); // Write error to output file
            return;                                               // Skip this line
        }

        // Convert the Roman numerals to decimal numbers
        num1 = romanToDecimal(roman1);
        num2 = romanToDecimal(roman2);

        // Perform the operation on the two numbers
        result = applyOperation(num1, num2, operator);

        // Convert the result to words and write to the output file
        convertToWords(result, resultWords);
        fprintf(output, "%s\n", resultWords);
    }
    else
    {
        fprintf(output, "Error parsing the line: %s\n", line); // Write error if line format is wrong
    }
}
