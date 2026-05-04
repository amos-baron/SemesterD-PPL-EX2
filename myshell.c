#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "LineParser.h"

void execute(cmdLine *pCmdLine, int debug) {
    // Task 1b: Handle "cd" internally
    if (strcmp(pCmdLine->arguments[0], "cd") == 0) {
        if (chdir(pCmdLine->arguments[1]) < 0) {
            perror("cd failed");
        }
        return;
    }

    // Task 2: Signal Commands
    if (strcmp(pCmdLine->arguments[0], "stop") == 0) {
        if (kill(atoi(pCmdLine->arguments[1]), SIGSTOP) < 0) perror("stop failed");
        return;
    }
    if (strcmp(pCmdLine->arguments[0], "wakeup") == 0) {
        if (kill(atoi(pCmdLine->arguments[1]), SIGCONT) < 0) perror("wakeup failed");
        return;
    }
    if (strcmp(pCmdLine->arguments[0], "ice") == 0) {
        if (kill(atoi(pCmdLine->arguments[1]), SIGINT) < 0) perror("ice failed");
        return;
    }
    if (strcmp(pCmdLine->arguments[0], "nuke") == 0) {
        // Kill process group (negative PID)
        if (kill(-atoi(pCmdLine->arguments[1]), SIGKILL) < 0) perror("nuke failed");
        return;
    }

    pid_t pid = fork();

    if (pid == 0) { // Child process
        // Task 3: I/O Redirection
        if (pCmdLine->inputRedirect) {
            int fdIn = open(pCmdLine->inputRedirect, O_RDONLY);
            if (fdIn < 0) { perror("Input redirection failed"); _exit(1); }
            dup2(fdIn, STDIN_FILENO);
            close(fdIn);
        }
        if (pCmdLine->outputRedirect) {
            int fdOut = open(pCmdLine->outputRedirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fdOut < 0) { perror("Output redirection failed"); _exit(1); }
            dup2(fdOut, STDOUT_FILENO);
            close(fdOut);
        }

        // Task 0 & 1a: Execution
        if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) < 0) {
            perror("Execution failed");
            _exit(1);
        }
    } else if (pid > 0) { // Parent process
        if (debug) {
            fprintf(stderr, "PID: %d\nExecuting: %s\n", pid, pCmdLine->arguments[0]);
        }
        
        // Task 1c: Wait if blocking is required (no '&')
        if (pCmdLine->blocking) {
            waitpid(pid, NULL, 0);
        }
    } else {
        perror("Fork failed");
    }
}

int main(int argc, char **argv) {
    char buffer[2048];
    char cwd[PATH_MAX];
    int debug = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) debug = 1;
    }

    while (1) {
        if (getcwd(cwd, PATH_MAX) != NULL) {
            printf("%s$ ", cwd);
        }
        
        if (!fgets(buffer, 2048, stdin)) break;
        if (strcmp(buffer, "quit\n") == 0) break;

        cmdLine *parsed = parseCmdLines(buffer);
        if (parsed) {
            execute(parsed, debug);
            freeCmdLines(parsed);
        }
    }
    return 0;
}