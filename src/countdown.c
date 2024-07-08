#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("\a\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler);
    int limit = 30 * 60; /* Default value */
    if (argc > 1)
        limit = atoi(argv[1]) * 60;
    while (limit >= 0) {
        printf("\r%02d:%02d", limit / 60, limit % 60);
        fflush(stdout);
        sleep(1);
        limit--;
    }
    printf("\a\n");
    return 0;
}
