#ifndef RECORDHANDLER
#define RECORDHANDLER

#include <time.h>
#include "linkedList.h"

typedef struct {
    char name[32];
    char time[32];
    int score;
    int dur;
    int lvl;
} record;

list *loadRecords();
void writeRecord(char *name, time_t time, int score, int dur, int lvl);

#endif