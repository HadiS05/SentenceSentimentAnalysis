#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stools.h"
#include <ctype.h>

#define INIT_BUFFER_SIZE 1024 // Character length of one line
#define INIT_LINES 10 // Initial guess of lines in file

/**
 * \brief                   Removes commas, exclamation, and periods from string
 * \param[in]               s: String to modify
 * \return                  Modified string
*/
char* removePunct(char *s) {
    /* Check to see if string is empty */
    if (s == NULL) {
        return NULL;
    }

    /* Allocate memory for new string (it'll have the same amount of memory but actually
       contain less characters.)
       (+1 for null char)*/
    char *noPunct = malloc(strlen(s) + 1);
    if (noPunct == NULL) {
        free(noPunct);
        return NULL; /* Return NULL if memory allocation fails */
    }

    int j = 0; // This keeps track of the new string's place
    /* Loop through original string until we hit the null char */
    for (int i = 0; s[i] != '\0'; i++) {
        /* As long as the char isn't punctuation we can store it in the new string. */
        if (s[i] != ',' && s[i] != '!' && s[i] != '.') {
            noPunct[j] = s[i];
            j++;
            /* j++ automatically handles the increment for the new string. */
        }
    }
    noPunct[j] = '\0'; // Final null char for new string
    return noPunct;
}

/**
 * \brief                   Function to split a line into an array of strings
 * \param[in]               line: The string to split
 * \param[in]               delim: The delimiter to split by
 * \param[out]              wordCount: The number of elements after splitting
 * \return                  Array of strings
*/
char** split(char *line, char delim, int* wordCount) {
    if (*line == '\0' || line == NULL) {
        *wordCount = 0;
        return NULL;
    }
    char* token = strtok(line, &delim); // Store the first token after splitting 
    if (token == NULL) {
        *wordCount = 0;
        return NULL;
    }
    int tokenCount = 0; // This will keep track of the words we split
    char** tokens = malloc((tokenCount)*sizeof(char*)); // This will be our new array
    while(token) {
        /* Adds new space for a word at the start of each loop */
        char** temp = realloc(tokens, (tokenCount + 1)*(sizeof(char*)));
        if (temp == NULL) {
            freeAllLines(tokens, tokenCount);
            return NULL;
        }
        tokens = temp;
        /* Before actually storing the word, we must get rid of punctuation by calling
           removePunct. This will get rid of ',', '!', and '.'*/
        char *removedPunctToken = removePunct(token);
        if (removedPunctToken == NULL) {
            freeAllLines(tokens, tokenCount);
            return NULL;
        }
        tokens[tokenCount] = removedPunctToken;
        tokenCount++; // New word count
        token = strtok(NULL, &delim); // Move on to next token
    }
    *wordCount = tokenCount; // Set the final word count
    return tokens;
}

char* toLowerCase(char *s) {
    /* First we will check if the string is empty */
    if (s == NULL) {
        return NULL; /* Return null */
    }

    /* We allocate memory for the new string (+1 for the null char)*/
    char *lowerCase = malloc(strlen(s) + 1);
    if (lowerCase == NULL) {
        free(lowerCase);
        return NULL; /* Return NULL if memory allocation fails */
    }

    /* Here we loop through each char */
    for (int i = 0; s[i]; i++) {
        /* We use unsigned char here because tolower expects a positive value from 0 to 255. */
        lowerCase[i] = tolower((unsigned char) s[i]);
    }
    lowerCase[strlen(s)] = '\0'; // Add the final null char

    return lowerCase;
}

