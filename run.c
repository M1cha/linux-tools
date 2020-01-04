#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    pid_t pid;
    __sighandler_t handler;

    argc--;
    argv++;

    if (argc < 1) {
        fprintf(stderr, "No command given\n");
        return -EINVAL;
    }

    pid = fork();
    if (pid == 0) {
        // CHILD

        // redirect streams
        int fd = open("/dev/null", O_RDWR);
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("can't change stdout");
            return -1;
        }
        if (dup2(fd, STDERR_FILENO) == -1) {
            perror("can't change stderr");
            return -1;
        }
        
        // signal SIGHUP
        handler = signal(SIGHUP, SIG_IGN);
        if (handler == SIG_ERR) {
            perror("can't ignore SIGHUP");
            return -1;
        }

        // run command
        execvp(*argv, argv);
        perror("execvp");

        return -1;
    }

    else {
        // PARENT
    }

    return 0;
}
