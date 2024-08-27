#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

extern int interrupted; // Control variable for infinite loops

void handle_sigint(); // SIGINT handler

#endif