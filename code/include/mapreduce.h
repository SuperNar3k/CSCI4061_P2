#ifndef MAPREDUCE_H
#define MAPREDUCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "utils.h"

char *inputFileDir;
char filePaths[MaxNumInputFiles][maxFileNameLength];
int emptyIdx = 0;

#endif