//Assignment 1
//By: Michael Tahirovic, Ben Hamilton, Zach Pereira
//Emails: tahi9525@mylaurier.ca, hami5217@mylaurier.ca, pere
//Date: 2024-09-26
//------------------------------------------------------------------------------------------
//Question 1:
//Write a C/C++ program in which a parent forks a child, and the child forks a grandchild.
//The grandchild writes: “My process id is X, my parent’s id is Y , and my Grandparent’s id is Z” (where
//X, Y , and Z are the process ids). The program should produce no other output.
//------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
pid_t pid1, pid2;
// Parent forks a child
pid1 = fork();
if (pid1 < 0) {
    // Fork failed
    (stderr, "Fork failed\n");
    exit(1);
} else if (pid1 == 0) {
    // Inside child process
    pid_t grandparent_id = getppid(); // Get the grandparent (parent's) ID
    // Child forks the grandchild
    pid2 = fork();
    if (pid2 < 0) {
    // Fork failed
    fprintf(stderr, "Fork failed\n");
    exit(1);
    } else if (pid2 == 0) {
        // Inside grandchild process
        printf("My process id is %d, my parent’s id is %d, and my grandparent’s id is %d\n",
        getpid(), getppid(), grandparent_id);
        exit(0); // Grandchild exits after printing
        } else {
            // Inside child process
            wait(NULL); // Wait for grandchild to finish
            exit(0); // Child exits after forking grandchild
            }
    } else {
    // Inside parent process
    wait(NULL); // Wait for child to finish
    exit(0); // Parent exits
    }
    return 0;
}
