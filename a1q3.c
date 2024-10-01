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
    int pipeSW[2]; // Pipe for sort to wc

    // Init first pipe
    if (pipe(pipePS) == -1) {
        perror("ps to sort pipe failed to initialize.");
        exit(1);
    }

    // Init second pipe
    if (pipe(pipeSW) == -1) {
        perror("sort to wc pipe failed to initialize.");
        exit(1);
    }

    // Fork process for 'ps'
    if (fork() == 0) {
        // Redirect stdout to pipePS
        dup2(pipePS[1], STDOUT_FILENO);
        close(pipePS[0]); // Close unused read end
        close(pipePS[1]); // Close write end after duplicating

        execlp("ps", "ps", "xao", "sid", NULL);
        perror("execlp ps failed.");
        exit(1);
    }

    // Fork second process for 'sort'
    if (fork() == 0) {
        // Redirect stdin to pipePS and stdout to pipeSW
        dup2(pipePS[0], STDIN_FILENO);
        dup2(pipeSW[1], STDOUT_FILENO);
        close(pipePS[0]); // Close read end after duplicating
        close(pipePS[1]); // Close unused write end
        close(pipeSW[0]); // Close unused read end
        close(pipeSW[1]); // Close write end after duplicating

        execlp("sort", "sort", "-u", NULL);
        perror("execlp sort failed.");
        exit(1);
    }

    // Fork third process for 'wc'
    if (fork() == 0) {
        // Redirect stdin to pipeSW
        dup2(pipeSW[0], STDIN_FILENO);
        close(pipeSW[0]); // Close read end after duplicating
        close(pipeSW[1]); // Close unused write end
        close(pipePS[0]); // Close unused PS ends
        close(pipePS[1]);

        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc failed.");
        exit(1);
    }

    // Close all pipe ends in the parent process
    close(pipePS[0]);
    close(pipePS[1]);
    close(pipeSW[0]);
    close(pipeSW[1]);

    // Wait for child processes to finish
    for (int i = 0; i < 3; i++)
        wait(NULL);

    return 0;
}

