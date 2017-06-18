#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int signal_sem (int, int);
int wait_sem(int, int);
void consumatore(key_t,key_t);
void produttore(key_t,key_t);

#endif
