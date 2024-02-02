#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "recordHandler.h"
#include "../defines.h"

FILE *dataptr;
bool lineEndReached;

record *loadRecords()
{
    dataptr = fopen(GAME_SAVE, "r");
    if (dataptr == NULL)
        return NULL;

    int n = 0;
    while (!feof(dataptr))
        if (fgetc(dataptr) == '\n')
            n++;
    n /= 2;

    record *ret = malloc(sizeof(record) * (n + 1));
    if (ret == NULL) {
        fclose(dataptr);
        return NULL;
    }

    fseek(dataptr, 0, SEEK_SET);

    for (int i = 0; i < n; i++) {
        while (fgetc(dataptr) != '\n');
        int tmp = fscanf(dataptr, "%d %ld %d ", &(ret[i].lvl), &(ret[i].seed), &(ret[i].score));
        strftime(ret[i].time, sizeof(ret[i].time), "%x %X %p", localtime(&(ret[i].seed)));
        if (tmp == EOF) break;
    }

    ret[n].seed = 0;
    ret[n].score = -1;
    strcpy(ret[n].time, "1900, Y2K babyyyy"); // little "easter egg" of mine :)

    fclose(dataptr);

    return ret;
}

void freeRecords(record *rc)
{
    free(rc);
}

void openInputRecord(int index) {
    dataptr = fopen(GAME_SAVE, "r");
    for (int i = 0; i < index; i++) {
        while (fgetc(dataptr) != '\n');
        while (fgetc(dataptr) != '\n');
    }
    lineEndReached = false;
}

void closeIORecord() {
    fclose(dataptr);
}

void openOutputRecord() {
    dataptr = fopen(GAME_SAVE, "a");
}

void writeMove(int gameTik, int move) {
    fprintf(dataptr, "%d:%d ", gameTik, move);
}

void writeStats(int lvl, time_t seed, int score) {
    fprintf(dataptr, "\n%d %ld %d\n", lvl, seed, score);
}

keyInputRecord readMove() {
    if (lineEndReached) {
        return (keyInputRecord){.gameTik=-1, .move=-1};
    }

    keyInputRecord ret;
    if (fscanf(dataptr, "%d:%d", &ret.gameTik, &ret.move) == EOF) lineEndReached = true;

    if (fgetc(dataptr) == '\n') lineEndReached = true;

    return ret;
}