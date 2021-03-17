# UMN CSCI 4061 Project 2
## Basic Map Reduce
## Spring 2021

**Purpose:** 
This program uses two main functions in order to process large datasets: `parse` and `reduce`. This basic MapReduce program takes in several documents and counts the number of words of different lengths, keeps track of the total number of those words with different lengths, and then outputs them into separate text files corresponding to the word length. The program uses pipelining for interprocess communication between stream and map processes. Both stream and map processes use the functions `fork`, `exec`, and `wait` in creating processes and executing `mapper` and `reducer`. Each mapper is responsible for a certain subset of input files, and each reducer calculates the final count of each word as found by the mapper. 

**How to compile:**  
`make clean`  
`./mapreduce #mappers #reducers inputDirectory`

**What this does:**


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