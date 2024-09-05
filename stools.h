#ifndef STOOLS_H
#define STOOLS_H

struct words{
    char *word;
    float score;
    float SD;
    int SIS_array[10];
};

char* removePunct(char *s);
char** split(char *line, char delim, int* wordCount);
char* toLowerCase(char *s);
float readWords(const char *filename, char **words, int wordCount);
float getAverageScore(struct words* target, int wordCount);
char** readAllLines(const char* filename, int *lineCount);
void print(char *line, float score);
void freeAllLines(char** lines, int lineCount);

#endif