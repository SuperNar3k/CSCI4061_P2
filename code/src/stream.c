#include "stream.h"

/**
 * Read lines from input file and write each line to pipes.
 * Each line will contain words as in Project 1 (Use STDOUT for writing).
 */                 
void emit(char * inputFileName) {
    int lineLen;
    char line[chunkSize];
    FILE *fp = getFilePointer(inputFileName);
    while((lineLen = getLineFromFile(fp, line, chunkSize)) != -1){ // Reads while there are still lines in the file
        fprintf(stderr, "%s", line);
        puts(line);
    }
    fclose(fp);
}

/***
 *
 * Stream process will read from the files created by Master.
 */
int main(int argc, char *argv[]) {
    mapperID = strtol(argv[1], NULL, 10);
    int nMappers = strtol(argv[2], NULL, 10);
    char* ipFdr = "MapperInput/Mapper";

    char path[maxFileNameLength];
    char line[chunkSize];

    sprintf(path, "%s_%d.txt", ipFdr, mapperID); 
    FILE *fp = getFilePointer(path);

    while(fscanf(fp, "%s\n", line) != EOF){
        emit(line);
    }
    fclose(fp);
    
    //close(STDIN_FILENO);
    return EXIT_SUCCESS;
}