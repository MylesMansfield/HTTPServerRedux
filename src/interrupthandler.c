#include <signal.h>
#include <stdio.h>

#include "interrupthandler.h"

int interrupted = 0;

void handle_sigint() {
    interrupted = 1;
    printf("\n");
    fflush(stdout);
}