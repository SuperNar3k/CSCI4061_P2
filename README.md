# UMN CSCI 4061 Project 2
## Basic Map Reduce
## Spring 2021

**Purpose:** 
This program uses two main functions in order to process large datasets: `parse` and `reduce`. This basic MapReduce program takes in several documents and counts the number of words of different lengths, keeps track of the total number of those words with different lengths, and then outputs them into separate text files corresponding to the word length. The program uses pipelining for interprocess communication between stream and map processes. Both stream and map processes use the functions `fork`, `exec`, and `wait` in creating processes and executing `mapper` and `reducer`. Each mapper is responsible for a certain subset of input files, and each reducer calculates the final count of each word as found by the mapper. 

**How to compile:**  
`make clean`  
`./mapreduce #mappers #reducers inputDirectory`

**What this does:**
The program begins by reading the command line arguments, making sure that there are no errors in the inputs. Afterwards, we traverse the input directory and divide the input files equally among mappers in a directory called MapperInput, where there are files called Mapper_ID.txt for each mapper. Then we make pipes to connect the streamers and mappers. We fork to create a child process for the streamer, where we close the read-end of the pipe and redirect the standard output to the write-end of the pipe. Next we call "execl" on the streamer which reads from the filepaths stored in Mapper_ID.txt, and write lines from the file to stdout. We also fork to create children processes for the mapper, where we close the write-end of the pipe and redirect STDIN to the read-end of the pipe. Next, we call "execl" on mapper, which then reads from stdin and counts the length of words and adds them to the corresponding index of interDS.

The original parent process waits for these mappers to finish, then begins creating children processes that call "execl" with information about the reducerID, and number of reducers and execute reduce. Here, each reducer recieves their own set of files in the Intermediate files that they must aggregate into the total count of particular lengths. The reducers call reduce, passing in the files they are tasked to reduce, reading how many words of a certain length there are, adding them to their corresponding index in finalDS, an array holding all the counts the reducer has seen so far. After this, each reducer calls writeFinalDSToFiles, which writes from the indices in finalDS where finalDS[i] != 0 into output/FinalData with a file named i.txt with the contents "i count". The parent waits for these reducers to finish and then returns EXIT_SUCCESS as it finishes itself.

**Assumptions:**
* Input file size has no limit.
* Make sure there are input files for the program to read.
* The input file sizes can vary, there is no limit.
* Number of mappers will be greater than or equal to number of reducers
* Maximum number of mappers or reducers will be limited to 20
* Maximum size of a file path is 200 bytes
* Mmaximum word length is 20
* Mximum number of input files is 100
* Maximum number of intermediate files is 400
* The chunk size to read lines will be atmost 1024 bytes

**Team names and x500s:** 
* Yna Roxas (roxas007)
* Narek Ohanyan (ohany004)
* Christopher Liu (liux3770)

**Contribution:**
* Yna Roxas: stream.c, reducer.c, part of README.md
* Narek Ohanyan: documentation, mapreducer.c, part of README.md
* Christopher Liu: mapper.c, getReducerTasks()
  
We tried our best to divide and conquer this project equally, giving each other more work if needed to make our contributions equal and fair to each other.