#include "linkedList.h"
#include "recordHandler.h"
#include "../defines.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

list *loadRecords()
{
    list *ret = NULL;
    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] opening record file %s\n", GAME_SAVE);
    FILE *fp = fopen(GAME_SAVE, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "[ERR] couldn't open record file\n");
    }

    record tmp;
    int fpRet;
    do
    {
        if (feof(fp))
            break;
        char *tmp2;
        tmp2 = fgets(tmp.name, 32, fp);
        tmp.name[strlen(tmp.name) - 1] = '\0';
        tmp2 = fgets(tmp.time, 32, fp);
        tmp.time[strlen(tmp.time) - 1] = '\0';
        fpRet = fscanf(fp, " %d %d %d ", &tmp.score, &tmp.dur, &tmp.lvl);
        addElement(&ret, sizeof(record), 0);
        *((record *)ret->val) = tmp;

    } while (fpRet != EOF);

    fclose(fp);

    return ret;
}

void writeRecord(char *name, time_t time, int score, int dur, int lvl)
{
    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] opening record file %s\n", GAME_SAVE);
    FILE *fp = fopen(GAME_SAVE, "a");
    if (fp == NULL)
    {
        fprintf(stderr, "[ERR] couldn't open record file\n");
    }

    char time_buf[32];
    strftime(time_buf, 32, "%c", localtime(&time));

    fprintf(fp, "%s\n%s\n%d\n%d\n%d\n\n", name, time_buf, score, dur, lvl);

    fclose(fp);
}