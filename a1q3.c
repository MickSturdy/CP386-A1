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
    // Fork first process for `ps xao sid`
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("Fork for ps failed.");
        exit(1);
    }
    if (pid1 == 0) { // Child process for `ps`
        // Redirect stdout to pipePS write end
        dup2(pipePS[1], STDOUT_FILENO);

        // Close unused pipe ends
        close(pipePS[0]); // Close read end of first pipe
        close(pipePS[1]); // Close write end of first pipe
        close(pipeSW[0]); // Close both ends of second pipe
        close(pipeSW[1]);

        // Execute `ps xao sid`
        execlp("ps", "ps", "xao", "sid", NULL);

        // If exec fails
        perror("ps command failed");
        exit(1);
    }

    // Fork second process for `sort -u`
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("Fork for sort failed.");
        exit(1);
    }
    if (pid2 == 0) { // Child process for `sort`
        // Redirect stdin to pipePS read end
        dup2(pipePS[0], STDIN_FILENO);

        // Redirect stdout to pipeSW write end
        dup2(pipeSW[1], STDOUT_FILENO);

        // Close all pipe ends not in use
        close(pipePS[0]); // Close read end of first pipe
        close(pipePS[1]); // Close write end of first pipe
        close(pipeSW[0]); // Close read end of second pipe
        close(pipeSW[1]); // Close write end of second pipe

        // Execute `sort -u`
        execlp("sort", "sort", "-u", NULL);

        // If exec fails
        perror("sort command failed");
        exit(1);
    }

    // Fork third process for `wc -l`
    pid_t pid3 = fork();
    if (pid3 < 0) {
        perror("Fork for wc failed.");
        exit(1);
    }
    if (pid3 == 0) { // Child process for `wc`
        // Redirect stdin to pipeSW read end
        dup2(pipeSW[0], STDIN_FILENO);

        // Close unused pipe ends
        close(pipePS[0]); // Close both ends of first pipe
        close(pipePS[1]);
        close(pipeSW[0]); // Close read end of second pipe
        close(pipeSW[1]); // Close write end of second pipe

        // Execute `wc -l`
        execlp("wc", "wc", "-l", NULL);

        // If exec fails
        perror("wc command failed");
        exit(1);
    }

    // Parent process: Close all pipe ends
    close(pipePS[0]);
    close(pipePS[1]);
    close(pipeSW[0]);
    close(pipeSW[1]);

    // Wait for all child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return 0;
}


