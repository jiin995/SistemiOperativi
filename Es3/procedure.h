#ifndef procedure_h
#define procedure_h

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

	int execback (char *,char *[]);
	int exnoback (char *,char *[]);

#endif

