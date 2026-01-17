#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("\e[0m\a\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler);
    int limit = 30 * 60; /* Default value */
    if (argc > 1)
        limit = atoi(argv[1]) * 60;
    while (limit >= 0) {
        int color = 0;
        if (limit == 0) color = 41;
        else if (limit % 60 == 0) color = 47;
        printf("\e[%dm\r%02d:%02d", color, limit / 60, limit % 60);
        fflush(stdout);
        sleep(1);
        limit--;
    }
    printf("\e[0m\a\a\a\a\a\n");
    return 0;
}
