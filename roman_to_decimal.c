#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Function prototypes
int romanToDecimal(const char *roman);
void convertToWords(int number, char *words);
int applyOperation(int num1, int num2, char op);
int isValidRoman(const char *roman);
void processInputLine(char *line, FILE *output);

// Main function
int main()
{
    FILE *input, *output;
    char line[256];

    // Open files for input and output
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    if (!input || !output)
    {
        printf("Error: Couldn't open input/output files.\n");
        return 1;
    }

    // Process each line from the input file
    while (fgets(line, sizeof(line), input))
    {
        processInputLine(line, output);
    }

    fclose(input);
    fclose(output);

    printf("Processing complete. Results are in output.txt.\n");
    return 0;
}

// Convert Roman numeral to decimal
int romanToDecimal(const char *roman)
{
    int values[256] = {0}; // Initialize all to zero
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0;
    int length = strlen(roman);

    for (int i = 0; i < length; i++)
    {
        int current = values[roman[i]]; // Current Roman character value
        int next = (i + 1 < length) ? values[roman[i + 1]] : 0;

        if (current < next)
        {
            total -= current; // Subtract if the current value is smaller than the next
        }
        else
        {
            total += current; // Otherwise add the current value
        }
    }

    return total;
}

// Check if the Roman numeral is valid
int isValidRoman(const char *roman)
{
    while (*roman)
    {
        if (!strchr("IVXLCDM", *roman))
        {
            return 0; // Invalid character found
        }
        roman++;
    }
    return 1; // All characters are valid
}

// Convert a number to words (for numbers larger than 3999, handles larger results)
void convertToWords(int number, char *words)
{
    const char *belowTwenty[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten",
                                 "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen",
                                 "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    if (number == 0)
    {
        strcpy(words, "Zero");
        return;
    }

    if (number < 0)
    {
        strcpy(words, "Negative ");
        number = -number;
    }
    else
    {
        words[0] = '\0';
    }

    char temp[512] = "";
    int index = 0;

    while (number > 0)
    {
        int chunk = number % 1000;
        if (chunk > 0)
        {
            char chunkWords[256] = "";

            if (chunk / 100 > 0)
            {
                strcat(chunkWords, belowTwenty[chunk / 100]);
                strcat(chunkWords, " Hundred ");
            }

            int remainder = chunk % 100;
            if (remainder < 20)
            {
                strcat(chunkWords, belowTwenty[remainder]);
            }
            else
            {
                strcat(chunkWords, tens[remainder / 10]);
                if (remainder % 10 > 0)
                {
                    strcat(chunkWords, " ");
                    strcat(chunkWords, belowTwenty[remainder % 10]);
                }
            }

            strcat(chunkWords, " ");
            strcat(chunkWords, thousands[index]);
            strcat(chunkWords, " ");

            // Prepend to form the result in reverse order
            char tempCopy[512];
            strcpy(tempCopy, temp);
            strcpy(temp, chunkWords);
            strcat(temp, tempCopy);
        }

        number /= 1000;
        index++;
    }

    // Remove any trailing spaces
    int len = strlen(temp);
    if (temp[len - 1] == ' ')
    {
        temp[len - 1] = '\0';
    }

    strcat(words, temp);
}

// Perform the operation (+, -, *)
int applyOperation(int num1, int num2, char op)
{
    switch (op)
    {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    default:
        return 0; // If the operator is invalid, return 0
    }
}

// Process a single line of input
void processInputLine(char *line, FILE *output)
{
    char roman1[50], roman2[50], operator;
    int num1, num2, result;
    char resultWords[512];

    if (sscanf(line, "%s %c %s", roman1, &operator, roman2) == 3)
    {
        if (!isValidRoman(roman1) || !isValidRoman(roman2))
        {
            fprintf(output, "Invalid Roman numeral in input.\n");
            return;
        }

        num1 = romanToDecimal(roman1);
        num2 = romanToDecimal(roman2);
        result = applyOperation(num1, num2, operator);

        // Convert the result to words if it's large or invalid range
        if (result > 3999)
        {
            convertToWords(result, resultWords);
            fprintf(output, "%s\n", resultWords);
        }
        else
        {
            convertToWords(result, resultWords);
            fprintf(output, "%s\n", resultWords);
        }
    }
    else
    {
        fprintf(output, "Invalid input format.\n");
    }
}
