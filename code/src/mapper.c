#include "mapper.h"

/**
 * Write word count that is stored in an intermediate data structure to files.
 * The file should be in a corresponding folder in output/IntermediateData/ 
 */
void writeInterDSToFiles(void) {
    for (int i = 0; i < MaxWordLength; i++){
        char filePath[maxFileNameLength];
        char line[chunkSize];

        if (interDS[i] == 0){ //The mapper did not count words of length "i + 1" so it does not write a intermediate file for this length
            continue;
        }
        
        // creates filePath to be of the form "output/IntermediateData/wordLength/m_mapperID.txt"
        sprintf(filePath, "%s/%d/m_%d.txt", intermediateDir, i + 1, mapperID); 
        
        // creates line to be of the form "wordLength count"
        sprintf(line, "%d %d", i + 1, interDS[i]);
        writeLineToFile(filePath, line);
    }
}

/**
 * parse lines from pipes, and count words by word length
 */
void parse(char * line) {
    int wordLen = 0;
    int lineLen = strlen(line);
    for (int i = 0; i < lineLen; i++){            
        // records the length of a word once whitespace or a newline is found
        if (line[i] == ' ' || line[i] == '\n'){
            //printf("wordLen: %d\n", wordLen);
            interDS[wordLen - 1]++;
            wordLen = 0;
        } else{
            wordLen++;
        }
    }
    interDS[wordLen - 1]++; //add length of the last word on the last line
}

int main(int argc, char *argv[]) {
    int lineLen;
    char line[chunkSize];
    mapperID = strtol(argv[1], NULL, 10);

    // you can read lines from pipes (from STDIN) (read lines in a while loop)
    // feel free to change
    while((lineLen = getLineFromFile(STDIN_FILENO, line, chunkSize)) != -1) {
        printf("%s \n", line);
        parse(line);
    }

    writeInterDSToFiles();

    return EXIT_SUCCESS;
}