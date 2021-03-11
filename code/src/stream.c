#include "stream.h"

/**
 * Read lines from input file and write each line to pipes.
 * Each line will contain words as in Project 1 (Use STDOUT for writing).
 */
void emit(char * inputFileName) {
    FILE *fp = open(inputFileName, "r");
    char line[chunkSize];
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        puts(line);
    }
}
/***
 *
 * Stream process will read from the files created by Master.
 */
int main(int argc, char *argv[]) {

    mapperID = strtol(argv[1], NULL, 10);
    int nMappers = strtol(argv[2], NULL, 10);
    char* ipFdr = "MapperInput/Mapper";
   /* TODO: Read lines from Mapper files and get the file names that will be processes in emit function.
    * Each MapperInput/MapperID.txt file will contain file names.
    * example of one line will be test/T1/subfolder/0.txt.
    */
    char path[chunkSize];
    char mappersID[chunkSize]
    ltoa(mapperID, mappersID, 10);
    strcat(ipFdr, mappersID);
    int fp = open(ipFdr, "r");
    while(fgets(path, sizeof(path), fp) != NULL)
    {
        emit(ipFdr);
    }
    fds[2];
    pipe(fds);
    dup2(fds[1], 1)
    dup2(0, fds[0])    
    close(fp);
    return EXIT_SUCCESS;
}