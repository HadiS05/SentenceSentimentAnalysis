#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stools.h"

int main(int argc, char *argv[]) {
    if (argc > 3) {
        printf("Too many arguments. Match ./mySA <dictionary_name.txt> <sample_lines.txt>\n");
        return -1;
    }else if (argc < 3) {
        printf("Too few arguments. Match ./mySA <dictionary_name.txt> <sample_lines.txt>\n");
        return -1;
    }
    char *dictName = argv[1];
    char *sampleText = argv[2];
    int lineCount = 0;
    int wordCount = 0;
    int len = 0;
    char **lines = readAllLines(sampleText, &lineCount);
    char **wordsInLine = NULL;
    char *singleLine = NULL;
    char delim = ' ';
    float score = 0;
    printf("%-*s%*s\n", 83, "Sample Lines", 20, "Score");
    printf("---------------------------------------------------------------------------------------------------------------------\n");
    if (lines == NULL || lineCount == 0) {
        printf("Cannot read the contents of %s.\n", sampleText);
        return -1;
    }
    for(int i = 0; i < lineCount; i++) {
        if (lines[i] == NULL) {
            continue;
        }
        len = strlen(lines[i]);
        printf("%-*s",83,lines[i]);
        wordCount = 0;
        wordsInLine = split(lines[i], delim, &wordCount);
        if (wordsInLine == NULL) {
            printf("\nCannot identify words in line. File may be empty.\n");
            freeAllLines(wordsInLine, wordCount);
            free(singleLine);
            return -1;
        }
        score = readWords(dictName,wordsInLine,wordCount);
        printf("%*.2f\n", 20, score);
        freeAllLines(wordsInLine, wordCount);
    }
    freeAllLines(lines,lineCount);
    return 0;
}