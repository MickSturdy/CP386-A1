//Assignment 1
//By: Michael Tahirovic, Ben Hamilton, Zach Pereira
//Emails: tahi9525@mylaurier.ca, hami5217@mylaurier.ca, pere
//Date: 2024-09-26
//------------------------------------------------------------------------------------------
//Question 2a:
//Suppose we have n programs p1,...,pn to be executed. The programs are described in an input text file. 
//(a) Suppose the programs are independent from each other, so that they can be executed in
//parallel. Write a parallel program executor, receiving the absolute path of the
//abovementioned input file as a command-line argument. This program runs all the programs
//listed in the file in parallel.
//------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(1);
    }
    // Open input file
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    // Read number of programs
    int n;
    if (fscanf(input_file, "%d", &n) != 1) {
        fprintf(stderr, "Error reading the number of programs\n");
        fclose(input_file);
        exit(1);
    }
    // Dynamically allocate memory for program paths
    char **paths = malloc(n * sizeof(char *));
    if (paths == NULL) {
        perror("Memory allocation failed");
        fclose(input_file);
        exit(1);
    }
    // Read program paths
    for (int i = 0; i < n; i++) {
        paths[i] = malloc(256 * sizeof(char)); // Allocating memory for each path
        if (paths[i] == NULL || fscanf(input_file, "%255s", paths[i]) != 1) {
            fprintf(stderr, "Error reading program path\n");
            fclose(input_file);
            for (int j = 0; j <= i; j++) free(paths[j]); // Free previously allocated memory
            free(paths);
            exit(1);
        }
    }
    fclose(input_file);
    // Fork child processes for each program to run in parallel
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process: Execute the program
            char *args[] = {paths[i], NULL};
            execvp(args[0], args);
            // If execvp fails
            perror("Error executing program");
            exit(1);
        }
    }
    // Parent process: Wait for all children to finish
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    // Free allocated memory
    for (int i = 0; i < n; i++) {
        (paths[i]);
    }
    free(paths);
    printf("All programs executed in parallel.\n");
    return 0;
}