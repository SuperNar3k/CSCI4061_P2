#include "stream.h"

/**
 * Read lines from input file and write each line to pipes.
 * Each line will contain words as in Project 1 (Use STDOUT for writing).
 */                 
void emit(char * inputFileName) {
    int lineLen;
    char line[chunkSize];
    FILE *fp = getFilePointer(inputFileName);

    while((lineLen = getLineFromFile(fp, line, chunkSize)) != -1){
        printf("%s\n", line);
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
   /* TODO: Read lines from Mapper files and get the file names that will be processes in emit function.
    * Each MapperInput/MapperID.txt file will contain file names.
    * example of one line will be test/T1/subfolder/0.txt.
    */
    char path[maxFileNameLength] = "\0";
    strcat(path, ipFdr);
    strcat(path, argv[1]);
    strcat(path, ".txt");

    emit(path);
    close(STDOUT_FILENO);

    return EXIT_SUCCESS;
}