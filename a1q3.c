//Assignment 1
//By: Michael Tahirovic, Ben Hamilton, Zach Pereira
//Emails: tahi9525@mylaurier.ca, hami5217@mylaurier.ca, pere
//Date: 2024-09-26
//------------------------------------------------------------------------------------------
//Question 3:
//Write a C/C++ program that replicates the functionality of the Unix pipeline command
//$ ps xao sid | sort -u | wc -l
//This command counts the number of unique session IDs currently active on the system. Your
//implementation should focus on process creation and file descriptor management. Create three
//separate processes corresponding to each part of the pipeline (ps, sort, and wc). Each process
//should execute the respective command as a new program. Your primary task is to ensure proper
//control and redirection of file descriptors to mimic the behavior of the pipe (|) operator in a Unix
//shell. Remember, the goal is not to reimplement the commands themselves but to orchestrate their
//execution and intercommunication in a C/C++ program.
//------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char *argv[]) {
    int pipePS[2]; // Pipe for ps to sort
    int pipeSW[2]; // Pipe for sort to ws

    // Init first pipe
    if (pipe(pipePS) == -1){
        perror("ps to sort pipe failed to initialize.");
        exit(1);
    }

    // Init second pipe
    if (pipe(pipeSW) == -1){
        perror("sort to ws pipe failed to initialize.");
        exit(1);
    }

    //Fork process


    return 0;
}

