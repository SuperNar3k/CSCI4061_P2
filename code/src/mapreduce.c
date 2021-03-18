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
		else{ // Saves the filePath
			strcpy(filePaths[emptyIdx], filename);
			emptyIdx++;
		}
	}
	closedir(dr);
}

void createInput(int nMappers){
	int fileCount = emptyIdx;
	int filesPerMapper = fileCount / nMappers;
	if (fileCount / nMappers > 0){
		filesPerMapper += 1;
	}

	for (int i = 0; i < nMappers; i++){ //Iterates for each mapper, writing filesPerMapper filepaths to Mapper_i.txt
		char mapperPath[maxFileNameLength];

		// Creates mapperPath to be of the form "MapperInput/Mapper_i.txt"
		sprintf(mapperPath, "MapperInput/Mapper_%d.txt", i + 1); 
		FILE* fp = fopen(mapperPath, "a");

		if (fp == NULL){
			exit(EXIT_FAILURE);
		}

		for (int j = i; j < fileCount; j = j + nMappers){ 
			fprintf(fp, "%s\n", filePaths[j]);
		}
		fclose(fp);
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

	mkdir("MapperInput", ACCESSPERMS);
	traverseInput(inputFileDir); // Traverses Input File Directory and records all names of files in filePaths
	createInput(nMappers); // Writes the Mapperi.txt files using the filepaths retrieved from traverseInput()
	
	int* pipes = malloc(nMappers * 2 * sizeof(int)); //Creates nMapper pipes

	for (int i = 0; i < nMappers; i++){
		int pipe_result = pipe(pipes + 2 * i);
		pid_t child = fork();
		if (pipe_result < 0) {
      	fprintf(stderr, "ERROR: Failed to open pipe\n");
     	exit(1);
    }

	if (child == 0){
		char mapperID = i + '1';
		close(pipes[2 * i]); // Close read end of pipe for the streamer
		dup2(pipes[2 * i + 1], STDOUT_FILENO); // Redirect output to the write end of the pipe
		execl("stream", "stream", &mapperID, argv[1], NULL);
		fprintf(stderr, "ERROR: failed to execute execl for streamers"); // only reached when execl fails
		exit(EXIT_FAILURE);
	}
	else if (child < 0) { // error creating child
	fprintf(stderr, "ERROR: failed to fork while spawning streamers\n");
		exit(EXIT_FAILURE);
	}
	close(pipes[2 * i + 1]); // Close write end of pipe for the parent
	}

	// Spawn nMappers mappers
	for (int i = 0; i < nMappers; i++){
		pid_t child = fork(); 

		if (child == 0){
			char mapperID = i + '1';
			dup2(pipes[2 * i], STDIN_FILENO); // Redirect input to the read end of the pipe			
			execl("mapper", "mapper", &mapperID, argv[1], NULL);
			fprintf(stderr, "ERROR: failed to execute execl for mappers"); // only reached when execl fails
			exit(EXIT_FAILURE);
		}
		else if (child < 0) { // error creating child
      		fprintf(stderr, "ERROR: failed to fork while spawning mappers\n");
			exit(EXIT_FAILURE);
		}
		close(pipes[2 * i]); // Close read end of pipe for the parent
	}

	// Wait for all children to complete execution
	for (int i = 0; i < nMappers*2; i++){
		pid_t terminated_pid = wait(NULL);
		if (terminated_pid == -1){ // error waiting for child
			fprintf(stderr, "ERROR: failed to wait for mapper\n");
			exit(EXIT_FAILURE);
		}
	}

	// Spawn nReducers reducers
	for (int i = 0; i < nReducers; i++){
		pid_t child = fork();

		if (child == 0){
			char reducerID = i + '1';
			execl("reducer", "reducer", &reducerID, argv[2], NULL);
			fprintf(stderr, "ERROR: failed to execute execl for reducers"); // only reached when execl fails
			exit(EXIT_FAILURE);
		}
		else if (child < 0) {// error creating child
      		fprintf(stderr, "ERROR: failed to fork while spawning reducers\n");
			exit(EXIT_FAILURE);
    	}
	}
	// Wait for all children to complete execution
	for (int i = 0; i < nReducers; i++){
		pid_t terminated_pid = wait(NULL);
		if (terminated_pid == -1){ // error waiting for child
			fprintf(stderr, "ERROR: failed to wait for reducer\n");
			exit(EXIT_FAILURE);
		}
	}
	

	free(pipes);
	return EXIT_SUCCESS;
}