/**
 * \brief                   Create a pointer to an array of structs that holds all the words of a string
 * \param[in]               filename: Path to the dictionary
 * \param[in]               words: Array of all the words
 * \param[in]               wordCount: Essentially the length of array words
 * \return                  Struct with all words of string tokenized 
*/
float readWords(const char *filename, char **words, int wordCount) {
    /* Variables used for storing fscanf outputs */
    char dictionaryWord[INIT_BUFFER_SIZE]; 
    float score = 0;
    float SD = 0;
    int SIS_array[10];
    /* Array of struct words used for storing scores, etc. */
    struct words* target = malloc(wordCount*sizeof(struct words));
    if (target == NULL) {
        printf("Cannot allocate memory for words. \n");
        freeAllLines(words,wordCount);
        free(target);
        return 0;
    }
    /* Now we will populate the words array */
    for (int i = 0; i < wordCount; i++) {
        /* Open the file each loop to restart the scanning */
        FILE *file = fopen(filename,"r");
        /* Check if the file exists, otherwise we should display error. */
        if (file == NULL) {
            printf("File not found.\n");
            free(target);
            return 0;
        }
        /* Store the word in the struct */
        target[i].word = words[i];
        char *lowerVersion = toLowerCase(target[i].word);
        if (lowerVersion == NULL) {
            free(target);
            return 0;
        }
        /* fscanf allows for automatic formatting and ensures that each line must be uniform
           to dictionary standards, otherwise it will not read it. */
        while(fscanf(file, " %[^\t]%f\t%f\t[%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]"
                        , dictionaryWord, &score, &SD, &SIS_array[0], &SIS_array[1]
                        , &SIS_array[2], &SIS_array[3], &SIS_array[4], &SIS_array[5]
                        , &SIS_array[6], &SIS_array[7], &SIS_array[8]
                        , &SIS_array[9]) == 13) {
            
            if (strcmp(target[i].word,dictionaryWord) == 0 || strcmp(lowerVersion,dictionaryWord) == 0) {
                /* We take advantage of lazy evaluation and make sure that if the original string is not
                   found, then we can check a possible lowercase version. */
                /* Assign the corresponding values if the words match.*/
                target[i].score = score;
                target[i].SD = SD;
                target[i].SIS_array[0] = SIS_array[0];
                target[i].SIS_array[1] = SIS_array[1];
                target[i].SIS_array[2] = SIS_array[2];
                target[i].SIS_array[3] = SIS_array[3];
                target[i].SIS_array[4] = SIS_array[4];
                target[i].SIS_array[5] = SIS_array[5];
                target[i].SIS_array[6] = SIS_array[6];
                target[i].SIS_array[7] = SIS_array[7];
                target[i].SIS_array[8] = SIS_array[8];
                target[i].SIS_array[9] = SIS_array[9];
                break; /* Break from the scanning so that we can move on to the next word. */
            } else {
                /* If the word is not found, then assign 0 to all values. */
                target[i].score = 0;
                target[i].SD = 0;
                target[i].SIS_array[0],
                target[i].SIS_array[1],
                target[i].SIS_array[2],
                target[i].SIS_array[3],
                target[i].SIS_array[4],
                target[i].SIS_array[5],
                target[i].SIS_array[6],
                target[i].SIS_array[7],
                target[i].SIS_array[8],
                target[i].SIS_array[9] = 0;
                /* This will be overwritten if the word is found in later lines, otherwise it will
                   remain the same. */
            }
        }
        fclose(file); // Close file so that it can be reopened 
        free(lowerVersion);
    }
    return getAverageScore(target,wordCount); // Returns a float value after calculating average score. 
}

/**
 * \brief                   Calculates average score of all words in line
 * \param[in]               target: Array of the struct "words"
 * \param[in]               wordCount: Integer amount of words in target line
 * \return                  Average of all words if wordCount is greater than 0, otherwise 0       
*/
float getAverageScore(struct words* target, int wordCount) {
    float sum = 0;
    for (int i = 0; i < wordCount; i++) {
        sum += target[i].score; // Simple averaging
    }
    free(target);
    /* Ternary operation to quickly check if wordCount is 0, which would give us an arithmetic issue. */
    return (wordCount != 0) ? (sum/wordCount) : 0;
}

/**
 * \brief                   Reads all lines of a file (or up to lineCount)
 * \param[in]               filename: The name of the file from which every sample line will be read
 * \param[in]               lineCount: Used to store the number of lines in the file using pointers.
 * \return                  Array of strings (lines from the specified file)
*/
char** readAllLines(const char* filename, int *lineCount) {
    FILE* file = fopen(filename, "r");
    char** lines = NULL; // Final result will be stored in here
    char buffer[INIT_BUFFER_SIZE]; // Temporarily stores file line
    int count = 0; // Count the number of lines we have

    if (file == NULL) {
        printf("Cannot open the file.");
        return NULL;
    }

    /* We are using fgets as it reads until the newline character
       It also has built-in security to ensure that the buffer does
       not overflow. */
    while (fgets(buffer, sizeof(buffer), file)) {
        /* Changes the newline char to a null char. */
        buffer[strlen(buffer)-1] = '\0';
        /* malloc for the line. +1 is added for the null character. */
        char* line = (char*)malloc(strlen(buffer) + 1);
        if (line == NULL) {
            printf("Could not allocate memory for line." 
            " Please make your lines shorter than 1024 characters.\n");
            freeAllLines(lines, count);
            fclose(file);
            return NULL;
        }
        strcpy(line, buffer); // Copies the buffer into the line string

        /* Ensure that array of lines is larger than total lines in file using count + 1*/
        lines = realloc(lines, (count + 1) * sizeof(char*));
        if (lines == NULL) {
            printf("Cannot allocate memory for new line, please ensure your file is not too large.\n");
            free(line); // Free the previous allocation since we couldn't make space for it
            break; // Break from the loop and just return whatever lines we got to
        }
        lines[count] = line; /* Actually adds the line to the final array. */
        count++; // Indicates the move to the next line
    }
    fclose(file);
    *lineCount = count; // Stores the count
    return lines; // Returns the lines
}

/**
 * \brief                   Simple pretty printing
 * \param[in]               line: The string to print
 * \param[in]               score: line's corresponding float
*/
void print(char *line, float score) {
    printf("%s          %lf\n", line, score);
}

/**
 * \brief                   Frees lines of an array of strings
 * \param[in]               lines: Array of strings
 * \param[in]               lineCount: Total number of lines in the array
*/
void freeAllLines(char** lines, int lineCount) {
    for (int i = 0; i < lineCount; i++) {
        free(lines[i]);
    }
    free(lines);
}