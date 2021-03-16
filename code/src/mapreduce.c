#include "mapreduce.h"

void traverseInput(char* path){
	DIR* dr = opendir(path);

	if(dr == NULL){
		exit(EXIT_FAILURE);
	}

	struct dirent* de;
	while(((de = readdir(dr)) != NULL)){ // Traverses director while there are still files
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0){
			continue;
		}

		char filename[maxFileNameLength] = "\0";
		strcat(filename, path);
		strcat(filename, "/");
		strcat(filename, de->d_name);

		if(de->d_type == DT_DIR){ // Calls traverseInput if the file is a directory
			traverseInput(filename);
		}
		else{ // Saves the file
			strcpy(filePaths[emptyIdx], filename);
			emptyIdx++;
		}
	}
}

void createInput(int nMappers){
	int fileCount = emptyIdx;
	int filesPerMapper = fileCount / nMappers;
	for (int i = 0; i < nMappers; i++){ //Iterates for each mapper, writing filesPerMapper filepaths to Mapperi.txt
		char mapperPath[maxFileNameLength];

		// Creates mapperPath to be of the form "MapperInput/Mapperi.txt"
		sprintf(mapperPath, "MapperInput/Mapper%d.txt", i); 
		FILE* fp = fopen(mapperPath, "w");

		if (fp == NULL){
			exit(EXIT_FAILURE);
		}

		for (int j = 0; j < filesPerMapper; j++){ 
			int idx = i * filesPerMapper + j;
			fprintf(fp, "%s\n", filePaths[idx]);
		}
	}
}

int main(int argc, char *argv[]) {
	
	if (argc < 3){ // checks number of arguments
		printf("usage: %s <numMappers> <numReducers> <inputFileDirectory>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	int nMappers 	= strtol(argv[1], NULL, 10);
	int nReducers 	= strtol(argv[2], NULL, 10);

    inputFileDir = argv[3];
    if(!isValidDir(inputFileDir))
        exit(EXIT_FAILURE);

	bookeepingCode();

	mkdir("MapperInput", 0777);
	traverseInput(inputFileDir); // Traverses Input File Directory and records all names of files in filePaths
	createInput(nMappers); // Writes the Mapperi.txt files using the filepaths retrieved from traverseInput()
	

	int* pipes = malloc(nMappers * 2 * sizeof(int)); //Creates nMapper pipes

	for (int i = 0; i < nMappers; i++){
		pid_t child = fork();

		pipe(pipes + 2 * i);
		if (child == 0){
			char mapperID = i + '1';
			close(pipes[2 * i]); // Close read end of pipe for the streamer
			//dup2(pipes[2 * i + 1], STDOUT_FILENO); // Redirect output to the write end of the pipe
			execl("stream", "stream", &mapperID, argv[1], NULL);
		}
		else if (child < 0) {// error creating child
      fprintf(stderr, "ERROR: failed to fork while spawning streamers\n");
			exit(EXIT_FAILURE);
		}

		close(pipes[2 * i]); // Close write end of pipe for the parent
	}

	
	for (int i = 0; i < nMappers; i++){
		pid_t child = fork(); 

		if (child == 0){
			char mapperID = i + '1';
			dup2(pipes[2 * i + 1], STDIN_FILENO); // Redirect input to the read end of the pipe			
			execl("mapper", "mapper", &mapperID, argv[1], NULL);
		}
		else if (child < 0) {// error creating child
      fprintf(stderr, "ERROR: failed to fork while spawning mappers\n");
			exit(EXIT_FAILURE);
		}
	}

	// TODO: wait for all children to complete execution

	// TODO: spawn reducers

	// TODO: wait for all children to complete execution


	free(pipes);
	return EXIT_SUCCESS;
}