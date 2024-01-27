#ifndef RECORDHANDLER
#define RECORDHANDLER

#include <time.h>

typedef struct {
    char time[32];
    int score;
    time_t seed;
    int lvl;
} record;

typedef struct {
    int gameTik, move;
} keyInputRecord;

record *loadRecords();
void freeRecords(record *rc);
void openInputRecord(int index);
void closeIORecord();
void openOutputRecord();
void writeMove(int gameTik, int move);
void writeStats(int lvl, time_t seed, int score);
keyInputRecord readMove();

#endif