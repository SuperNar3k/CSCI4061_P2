# UMN CSCI 4061 Project 2
## Basic Map Reduce
## Spring 2021

**Purpose:** 
This program uses two main functions in order to process large datasets: `parse` and `reduce`. This basic MapReduce program takes in several documents and counts the number of words of different lengths, keeps track of the total number of those words with different lengths, and then outputs them into separate text files corresponding to the word length. The program uses pipelining for interprocess communication between stream and map processes. Both stream and map processes use the functions `fork`, `exec`, and `wait` in creating processes and executing `mapper` and `reducer`. Each mapper is responsible for a certain subset of input files, and each reducer calculates the final count of each word as found by the mapper. 

**How to compile:**  
`make clean`  
`./mapreduce #mappers #reducers inputDirectory`

**What this does:**
The program begins by reading the command line arguments, making sure that there are no errors in the inputs. Afterwards, it begins creating children processes that call "execl" with information about the mapperID, file directory, and total number of mappers and execute mapper. The amount of these mappers is dependent on the amount inputted for #mappers in the command line. Each of these children running map recieve their own set of files they must read and get word length counts from. The mappers call map, passing in these file names and finding the lengths of words and adding their count to the corresponding index of the intermediate data structure which is interDS, an array. Then, each mapper calls writeInterDSToFiles, which writes the indices in interDS where interDS[i] != 0 into Intermediate Files according to the word lengths found. For example, if interDS[0] = 3, this means the mapper found 3 words of length 1, so it would write into output/IntermediateData/1 a file called mapper_ID.txt with the contents "1 3".

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