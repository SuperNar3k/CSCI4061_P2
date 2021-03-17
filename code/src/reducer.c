#include "reducer.h"

/**
 * Write final word count to files.
 * The file should be in a corresponding folder in output/FinalData/ 
 */
void writeFinalDSToFiles(void) {
    for (int i = 0; i < MaxWordLength; i++){
        char filePath[maxFileNameLength];
        char line[chunkSize];
        
        if (finalDS[i] == 0){ //The reducer did not count words of length "i + 1" so it does not write a Final file for this length
            continue;
        }

        // creates filePath to be of the form "output/FinalData/[wordlength].txt"
        sprintf(filePath, "%s/%d.txt", finalDir, i + 1);
        
        // creates line to be of the form "wordLength finalcount"
        sprintf(line, "%d %d", i + 1, finalDS[i]);
        writeLineToFile(filePath, line);
    }
}


/**
 * Read lines from files, and calculate a total count for a specific word length
 */
void reduce(char * intermediateFileName) {
}

int main(int argc, char *argv[]) {

	// initialize 
	reducerID = strtol(argv[1], NULL, 10);
	int nReducers = strtol(argv[2], NULL, 10);

    //getReducerTasks function returns a list of intermediate file names that this reducer should process
    char *myTasks[MaxNumIntermediateFiles] = {NULL};

    //TODO: you can write your own getReducerTasks in utils file or change this however you like.
    int nTasks = getReducerTasks(nReducers, reducerID, intermediateDir, &myTasks[0]);

    int tIdx;
    for (tIdx = 0; tIdx < nTasks; tIdx++) {
        printf("reducer[%d] - %s\n", reducerID, myTasks[tIdx]);
        reduce(myTasks[tIdx]);
        free(myTasks[tIdx]);
    }

    writeFinalDSToFiles();

	return EXIT_SUCCESS;
}