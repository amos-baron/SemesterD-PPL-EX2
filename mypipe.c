#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    char buffer[1024];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == 0) { // Child: Reads from pipe
        close(pipefd[1]); // Close unused write end
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(pipefd[0]);
        _exit(0);
    } else { // Parent: Writes to pipe
        close(pipefd[0]); // Close unused read end
        write(pipefd[1], argv[1], strlen(argv[1]) + 1);
        close(pipefd[1]);
        wait(NULL); // Wait for child to finish
    }

    return 0;
}